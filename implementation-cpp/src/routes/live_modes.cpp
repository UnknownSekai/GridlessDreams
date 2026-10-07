#include "live_modes.h"

#include <msgpack.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "lives.h"
#include "../db.h"
#include "../db/user.h"
#include "../master_data.h"
#include "../pipeline.h"
#include "../wire.h"
#include "../helpers/unions.h"
#include "../helpers/daily.h"
#include "../helpers/game_state.h"
#include "../helpers/live.h"
#include "../helpers/live_result.h"
#include "../helpers/progression.h"
#include "../helpers/score.h"

// Ports routes/live_modes.py: master-driven Anthology/audition progression + lesson &
// music-course lives. These wrap the routes/lives.py Start/Finish/Retire handlers (run
// inside this request's transaction, via the shared global db) and layer progression on
// top, so the base live logic stays untouched. install_live_modes() prepends them.

namespace routes {

namespace {

using json = wire::json;               // ordered_json: master rows, payloads, wire values
using db::user::create_active_live;
using db::user::delete_active_lives;

// ---- private-member access: prepend handlers to httplib's post_handlers_ ----
// post_handlers_ is private; an explicit template instantiation ignores access control.
using HandlersVec =
    std::vector<std::pair<std::unique_ptr<httplib::detail::MatcherBase>, httplib::Server::Handler>>;

template <typename Tag>
struct AccessResult {
    static typename Tag::type ptr;
};
template <typename Tag>
typename Tag::type AccessResult<Tag>::ptr;

template <typename Tag, typename Tag::type P>
struct AccessRob {
    struct Filler {
        Filler() { AccessResult<Tag>::ptr = P; }
    };
    static Filler filler;
};
template <typename Tag, typename Tag::type P>
typename AccessRob<Tag, P>::Filler AccessRob<Tag, P>::filler;

struct PostHandlersTag {
    using type = HandlersVec httplib::Server::*;
};
template struct AccessRob<PostHandlersTag, &httplib::Server::post_handlers_>;

// ---- json type bridging (db rows are nlohmann::json, wire/master are ordered_json) ----
json to_oj(const nlohmann::json& v) { return json::parse(v.dump()); }
nlohmann::json to_nj(const json& v) { return nlohmann::json::parse(v.dump()); }

int64_t now_micros() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

bool ends_with(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

long long mp_int(const msgpack::object& o) {
    if (o.type == msgpack::type::POSITIVE_INTEGER) return static_cast<long long>(o.via.u64);
    if (o.type == msgpack::type::NEGATIVE_INTEGER) return o.via.i64;
    return 0;
}

// unpack the 5 concatenated packs of a common_response body (parts()). our own
// responses are not lz4-compressed, so _decompress is a no-op; each handle is
// self-contained (msgpack copies variable data into its own zone).
std::vector<msgpack::object_handle> parts(const std::string& body) {
    std::vector<msgpack::object_handle> out;
    size_t off = 0;
    while (off < body.size()) out.push_back(msgpack::unpack(body.data(), body.size(), off));
    return out;
}

// decode a faults pack (array of Fault model arrays) back to field-named jsons.
json decode_faults(const msgpack::object& packed) {
    json out = json::array();
    if (packed.type != msgpack::type::ARRAY) return out;
    for (uint32_t i = 0; i < packed.via.array.size; ++i)
        out.push_back(wire::from_array("Fault", packed.via.array.ptr[i]));
    return out;
}

// decode a present pack ([key, valueArray] entries) into [key, name, value] union
// entries so pipeline::respond re-encodes them byte-identically via to_wire.
json decode_present(const msgpack::object& packed) {
    json out = json::array();
    if (packed.type != msgpack::type::ARRAY) return out;
    for (uint32_t i = 0; i < packed.via.array.size; ++i) {
        const msgpack::object& entry = packed.via.array.ptr[i];
        if (entry.type != msgpack::type::ARRAY || entry.via.array.size < 2) continue;
        long long key = mp_int(entry.via.array.ptr[0]);
        const std::string& name = unions::IDATA_OBJECT.at(static_cast<int>(key));
        json value = wire::from_array(name.c_str(), entry.via.array.ptr[1]);
        out.push_back(wire::union_entry(key, name.c_str(), value));
    }
    return out;
}

// value[0] of a present entry: the entity's key-0 field (its id). mirrors the
// python (v[0], v[1][0]) dedup key for entities whose pk is not "id_".
long long present_id0(const std::string& name, const json& value) {
    static const std::vector<std::pair<std::string, std::string>> special = {
        {"ConcertStage", "concert_stage_master_id"},
        {"CharacterLesson", "character_base_master_id"},
    };
    std::string fn = "id_";
    for (const auto& kv : special)
        if (kv.first == name) fn = kv.second;
    auto it = value.find(fn);
    return it != value.end() && it->is_number() ? it->get<long long>() : 0;
}

// (int(thing_type), thing_id, thing_quantity) triples from a reward-rows list.
json rewards(const json& rows) {
    json out = json::array();
    if (rows.is_array())
        for (const json& r : rows)
            out.push_back(json::array({r.at("thing_type").get<long long>(),
                                       r.at("thing_id").get<long long>(),
                                       r.at("thing_quantity").get<long long>()}));
    return out;
}

bool concert_available(const json& stage, const std::set<long long>& cleared) {
    long long cmid = stage.at("concert_master_id").get<long long>();
    std::vector<long long> siblings;
    for (const json& m : master_data::table("ConcertStageMaster"))
        if (m.at("concert_master_id").get<long long>() == cmid)
            siblings.push_back(m.at("id_").get<long long>());
    std::sort(siblings.begin(), siblings.end());
    long long sid = stage.at("id_").get<long long>();
    size_t pos = static_cast<size_t>(
        std::find(siblings.begin(), siblings.end(), sid) - siblings.begin());
    return cleared.count(sid) || pos == 0 || (pos > 0 && cleared.count(siblings[pos - 1]) > 0);
}

std::vector<json> phases(long long ident) {
    std::vector<json> out;
    for (const json& p : master_data::table("AuditionPhaseMaster"))
        if (p.at("auditionaster_id").get<long long>() == ident) out.push_back(p);
    std::stable_sort(out.begin(), out.end(), [](const json& a, const json& b) {
        return a.at("phase").get<long long>() < b.at("phase").get<long long>();
    });
    return out;
}

long long attained(long long ident, long long score, long long acts, bool cleared) {
    long long best = 0;
    if (cleared) {
        for (const json& p : phases(ident)) {
            long long star = 0;
            auto it = p.find("star_act_count");
            if (it != p.end() && it->is_number()) star = it->get<long long>();
            if (score >= p.at("clear_score").get<long long>() && acts >= star)
                best = std::max(best, p.at("phase").get<long long>());
        }
    }
    return best;
}

long long rand_live_id() {
    static std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<long long> dist(1000000LL, 9999999999LL);
    return dist(rng);
}

// run an upstream base handler inside this request's active transaction (the global db
// is already in the transaction, so no app-swap BoundApp is needed) and return its
// decoded 5-pack body.
std::vector<msgpack::object_handle> call_bound(const httplib::Request& req,
                                               void (*fn)(const httplib::Request&,
                                                          httplib::Response&)) {
    httplib::Response upstream;
    fn(req, upstream);
    return parts(upstream.body);
}

// ---- /api/Lives/Start + /api/Lives/StartConcert ----
void start(const httplib::Request& req, httplib::Response& res) {
    try {
        json p = pipeline::read_request(req, "StartLivePayload");
        if (p.is_null()) throw game_state::Rejected("Missing payload");
        bool concert = ends_with(req.path, "StartConcert");
        std::string mode = concert ? "concert" : "audition";
        // ident is 0 (falsy) when the nullable stage field is nil/absent, matching python `if ident`
        long long ident = concert ? p.value("concert_stage_master_id", static_cast<long long>(0))
                                  : p.value("audition_master_id", static_cast<long long>(0));
        const json* m =
            ident ? game_state::master(concert ? "concert_stage_master" : "audition_master", ident)
                  : nullptr;
        long long live_master_id = p.value("live_master_id", static_cast<long long>(0));
        const json* chart = game_state::master("live_master", live_master_id);
        if (chart == nullptr || (concert && m == nullptr) || (ident && m == nullptr) ||
            (m != nullptr &&
             chart->at("music_master_id").get<long long>() != m->at("music_master_id").get<long long>()))
            throw game_state::Rejected("Invalid stage or chart");

        game_state::State s = game_state::transaction(req);
        long long party_id = p.value("party_id", static_cast<long long>(0));
        if (s.one("Party", nlohmann::json{{"id", party_id}}) == nullptr)
            throw game_state::Rejected("Missing party");
        if (concert) {
            std::set<long long> cleared;
            for (const nlohmann::json& r : s.rows("ConcertStage"))
                cleared.insert(r.at("concertStageMasterId").get<long long>());
            if (!concert_available(*m, cleared)) throw game_state::Rejected("Stage is locked");
        }
        progression::daily(s);
        std::vector<msgpack::object_handle> r = call_bound(req, &routes::lives_start);

        std::optional<nlohmann::json> active = db::fetchrow(
            db::SelectQuery("ActiveLive", "SELECT * FROM active_live WHERE \"userId\"=$1", s.uid));
        const json* music =
            game_state::master("music_master", chart->at("music_master_id").get<long long>());
        long long xp = 0;
        if (active.has_value() && active->at("staminaSpent").get<bool>() && music != nullptr) {
            long long sc = music->at("stamina_consumption").get<long long>();
            long long ratio = p.value("stamina_consumption_ratio", static_cast<long long>(0));
            double rank_xp_per_stamina =
                progression::rules().at("rank_xp_per_stamina").get<double>();
            xp = static_cast<long long>(static_cast<double>(sc) *
                                        static_cast<double>(std::max<long long>(1, ratio)) *
                                        rank_xp_per_stamina);
        }
        json extra = json::object();
        extra["rank_xp"] = xp;
        extra["auto"] = p.value("is_auto_play", false);
        progression::context(s, m != nullptr ? mode : std::string("normal"), ident, extra);
        db::execute(db::ExecutableQuery(
            "DELETE FROM preservation_course_run WHERE \"userId\"=$1", s.uid));

        json unit = wire::from_array("LiveUnit", r[1].get());
        if (m != nullptr) {
            std::optional<long long> sense;
            auto sit = m->find("sense_notation_master_id");
            if (sit != m->end() && !sit->is_null()) sense = sit->get<long long>();
            json lte = live::build_live_time_event(s.uid, party_id,
                                                   m->at("music_master_id").get<long long>(), sense);
            json time_events = json::object();
            if (lte.contains("timings") && lte["timings"].is_array())
                for (const json& t : lte["timings"])
                    time_events[std::to_string(t.at("timing_seconds").get<long long>())] =
                        t.at("event");
            unit["time_events"] = time_events;
        }
        s.commit();

        json faults = decode_faults(r[0].get());
        json present = decode_present(r[2].get());
        for (const json& e : s.present()) present.push_back(e);
        pipeline::respond(res, "LiveUnit", unit, faults, present);
    } catch (const game_state::Rejected& e) {
        pipeline::respond(res, nullptr, json(nullptr),
                          json::array({pipeline::fault("InvalidLiveStage", e.what())}));
    }
}

void apply_progress(game_state::State& s, const std::string& mode, long long ident, long long score,
                    long long acts, bool cleared, json& result) {
    const json* m = game_state::master(
        mode == "concert" ? "concert_stage_master" : "audition_master", ident);
    if (m == nullptr) throw game_state::Rejected("Missing stage master");
    if (mode == "concert") {
        json received = json::array();
        if (cleared && score >= m->at("clear_score").get<long long>() &&
            s.one("ConcertStage", nlohmann::json{{"concertStageMasterId", ident}}) == nullptr) {
            received = s.grant(rewards(m->value("rewards", json(nullptr))));
            s.insert("ConcertStage", nlohmann::json{{"concertStageMasterId", ident}});
        }
        json cr = json::object();
        cr["rewards"] = received;
        result["concert_result"] = cr;
        return;
    }
    nlohmann::json* row = s.one("AuditionClear", nlohmann::json{{"auditionMasterId", ident}});
    long long before =
        row != nullptr
            ? std::max(row->at("clearPhase").get<long long>(), row->at("skipClearPhase").get<long long>())
            : 0;
    long long achieved = std::min(before + 1, attained(ident, score, acts, cleared));
    result["audition_master_id"] = ident;
    result["audition_before_phase"] = before;
    result["audition_after_phase"] = std::max(before, achieved);
    std::vector<json> things;  // (type, id, quantity) triples
    // the official challenge clear awards the stages in their master order
    long long group = m->at("audition_group_number").get<long long>();
    std::vector<json> targets;
    for (const json& x : master_data::table("AuditionMaster"))
        if (x.at("audition_group_number").get<long long>() == group &&
            x.at("id_").get<long long>() <= ident)
            targets.push_back(x);
    std::stable_sort(targets.begin(), targets.end(), [](const json& a, const json& b) {
        return a.at("id_").get<long long>() < b.at("id_").get<long long>();
    });
    for (const json& target : targets) {
        long long tid = target.at("id_").get<long long>();
        std::vector<json> target_phases = phases(tid);
        long long max_phase = 0;
        for (const json& pp : target_phases) max_phase = std::max(max_phase, pp.at("phase").get<long long>());
        long long new_phase = std::min(achieved, max_phase);
        if (!new_phase) continue;
        nlohmann::json* existing = s.one("AuditionClear", nlohmann::json{{"auditionMasterId", tid}});
        long long old = existing != nullptr
                            ? std::max(existing->at("clearPhase").get<long long>(),
                                       existing->at("skipClearPhase").get<long long>())
                            : 0;
        if (new_phase <= old) continue;
        for (const json& phase : target_phases) {
            long long ph = phase.at("phase").get<long long>();
            if (old < ph && ph <= new_phase) {
                const json* package = game_state::master(
                    "audition_reward_package_master",
                    phase.at("audition_reward_package_master_id").get<long long>());
                if (package == nullptr) throw game_state::Rejected("Missing reward package");
                for (const json& tr : rewards(package->value("rewards", json(nullptr))))
                    things.push_back(tr);
            }
        }
        std::string field = tid == ident ? "clearPhase" : "skipClearPhase";
        if (existing != nullptr) {
            if (new_phase > existing->at(field).get<long long>())
                s.update("AuditionClear", existing, nlohmann::json{{field, new_phase}});
        } else {
            nlohmann::json values{{"auditionMasterId", tid},
                                  {"clearPhase", 0},
                                  {"skipClearPhase", 0},
                                  {"auditionClearPartyId", 0}};
            values[field] = new_phase;
            s.insert("AuditionClear", values);
        }
    }
    // preserve reward entries in official order (including repeated item types)
    std::stable_sort(things.begin(), things.end(),
                     [](const json& a, const json& b) { return a[0].get<long long>() < b[0].get<long long>(); });
    json audition_rewards = json::array();
    for (const json& thing : things) {
        json one = json::array();
        one.push_back(thing);
        json granted = s.grant(one);
        for (const json& g : granted) audition_rewards.push_back(g);
    }
    result["audition_rewards"] = audition_rewards;
}

// ---- /api/Lives/FinishAndValidate ----
void finish(const httplib::Request& req, httplib::Response& res) {
    try {
        json p = pipeline::read_request(req, "FinishLivePayload");
        if (p.is_null() || !score::verify_score_blocks(p))
            throw game_state::Rejected("Invalid score blocks");
        json result;
        json present = json::array();
        {
            game_state::State s = game_state::transaction(req);
            std::optional<nlohmann::json> active = db::fetchrow(db::SelectQuery(
                "ActiveLive", "SELECT * FROM active_live WHERE \"userId\"=$1", s.uid));
            if (!active.has_value()) throw game_state::Rejected("No active live");
            std::optional<nlohmann::json> context_row = db::fetchrow(db::SelectQuery(
                "PreservationLiveContext",
                "SELECT * FROM preservation_live_context WHERE \"userId\"=$1", s.uid));
            std::vector<msgpack::object_handle> r =
                call_bound(req, &routes::lives_finish_and_validate);
            const msgpack::object& faults_pack = r[0].get();
            if (faults_pack.type == msgpack::type::ARRAY && faults_pack.via.array.size > 0)
                throw game_state::Rejected("Live validation failed");
            result = wire::from_array("FinishLiveResult", r[1].get());
            if (context_row.has_value()) {
                std::pair<long long, bool> totals = live_result::play_totals(p);
                long long score = totals.first;
                bool cleared = totals.second;
                std::string mode = context_row->at("mode").get<std::string>();
                if (mode == "concert" || mode == "audition") {
                    long long acts =
                        p.contains("star_act_score_blocks") && p["star_act_score_blocks"].is_array()
                            ? static_cast<long long>(p["star_act_score_blocks"].size())
                            : 0;
                    apply_progress(s, mode, context_row->at("masterId").get<long long>(), score, acts,
                                   cleared, result);
                }
                progression::finish_context(s, to_oj(*context_row), p, result);
            }
            db::execute(db::ExecutableQuery(
                "DELETE FROM preservation_live_context WHERE \"userId\"=$1", s.uid));
            json changed = s.present();
            std::set<std::pair<long long, long long>> keys;
            for (const json& v : changed)
                keys.insert({v[0].get<long long>(), present_id0(v[1].get<std::string>(), v[2])});
            for (const json& v : decode_present(r[2].get())) {
                std::pair<long long, long long> k{v[0].get<long long>(),
                                                  present_id0(v[1].get<std::string>(), v[2])};
                if (keys.count(k) == 0) present.push_back(v);
            }
            for (const json& v : changed) present.push_back(v);
            s.commit();
        }
        pipeline::respond(res, "FinishLiveResult", result, json::array(), present);
    } catch (const game_state::Rejected& e) {
        pipeline::respond(res, "FinishLiveResult", json::object(),
                          json::array({pipeline::fault("InvalidLiveState", e.what())}));
    }
}

// ---- /api/Lives/Retire ----
void retire(const httplib::Request& req, httplib::Response& res) {
    game_state::State s = game_state::transaction(req);
    db::execute(db::ExecutableQuery(
        "DELETE FROM preservation_live_context WHERE \"userId\"=$1", s.uid));
    db::execute(db::ExecutableQuery(
        "DELETE FROM preservation_course_run WHERE \"userId\"=$1", s.uid));
    routes::lives_retire(req, res);
    s.commit();
}

// ---- /api/Lessons/{base}/CreateParty ----
void create_lesson(const httplib::Request& req, httplib::Response& res, long long base) {
    try {
        game_state::State s = game_state::transaction(req);
        progression::lesson_party(s, base);
        s.commit();
        json result = json::object();
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "BooleanResult", json::object());
    }
}

// ---- /api/Lessons/{base}/SetParty ----
void set_lesson(const httplib::Request& req, httplib::Response& res, long long base) {
    try {
        json p = pipeline::read_request(req, "SetLessonPartyPayload");
        if (p.is_null() || !p.contains("slots") || !p["slots"].is_array() || p["slots"].empty())
            throw game_state::Rejected();
        game_state::State s = game_state::transaction(req);
        json lrow = progression::lesson_party(s, base);  // ensure + read-snapshot
        nlohmann::json* row = s.one("CharacterLesson", nlohmann::json{{"characterBaseMasterId", base}});
        if (row == nullptr) throw game_state::Rejected();
        std::vector<progression::Slot> slots;
        std::set<long long> seen;
        std::set<long long> positions;
        for (const json& x : p["slots"]) {
            long long order = x.value("order", static_cast<long long>(0));
            long long cid = x.value("character_id", static_cast<long long>(0));
            if (!(1 <= order && order <= 5) || positions.count(order) > 0)
                throw game_state::Rejected();
            positions.insert(order);
            if (cid) {
                nlohmann::json* c = s.one("Character", nlohmann::json{{"id", cid}});
                const json* cm = c != nullptr
                                     ? game_state::master("character_master",
                                                          c->at("characterMasterId").get<long long>())
                                     : nullptr;
                if (c == nullptr || seen.count(cid) > 0 || cm == nullptr ||
                    cm->at("character_base_master_id").get<long long>() != base)
                    throw game_state::Rejected();
                seen.insert(cid);
            }
            slots.emplace_back(order, cid ? std::optional<long long>(cid) : std::nullopt);
        }
        if (seen.empty()) throw game_state::Rejected();
        for (long long i = 1; i <= 5; ++i)
            if (positions.count(i) == 0) slots.emplace_back(i, std::nullopt);
        long long leader = lrow.value("leaderPosition", static_cast<long long>(0));
        if (leader) {
            bool ok = false;
            for (const progression::Slot& sl : slots)
                if (sl.first == leader && sl.second.has_value()) ok = true;
            if (!ok) leader = 0;
        }
        nlohmann::json values;
        values["setCharacters"] = to_nj(progression::stored_slots(slots));
        values["leaderPosition"] = leader;
        s.update("CharacterLesson", row, values);
        s.commit();
        json result = json::object();
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "BooleanResult", json::object());
    }
}

// ---- /api/Lessons/{base}/SetPartyLeader/{position} ----
void set_leader(const httplib::Request& req, httplib::Response& res, long long base,
                long long position) {
    try {
        game_state::State s = game_state::transaction(req);
        json lrow = progression::lesson_party(s, base);
        nlohmann::json* row = s.one("CharacterLesson", nlohmann::json{{"characterBaseMasterId", base}});
        if (row == nullptr) throw game_state::Rejected();
        if (!(0 <= position && position <= 5)) throw game_state::Rejected();
        if (position) {
            bool ok = false;
            for (const progression::Slot& sl : progression::lesson_slots(lrow))
                if (sl.first == position && sl.second.has_value()) ok = true;
            if (!ok) throw game_state::Rejected();
        }
        s.update("CharacterLesson", row, nlohmann::json{{"leaderPosition", position}});
        s.commit();
        json result = json::object();
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "BooleanResult", json::object());
    }
}

// ---- /api/Lives/StartLesson ----
void start_lesson(const httplib::Request& req, httplib::Response& res) {
    try {
        json p = pipeline::read_request(req, "StartLessonPayload");
        if (p.is_null()) throw game_state::Rejected();
        long long base = p.value("character_base_master_id", static_cast<long long>(0));
        long long chart = p.value("live_master_id", static_cast<long long>(0));
        if (game_state::master("live_master", chart) == nullptr)
            throw game_state::Rejected("Unknown chart");
        game_state::State s = game_state::transaction(req);
        json lrow = progression::lesson_party(s, base);
        progression::daily(s);
        std::vector<progression::Slot> lesson = progression::lesson_slots(lrow);
        bool any_actor = false;
        for (const progression::Slot& sl : lesson)
            if (sl.second.has_value() && sl.second.value() != 0) any_actor = true;
        if (!any_actor) throw game_state::Rejected("Empty lesson party");

        // a temporary party exists only within this transaction and is removed before return,
        // so no phantom party or slot ever appears on the account
        long long party_id = 0;
        for (const nlohmann::json& r : s.rows("Party"))
            party_id = std::max(party_id, r.at("id").get<long long>());
        party_id += 1;
        long long leader = lrow.value("leaderPosition", static_cast<long long>(0));
        db::execute(db::user::upsert_party(
            s.uid, nlohmann::json{{"id", party_id},
                                  {"order", 0},
                                  {"name", "Lesson"},
                                  {"leaderPosition", leader ? leader : 1}}));
        long long sid = 0;
        for (const nlohmann::json& r : s.rows("PartySlot"))
            sid = std::max(sid, r.at("id").get<long long>());
        sid += 1;
        for (const progression::Slot& sl : lesson) {
            if (!sl.second.has_value() || sl.second.value() == 0) continue;
            long long ident = sl.second.value();
            nlohmann::json* actor = s.one("Character", nlohmann::json{{"id", ident}});
            const json* cm = actor != nullptr
                                 ? game_state::master("character_master",
                                                      actor->at("characterMasterId").get<long long>())
                                 : nullptr;
            if (actor == nullptr || cm == nullptr ||
                cm->at("character_base_master_id").get<long long>() != base)
                throw game_state::Rejected("Invalid lesson actor");
            db::execute(db::user::upsert_party_slot(
                s.uid, nlohmann::json{{"id", sid},
                                      {"partyId", party_id},
                                      {"position", sl.first},
                                      {"characterId", ident},
                                      {"posterId", nullptr},
                                      {"accessoryId", nullptr},
                                      {"bonusAbilityEnableFlags", 0}}));
            sid += 1;
        }
        std::pair<json, long long> built = live::build_live_unit(s.uid, party_id, chart);
        json unit = built.first;
        long long live_id = built.second;
        db::execute(db::ExecutableQuery(
            "DELETE FROM party_slot WHERE \"userId\"=$1 AND \"partyId\"=$2", s.uid, party_id));
        db::execute(db::ExecutableQuery("DELETE FROM party WHERE \"userId\"=$1 AND id=$2", s.uid,
                                        party_id));
        db::execute(delete_active_lives(s.uid));
        db::execute(create_active_live(s.uid, live_id, chart, 0, 1, false));
        db::execute(db::ExecutableQuery(
            "DELETE FROM preservation_course_run WHERE \"userId\"=$1", s.uid));
        json extra = json::object();
        extra["rank_xp"] = to_oj(progression::rules().at("lesson_rank_xp"));
        progression::context(s, "lesson", base, extra);
        s.commit();
        pipeline::respond(res, "LiveUnit", unit, json::array(), s.present());
    } catch (const game_state::Rejected& e) {
        pipeline::respond(res, nullptr, json(nullptr),
                          json::array({pipeline::fault("InvalidLesson", e.what())}));
    }
}

// ---- /api/Lives/StartMusicCourseLive ----
void start_course(const httplib::Request& req, httplib::Response& res) {
    try {
        json p = pipeline::read_request(req, "StartLivePayload");
        if (p.is_null()) throw game_state::Rejected();
        std::tuple<json, std::vector<json>, long long> fc =
            progression::find_course(p.value("music_course_detail_master_id", static_cast<long long>(0)));
        const json& course = std::get<0>(fc);
        const std::vector<json>& details = std::get<1>(fc);
        long long index = std::get<2>(fc);
        const json& d = details[static_cast<size_t>(index)];
        long long gauge = p.value("music_course_gauge_type", static_cast<long long>(0));
        // d.live_master_id is Optional; a null/absent id never equals the chart (python None != int)
        auto dlmit = d.find("live_master_id");
        bool chart_mismatch = dlmit == d.end() || dlmit->is_null() ||
                              dlmit->get<long long>() != p.value("live_master_id", static_cast<long long>(0));
        if (chart_mismatch || !(gauge == 0 || gauge == 1))
            throw game_state::Rejected("Unsupported course chart");
        game_state::State s = game_state::transaction(req);
        std::optional<nlohmann::json> old = db::fetchrow(db::SelectQuery(
            "PreservationCourseRun",
            "SELECT * FROM preservation_course_run WHERE \"userId\"=$1", s.uid));
        std::optional<nlohmann::json> active = db::fetchrow(db::SelectQuery(
            "PreservationLiveContext",
            "SELECT * FROM preservation_live_context WHERE \"userId\"=$1", s.uid));
        if (active.has_value() && active->at("mode").get<std::string>() == "course")
            throw game_state::Rejected("Finish or retire the active stage first");
        nlohmann::json data;
        if (index == 0) {
            json usage = progression::daily(s);
            const nlohmann::json& policy = progression::rules();
            std::string paid = "unlimited";
            if (!policy.at("unlimited_attempts").get<bool>()) {
                if (usage.at("musicCourseFreeChallengeTimes").get<long long>() <
                    policy.at("course_free_attempts").get<long long>()) {
                    progression::use_daily(s, "musicCourseFreeChallengeTimes");
                    paid = "free";
                } else {
                    // required_item_master_id is Optional; keep it null so the lookup misses
                    // (python s.one(Item, itemMasterId=None)) rather than crash on .get<long long>()
                    nlohmann::json req_item = nlohmann::json(nullptr);
                    auto riit = course.find("required_item_master_id");
                    if (riit != course.end()) req_item = *riit;
                    nlohmann::json* item =
                        s.one("Item", nlohmann::json{{"itemMasterId", req_item}});
                    long long amount = course.at("required_amount").get<long long>();
                    if (item != nullptr && item->at("stock").get<long long>() >= amount && amount > 0) {
                        s.pay(std::map<int64_t, int64_t>{{req_item.get<int64_t>(), amount}});
                        paid = "ticket";
                    } else if (policy.at("waive_missing_course_tickets").get<bool>()) {
                        paid = "waived";
                    } else {
                        throw game_state::Rejected("No course entry remaining");
                    }
                }
            }
            data = nlohmann::json{{"course", course.at("id_").get<long long>()},
                                  {"gauge", gauge},
                                  {"next", 0},
                                  {"rates", nlohmann::json::array()},
                                  {"lamps", nlohmann::json::array()},
                                  {"payment", paid},
                                  {"started_at", now_micros()}};
            db::execute(db::ExecutableQuery(
                "DELETE FROM preservation_course_run WHERE \"userId\"=$1", s.uid));
            db::execute(db::ExecutableQuery(
                "INSERT INTO preservation_course_run (\"userId\",data) VALUES ($1,$2)", s.uid, data));
        } else {
            if (!old.has_value()) throw game_state::Rejected("Course has not started");
            data = old->at("data");
            if (data.at("course").get<long long>() != course.at("id_").get<long long>() ||
                data.at("next").get<long long>() != index ||
                data.at("gauge").get<long long>() != gauge)
                throw game_state::Rejected("Wrong course stage");
            if (data.at("started_at").get<long long>() < daily::most_recent_reset(now_micros()))
                throw game_state::Rejected("Course crossed daily reset");
        }
        long long live_id = rand_live_id();
        db::execute(delete_active_lives(s.uid));
        db::execute(create_active_live(s.uid, live_id,
                                       p.value("live_master_id", static_cast<long long>(0)), 0, 0,
                                       false));
        progression::context(s, "course", d.at("id_").get<long long>(), json::object());
        s.commit();
        json unit = json::object();
        unit["u_active_live_id"] = live_id;
        pipeline::respond(res, "LiveUnit", unit, json::array(), s.present());
    } catch (const game_state::Rejected& e) {
        pipeline::respond(res, nullptr, json(nullptr),
                          json::array({pipeline::fault("InvalidCourse", e.what())}));
    }
}

}  // namespace

void install_live_modes(httplib::Server& svr) {
    // register the override handlers, then rotate the just-added entries to the front of
    // post_handlers_ so they take precedence over the lives/lessons base handlers
    // (mirrors live_modes.install() prepending its router).
    HandlersVec& handlers = svr.*AccessResult<PostHandlersTag>::ptr;
    size_t before = handlers.size();

    svr.Post("/api/Lives/Start", start);
    svr.Post("/api/Lives/StartConcert", start);
    svr.Post("/api/Lives/FinishAndValidate", finish);
    svr.Post("/api/Lives/Retire", retire);
    svr.Post("/api/Lessons/:base/CreateParty",
             [](const httplib::Request& req, httplib::Response& res) {
                 create_lesson(req, res, std::stoll(req.path_params.at("base")));
             });
    svr.Post("/api/Lessons/:base/SetParty",
             [](const httplib::Request& req, httplib::Response& res) {
                 set_lesson(req, res, std::stoll(req.path_params.at("base")));
             });
    svr.Post("/api/Lessons/:base/SetPartyLeader/:position",
             [](const httplib::Request& req, httplib::Response& res) {
                 set_leader(req, res, std::stoll(req.path_params.at("base")),
                            std::stoll(req.path_params.at("position")));
             });
    svr.Post("/api/Lives/StartLesson", start_lesson);
    svr.Post("/api/Lives/StartMusicCourseLive", start_course);

    std::rotate(handlers.begin(), handlers.begin() + static_cast<std::ptrdiff_t>(before),
                handlers.end());
}

}  // namespace routes
