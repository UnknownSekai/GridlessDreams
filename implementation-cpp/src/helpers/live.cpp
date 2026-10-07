#include "helpers/live.h"

#include <algorithm>
#include <map>
#include <mutex>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "helpers/effects.h"
#include "master_data.h"

// Ports helpers/live.py. The returned LiveUnit carries actors/time_events as ordered_json
// objects keyed by the decimal int (position / timing-second). In the wire they are
// Dictionary<int,V> with integer map keys and a value model that base=="Dictionary" cannot
// name, so the live routes special-case these two fields when serializing (see PORT plan).

namespace live {

namespace {

using ll = long long;
using ojson = wire::json;   // ordered_json: master rows + wire entities
using djson = db::json;     // db rows (camelCase)

// floor division matching python's `//` (all operands here are non-negative).
ll floordiv(ll a, ll b) {
    ll q = a / b, r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) --q;
    return q;
}

template <class J>
ll jint(const J& o, const char* k, ll dflt) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null() || !it->is_number_integer()) return dflt;
    return it->template get<ll>();
}

template <class J>
double jnum(const J& o, const char* k, double dflt) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null() || !it->is_number()) return dflt;
    return it->template get<double>();
}

template <class J>
bool jbool(const J& o, const char* k, bool dflt) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null() || !it->is_boolean()) return dflt;
    return it->template get<bool>();
}

const djson* dget(const std::unordered_map<ll, djson>& m, ll id) {
    auto it = m.find(id);
    return it == m.end() ? nullptr : &it->second;
}

const ojson* mget(const std::unordered_map<ll, const ojson*>& m, ll id) {
    auto it = m.find(id);
    return it == m.end() ? nullptr : it->second;
}

std::optional<ojson> opt_master(const ojson* p) {
    if (!p) return std::nullopt;
    return *p;
}

// accessories.get(slot.<key>) / posters.get(slot.<key>) -- null id or missing -> none.
std::optional<ojson> acc_opt(const std::unordered_map<ll, ojson>& m, const djson& slot,
                             const char* key) {
    auto it = slot.find(key);
    if (it == slot.end() || !it->is_number_integer()) return std::nullopt;
    auto mit = m.find(it->get<ll>());
    if (mit == m.end()) return std::nullopt;
    return mit->second;
}

std::unordered_map<ll, djson> index_db(std::vector<djson> rows, const char* key) {
    std::unordered_map<ll, djson> m;
    for (djson& r : rows) m[jint(r, key, 0)] = std::move(r);  // last duplicate wins
    return m;
}

std::unordered_map<ll, ojson> index_ord(std::vector<djson> rows, const char* key) {
    std::unordered_map<ll, ojson> m;
    for (djson& r : rows) m[jint(r, key, 0)] = ojson(r);
    return m;
}

// _MASTERS: {id_: row} over a master table, built once per table (header_name PascalCase).
const std::unordered_map<ll, const ojson*>& by_id(const std::string& header) {
    static std::map<std::string, std::unordered_map<ll, const ojson*>> cache;
    static std::mutex mu;
    std::lock_guard<std::mutex> g(mu);
    auto it = cache.find(header);
    if (it != cache.end()) return it->second;
    std::unordered_map<ll, const ojson*> m;
    for (const ojson& row : master_data::table(header)) {
        auto f = row.find("id_");
        if (f != row.end() && f->is_number_integer()) m[f->get<ll>()] = &row;
    }
    return cache.emplace(header, std::move(m)).first->second;
}

// SenseTypes and SenseLightTypes share values; a sense of light-less type yields no lights.
std::vector<ll> lights_of(ll sense_type, ll count) {
    if (sense_type < enums::SenseLightTypes::Variable || sense_type > enums::SenseLightTypes::Special)
        return {};
    std::vector<ll> out;
    ll n = std::max<ll>(0, count);
    out.reserve(static_cast<size_t>(n));
    for (ll i = 0; i < n; ++i) out.push_back(sense_type);
    return out;
}

bool has_cancel_sense(const std::vector<effects::AppliedEffect>& effs) {
    for (const auto& e : effs) {
        auto it = e.master.find("type");
        if (it != e.master.end() && it->is_number_integer() &&
            it->get<ll>() == enums::EffectTypes::FinalPerformanceUpCancelSense)
            return true;
    }
    return false;
}

// _STATUS_LEVEL: {level: character_status_level}, built once.
ll status_level(ll level) {
    static const std::unordered_map<ll, ll> m = [] {
        std::unordered_map<ll, ll> r;
        for (const ojson& row : master_data::table("CharacterLevelMaster"))
            r[jint(row, "level", 0)] = jint(row, "character_status_level", 0);
        return r;
    }();
    auto it = m.find(level);
    return it == m.end() ? 0 : it->second;
}

// _STAR_RANK_BONUS: {rank: status_bonus} (percent points), built once.
double star_rank_bonus(ll star_rank) {
    static const std::unordered_map<ll, double> m = [] {
        std::unordered_map<ll, double> r;
        for (const ojson& row : master_data::table("CharacterStarRankMaster"))
            r[jint(row, "rank", 0)] = jnum(row, "status_bonus", 0.0);
        return r;
    }();
    auto it = m.find(star_rank);
    return it == m.end() ? 0.0 : it->second;
}

// base_stat = min_level_status + base_stat_bonuses + story_read_bonus;
// level_scaled = floor(base_stat * status_level(level) / 100); factor = (100 + intrinsic)/100;
// char_status = floor(level_scaled applied as base_stat * sl * factor).
std::tuple<ll, ll, ll> character_base_status(const ojson* character_master, ll level,
                                             std::tuple<ll, ll, ll> base_stat_bonuses,
                                             ll story_read_bonus, double intrinsic) {
    const ojson* mls = nullptr;
    if (character_master) {
        auto it = character_master->find("min_level_status");
        if (it != character_master->end() && !it->is_null()) mls = &(*it);
    }
    if (!mls) return {0, 0, 0};
    double sl = static_cast<double>(status_level(level)) / 100.0;
    double f = (100.0 + intrinsic) / 100.0;
    ll b0 = std::get<0>(base_stat_bonuses), b1 = std::get<1>(base_stat_bonuses),
       b2 = std::get<2>(base_stat_bonuses);
    ll v = static_cast<ll>(static_cast<double>(jint(*mls, "vocal", 0) + b0 + story_read_bonus) * sl * f);
    ll e = static_cast<ll>(static_cast<double>(jint(*mls, "expression", 0) + b1 + story_read_bonus) * sl * f);
    ll c = static_cast<ll>(static_cast<double>(jint(*mls, "concentration", 0) + b2 + story_read_bonus) * sl * f);
    return {v, e, c};
}

// 3-stage integer-truncation pipeline: stage1 flat + component% (floored per source), stage2
// performance% (floored per source), stage3 multiplication. Returns a LiveStatus entity.
ojson calculate_slot_status(std::tuple<ll, ll, ll> char_status, std::tuple<ll, ll, ll> flat_bonuses,
                            const std::map<ll, std::tuple<ll, ll, ll>>& comp_pcts,
                            const std::map<ll, ll>& perf_pcts, double multiplier) {
    ll v_base = std::get<0>(char_status), e_base = std::get<1>(char_status),
       c_base = std::get<2>(char_status);
    ll sum_cv = 0, sum_ce = 0, sum_cc = 0;
    for (const auto& kv : comp_pcts) {
        const auto& p = kv.second;
        sum_cv += floordiv(v_base * std::get<0>(p), 10000);
        sum_ce += floordiv(e_base * std::get<1>(p), 10000);
        sum_cc += floordiv(c_base * std::get<2>(p), 10000);
    }
    ll v_s1 = v_base + std::get<0>(flat_bonuses) + sum_cv;
    ll e_s1 = e_base + std::get<1>(flat_bonuses) + sum_ce;
    ll c_s1 = c_base + std::get<2>(flat_bonuses) + sum_cc;
    ll sum_pv = 0, sum_pe = 0, sum_pc = 0;
    for (const auto& kv : perf_pcts) {
        ll perf = kv.second;
        sum_pv += floordiv(v_s1 * perf, 10000);
        sum_pe += floordiv(e_s1 * perf, 10000);
        sum_pc += floordiv(c_s1 * perf, 10000);
    }
    ll v_final = v_s1 + sum_pv;
    ll e_final = e_s1 + sum_pe;
    ll c_final = c_s1 + sum_pc;
    if (multiplier != 1.0) {
        v_final = static_cast<ll>(static_cast<double>(v_final) * multiplier);
        e_final = static_cast<ll>(static_cast<double>(e_final) * multiplier);
        c_final = static_cast<ll>(static_cast<double>(c_final) * multiplier);
    }
    ojson st = ojson::object();
    st["concentration"] = c_final;
    st["expression"] = e_final;
    st["vocal"] = v_final;
    st["total_status"] = c_final + e_final + v_final;
    return st;
}

ojson party_composition(const std::vector<djson>& slots,
                        const std::unordered_map<ll, djson>& chars,
                        const std::unordered_map<ll, const ojson*>& character_master,
                        const std::unordered_map<ll, const ojson*>& sense_master) {
    const auto& cbm_master = by_id("CharacterBaseMaster");
    std::vector<ojson> members;
    for (const djson& slot : slots) {
        const djson* ch = dget(chars, jint(slot, "characterId", 0));
        const ojson* cm = ch ? mget(character_master, jint(*ch, "characterMasterId", 0)) : nullptr;
        if (cm == nullptr) continue;
        const ojson* cbm = mget(cbm_master, jint(*cm, "character_base_master_id", 0));
        const ojson* sm = mget(sense_master, jint(*cm, "sense_master_id", 0));
        ojson member = ojson::object();
        member["character_master_id"] = jint(*ch, "characterMasterId", 0);
        member["character_base_master_id"] = jint(*cm, "character_base_master_id", 0);
        member["company"] = cbm ? jint(*cbm, "company_master_id", 0) : 0;
        member["attribute"] = jint(*cm, "attribute", 0);
        member["sense_type"] = sm ? jint(*sm, "type", 0) : 0;
        auto pit = slot.find("posterId");
        if (pit != slot.end() && pit->is_number_integer())
            member["poster_id"] = pit->get<ll>();
        else
            member["poster_id"] = nullptr;
        members.push_back(std::move(member));
    }
    return effects::party_composition(members);
}

// position -> firing-sense info (sense id/master/cool_time/lights, incl. effect-added lights).
struct PosSense {
    ll sense_master_id = 0;
    ll sense_id = 0;
    ll cool_time = 0;
    std::vector<ll> acquirable_lights;
};

std::map<ll, PosSense> position_senses(
    const std::vector<djson>& slots, const std::unordered_map<ll, djson>& chars,
    const std::unordered_map<ll, const ojson*>& character_master,
    const std::unordered_map<ll, const ojson*>& sense_master,
    const std::unordered_map<ll, const ojson*>& star_act_master,
    const std::unordered_map<ll, ojson>& accessories,
    const std::unordered_map<ll, ojson>& posters, const std::optional<ojson>& comp) {
    std::map<ll, PosSense> pos_sense;
    for (const djson& slot : slots) {
        const djson* ch = dget(chars, jint(slot, "characterId", 0));
        if (ch == nullptr) continue;
        const ojson* cm = mget(character_master, jint(*ch, "characterMasterId", 0));
        const ojson* sm = cm ? mget(sense_master, jint(*cm, "sense_master_id", 0)) : nullptr;
        if (cm == nullptr || sm == nullptr) continue;
        const ojson* sam = cm ? mget(star_act_master, jint(*cm, "star_act_master_id", 0)) : nullptr;
        auto effs = effects::collect_slot_effects(
            opt_master(sm), opt_master(sam), jint(*ch, "senseLevel", 0),
            acc_opt(accessories, slot, "accessoryId"), acc_opt(posters, slot, "posterId"), comp,
            jint(slot, "bonusAbilityEnableFlags", 0));
        bool cancel = has_cancel_sense(effs);
        ll stype = jint(*sm, "type", 0);
        PosSense ps;
        ps.sense_master_id = jint(*cm, "sense_master_id", 0);
        ps.sense_id = jint(*ch, "id", 0) * 100;
        ps.cool_time = cancel ? 1 : jint(*sm, "cool_time", 0);
        if (!cancel) {
            ps.acquirable_lights = lights_of(stype, jint(*sm, "light_count", 0));
            auto extra = effects::added_lights(effs, stype);
            ps.acquirable_lights.insert(ps.acquirable_lights.end(), extra.begin(), extra.end());
        }
        pos_sense[jint(slot, "position", 0)] = std::move(ps);
    }
    return pos_sense;
}

// StarActConditionMaster light fields -> SenseLightTypes, in python's _LIGHT_FIELDS order.
const std::pair<ll, const char*> kLightFields[] = {
    {enums::SenseLightTypes::Support, "support_light"},
    {enums::SenseLightTypes::Control, "control_light"},
    {enums::SenseLightTypes::Amplification, "amplification_light"},
    {enums::SenseLightTypes::Special, "special_light"},
};

// (typed light requirements, free-light count) for the leader's star act, with
// DecreaseRequire*Light reductions applied.
std::pair<std::map<ll, ll>, ll> star_act_condition(
    const std::vector<djson>& slots, const std::unordered_map<ll, djson>& chars,
    const std::unordered_map<ll, const ojson*>& character_master, ll leader_position,
    const std::vector<effects::AppliedEffect>& leader_effects) {
    const djson* leader = nullptr;
    for (const djson& s : slots)
        if (jint(s, "position", 0) == leader_position) {
            leader = &s;
            break;
        }
    if (leader == nullptr) {
        for (const djson& s : slots)
            if (leader == nullptr || jint(s, "position", 0) < jint(*leader, "position", 0)) leader = &s;
    }
    const djson* ch = leader ? dget(chars, jint(*leader, "characterId", 0)) : nullptr;
    const ojson* cm = ch ? mget(character_master, jint(*ch, "characterMasterId", 0)) : nullptr;
    const ojson* sam = cm ? mget(by_id("StarActMaster"), jint(*cm, "star_act_master_id", 0)) : nullptr;
    const ojson* cond =
        sam ? mget(by_id("StarActConditionMaster"), jint(*sam, "star_act_condition_master_id", 0))
            : nullptr;
    if (cond == nullptr) return {{}, 0};
    std::map<ll, ll> reductions = effects::decrease_require_lights(leader_effects);
    std::map<ll, ll> typed;
    for (const auto& lf : kLightFields) {
        ll base_req = jint(*cond, lf.second, 0);
        if (base_req) {
            auto rit = reductions.find(lf.first);
            ll red = rit == reductions.end() ? 0 : rit->second;
            typed[lf.first] = std::max<ll>(0, base_req - red);
        }
    }
    return {typed, jint(*cond, "free_light", 0)};
}

// the light pool + star-act trigger evaluation. a required-color light fills that color's slot,
// an off-color light fills a free slot, a Variable light is a wildcard; reaching the required
// total fires the star act and consumes all collected lights.
struct SenseLightsManager {
    std::vector<ll> lights;
    std::map<ll, ll> fixed;
    ll free = 0;
    std::map<ll, ll> required;
    ll required_free;
    ll required_count;

    SenseLightsManager(std::map<ll, ll> req, ll req_free)
        : required(std::move(req)), required_free(req_free) {
        ll s = 0;
        for (const auto& kv : required) {
            fixed[kv.first] = 0;
            s += kv.second;
        }
        required_count = s + required_free;
    }

    void clear() {
        for (auto& kv : fixed) kv.second = 0;
        lights.clear();
        free = 0;
    }

    std::pair<bool, std::vector<ll>> grant(const std::vector<ll>& incoming) {
        std::vector<ll> added;
        bool fired = false;
        for (ll light : incoming) {
            if (light == enums::SenseLightTypes::Variable) {
                lights.push_back(light);
                added.push_back(light);
            } else {
                auto rit = required.find(light);
                ll reqv = rit == required.end() ? 0 : rit->second;
                auto fit = fixed.find(light);
                ll fixv = fit == fixed.end() ? 0 : fit->second;
                if (reqv > fixv) {
                    lights.push_back(light);
                    fixed[light] += 1;
                    added.push_back(light);
                } else if (free < required_free) {
                    lights.push_back(light);
                    free += 1;
                    added.push_back(light);
                } else {
                    continue;
                }
            }
            if (required_count && static_cast<ll>(lights.size()) >= required_count) {
                clear();
                fired = true;
                break;
            }
        }
        return {fired, added};
    }
};

ojson make_timing_event(const std::vector<ll>& total, const std::vector<ll>& grant,
                        bool is_star_act, bool lost, const std::vector<ll>& acquirable,
                        const std::vector<ll>& sense_ids, ll sense_master_id) {
    ojson ev = ojson::object();
    ev["total_sense_lights"] = total;
    ev["grant_sense_lights"] = grant;
    ev["is_star_act"] = is_star_act;
    ev["lost_lights"] = lost;
    ev["acquirable_lights"] = acquirable;
    ev["sense_ids"] = sense_ids;
    ev["sense_voice_ids"] = ojson::array();
    ev["sense_master_id"] = sense_master_id;
    return ev;
}

struct SenseTimingEvent {
    ll timing_seconds;
    ll position;
    ojson event;
};

struct LiveTimeEventData {
    std::vector<SenseTimingEvent> timings;
    std::vector<std::pair<ll, ll>> cool_times;  // (position, cool_time)
};

LiveTimeEventData live_time_event(const std::map<ll, PosSense>& pos_sense, ll music_time_second,
                                  const std::optional<ojson>& sense_notation,
                                  const std::pair<std::map<ll, ll>, ll>& condition,
                                  const std::vector<ll>& initial_lights) {
    const std::map<ll, ll>& required = condition.first;
    ll required_free = condition.second;

    std::vector<std::pair<ll, ll>> windows;  // (timing_second, position)
    if (sense_notation.has_value()) {
        auto dit = sense_notation->find("details");
        if (dit != sense_notation->end() && dit->is_array())
            for (const ojson& d : *dit)
                windows.emplace_back(jint(d, "timing_second", 0), jint(d, "position", 0));
    } else {
        // normal live: 8 windows evenly spread, firing in formation order 1-2-3-4-5-3-2-1.
        ll step = floordiv(music_time_second, 9);
        static const ll formation[8] = {1, 2, 3, 4, 5, 3, 2, 1};
        for (int i = 1; i <= 8; ++i) windows.emplace_back(step * i, formation[i - 1]);
    }
    std::stable_sort(windows.begin(), windows.end());

    SenseLightsManager mgr(required, required_free);
    std::map<ll, ll> last_act;
    LiveTimeEventData out;

    // opening window (timing_seconds=0): grant initial StartLive lights, evaluate opening star act.
    if (!initial_lights.empty()) {
        auto res = mgr.grant(initial_lights);
        ojson ev = make_timing_event(mgr.lights, res.first ? res.second : std::vector<ll>{}, res.first,
                                     false, initial_lights, {}, 0);
        out.timings.push_back({0, 0, std::move(ev)});
    }

    for (const auto& w : windows) {
        ll sec = w.first, pos = w.second;
        auto sit = pos_sense.find(pos);
        const PosSense* sense = sit == pos_sense.end() ? nullptr : &sit->second;
        auto lit = last_act.find(pos);
        bool have_prev = lit != last_act.end();
        ll cool = sense ? sense->cool_time : 0;
        // a window whose sense is still on cool_time can't fire -> collected lights lost.
        bool lost = sense == nullptr || (have_prev && (sec - lit->second) < cool);
        bool fired = false;
        std::vector<ll> added;
        if (lost) {
            mgr.clear();
        } else {
            last_act[pos] = sec;
            auto res = mgr.grant(sense->acquirable_lights);
            fired = res.first;
            added = res.second;
        }
        ojson ev = make_timing_event(mgr.lights, added, fired, lost,
                                     sense ? sense->acquirable_lights : std::vector<ll>{},
                                     sense ? std::vector<ll>{sense->sense_id} : std::vector<ll>{},
                                     sense ? sense->sense_master_id : 0);
        out.timings.push_back({sec, pos, std::move(ev)});
    }
    for (const auto& kv : pos_sense) out.cool_times.emplace_back(kv.first, kv.second.cool_time);
    return out;
}

ojson live_time_event_json(const LiveTimeEventData& d) {
    ojson timings = ojson::array();
    for (const SenseTimingEvent& t : d.timings) {
        ojson ste = ojson::object();
        ste["timing_seconds"] = t.timing_seconds;
        ste["position"] = t.position;
        ste["event"] = t.event;
        timings.push_back(std::move(ste));
    }
    ojson cool_times = ojson::array();
    for (const auto& c : d.cool_times) {
        ojson sc = ojson::object();
        sc["position"] = c.first;
        sc["cool_time"] = c.second;
        cool_times.push_back(std::move(sc));
    }
    ojson out = ojson::object();
    out["timings"] = std::move(timings);
    out["cool_times"] = std::move(cool_times);
    return out;
}

ll random_live_id() {
    static thread_local std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<ll> dist(1000000LL, 9999999999LL);
    return dist(rng);
}

}  // namespace

wire::json build_live_time_event(long long user_id, long long party_id, long long music_master_id,
                                 std::optional<long long> sense_notation_master_id) {
    std::vector<djson> slots;
    for (djson& s : db::fetch(db::user::get_party_slots(user_id)))
        if (jint(s, "partyId", 0) == party_id) slots.push_back(std::move(s));
    std::unordered_map<ll, djson> chars = index_db(db::fetch(db::user::get_characters(user_id)), "id");
    std::unordered_map<ll, ojson> accessories =
        index_ord(db::fetch(db::user::get_accessorys(user_id)), "id");
    std::unordered_map<ll, ojson> posters = index_ord(db::fetch(db::user::get_posters(user_id)), "id");
    ll leader_position = 1;
    for (djson& p : db::fetch(db::user::get_partys(user_id)))
        if (jint(p, "id", 0) == party_id) {
            leader_position = jint(p, "leaderPosition", 1);
            break;
        }
    const auto& character_master = by_id("CharacterMaster");
    const auto& sense_master = by_id("SenseMaster");
    ojson comp = party_composition(slots, chars, character_master, sense_master);
    std::optional<ojson> comp_opt = comp;
    std::map<ll, PosSense> pos_sense = position_senses(slots, chars, character_master, sense_master,
                                                       by_id("StarActMaster"), accessories, posters,
                                                       comp_opt);
    const ojson* music = mget(by_id("MusicMaster"), music_master_id);
    ll duration = music ? jint(*music, "music_time_second", 0) : 0;
    const ojson* snm = nullptr;
    if (sense_notation_master_id.has_value() && *sense_notation_master_id != 0)
        snm = mget(by_id("SenseNotationMaster"), *sense_notation_master_id);

    std::vector<ll> initial_lights;
    std::vector<effects::AppliedEffect> leader_effects;
    for (const djson& s : slots) {
        const djson* ch = dget(chars, jint(s, "characterId", 0));
        const ojson* cm = ch ? mget(character_master, jint(*ch, "characterMasterId", 0)) : nullptr;
        const ojson* sm = cm ? mget(sense_master, jint(*cm, "sense_master_id", 0)) : nullptr;
        const ojson* sam = cm ? mget(by_id("StarActMaster"), jint(*cm, "star_act_master_id", 0)) : nullptr;
        bool is_leader = jint(s, "position", 0) == leader_position;
        auto effs = effects::collect_slot_effects(
            opt_master(sm), opt_master(sam), ch ? jint(*ch, "senseLevel", 0) : 0,
            acc_opt(accessories, s, "accessoryId"), acc_opt(posters, s, "posterId"), comp_opt,
            jint(s, "bonusAbilityEnableFlags", 0), is_leader);
        if (cm && ch) {
            auto bl = effects::bloom_effects(jint(*cm, "bloom_bonus_group_master_id", 0),
                                             jint(*ch, "talentStage", 0), comp_opt);
            effs.insert(effs.end(), bl.begin(), bl.end());
        }
        if (is_leader) leader_effects = effs;
        auto sl = effects::start_lights(effs, sm ? jint(*sm, "type", 0) : 0);
        initial_lights.insert(initial_lights.end(), sl.begin(), sl.end());
    }
    if (initial_lights.size() > 10) initial_lights.resize(10);
    auto condition = star_act_condition(slots, chars, character_master, leader_position, leader_effects);
    LiveTimeEventData lte = live_time_event(pos_sense, duration, opt_master(snm), condition,
                                            initial_lights);
    return live_time_event_json(lte);
}

std::pair<wire::json, long long> build_live_unit(long long user_id, long long party_id,
                                                 long long live_master_id) {
    std::vector<djson> slots;
    for (djson& s : db::fetch(db::user::get_party_slots(user_id)))
        if (jint(s, "partyId", 0) == party_id) slots.push_back(std::move(s));
    std::stable_sort(slots.begin(), slots.end(), [](const djson& a, const djson& b) {
        return jint(a, "position", 0) < jint(b, "position", 0);
    });
    std::unordered_map<ll, djson> chars = index_db(db::fetch(db::user::get_characters(user_id)), "id");
    std::unordered_map<ll, djson> bases = index_db(db::fetch(db::user::get_character_bases(user_id)), "id");
    std::unordered_map<ll, ojson> accessories =
        index_ord(db::fetch(db::user::get_accessorys(user_id)), "id");
    std::unordered_map<ll, ojson> posters = index_ord(db::fetch(db::user::get_posters(user_id)), "id");
    ll album_level = 0;
    for (djson& a : db::fetch(db::user::get_albums(user_id))) {
        album_level = jint(a, "level", 0);
        break;
    }
    std::map<ll, ll> circle_levels;
    for (djson& cs : db::fetch(db::user::get_circle_supports(user_id)))
        circle_levels[jint(cs, "company", 0)] = jint(cs, "level", 0);
    const auto& character_master = by_id("CharacterMaster");
    const auto& sense_master = by_id("SenseMaster");
    const auto& star_act_master = by_id("StarActMaster");
    ll leader_position = 1;
    for (djson& p : db::fetch(db::user::get_partys(user_id)))
        if (jint(p, "id", 0) == party_id) {
            leader_position = jint(p, "leaderPosition", 1);
            break;
        }
    const djson* leader_slot = nullptr;
    for (const djson& s : slots)
        if (jint(s, "position", 0) == leader_position) {
            leader_slot = &s;
            break;
        }
    const djson* leader_ch = leader_slot ? dget(chars, jint(*leader_slot, "characterId", 0)) : nullptr;
    const ojson* leader_cm =
        leader_ch ? mget(character_master, jint(*leader_ch, "characterMasterId", 0)) : nullptr;
    ojson comp = party_composition(slots, chars, character_master, sense_master);
    std::optional<ojson> comp_opt = comp;

    // pass 1: collect each slot's full effects, aggregate the party's All-range effects.
    std::map<ll, std::vector<effects::AppliedEffect>> slot_effects;
    std::vector<effects::AppliedEffect> party_all = effects::album_effects(album_level, comp_opt);
    for (const djson& slot : slots) {
        ll position = jint(slot, "position", 0);
        const djson* ch = dget(chars, jint(slot, "characterId", 0));
        const ojson* cm = ch ? mget(character_master, jint(*ch, "characterMasterId", 0)) : nullptr;
        const ojson* sm = cm ? mget(sense_master, jint(*cm, "sense_master_id", 0)) : nullptr;
        const ojson* sam = cm ? mget(star_act_master, jint(*cm, "star_act_master_id", 0)) : nullptr;
        auto eff = effects::collect_slot_effects(
            opt_master(sm), opt_master(sam), ch ? jint(*ch, "senseLevel", 0) : 0,
            acc_opt(accessories, slot, "accessoryId"), acc_opt(posters, slot, "posterId"), comp_opt,
            jint(slot, "bonusAbilityEnableFlags", 0), position == leader_position);
        if (cm && ch) {
            auto bl = effects::bloom_effects(jint(*cm, "bloom_bonus_group_master_id", 0),
                                             jint(*ch, "talentStage", 0), comp_opt);
            eff.insert(eff.end(), bl.begin(), bl.end());
        }
        if (leader_cm && cm) {
            auto ls = effects::leader_sense_effects(opt_master(leader_cm), opt_master(cm));
            eff.insert(eff.end(), ls.begin(), ls.end());
        }
        auto all_range = effects::range_split(eff).second;
        party_all.insert(party_all.end(), all_range.begin(), all_range.end());
        slot_effects[position] = std::move(eff);
    }

    // MaxPrincipal = base + party-wide All-range + every slot's own PrincipalGaugeLimitUp effects.
    std::vector<effects::AppliedEffect> principal_list = party_all;
    for (const auto& kv : slot_effects) {  // slot_effects.values(): once per position
        auto self_range = effects::range_split(kv.second).first;
        principal_list.insert(principal_list.end(), self_range.begin(), self_range.end());
    }
    ll principal_max = effects::max_principal(principal_list);

    std::map<ll, ojson> actors;
    ojson senses = ojson::array();
    ojson start_effects_all = ojson::array();
    ojson star_act = ojson::object();  // default StarAct()
    star_act["effect_branches"] = ojson::array();
    for (const djson& slot : slots) {
        ll position = jint(slot, "position", 0);
        const djson* ch = dget(chars, jint(slot, "characterId", 0));
        if (ch == nullptr) continue;
        const djson* base = dget(bases, jint(*ch, "characterBaseId", 0));
        const djson* sec = nullptr;
        {
            auto sit = ch->find("secondaryCharacterBaseId");
            if (sit != ch->end() && sit->is_number_integer() && sit->get<ll>() != 0)
                sec = dget(bases, sit->get<ll>());
        }
        const ojson* cm = mget(character_master, jint(*ch, "characterMasterId", 0));
        const ojson* sm = cm ? mget(sense_master, jint(*cm, "sense_master_id", 0)) : nullptr;
        const ojson* sam = cm ? mget(star_act_master, jint(*cm, "star_act_master_id", 0)) : nullptr;
        const std::vector<effects::AppliedEffect>& full = slot_effects[position];
        const ojson* cbm =
            cm ? mget(by_id("CharacterBaseMaster"), jint(*cm, "character_base_master_id", 0)) : nullptr;
        ojson ch_dict = ojson::object();
        ch_dict["character_master_id"] = cm ? jint(*cm, "id_", 0) : 0;
        ch_dict["character_base_master_id"] = cbm ? jint(*cbm, "id_", 0) : 0;
        ch_dict["company"] = cbm ? jint(*cbm, "company_master_id", 0) : 0;
        ch_dict["attribute"] = cm ? jint(*cm, "attribute", 0) : 0;
        ch_dict["sense_type"] = sm ? jint(*sm, "type", 0) : 0;
        auto matched_party = effects::filter_target_effects(party_all, ch_dict);
        auto matched_own = effects::filter_target_effects(effects::range_split(full).first, ch_dict);
        std::vector<effects::AppliedEffect> circle;
        if (cbm) circle = effects::circle_effects(jint(*cbm, "company_master_id", 0), circle_levels, comp_opt);
        std::vector<effects::AppliedEffect> eff = matched_own;
        eff.insert(eff.end(), matched_party.begin(), matched_party.end());
        eff.insert(eff.end(), circle.begin(), circle.end());

        // intrinsic (percent points): awakening phase (10%/phase) + star rank bonus + BaseCorrection.
        double intrinsic = static_cast<double>(jint(*ch, "awakeningPhase", 0)) * 10.0 +
                           star_rank_bonus(base ? jint(*base, "starRank", 0) : 0) +
                           effects::base_correction(eff);
        ll reo = jint(*ch, "readEpisodeOrder", 0);
        ll story_bonus = (reo >= 1 ? 2 : 0) + (reo >= 2 ? 3 : 0);
        auto cbs = character_base_status(cm, jint(*ch, "level", 0), effects::base_stat_bonus(eff),
                                         story_bonus, intrinsic);
        double mult = effects::final_performance_multiplier(eff);
        ojson current_status = calculate_slot_status(
            cbs, effects::component_flat_bonus(eff), effects::component_percent_bonuses_by_source(eff),
            effects::performance_percent_bonuses_by_source(eff), mult);

        ojson actor = ojson::object();
        actor["id_"] = jint(*ch, "id", 0);
        actor["character_base_master_id"] = base ? jint(*base, "characterBaseMasterId", 0) : 0;
        actor["character_master_id"] = jint(*ch, "characterMasterId", 0);
        actor["awakening_phase"] = jint(*ch, "awakeningPhase", 0);
        actor["talent_stage"] = jint(*ch, "talentStage", 0);
        actor["position"] = position;
        actor["sense_level"] = jint(*ch, "senseLevel", 0);
        actor["base_status"] = current_status;
        actor["current_status"] = current_status;
        actor["display_awakening_status"] = jbool(*ch, "displayAwakeningStatus", false);
        if (sec)
            actor["secondary_character_base_master_id"] = jint(*sec, "characterBaseMasterId", 0);
        else
            actor["secondary_character_base_master_id"] = nullptr;
        actor["secondary_sense_level"] = jint(*ch, "secondarySenseLevel", 0);
        actor["selection_type"] = jint(*ch, "selectionType", 0);
        actors[position] = std::move(actor);

        bool cancel = has_cancel_sense(eff);
        if (cm && sm) {
            ojson sense_effect = ojson::object();
            sense_effect["score_factor"] = cancel ? 0 : jint(*sm, "acquirable_score_percent", 0);
            sense_effect["principal"] = 0;
            sense_effect["branch_condition"] = jint(*sm, "branch_condition1", 0);
            sense_effect["effect_branches"] = ojson::array();
            ojson sense = ojson::object();
            sense["id_"] = jint(*ch, "id", 0) * 100;
            sense["actor_id"] = jint(*ch, "id", 0);
            sense["cool_time"] = cancel ? 1 : jint(*sm, "cool_time", 0);
            sense["sense_type"] = jint(*sm, "type", 0);
            if (cancel) {
                sense["acquirable_lights"] = ojson::array();
            } else {
                auto lights = lights_of(jint(*sm, "type", 0), jint(*sm, "light_count", 0));
                auto extra = effects::added_lights(eff, jint(*sm, "type", 0));
                lights.insert(lights.end(), extra.begin(), extra.end());
                sense["acquirable_lights"] = lights;
            }
            // non-Optional list fields default to [] in python (model default_factory);
            // generated keys mark them nullable so a missing field would encode as nil.
            sense["pre_sense_effect"] = ojson::array();
            sense["sense_effect"] = std::move(sense_effect);
            sense["poster_effect"] = ojson::array();
            sense["accessory_effect"] = ojson::array();
            sense["sense_master_id"] = jint(*cm, "sense_master_id", 0);
            sense["original_actor_id"] = jint(*ch, "id", 0);
            senses.push_back(std::move(sense));
        }
        // start effects emitted once from their source slot.
        auto se = effects::start_effects(full, jint(*ch, "id", 0),
                                         static_cast<ll>(start_effects_all.size()));
        for (auto& e : se) start_effects_all.push_back(std::move(e));
        if (position == leader_position && sam) {  // leader drives the star act
            star_act = ojson::object();
            star_act["score_factor"] = jint(*sam, "acquirable_score_percent", 0);
            star_act["actor_id"] = jint(*ch, "id", 0);
            star_act["branch_condition"] = jint(*sam, "branch_condition1", 0);
            star_act["effect_branches"] = ojson::array();
        }
    }

    ll total_status = 0;
    for (const auto& kv : actors) total_status += kv.second.at("current_status").at("total_status").get<ll>();

    const ojson* lm = mget(by_id("LiveMaster"), live_master_id);
    bool is_first_olivier = false;
    if (lm && jint(*lm, "difficulty", 0) == enums::MusicDifficulties::Olivier) {
        ll target = jint(*lm, "music_master_id", 0);
        for (const djson& m : db::fetch(db::user::get_musics(user_id)))
            if (jint(m, "musicMasterId", 0) == target) {
                is_first_olivier =
                    jint(m, "olivierReleaseStatus", 0) == enums::OlivierReleaseStatuses::Challengeable;
                break;
            }
    }

    // normal live: time events spread over the song duration (no sense notation).
    const ojson* music = lm ? mget(by_id("MusicMaster"), jint(*lm, "music_master_id", 0)) : nullptr;
    std::map<ll, PosSense> pos_sense = position_senses(slots, chars, character_master, sense_master,
                                                       star_act_master, accessories, posters, comp_opt);
    std::vector<effects::AppliedEffect> leader_effects;
    {
        auto it = slot_effects.find(leader_position);
        if (it != slot_effects.end()) leader_effects = it->second;
    }
    auto condition = star_act_condition(slots, chars, character_master, leader_position, leader_effects);
    std::vector<ll> initial_lights;
    for (const djson& slot : slots) {
        const std::vector<effects::AppliedEffect>& full = slot_effects[jint(slot, "position", 0)];
        const djson* ch = dget(chars, jint(slot, "characterId", 0));
        const ojson* cm = ch ? mget(character_master, jint(*ch, "characterMasterId", 0)) : nullptr;
        const ojson* sm = cm ? mget(sense_master, jint(*cm, "sense_master_id", 0)) : nullptr;
        auto sl = effects::start_lights(full, sm ? jint(*sm, "type", 0) : 0);
        initial_lights.insert(initial_lights.end(), sl.begin(), sl.end());
    }
    if (initial_lights.size() > 10) initial_lights.resize(10);

    LiveTimeEventData lte = live_time_event(pos_sense, music ? jint(*music, "music_time_second", 0) : 0,
                                            std::nullopt, condition, initial_lights);
    std::map<ll, ojson> time_events;  // keyed by timing second; last window per second wins
    for (const SenseTimingEvent& t : lte.timings) time_events[t.timing_seconds] = t.event;

    // lights needed for a star act = the leader's typed requirements + free lights.
    ll star_act_sense_light_count = condition.second;
    for (const auto& kv : condition.first) star_act_sense_light_count += kv.second;

    ll live_id = random_live_id();

    ojson actors_obj = ojson::object();
    for (const auto& kv : actors) actors_obj[std::to_string(kv.first)] = kv.second;
    ojson time_events_obj = ojson::object();
    for (const auto& kv : time_events) time_events_obj[std::to_string(kv.first)] = kv.second;

    ojson unit = ojson::object();
    unit["actors"] = std::move(actors_obj);
    unit["time_events"] = std::move(time_events_obj);
    unit["possible_senses"] = std::move(senses);
    unit["start_effects"] = std::move(start_effects_all);
    unit["star_act"] = std::move(star_act);
    unit["total_status"] = total_status;
    unit["star_act_sense_light_count"] = star_act_sense_light_count;
    unit["max_principal"] = principal_max;
    // base_score_difficulty_auto_coefficient: per-difficulty, server-set (tutorial 0.95).
    unit["base_score_difficulty_auto_coefficient"] = 0.95;
    unit["is_first_play_olivier"] = is_first_olivier;
    unit["u_active_live_id"] = live_id;
    return {std::move(unit), live_id};
}

}  // namespace live
