#include "routes/mission.h"

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <optional>
#include <string>
#include <vector>

#include "generated/enums_generated.h"
#include "helpers/game_state.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

namespace routes {
namespace {

using ojson = nlohmann::ordered_json;  // master rows + wire results
using rjson = nlohmann::json;          // db rows (camelCase columns)

// days since the unix epoch for a civil (y, m, d); mirrors datetime->epoch.
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

// epoch seconds of an iso-8601 string; mirrors datetime.fromisoformat after the
// Z->+00:00 replacement (naive -> utc). false for anything not a valid date(+time).
bool iso_epoch_seconds(const std::string& s, long long& out) {
    size_t i = 0, n = s.size();
    auto read = [&](int len, int& v) -> bool {
        v = 0;
        int k = 0;
        for (; k < len && i < n && s[i] >= '0' && s[i] <= '9'; ++k, ++i) v = v * 10 + (s[i] - '0');
        return k == len;
    };
    int year, mon, day;
    if (!read(4, year)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, mon)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, day)) return false;

    int hh = 0, mm = 0, ss = 0;
    long long tz = 0;
    if (i < n) {
        ++i;  // date/time separator
        if (!read(2, hh)) return false;
        if (i >= n || s[i] != ':') return false;
        ++i;
        if (!read(2, mm)) return false;
        if (i < n && s[i] == ':') {
            ++i;
            if (!read(2, ss)) return false;
        }
        if (i < n && (s[i] == '.' || s[i] == ',')) {
            ++i;
            while (i < n && s[i] >= '0' && s[i] <= '9') ++i;
        }
        if (i < n) {
            char c = s[i];
            if (c == 'Z' || c == 'z') {
                ++i;
            } else if (c == '+' || c == '-') {
                int sign = (c == '-') ? -1 : 1;
                ++i;
                int oh = 0, om = 0, os_ = 0;
                if (!read(2, oh)) return false;
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, om);
                } else {
                    read(2, om);
                }
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, os_);
                }
                tz = sign * (oh * 3600LL + om * 60LL + os_);
            }
        }
    }
    out = days_from_civil(year, static_cast<unsigned>(mon), static_cast<unsigned>(day)) * 86400LL +
          hh * 3600LL + mm * 60LL + ss - tz;
    return true;
}

// claim rewards for every cleared, in-window mission (optionally filtered by id or category),
// advancing multi-stage missions to their next stage.
void receive_missions(const httplib::Request& req, httplib::Response& res,
                      std::optional<int64_t> mission_id = std::nullopt,
                      std::optional<int64_t> category = std::nullopt) {
    long long now = static_cast<long long>(std::time(nullptr));

    // start-inclusive availability (end=true -> end-exclusive); a falsy date reads as no restriction
    auto available = [&](const ojson* m, const char* key, bool end) -> bool {
        auto it = m->find(key);
        if (it == m->end() || it->is_null() || !it->is_string() ||
            it->get_ref<const std::string&>().empty())
            return true;
        long long date = 0;
        if (!iso_epoch_seconds(it->get_ref<const std::string&>(), date)) return true;
        return end ? (now < date) : (date <= now);
    };

    try {
        game_state::State s = game_state::transaction(req);
        ojson rewards = ojson::array();
        for (rjson& row : s.rows("Mission")) {
            const ojson* m =
                game_state::master("mission_master", row.at("missionMasterId").get<int64_t>());
            if (m == nullptr ||
                (mission_id.has_value() && m->at("id_").get<int64_t>() != *mission_id) ||
                (category.has_value() && m->at("mission_category").get<int64_t>() != *category) ||
                row.at("isRewardReceived").get<bool>() || !row.at("isCleared").get<bool>() ||
                !available(m, "start_date", false) || !available(m, "end_date", true))
                continue;
            std::vector<const ojson*> stages;
            const ojson& stages_arr = m->at("stages");
            if (stages_arr.is_array())
                for (const ojson& st : stages_arr) stages.push_back(&st);
            std::stable_sort(stages.begin(), stages.end(), [](const ojson* a, const ojson* b) {
                return a->at("mission_stage_order").get<int64_t>() <
                       b->at("mission_stage_order").get<int64_t>();
            });
            int64_t current = row.at("currentMissionStageMasterId").get<int64_t>();
            const ojson* stage = nullptr;
            for (const ojson* x : stages)
                if (x->at("id_").get<int64_t>() == current) {
                    stage = x;
                    break;
                }
            int64_t count = row.at("missionCurrentCount").get<int64_t>();
            if (stage == nullptr || count < stage->at("stage_goal_value").get<int64_t>() ||
                !available(stage, "start_date", false))
                continue;
            const ojson& stage_rewards = stage->at("rewards");
            if (stage_rewards.is_array())
                for (const ojson& r : stage_rewards)
                    rewards.push_back(ojson::array({r.at("thing_type").get<int64_t>(),
                                                    r.at("thing_id").get<int64_t>(),
                                                    r.at("thing_quantity").get<int64_t>()}));
            int64_t stage_order = stage->at("mission_stage_order").get<int64_t>();
            const ojson* following = nullptr;
            for (const ojson* x : stages)
                if (x->at("mission_stage_order").get<int64_t>() > stage_order) {
                    following = x;
                    break;
                }
            if (following != nullptr) {
                s.update(
                    "Mission", &row,
                    rjson{{"currentMissionStageMasterId", following->at("id_").get<int64_t>()},
                          {"isCleared", count >= following->at("stage_goal_value").get<int64_t>()},
                          {"isRewardReceived", false}});
            } else {
                s.update("Mission", &row, rjson{{"isRewardReceived", true}});
            }
        }
        ojson received = s.grant(rewards);
        ojson present = s.present();
        s.commit();
        pipeline::respond(res, "ReceivedThing", received, ojson::array(), present);
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "ReceivedThing", ojson::array());
    }
}

}  // namespace

void register_mission(httplib::Server& svr) {
    // /api/Missions/PickupActor/bulkReceiveRewards/{mPickupCharacterMissionMasterId}
    svr.Post("/api/Missions/PickupActor/bulkReceiveRewards/:mPickupCharacterMissionMasterId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "ReceivedThing", ojson::array());
             });

    // /api/Missions/ExchangeMissionPoint?unit=
    svr.Post("/api/Missions/ExchangeMissionPoint",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    // /api/Missions/receiveRewards?missionCategory=
    svr.Post("/api/Missions/receiveRewards",
             [](const httplib::Request& req, httplib::Response& res) {
                 if (!req.has_param("missionCategory")) {
                     pipeline::respond(res, "ReceivedThing", ojson::array());
                     return;
                 }
                 receive_missions(req, res, std::nullopt,
                                  std::stoll(req.get_param_value("missionCategory")));
             });

    // /api/Missions/MissionPassReceiveRewards/{missionPassId}
    svr.Post("/api/Missions/MissionPassReceiveRewards/:missionPassId",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(
                     res, "MissionPassRewardsResult",
                     ojson{{"reward_result", enums::MissionPassRewardStatus::NotReceived}});
             });

    // /api/Missions/{missionId}/receiveCurrentRewards
    svr.Post("/api/Missions/:missionId/receiveCurrentRewards",
             [](const httplib::Request& req, httplib::Response& res) {
                 receive_missions(req, res, std::stoll(req.path_params.at("missionId")),
                                  std::nullopt);
             });

    // /api/Missions/PickupActor/receiveRewards/{mPickupCharacterMissionMasterId}/{mPickupCharacterMissionDetailMasterId}
    svr.Post(
        "/api/Missions/PickupActor/receiveRewards/:mPickupCharacterMissionMasterId/"
        ":mPickupCharacterMissionDetailMasterId",
        [](const httplib::Request&, httplib::Response& res) {
            pipeline::respond(res, "ReceivedThing", ojson::array());
        });
}

}  // namespace routes
