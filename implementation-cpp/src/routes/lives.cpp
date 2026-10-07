#include "lives.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../db.h"
#include "../db/user.h"
#include "../master_data.h"
#include "../pipeline.h"
#include "../wire.h"
#include "../helpers/live.h"
#include "../helpers/live_drops.h"
#include "../helpers/live_rate.h"
#include "../helpers/live_result.h"
#include "../helpers/music_unlock.h"
#include "../helpers/score.h"
#include "../helpers/stamina.h"
#include "../helpers/things.h"
#include "../helpers/user_data.h"
#include "generated/enums_generated.h"

// Ports routes/lives.py.

namespace routes {

namespace {

using wire::json;
using db::user::create_active_live;
using db::user::delete_active_lives;
using db::user::get_active_live;
using db::user::get_lives;
using db::user::get_sp_rates;
using db::user::get_users;
using db::user::next_live_id;
using db::user::update_live_result;
using db::user::update_player_rate;
using db::user::update_sp_rate_point;
using db::user::upsert_live;
using db::user::upsert_sp_rate;

// LiveMaster -> its MusicMaster. The two id->row maps are built once on first use
// (mirrors the module-global _LIVE_MASTER / _MUSIC_MASTER lazy fill).
const json* _music_of(long long live_master_id) {
    static const std::unordered_map<long long, const json*> live_master = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& row : master_data::table("LiveMaster"))
            m[row.at("id_").get<long long>()] = &row;
        return m;
    }();
    static const std::unordered_map<long long, const json*> music_master = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& row : master_data::table("MusicMaster"))
            m[row.at("id_").get<long long>()] = &row;
        return m;
    }();
    auto lit = live_master.find(live_master_id);
    if (lit == live_master.end()) return nullptr;
    auto mit = music_master.find(lit->second->at("music_master_id").get<long long>());
    return mit != music_master.end() ? mit->second : nullptr;
}

bool _is_long_version(long long live_master_id) {
    const json* music = _music_of(live_master_id);
    return music != nullptr ? music->at("is_long_version").get<bool>() : false;
}

long long _stamina_cost(long long live_master_id, long long ratio) {
    // MusicMaster.stamina_consumption, scaled by the play's ratio
    const json* music = _music_of(live_master_id);
    long long base = music != nullptr ? music->at("stamina_consumption").get<long long>() : 0;
    return base * std::max<long long>(1, ratio);
}

// python round(x, 4): nearest 1e-4, ties to even
double round4(double x) { return std::nearbyint(x * 10000.0) / 10000.0; }

// an int payload field or its default(0); a missing/nil field is absent from the decoded json.
long long pint(const json& payload, const char* key) {
    return payload.value(key, static_cast<long long>(0));
}

}  // namespace

// /api/Lives/FinishAndValidate
void lives_finish_and_validate(const httplib::Request& req, httplib::Response& res) {
    std::optional<long long> user_id = user_data::current_user_id(req);
    json payload = pipeline::read_request(req, "FinishLivePayload");
    if (payload.is_null()) {
        pipeline::respond(res, "FinishLiveResult", json::object());
        return;
    }
    if (!score::verify_score_blocks(payload)) {
        // score-block hash chain does not reproduce -> tampered score, reject
        pipeline::respond(res, "FinishLiveResult", json::object(),
                          json::array({pipeline::fault("InvalidScoreBlockHash")}));
        return;
    }
    if (!user_id.has_value()) {
        pipeline::respond(res, "FinishLiveResult", json::object());
        return;
    }

    const json base_blocks = payload.value("base_score_blocks", json());
    const json star_blocks = payload.value("star_act_score_blocks", json());

    // the client reports score=0 / max_combo=0 / is_cleared=false / judges=null and leaves
    // the rest to us, so lamp + rate + score all come out of the (hash-verified) score blocks
    std::pair<long long, bool> totals = live_result::play_totals(payload);
    long long score = totals.first;
    bool is_cleared = totals.second;
    int new_lamp = live_result::clear_lamp(is_cleared, base_blocks);
    double this_rate = live_result::achievement_rate(base_blocks);

    std::optional<db::json> user = db::fetchrow(get_users(*user_id));
    std::optional<db::json> active = db::fetchrow(get_active_live(*user_id));
    long long live_master_id =
        active.has_value() ? active->at("liveMasterId").get<long long>() : 0;
    std::vector<db::json> lives;
    if (live_master_id) lives = db::fetch(get_lives(*user_id));
    const db::json* existing = nullptr;
    for (const db::json& r : lives) {
        if (r.at("liveMasterId").get<long long>() == live_master_id) {
            existing = &r;
            break;
        }
    }

    long long before_lamp =
        existing != nullptr ? existing->at("clearLamp").get<long long>() : enums::ClearLamps::None_;
    double prev_rate = existing != nullptr ? existing->at("achievementRate").get<double>() : 0.0;
    bool is_high_score = this_rate > prev_rate;

    long long best_lamp = std::max<long long>(before_lamp, new_lamp);  // worst..best int order
    double best_rate = std::max(prev_rate, this_rate);
    long long best_grade = live_result::rate_grade(best_rate);
    long long times = (existing != nullptr ? existing->at("timesCompleted").get<long long>() : 0) + 1;
    // notation_rate is the chart's (best) live rate: level + interpolated achievement bonus
    double notation_rate = live_rate::chart_live_rate(live_master_id, best_rate).value_or(0.0);

    // the owned Live row's fields that this finish doesn't change carry over (or default)
    long long live_id = existing != nullptr ? existing->at("id").get<long long>() : 0;
    long long live_status =
        existing != nullptr ? existing->at("status").get<long long>() : enums::LiveReleaseStatus::None_;
    if (existing != nullptr) {
        db::execute(update_live_result(*user_id, live_master_id, times, best_rate, notation_rate,
                                       best_lamp, best_grade));
    } else if (live_master_id) {
        std::optional<db::json> id_row = db::fetchrow(next_live_id());
        live_id = id_row.has_value() ? id_row->at("value").get<long long>() : live_master_id;
        json row = json::object();
        row["id"] = live_id;
        row["liveMasterId"] = live_master_id;
        row["timesCompleted"] = times;
        row["achievementRate"] = best_rate;
        row["notationRate"] = notation_rate;
        row["clearLamp"] = best_lamp;
        row["status"] = live_status;
        row["rateGrade"] = best_grade;
        db::execute(upsert_live(*user_id, row));
    }

    db::execute(delete_active_lives(*user_id));  // consume the session

    // the updated owned Live row goes in present so the client's records reflect it
    json present = json::array();
    if (live_master_id) {
        json live_row = json::object();
        live_row["userId"] = *user_id;
        live_row["id"] = live_id;
        live_row["liveMasterId"] = live_master_id;
        live_row["timesCompleted"] = times;
        live_row["achievementRate"] = best_rate;
        live_row["notationRate"] = notation_rate;
        live_row["clearLamp"] = best_lamp;
        live_row["status"] = live_status;
        live_row["rateGrade"] = best_grade;
        present.push_back(user_data::data_object("Live", live_row));
    }

    // Only Extra/Stella/Olivier clears can change release state -- skip everything else.
    // When they can, re-derive every owned song, then push all changes in one UPDATE.
    if (music_unlock::affects_unlocks(live_master_id)) {
        // the Extra GOOD budget can only be judged here, off this play's own blocks
        std::vector<nlohmann::json> changed = music_unlock::apply_unlocks(
            *user_id, music_unlock::stella_unlocked_by(
                          live_master_id, is_cleared,
                          live_result::good_or_worse_count(base_blocks)));
        for (const nlohmann::json& music : changed)
            present.push_back(user_data::data_object("Music", music));
    }

    // live drops: resolve the setting's drop frames whose lot condition this play met,
    // then consolidate + batch-grant their rewards. Long-version songs never drop.
    std::vector<json> live_drops;
    if (!_is_long_version(live_master_id)) {
        long long setting_id =
            active.has_value() ? active->at("liveSettingMasterId").get<long long>() : 0;
        long long star_act_count = star_blocks.is_array() ? static_cast<long long>(star_blocks.size()) : 0;
        std::vector<json> frames = live_drops::resolve_frames(
            setting_id,
            active.has_value() ? active->at("staminaSpent").get<bool>() : false, score,
            star_act_count, this_rate);
        live_drops = live_drops::grant_frames(*user_id, frames);
    }

    // live rate: this chart's (past-best, this-time) + the top-30 total before/after this
    // play. The profile's playerRate IS that top-30 total, so keep it in sync when it moves.
    // (0.0, 0.0) for charts with no live rate (Olivier / long-version) -- not null
    std::pair<double, double> lr =
        live_rate::chart_live_rate_result(live_master_id, this_rate, prev_rate);
    json live_rate_result = json::object();
    live_rate_result["best_ever"] = lr.first;
    live_rate_result["this_time"] = lr.second;
    std::vector<std::pair<long long, double>> before_pairs;
    for (const db::json& r : lives)
        before_pairs.emplace_back(r.at("liveMasterId").get<long long>(),
                                  r.at("achievementRate").get<double>());
    double total_before = live_rate::total_rate(before_pairs);
    std::vector<std::pair<long long, double>> after_pairs;
    for (const db::json& r : lives) {
        if (r.at("liveMasterId").get<long long>() != live_master_id)
            after_pairs.emplace_back(r.at("liveMasterId").get<long long>(),
                                     r.at("achievementRate").get<double>());
    }
    if (live_master_id) after_pairs.emplace_back(live_master_id, best_rate);
    double total_after = live_rate::total_rate(after_pairs);
    if (total_after != total_before) db::execute(update_player_rate(*user_id, total_after));

    // Olivier (difficulty 5) clears award star-badge points (SpRate) instead of a live
    // rate: keep the chart's best points and report the before/after per-chart + total.
    json sp_rate_update_result = nullptr;
    std::optional<int> this_points = live_rate::olivier_sp_rate_points(
        live_master_id, this_rate, new_lamp, live_result::non_perfect_star_count(base_blocks));
    if (this_points.has_value()) {
        std::vector<db::json> sp_rows = db::fetch(get_sp_rates(*user_id));
        const db::json* existing_sp = nullptr;
        for (const db::json& r : sp_rows) {
            if (r.at("liveMasterId").get<long long>() == live_master_id) {
                existing_sp = &r;
                break;
            }
        }
        long long before_best = existing_sp != nullptr ? existing_sp->at("point").get<long long>() : 0;
        long long before_total = 0;
        for (const db::json& r : sp_rows) before_total += r.at("point").get<long long>();
        long long new_best = std::max<long long>(before_best, *this_points);
        long long after_total = before_total - before_best + new_best;
        long long sp_id;
        if (existing_sp != nullptr) {
            sp_id = existing_sp->at("id").get<long long>();
            if (new_best != before_best)
                db::execute(update_sp_rate_point(*user_id, sp_id, new_best));
        } else {
            sp_id = 0;
            for (const db::json& r : sp_rows) sp_id = std::max<long long>(sp_id, r.at("id").get<long long>());
            sp_id += 1;
            json row = json::object();
            row["id"] = sp_id;
            row["liveMasterId"] = live_master_id;
            row["point"] = new_best;
            db::execute(upsert_sp_rate(*user_id, row));
        }
        sp_rate_update_result = json::object();
        sp_rate_update_result["best_ever"] = before_best;
        sp_rate_update_result["this_time"] = *this_points;
        sp_rate_update_result["best_ever_total"] = before_total;
        sp_rate_update_result["this_time_total"] = after_total;
        json sp_row = json::object();
        sp_row["userId"] = *user_id;
        sp_row["id"] = sp_id;
        sp_row["liveMasterId"] = live_master_id;
        sp_row["point"] = new_best;
        present.push_back(user_data::data_object("SpRate", sp_row));
    }

    // refresh what this finish changed for the client: drop rewards + the player rating
    std::set<std::string> refresh;
    if (!live_drops.empty()) {
        for (const json& d : live_drops) {
            std::optional<std::string> pt =
                things::present_type(d.at("received_thing").at("type").get<long long>());
            if (pt.has_value()) refresh.insert(*pt);
        }
    }
    if (total_after != total_before) refresh.insert("UserProfile");
    if (!refresh.empty()) {
        std::vector<user_data::PresentSpec> specs(refresh.begin(), refresh.end());  // sorted
        json extra = user_data::build_present(*user_id, specs);
        for (const json& e : extra) present.push_back(e);
    }

    json rate_result = json::object();
    json ach_result = json::object();
    ach_result["best_ever"] = round4(prev_rate);  // best_ever = past best only
    ach_result["this_time"] = round4(this_rate);
    rate_result["achievement_rate_result"] = ach_result;
    rate_result["live_rate_result"] = live_rate_result;
    rate_result["total_rate_before"] = total_before;
    rate_result["total_rate_after"] = total_after;

    // TODO: calculate player rank pts
    json player_rank_point_result = json::object();
    player_rank_point_result["rank_before"] = (*user).at("playerRank");
    player_rank_point_result["rank_after"] = (*user).at("playerRank");
    player_rank_point_result["rank_point_before"] = (*user).at("currentRankPoint");
    player_rank_point_result["rank_point_after"] = (*user).at("currentRankPoint");
    player_rank_point_result["rank_point_acquired"] = 0;
    player_rank_point_result["stamina_before"] = (*user).at("currentStamina");

    json result = json::object();
    result["clear_lamp"] = new_lamp;
    result["before_clear_lamp"] = before_lamp;
    result["rate_grade"] = live_result::rate_grade(this_rate);
    result["is_high_score"] = is_high_score;
    result["achievement_rate_average"] = 0.0;  // TODO: add global average acc
    result["rate_result"] = rate_result;
    result["sp_rate_update_result"] = sp_rate_update_result;
    result["player_rank_point_result"] = player_rank_point_result;
    result["live_drop_things"] = live_drops;
    // These must be empty lists, not null -- the result panel iterates them and a null
    // list NREs the client. Empty for now; TODO implement each:
    result["league_rewards"] = json::array();                     // TODO: league-mode finish rewards
    result["audition_rewards"] = json::array();                   // TODO: audition finish rewards
    result["story_event_rewards"] = json::array();                // TODO: story-event finish rewards
    result["accessory_auto_sell_convert_things"] = json::array();  // TODO: accessories auto-sold to items on drop

    pipeline::respond(res, "FinishLiveResult", result, json::array(), present);
}

// /api/Lives/Retire
void lives_retire(const httplib::Request& req, httplib::Response& res) {
    std::optional<long long> user_id = user_data::current_user_id(req);
    if (user_id.has_value()) db::execute(delete_active_lives(*user_id));
    json result = json::object();
    result["is_success"] = true;
    pipeline::respond(res, "BooleanResult", result);
}

// /api/Lives/Start
void lives_start(const httplib::Request& req, httplib::Response& res) {
    std::optional<long long> user_id = user_data::current_user_id(req);
    json payload = pipeline::read_request(req, "StartLivePayload");
    json present = json::array();
    json unit = json::object();
    if (user_id.has_value() && !payload.is_null()) {
        std::optional<db::json> user = db::fetchrow(get_users(*user_id));
        bool stamina_spent = false;
        long long live_master_id = pint(payload, "live_master_id");
        long long party_id = pint(payload, "party_id");
        long long live_setting_master_id = pint(payload, "live_setting_master_id");
        // long-version songs never consume stamina
        if (user.has_value() && payload.value("use_stamina", false) &&
            !_is_long_version(live_master_id)) {
            long long cost = _stamina_cost(live_master_id, pint(payload, "stamina_consumption_ratio"));
            if (cost > 0 && stamina::adjust_and_check_stamina(
                                *user_id, -cost, user->at("playerRank").get<long long>())) {
                stamina_spent = true;
                user = db::fetchrow(get_users(*user_id));  // reflect spent stamina
            }
        }
        db::execute(delete_active_lives(*user_id));  // one active live per user
        std::pair<json, long long> built = live::build_live_unit(*user_id, party_id, live_master_id);
        unit = built.first;
        long long live_id = built.second;
        db::execute(create_active_live(*user_id, live_id, live_master_id, party_id,
                                       live_setting_master_id, stamina_spent));
        if (user.has_value()) present.push_back(user_data::data_object("User", *user));
    }
    pipeline::respond(res, "LiveUnit", unit, json::array(), present);
}

void register_lives(httplib::Server& svr) {
    // /api/Lives/CalculateLessonTimingEvents
    svr.Post("/api/Lives/CalculateLessonTimingEvents",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "CalculateLessonTimeEventPayload");
                 pipeline::respond(res, "LiveTimeEvent", json::object());
             });

    // /api/Lives/CalculateTimingEvents
    svr.Post("/api/Lives/CalculateTimingEvents",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = pipeline::read_request(req, "CalculateTimeEventPayload");
                 json lte = json::object();
                 if (user_id.has_value() && !payload.is_null()) {
                     std::optional<long long> sense;
                     auto it = payload.find("sense_notation_master_id");
                     if (it != payload.end() && !it->is_null()) sense = it->get<long long>();
                     lte = live::build_live_time_event(*user_id, pint(payload, "party_id"),
                                                       pint(payload, "music_master_id"), sense);
                 }
                 pipeline::respond(res, "LiveTimeEvent", lte);
             });

    // /api/Lives/Music/EditBookmark
    svr.Post("/api/Lives/Music/EditBookmark",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "EditBookmarkPayload");
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    // /api/Lives/FinishAndValidate
    svr.Post("/api/Lives/FinishAndValidate", lives_finish_and_validate);

    // /api/Lives/FinishAnotherNotationLive
    svr.Post("/api/Lives/FinishAnotherNotationLive",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "FinishAnotherNotationLivePayload");
                 pipeline::respond(res, "FinishLiveResult", json::object());
             });

    // /api/Lives/GetCourseCircleRanking/{mMusicCourseId}
    svr.Post("/api/Lives/GetCourseCircleRanking/:mMusicCourseId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "", json::array());
             });

    // /api/Lives/GetCourseFriendRanking/{mMusicCourseId}
    svr.Post("/api/Lives/GetCourseFriendRanking/:mMusicCourseId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "", json::array());
             });

    // /api/Lives/GetCourseNearRanking/{mMusicCourseId}
    svr.Post("/api/Lives/GetCourseNearRanking/:mMusicCourseId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "", json::array());
             });

    // /api/Lives/GetCourseTopRanking/{mMusicCourseId}
    svr.Post("/api/Lives/GetCourseTopRanking/:mMusicCourseId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "", json::array());
             });

    // /api/Lives/GetMultiLiveInformation/{multiLiveId}
    svr.Get("/api/Lives/GetMultiLiveInformation/:multiLiveId",
            [](const httplib::Request&, httplib::Response& res) {
                pipeline::respond(res, "MultiLiveInformation", json::object());
            });

    // /api/Lives/MatchingGhostLive
    svr.Post("/api/Lives/MatchingGhostLive",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "MatchingGhostLiveResult", json::object());
             });

    // /api/Lives/ReadConcertTips?mConcertId=
    svr.Post("/api/Lives/ReadConcertTips",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    // /api/Lives/ReceiveTeamChallengeRewards?uMultiLiveId=
    svr.Post("/api/Lives/ReceiveTeamChallengeRewards",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "", json::array());
             });

    // /api/Lives/Retire
    svr.Post("/api/Lives/Retire", lives_retire);

    // /api/Lives/SelectMusicCourseRandomMusic
    svr.Post("/api/Lives/SelectMusicCourseRandomMusic",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "SelectMusicCourseRandomMusicPayload");
                 pipeline::respond(res, "MusicCourseRandomSelectResult", json::object());
             });

    // /api/Lives/Start
    svr.Post("/api/Lives/Start", lives_start);

    // /api/Lives/StartBonusLive
    svr.Post("/api/Lives/StartBonusLive", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "StartLivePayload");
        // scripted: time_events = build_live_time_event(conn, user_id, party_id,
        //   music_master_id, bonus_live_stage_master.sense_notation_master_id)
        pipeline::respond(res, "LiveUnit", json::object());
    });

    // /api/Lives/StartConcert
    svr.Post("/api/Lives/StartConcert", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "StartLivePayload");
        // scripted: time_events = build_live_time_event(conn, user_id, party_id,
        //   music_master_id, concert_stage_master.sense_notation_master_id)
        pipeline::respond(res, "LiveUnit", json::object());
    });

    // /api/Lives/StartGhostLive
    svr.Post("/api/Lives/StartGhostLive", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "StartLivePayload");
        // scripted: time_events = build_live_time_event(conn, user_id, party_id,
        //   music_master_id, ghost_live_master.sense_notation_master_id)
        pipeline::respond(res, "LiveUnit", json::object());
    });

    // /api/Lives/StartLesson
    svr.Post("/api/Lives/StartLesson", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "StartLessonPayload");
        // scripted: time_events = build_live_time_event(conn, user_id, party_id,
        //   music_master_id, sense_notation_master_id)  # lesson sense-notation source TBD
        pipeline::respond(res, "LiveUnit", json::object());
    });

    // /api/Lives/StartMultiLive
    svr.Post("/api/Lives/StartMultiLive", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "StartMultiLivePayload");
        // scripted: time_events = build_live_time_event(conn, user_id, party_id,
        //   music_master_id, league_master.sense_notation_master_id)  # league/multi source TBD
        pipeline::respond(res, "LiveUnit", json::object());
    });

    // /api/Lives/StartMultiRoomLive
    svr.Post("/api/Lives/StartMultiRoomLive",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "StartMultiRoomLivePayload");
                 // scripted: time_events = build_live_time_event(conn, user_id, party_id,
                 //   music_master_id, league_master.sense_notation_master_id)  # league/multi source TBD
                 pipeline::respond(res, "LiveUnit", json::object());
             });

    // /api/Lives/StartMusicCourseLive
    svr.Post("/api/Lives/StartMusicCourseLive",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "StartLivePayload");
                 // scripted: time_events = build_live_time_event(conn, user_id, party_id,
                 //   music_master_id, sense_notation_master_id)  # music-course sense-notation source TBD
                 pipeline::respond(res, "LiveUnit", json::object());
             });

    // /api/Lives/StartTournament
    svr.Post("/api/Lives/StartTournament", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "StartTournamentPayload");
        // scripted: time_events = build_live_time_event(conn, user_id, party_id,
        //   music_master_id, league_master.sense_notation_master_id)  # tournament source TBD
        pipeline::respond(res, "LiveUnit", json::object());
    });

    // /api/Lives/StartTrialPartyEventStage
    svr.Post("/api/Lives/StartTrialPartyEventStage",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "StartLivePayload");
                 // scripted: time_events = build_live_time_event(conn, user_id, party_id,
                 //   music_master_id, trial_party_event_stage_master.sense_notation_master_id)
                 pipeline::respond(res, "LiveUnit", json::object());
             });

    // /api/Lives/StartTripleCastLive
    svr.Post("/api/Lives/StartTripleCastLive",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "StartTripleCastLivePayload");
                 // scripted: one build_live_time_event per unit, from
                 //   triple_cast_master.sense_notation_master_id{1,2,3}
                 pipeline::respond(res, "StartTripleCastLiveResult", json::object());
             });

    // /api/Lives/UpdateClearLamps/{multiLiveId}
    svr.Post("/api/Lives/UpdateClearLamps/:multiLiveId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "UpdateClearLampResult", json::object());
             });
}

}  // namespace routes
