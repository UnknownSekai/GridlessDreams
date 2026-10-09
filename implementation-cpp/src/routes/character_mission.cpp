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

// ports routes/character_mission.py

namespace routes {
namespace {

using ojson = nlohmann::ordered_json;  // master rows + wire results
using rjson = nlohmann::json;          // db rows (camelCase columns)

// claim every pending character-mission stage reward for one base and rank it up. star points
// from a category are split evenly across that category's stages. returns a StarPointResult.
ojson claim_star_rewards(game_state::State& s, long long base_master_id) {
    rjson* base = s.one("CharacterBase", rjson{{"characterBaseMasterId", base_master_id}});
    if (base == nullptr) throw game_state::Rejected();
    long long rank_before = base->at("starRank").get<long long>();
    long long points_before = base->at("totalStarPoint").get<long long>();
    long long points = 0;
    std::unordered_map<long long, const ojson*> categories;
    for (const ojson& x : master_data::table("CharacterMissionCategoryLevelMaster"))
        categories[x.at("id_").get<long long>()] = &x;
    // sizes: how many stages each category spans across every mission
    std::unordered_map<long long, long long> sizes;
    for (const ojson& m : master_data::table("CharacterMissionMaster")) {
        const ojson& stages = m.at("stages");
        if (stages.is_array())
            for (const ojson& st : stages)
                sizes[st.at("character_mission_category_level_master_id").get<long long>()] += 1;
    }
    long long key_level = base->at("keyMissionLevel").get<long long>();
    for (rjson& row : s.rows("CharacterMission")) {
        if (row.at("characterBaseMasterId").get<long long>() != base_master_id) continue;
        const ojson* m = game_state::master("character_mission_master",
                                            row.at("characterMissionMasterId").get<long long>());
        std::vector<const ojson*> stages;
        const ojson& stages_arr = m->at("stages");
        if (stages_arr.is_array())
            for (const ojson& st : stages_arr) stages.push_back(&st);
        std::stable_sort(stages.begin(), stages.end(), [](const ojson* a, const ojson* b) {
            return a->at("stage_order").get<long long>() < b->at("stage_order").get<long long>();
        });
        std::vector<const ojson*> available;
        for (const ojson* st : stages) {
            long long cid = st->at("character_mission_category_level_master_id").get<long long>();
            if (categories.at(cid)->at("level").get<long long>() <= key_level + 1)
                available.push_back(st);
        }
        long long reward_received = row.at("rewardReceivedStageOrder").get<long long>();
        long long cleared_stage = row.at("clearedStageOrder").get<long long>();
        std::vector<const ojson*> pending;
        for (const ojson* st : available) {
            long long so = st->at("stage_order").get<long long>();
            if (reward_received < so && so <= cleared_stage) pending.push_back(st);
        }
        if (pending.empty()) continue;
        for (const ojson* st : pending) {
            long long cid = st->at("character_mission_category_level_master_id").get<long long>();
            points += categories.at(cid)->at("give_star_point").get<long long>() / sizes.at(cid);
        }
        long long claimed = pending.front()->at("stage_order").get<long long>();
        for (const ojson* st : pending)
            claimed = std::max(claimed, st->at("stage_order").get<long long>());
        // a completed category stays at its last stage until its next level unlocks
        const ojson* current = available.back();
        for (const ojson* st : available)
            if (st->at("stage_order").get<long long>() > claimed) {
                current = st;
                break;
            }
        long long completed = row.at("completedLevel").get<long long>();
        for (const ojson* st : available) {
            if (st->at("stage_order").get<long long>() > claimed) continue;
            long long cid = st->at("character_mission_category_level_master_id").get<long long>();
            bool has_later = false;
            for (const ojson* other : stages)
                if (other->at("stage_order").get<long long>() > claimed &&
                    other->at("character_mission_category_level_master_id").get<long long>() == cid) {
                    has_later = true;
                    break;
                }
            if (!has_later)
                completed = std::max(completed, categories.at(cid)->at("level").get<long long>());
        }
        s.update("CharacterMission", &row,
                 rjson{{"rewardReceivedStageOrder", claimed},
                       {"currentStageMasterId", current->at("id_").get<long long>()},
                       {"completedLevel", completed}});
    }
    long long rank = rank_before;
    long long balance = points_before + points;
    while (true) {
        const ojson* level = game_state::master("character_star_rank_master", rank, "rank");
        if (level == nullptr || level->at("next_rank_point").get<long long>() <= 0 ||
            balance < level->at("next_rank_point").get<long long>() ||
            game_state::master("character_star_rank_master", rank + 1, "rank") == nullptr)
            break;
        balance -= level->at("next_rank_point").get<long long>();
        rank += 1;
    }
    s.update("CharacterBase", base, rjson{{"starRank", rank}, {"totalStarPoint", balance}});
    ojson rewards = ojson::array();
    for (const ojson& r : master_data::table("StarRankRewardMaster")) {
        long long reward_rank = r.at("rank").get<long long>();
        if (r.at("character_base_master_id").get<long long>() == base_master_id &&
            rank_before < reward_rank && reward_rank <= rank) {
            const ojson* group =
                game_state::master("character_star_rank_reward_group_master",
                                   r.at("character_star_rank_reward_group_master_id").get<long long>());
            if (group != nullptr) {
                const ojson& grp_rewards = group->at("rewards");
                if (grp_rewards.is_array())
                    for (const ojson& t : grp_rewards)
                        rewards.push_back(ojson::array({t.at("thing_type").get<long long>(),
                                                        t.at("thing_id").get<long long>(),
                                                        t.at("thing_quantity").get<long long>()}));
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
                 long long m_character_base_id = std::stoll(req.path_params.at("mCharacterBaseId"));
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
                         long long bid = base.at("characterBaseMasterId").get<long long>();
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
                 long long m_character_base_id = std::stoll(req.path_params.at("mCharacterBaseId"));
                 try {
                     game_state::State s = game_state::transaction(req);
                     rjson* row =
                         s.one("CharacterBase", rjson{{"characterBaseMasterId", m_character_base_id}});
                     if (row == nullptr) throw game_state::Rejected();
                     const ojson* m = game_state::master(
                         "character_key_mission_master", row->at("keyMissionLevel").get<long long>() + 1,
                         "level");
                     if (m == nullptr) throw game_state::Rejected();
                     std::unordered_map<long long, long long> counts;
                     for (rjson& r : s.rows("CharacterMission"))
                         if (r.at("characterBaseMasterId").get<long long>() == m_character_base_id)
                             counts[r.at("characterMissionMasterId").get<long long>()] =
                                 r.at("currentCount").get<long long>();
                     long long m_level = m->at("level").get<long long>();
                     std::vector<long long> category_ids;
                     for (const ojson& x : master_data::table("CharacterMissionCategoryLevelMaster"))
                         if (x.at("level").get<long long>() == m_level)
                             category_ids.push_back(x.at("id_").get<long long>());
                     long long complete = 0;
                     for (long long category : category_ids) {
                         std::vector<std::pair<long long, long long>> goals;
                         for (const ojson& x : master_data::table("CharacterMissionMaster")) {
                             const ojson& stages = x.at("stages");
                             if (stages.is_array())
                                 for (const ojson& st : stages)
                                     if (st.at("character_mission_category_level_master_id")
                                             .get<long long>() == category)
                                         goals.emplace_back(x.at("id_").get<long long>(),
                                                            st.at("goal_count").get<long long>());
                         }
                         if (!goals.empty()) {
                             bool all_ok = true;
                             for (const auto& g : goals) {
                                 auto it = counts.find(g.first);
                                 long long cnt = it == counts.end() ? 0 : it->second;
                                 if (cnt < g.second) {
                                     all_ok = false;
                                     break;
                                 }
                             }
                             if (all_ok) complete += 1;
                         }
                     }
                     if (complete < m->at("required_category_count").get<long long>())
                         throw game_state::Rejected();
                     s.update("CharacterBase", row, rjson{{"keyMissionLevel", m_level}});
                     ojson result = game_state::star_points(s, m_character_base_id,
                                                            m->at("give_star_point").get<long long>());
                     s.commit();
                     pipeline::respond(res, "StarPointResult", result, ojson::array(), s.present());
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "StarPointResult", ojson::object());
                 }
             });
}

}  // namespace routes
