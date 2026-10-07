#include "routes/character_mission.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "helpers/game_state.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/character_mission.py.

namespace routes {
namespace {

using ojson = nlohmann::ordered_json;  // master rows + wire results
using rjson = nlohmann::json;          // db rows (camelCase columns)

// claim every pending character-mission stage reward for one base and rank it up. star points
// from a category are split evenly across that category's stages. returns a StarPointResult.
ojson claim_star_rewards(game_state::State& s, int64_t base_master_id) {
    rjson* base = s.one("CharacterBase", rjson{{"characterBaseMasterId", base_master_id}});
    if (base == nullptr) throw game_state::Rejected();
    int64_t rank_before = base->at("starRank").get<int64_t>();
    int64_t points_before = base->at("totalStarPoint").get<int64_t>();
    int64_t points = 0;
    std::unordered_map<int64_t, const ojson*> categories;
    for (const ojson& x : master_data::table("CharacterMissionCategoryLevelMaster"))
        categories[x.at("id_").get<int64_t>()] = &x;
    // sizes: how many stages each category spans across every mission
    std::unordered_map<int64_t, int64_t> sizes;
    for (const ojson& m : master_data::table("CharacterMissionMaster")) {
        const ojson& stages = m.at("stages");
        if (stages.is_array())
            for (const ojson& st : stages)
                sizes[st.at("character_mission_category_level_master_id").get<int64_t>()] += 1;
    }
    int64_t key_level = base->at("keyMissionLevel").get<int64_t>();
    for (rjson& row : s.rows("CharacterMission")) {
        if (row.at("characterBaseMasterId").get<int64_t>() != base_master_id) continue;
        const ojson* m = game_state::master("character_mission_master",
                                            row.at("characterMissionMasterId").get<int64_t>());
        std::vector<const ojson*> stages;
        const ojson& stages_arr = m->at("stages");
        if (stages_arr.is_array())
            for (const ojson& st : stages_arr) stages.push_back(&st);
        std::stable_sort(stages.begin(), stages.end(), [](const ojson* a, const ojson* b) {
            return a->at("stage_order").get<int64_t>() < b->at("stage_order").get<int64_t>();
        });
        std::vector<const ojson*> available;
        for (const ojson* st : stages) {
            int64_t cid = st->at("character_mission_category_level_master_id").get<int64_t>();
            if (categories.at(cid)->at("level").get<int64_t>() <= key_level + 1)
                available.push_back(st);
        }
        int64_t reward_received = row.at("rewardReceivedStageOrder").get<int64_t>();
        int64_t cleared_stage = row.at("clearedStageOrder").get<int64_t>();
        std::vector<const ojson*> pending;
        for (const ojson* st : available) {
            int64_t so = st->at("stage_order").get<int64_t>();
            if (reward_received < so && so <= cleared_stage) pending.push_back(st);
        }
        if (pending.empty()) continue;
        for (const ojson* st : pending) {
            int64_t cid = st->at("character_mission_category_level_master_id").get<int64_t>();
            points += categories.at(cid)->at("give_star_point").get<int64_t>() / sizes.at(cid);
        }
        int64_t claimed = pending.front()->at("stage_order").get<int64_t>();
        for (const ojson* st : pending)
            claimed = std::max(claimed, st->at("stage_order").get<int64_t>());
        // a completed category stays at its last stage until its next level unlocks
        const ojson* current = available.back();
        for (const ojson* st : available)
            if (st->at("stage_order").get<int64_t>() > claimed) {
                current = st;
                break;
            }
        int64_t completed = row.at("completedLevel").get<int64_t>();
        for (const ojson* st : available) {
            if (st->at("stage_order").get<int64_t>() > claimed) continue;
            int64_t cid = st->at("character_mission_category_level_master_id").get<int64_t>();
            bool has_later = false;
            for (const ojson* other : stages)
                if (other->at("stage_order").get<int64_t>() > claimed &&
                    other->at("character_mission_category_level_master_id").get<int64_t>() == cid) {
                    has_later = true;
                    break;
                }
            if (!has_later)
                completed = std::max(completed, categories.at(cid)->at("level").get<int64_t>());
        }
        s.update("CharacterMission", &row,
                 rjson{{"rewardReceivedStageOrder", claimed},
                       {"currentStageMasterId", current->at("id_").get<int64_t>()},
                       {"completedLevel", completed}});
    }
    int64_t rank = rank_before;
    int64_t balance = points_before + points;
    while (true) {
        const ojson* level = game_state::master("character_star_rank_master", rank, "rank");
        if (level == nullptr || level->at("next_rank_point").get<int64_t>() <= 0 ||
            balance < level->at("next_rank_point").get<int64_t>() ||
            game_state::master("character_star_rank_master", rank + 1, "rank") == nullptr)
            break;
        balance -= level->at("next_rank_point").get<int64_t>();
        rank += 1;
    }
    s.update("CharacterBase", base, rjson{{"starRank", rank}, {"totalStarPoint", balance}});
    ojson rewards = ojson::array();
    for (const ojson& r : master_data::table("StarRankRewardMaster")) {
        int64_t reward_rank = r.at("rank").get<int64_t>();
        if (r.at("character_base_master_id").get<int64_t>() == base_master_id &&
            rank_before < reward_rank && reward_rank <= rank) {
            const ojson* group =
                game_state::master("character_star_rank_reward_group_master",
                                   r.at("character_star_rank_reward_group_master_id").get<int64_t>());
            if (group != nullptr) {
                const ojson& grp_rewards = group->at("rewards");
                if (grp_rewards.is_array())
                    for (const ojson& t : grp_rewards)
                        rewards.push_back(ojson::array({t.at("thing_type").get<int64_t>(),
                                                        t.at("thing_id").get<int64_t>(),
                                                        t.at("thing_quantity").get<int64_t>()}));
            }
        }
    }
    ojson received = s.grant(rewards);
    ojson result;
    result["rank_before"] = rank_before;
    result["rank_after"] = rank;
    result["star_point_before"] = points_before;
    result["star_point_after"] = balance;
    result["star_point_acquired"] = points;
    result["received_reward"] = received;
    return result;
}

}  // namespace

void register_character_mission(httplib::Server& svr) {
    svr.Post("/api/CharacterMissions/checkInitializeMissions",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    svr.Post("/api/CharacterMissions/:mCharacterBaseId/receiveAllMission",
             [](const httplib::Request& req, httplib::Response& res) {
                 int64_t m_character_base_id = std::stoll(req.path_params.at("mCharacterBaseId"));
                 try {
                     game_state::State s = game_state::transaction(req);
                     ojson result = claim_star_rewards(s, m_character_base_id);
                     s.commit();
                     pipeline::respond(res, "StarPointResult", result, ojson::array(), s.present());
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "StarPointResult", ojson::object());
                 }
             });

    svr.Post("/api/CharacterMissions/BulkReceiveAllMission",
             [](const httplib::Request& req, httplib::Response& res) {
                 (void)pipeline::read_request(req);  // payload (unused)
                 try {
                     game_state::State s = game_state::transaction(req);
                     ojson results = ojson::array();
                     // snapshot the base list: grant() may refetch CharacterBase mid-loop
                     std::vector<rjson> bases = s.rows("CharacterBase");
                     for (const rjson& base : bases) {
                         int64_t bid = base.at("characterBaseMasterId").get<int64_t>();
                         ojson result;
                         try {
                             result = claim_star_rewards(s, bid);
                         } catch (const game_state::Rejected&) {
                             continue;
                         }
                         ojson entry;
                         entry["character_base_master_id"] = bid;
                         entry["star_point_result"] = result;
                         results.push_back(entry);
                     }
                     s.commit();
                     pipeline::respond(res, "CharacterBaseStarPointResult", results, ojson::array(),
                                       s.present());
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "CharacterBaseStarPointResult", ojson::array());
                 }
             });

    svr.Post("/api/CharacterMissions/:mCharacterBaseId/receiveKeyMission",
             [](const httplib::Request& req, httplib::Response& res) {
                 int64_t m_character_base_id = std::stoll(req.path_params.at("mCharacterBaseId"));
                 try {
                     game_state::State s = game_state::transaction(req);
                     rjson* row =
                         s.one("CharacterBase", rjson{{"characterBaseMasterId", m_character_base_id}});
                     if (row == nullptr) throw game_state::Rejected();
                     const ojson* m = game_state::master(
                         "character_key_mission_master", row->at("keyMissionLevel").get<int64_t>() + 1,
                         "level");
                     if (m == nullptr) throw game_state::Rejected();
                     std::unordered_map<int64_t, int64_t> counts;
                     for (rjson& r : s.rows("CharacterMission"))
                         if (r.at("characterBaseMasterId").get<int64_t>() == m_character_base_id)
                             counts[r.at("characterMissionMasterId").get<int64_t>()] =
                                 r.at("currentCount").get<int64_t>();
                     int64_t m_level = m->at("level").get<int64_t>();
                     std::vector<int64_t> category_ids;
                     for (const ojson& x : master_data::table("CharacterMissionCategoryLevelMaster"))
                         if (x.at("level").get<int64_t>() == m_level)
                             category_ids.push_back(x.at("id_").get<int64_t>());
                     int64_t complete = 0;
                     for (int64_t category : category_ids) {
                         std::vector<std::pair<int64_t, int64_t>> goals;
                         for (const ojson& x : master_data::table("CharacterMissionMaster")) {
                             const ojson& stages = x.at("stages");
                             if (stages.is_array())
                                 for (const ojson& st : stages)
                                     if (st.at("character_mission_category_level_master_id")
                                             .get<int64_t>() == category)
                                         goals.emplace_back(x.at("id_").get<int64_t>(),
                                                            st.at("goal_count").get<int64_t>());
                         }
                         if (!goals.empty()) {
                             bool all_ok = true;
                             for (const auto& g : goals) {
                                 auto it = counts.find(g.first);
                                 int64_t cnt = it == counts.end() ? 0 : it->second;
                                 if (cnt < g.second) {
                                     all_ok = false;
                                     break;
                                 }
                             }
                             if (all_ok) complete += 1;
                         }
                     }
                     if (complete < m->at("required_category_count").get<int64_t>())
                         throw game_state::Rejected();
                     s.update("CharacterBase", row, rjson{{"keyMissionLevel", m_level}});
                     ojson result = game_state::star_points(s, m_character_base_id,
                                                            m->at("give_star_point").get<int64_t>());
                     s.commit();
                     pipeline::respond(res, "StarPointResult", result, ojson::array(), s.present());
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "StarPointResult", ojson::object());
                 }
             });
}

}  // namespace routes
