#include "helpers/progression.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "config.h"
#include "db.h"
#include "helpers/daily.h"
#include "helpers/game_state.h"
#include "helpers/live_result.h"
#include "helpers/stamina.h"
#include "master_data.h"

// ports helpers/progression.py. lesson/course lifecycles + player-rank XP +
// daily-usage accounting. lesson star points and rank XP per stamina are
// explicit policy constants (constants.yml), not reverse engineering.

namespace progression {

using gjson = game_state::json;    // db rows (camelCase columns, plain json)
using ojson = game_state::ojson;   // master rows (ordered json)

namespace {

constexpr int64_t kMicro = 1'000'000;

// time.time_ns() // 1000 (epoch micros)
int64_t now_micros() { return static_cast<int64_t>(std::time(nullptr)) * kMicro; }

// python round(x, 4): nearest 1e-4, ties to even (FE_TONEAREST)
double round4(double x) { return std::nearbyint(x * 10000.0) / 10000.0; }

// python str(float): shortest round-tripping repr, fixed notation for this
// field's magnitudes, always carrying a decimal point.
std::string py_float_str(double v) {
    if (std::isnan(v)) return "nan";
    if (std::isinf(v)) return v < 0 ? "-inf" : "inf";
    char buf[64];
    int prec = 17;
    for (int p = 1; p <= 17; ++p) {
        std::snprintf(buf, sizeof(buf), "%.*g", p, v);
        if (std::strtod(buf, nullptr) == v) {
            prec = p;
            break;
        }
    }
    std::string s(buf);
    // %g may pick scientific notation; python repr stays fixed in this range
    if (s.find('e') != std::string::npos || s.find('E') != std::string::npos) {
        for (int dp = 0; dp <= 17; ++dp) {
            std::snprintf(buf, sizeof(buf), "%.*f", dp, v);
            if (std::strtod(buf, nullptr) == v) {
                s = buf;
                break;
            }
        }
    }
    if (s.find('.') == std::string::npos) s += ".0";
    return s;
}

// python truthiness for a json value
bool is_truthy(const gjson& v) {
    if (v.is_null()) return false;
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number_integer()) return v.get<int64_t>() != 0;
    if (v.is_number_unsigned()) return v.get<uint64_t>() != 0;
    if (v.is_number_float()) return v.get<double>() != 0.0;
    if (v.is_string()) return !v.get<std::string>().empty();
    if (v.is_array() || v.is_object()) return !v.empty();
    return false;
}

// a payload list attribute by reference, null when absent (python `obj.attr`)
const json& payload_member(const json& p, const char* key) {
    static const json kNull = json(nullptr);
    if (p.is_object()) {
        auto it = p.find(key);
        if (it != p.end()) return *it;
    }
    return kNull;
}

// self.dirty[(entity, pk)] = value -- replace in place (keeping position) or append
void dirty_set(game_state::State& s, const std::string& entity, int64_t pk, const gjson& value) {
    for (auto& e : s.dirty)
        if (e.first.first == entity && e.first.second == pk) {
            e.second = value;
            return;
        }
    s.dirty.emplace_back(std::make_pair(entity, pk), value);
}

}  // namespace

const nlohmann::json& rules() { return constants::raw(); }

json daily(game_state::State& s, std::optional<int64_t> now) {
    int64_t at = now.has_value() ? *now : now_micros();
    gjson* row = s.one("DailyLimit");
    gjson zero = {
        {"autoPlayTimes", 0},
        {"dailyLessonTimes", 0},
        {"musicCourseFreeChallengeTimes", 0},
        {"lastRefreshedAt", at},
    };
    if (row == nullptr) return s.insert("DailyLimit", zero);
    int64_t last = 0;
    auto it = row->find("lastRefreshedAt");
    if (it != row->end() && !it->is_null()) last = it->get<int64_t>();
    if (last < daily::most_recent_reset(at)) s.update("DailyLimit", row, zero);
    return *row;
}

void use_daily(game_state::State& s, const std::string& field) {
    json row = daily(s);
    if (!rules().at("unlimited_attempts").get<bool>()) {
        int64_t next = row.at(field).get<int64_t>() + 1;
        gjson* cached = s.one("DailyLimit");
        s.update("DailyLimit", cached, gjson{{field, next}});
    }
}

void context(game_state::State& s, const std::string& mode, long long ident, const json& extra) {
    db::execute(db::ExecutableQuery(
        "DELETE FROM preservation_live_context WHERE \"userId\"=$1", s.uid));
    db::execute(db::ExecutableQuery(
        "INSERT INTO preservation_live_context (\"userId\",mode,\"masterId\",extra) "
        "VALUES ($1,$2,$3,$4)",
        s.uid, mode, ident, extra));
}

std::vector<Slot> lesson_slots(const json& row) {
    std::vector<Slot> out;
    auto it = row.find("setCharacters");
    if (it == row.end() || !it->is_array()) return out;
    for (const json& x : *it) {
        if (x.is_object()) {
            long long position = x.at("position").get<long long>();
            std::optional<long long> cid;
            auto cit = x.find("setCharacterId");
            if (cit != x.end() && !cit->is_null()) cid = cit->get<long long>();
            out.emplace_back(position, cid);
        } else {
            long long position = x.at(0).get<long long>();
            std::optional<long long> cid;
            if (x.size() > 1 && !x.at(1).is_null()) cid = x.at(1).get<long long>();
            out.emplace_back(position, cid);
        }
    }
    return out;
}

json stored_slots(const std::vector<Slot>& slots) {
    std::vector<Slot> ordered(slots);
    std::sort(ordered.begin(), ordered.end());
    json out = json::array();
    for (const Slot& slot : ordered) {
        json entry = json::object();
        entry["position"] = slot.first;
        entry["setCharacterId"] = slot.second.has_value() ? json(*slot.second) : json(nullptr);
        out.push_back(entry);
    }
    return out;
}

json lesson_party(game_state::State& s, long long base) {
    if (s.one("CharacterBase", gjson{{"characterBaseMasterId", base}}) == nullptr)
        throw game_state::Rejected("Character not owned");
    gjson* existing = s.one("CharacterLesson", gjson{{"characterBaseMasterId", base}});
    if (existing != nullptr) return *existing;
    std::vector<gjson> owned;
    for (const gjson& r : s.rows("Character")) {
        long long cmid = r.at("characterMasterId").get<long long>();
        const ojson* cm = game_state::master("character_master", cmid);
        if (cm->at("character_base_master_id").get<long long>() == base) owned.push_back(r);
    }
    // reverse sort by (level, awakeningPhase, id); stable keeps equal-key order
    std::stable_sort(owned.begin(), owned.end(), [](const gjson& a, const gjson& b) {
        auto key = [](const gjson& c) {
            return std::make_tuple(c.at("level").get<long long>(),
                                   c.at("awakeningPhase").get<long long>(),
                                   c.at("id").get<long long>());
        };
        return key(a) > key(b);
    });
    if (owned.empty()) throw game_state::Rejected("No lesson actors");
    std::vector<Slot> slots;
    for (int i = 0; i < 5; ++i) {
        std::optional<long long> cid;
        if (i < static_cast<int>(owned.size())) cid = owned[i].at("id").get<long long>();
        slots.emplace_back(static_cast<long long>(i + 1), cid);
    }
    gjson values = {
        {"characterBaseMasterId", base},
        {"setCharacters", stored_slots(slots)},
        {"bestScore", 0},
        {"leaderPosition", 0},
        {"rewardReceivedHighScore", 0},
    };
    return s.insert("CharacterLesson", values);
}

json rank_xp(game_state::State& s, long long amount) {
    gjson* row = s.one("User");
    long long before = row->at("playerRank").get<long long>();
    long long old = row->at("currentRankPoint").get<long long>();
    long long stamina = row->at("currentStamina").get<long long>();
    long long rank = before;
    long long balance = old + amount;
    std::unordered_map<long long, const ojson*> levels;
    long long max_released = 0;
    bool have_released = false;
    for (const ojson& x : master_data::table("PlayerRankMaster")) {
        levels[x.at("rank").get<long long>()] = &x;
        if (x.at("is_released_rank").get<bool>()) {
            long long r = x.at("rank").get<long long>();
            if (!have_released || r > max_released) {
                max_released = r;
                have_released = true;
            }
        }
    }
    long long cap = std::min(row->at("playerRankLimit").get<long long>(), max_released);
    long long restore = 0;
    while (rank < cap) {
        auto mit = levels.find(rank);
        const ojson* m = mit != levels.end() ? mit->second : nullptr;
        if (m == nullptr || m->at("point_to_level_up").get<long long>() <= 0 ||
            balance < m->at("point_to_level_up").get<long long>() ||
            levels.find(rank + 1) == levels.end())
            break;
        balance -= m->at("point_to_level_up").get<long long>();
        rank += 1;
        restore += stamina::max_stamina(rank);
    }
    s.update("User", row, gjson{{"playerRank", rank}, {"currentRankPoint", balance}});
    if (restore) {
        stamina::adjust_and_check_stamina(s.uid, restore, rank);
        s.tables.erase("User");
        row = s.one("User");
        dirty_set(s, "User", row->at("id").get<int64_t>(), *row);
    }
    if (rank != before)
        for (long long ident : {static_cast<long long>(1000), static_cast<long long>(1100)})
            game_state::mission_progress(s, ident, 1, false, rank);
    json result;
    result["rank_before"] = before;
    result["rank_after"] = rank;
    result["rank_point_before"] = old;
    result["rank_point_after"] = balance;
    result["rank_point_acquired"] = amount;
    result["stamina_before"] = stamina;
    return result;
}

void finish_lesson(game_state::State& s, long long base, const json& p, json& result) {
    json party = lesson_party(s, base);
    std::pair<long long, bool> totals = live_result::play_totals(p);
    long long score = totals.first;
    bool cleared = totals.second;
    long long before = party.at("bestScore").get<long long>();
    long long reward_received = party.at("rewardReceivedHighScore").get<long long>();
    ojson things = ojson::array();
    for (const ojson& threshold : master_data::table("CharacterLessonScoreRewardMaster")) {
        long long required = threshold.at("required_score").get<long long>();
        if (threshold.at("character_base_master_id").get<long long>() == base &&
            reward_received < required && required <= score) {
            const ojson* group = game_state::master(
                "lesson_score_reward_group_master",
                threshold.at("lesson_score_group_master_id").get<long long>());
            const ojson& rewards = group->at("rewards");
            if (rewards.is_array())
                for (const ojson& x : rewards)
                    things.push_back(ojson::array({x.at("thing_type").get<long long>(),
                                                   x.at("thing_id").get<long long>(),
                                                   x.at("thing_quantity").get<long long>()}));
        }
    }
    ojson received = s.grant(things);
    gjson* row = s.one("CharacterLesson", gjson{{"characterBaseMasterId", base}});
    s.update("CharacterLesson", row,
             gjson{{"bestScore", std::max(before, score)},
                   {"rewardReceivedHighScore", std::max(reward_received, score)}});
    use_daily(s, "dailyLessonTimes");
    // generous preservation fallback, not an exact formula (constants.yml)
    long long points = cleared ? rules().at("lesson_star_points").get<long long>() : 0;
    ojson star = game_state::star_points(s, base, points);
    // {base} | {secondary base of each actor in a slot}; first-seen dedup order
    std::vector<long long> bases;
    bases.push_back(base);
    std::vector<Slot> slots = lesson_slots(party);
    for (const gjson& a : s.rows("Character")) {
        long long aid = a.at("id").get<long long>();
        bool in_slot = false;
        for (const Slot& slot : slots)
            if (slot.second.has_value() && slot.second.value() == aid) {
                in_slot = true;
                break;
            }
        if (!in_slot) continue;
        const ojson* cm = game_state::master("character_master",
                                             a.at("characterMasterId").get<long long>());
        const ojson& sec = cm->at("secondary_character_base_master_id");
        if (sec.is_null()) continue;
        long long sval = sec.get<long long>();
        if (std::find(bases.begin(), bases.end(), sval) == bases.end()) bases.push_back(sval);
    }
    for (long long ident : bases)
        if (ident) game_state::character_progress(s, ident, 1, 1);
    game_state::mission_progress(s, 100200, 1, true);
    json lesson_result;
    lesson_result["character_base_master_id"] = base;
    lesson_result["star_point_result"] = star;
    lesson_result["high_score_rewards"] = received;
    lesson_result["high_score_before"] = before;
    lesson_result["high_score_after"] = std::max(before, score);
    result["lesson_result"] = lesson_result;
}

std::tuple<json, std::vector<json>, long long> find_course(long long detail_id) {
    for (const ojson& m : master_data::table("MusicCourseMaster")) {
        std::vector<json> details;
        const ojson& raw = m.at("details");
        if (raw.is_array())
            for (const ojson& d : raw) details.push_back(d);
        std::stable_sort(details.begin(), details.end(), [](const json& a, const json& b) {
            return a.at("set_list_number").get<long long>() <
                   b.at("set_list_number").get<long long>();
        });
        for (long long i = 0; i < static_cast<long long>(details.size()); ++i)
            if (details[i].at("id_").get<long long>() == detail_id)
                return std::make_tuple(json(m), details, i);
    }
    throw game_state::Rejected("Unknown course stage");
}

void finish_course(game_state::State& s, long long ident, const json& p, json& result) {
    std::tuple<json, std::vector<json>, long long> found = find_course(ident);
    const json& course = std::get<0>(found);
    const std::vector<json>& details = std::get<1>(found);
    long long index = std::get<2>(found);
    std::optional<gjson> run = db::fetchrow(db::SelectQuery(
        nullptr, "SELECT * FROM preservation_course_run WHERE \"userId\"=$1", s.uid));
    if (!run || run->at("data").at("next").get<long long>() != index)
        throw game_state::Rejected("Missing course session");
    gjson data = run->at("data");
    std::pair<long long, bool> totals = live_result::play_totals(p);
    bool cleared = totals.second;
    if (data.at("started_at").get<long long>() < daily::most_recent_reset(now_micros()))
        throw game_state::Rejected("Course crossed daily reset");
    const json& base_blocks = payload_member(p, "base_score_blocks");
    data["rates"].push_back(round4(live_result::achievement_rate(base_blocks)));
    data["lamps"].push_back(
        static_cast<long long>(live_result::clear_lamp(cleared, base_blocks)));
    data["next"] = index + 1;
    result["player_rank_point_result"] = nullptr;
    long long last_index = static_cast<long long>(details.size()) - 1;
    if (!cleared || index == last_index) {
        gjson* old = s.one("MusicCourse",
                           gjson{{"musicCourseMasterId", course.at("id_").get<long long>()}});
        bool all_lamps = true;
        for (const gjson& l : data.at("lamps"))
            if (l.get<long long>() == 0) {
                all_lamps = false;
                break;
            }
        long long grade;
        if (cleared && all_lamps && index == last_index)
            grade = data.at("gauge").get<long long>() == 1 ? 3 : 2;
        else
            grade = 1;
        long long lamp = 0;
        if (grade > 1) {
            bool first = true;
            for (const gjson& l : data.at("lamps")) {
                long long v = l.get<long long>();
                if (first || v < lamp) {
                    lamp = v;
                    first = false;
                }
            }
        }
        double rates_sum = 0.0;
        for (const gjson& r : data.at("rates")) rates_sum += r.get<double>();
        double total = round4(rates_sum);
        long long beforegrade = old ? old->at("certificationGrade").get<long long>() : 0;
        double best = 0.0;
        if (old) {
            const gjson& rec = old->at("totalAchievementRatePercentRecord");
            if (rec.is_string() && !rec.get<std::string>().empty())
                best = std::strtod(rec.get<std::string>().c_str(), nullptr);
            else if (rec.is_number())
                best = rec.get<double>();
        }
        long long old_clear = old ? old->at("clearLamp").get<long long>() : 0;
        gjson values = {
            {"clearLamp", std::max(old_clear, lamp)},
            {"certificationGrade", std::max(beforegrade, grade)},
            {"totalAchievementRatePercentRecord", py_float_str(std::max(best, total))},
        };
        if (old)
            s.update("MusicCourse", old, values);
        else {
            gjson ins = values;
            ins["musicCourseMasterId"] = course.at("id_").get<long long>();
            s.insert("MusicCourse", ins);
        }
        // master rewards apply once when a new certification grade is attained
        ojson things = ojson::array();
        for (const ojson& group : master_data::table("MusicCourseRewardGroupMaster")) {
            long long req = group.at("required_certification_grade").get<long long>();
            if (group.at("music_course_master_id").get<long long>() ==
                    course.at("id_").get<long long>() &&
                beforegrade < req && req <= grade) {
                const ojson& rewards = group.at("rewards");
                if (rewards.is_array())
                    for (const ojson& x : rewards)
                        things.push_back(
                            ojson::array({x.at("thing_type").get<long long>(),
                                          x.at("thing_id").get<long long>(),
                                          x.at("thing_quantity").get<long long>()}));
            }
        }
        s.grant(things);
        db::execute(db::ExecutableQuery(
            "DELETE FROM preservation_course_run WHERE \"userId\"=$1", s.uid));
    } else {
        db::execute(db::ExecutableQuery(
            "UPDATE preservation_course_run SET data=$2 WHERE \"userId\"=$1", s.uid, data));
    }
}

void finish_context(game_state::State& s, const json& context_row, const json& p, json& result) {
    std::string mode = context_row.at("mode").get<std::string>();
    gjson extra = context_row.at("extra");
    if (!extra.is_object()) extra = gjson::object();  // python `... or {}`
    if (mode == "course") {
        finish_course(s, context_row.at("masterId").get<long long>(), p, result);
        return;
    }
    if (mode == "lesson") finish_lesson(s, context_row.at("masterId").get<long long>(), p, result);
    auto ait = extra.find("auto");
    if (ait != extra.end() && is_truthy(*ait)) use_daily(s, "autoPlayTimes");
    auto rit = extra.find("rank_xp");
    long long rxp = rit != extra.end() && rit->is_number() ? rit->get<long long>() : 0;
    if (rxp > 0) result["player_rank_point_result"] = rank_xp(s, rxp);
    std::pair<long long, bool> totals = live_result::play_totals(p);
    bool cleared = totals.second;
    game_state::mission_progress(s, 1600);
    if (cleared) game_state::mission_progress(s, 1800);
    const json& base_blocks = payload_member(p, "base_score_blocks");
    long long count = base_blocks.is_array() ? static_cast<long long>(base_blocks.size()) : 0;
    game_state::mission_progress(s, 3600, count);
}

}  // namespace progression
