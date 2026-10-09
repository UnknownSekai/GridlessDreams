#include "routes/characters.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "helpers/character_enhance.h"
#include "helpers/character_level.h"
#include "helpers/game_state.h"
#include "helpers/user_data.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/characters.py

namespace routes {
namespace {

using json = wire::json;  // ordered_json: payloads, results, present

long long jint(const json& obj, const char* key, long long def = 0) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->get<long long>();
}

bool jbool(const json& obj, const char* key, bool def = false) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->get<bool>();
}

// one of the caller's characters by id, or none
std::optional<db::json> _character(long long user_id, long long character_id) {
    for (const db::json& c : db::fetch(db::user::get_characters(user_id)))
        if (c.at("id").get<long long>() == character_id) return c;
    return std::nullopt;
}

// everything the caller owns, {itemMasterId: stock}
std::map<long long, long long> _item_stock(long long user_id) {
    std::map<long long, long long> out;
    for (const db::json& i : db::fetch(db::user::get_items(user_id)))
        out[i.at("itemMasterId").get<long long>()] = i.at("stock").get<long long>();
    return out;
}

// charge a whole item bill in one statement; writes nothing unless every line is affordable
bool _pay_items(long long user_id, const std::map<long long, long long>& cost,
                std::optional<std::map<long long, long long>> stock = std::nullopt) {
    if (cost.empty()) return false;
    std::map<long long, long long> st = stock.has_value() ? *stock : _item_stock(user_id);
    for (const auto& [item, quantity] : cost) {
        auto it = st.find(item);
        long long have = it == st.end() ? 0 : it->second;
        if (have < quantity) return false;
    }
    std::vector<std::pair<long long, long long>> deltas;
    for (const auto& [i, q] : cost) deltas.emplace_back(i, -q);
    db::user::increment_item_stocks(user_id, deltas);
    return true;
}

// the exp items out of a stock -- what a level-up can spend
std::map<long long, long long> _experience_pool(const std::map<long long, long long>& stock) {
    std::map<long long, long long> out;
    for (const auto& [item, owned] : stock)
        if (character_level::experience_item(item).has_value()) out[item] = owned;
    return out;
}

// official one/two-item captures round each item before multiplying quantity
long long _actor_item_experience(const std::map<long long, long long>& costs, double bonus) {
    long long total = 0;
    for (const auto& [i, n] : costs) {
        std::optional<character_level::json> item = character_level::experience_item(i);
        total += n * static_cast<long long>(std::floor(
                         static_cast<double>(item->at("acquirable_experience").get<long long>()) *
                         (1.0 + bonus / 100.0)));
    }
    return total;
}

// the master curve is for rarity 4; lower rarities pay a fraction of each level's step
std::pair<long long, long long> _actor_experience(long long level, long long current, long long gained,
                                                  long long cap, long long rarity) {
    static const std::map<long long, double> factor_map = {{1, 0.3}, {2, 0.5}, {3, 0.8}, {4, 1.0}};
    double factor = factor_map.at(rarity);
    std::map<long long, long long> levels;
    for (const auto& x : master_data::table("CharacterLevelMaster"))
        levels[x.at("level").get<long long>()] = x.at("experience_to_level_up").get<long long>();
    long long experience = current + gained;
    while (level < cap) {
        long long need =
            static_cast<long long>(std::floor(static_cast<double>(levels.at(level)) * factor));
        if (need <= 0 || experience < need) break;
        experience -= need;
        level += 1;
    }
    return {level, experience};
}

}  // namespace

void register_characters(httplib::Server& svr) {
    // /api/Characters/{characterId}/AddExperience
    svr.Post("/api/Characters/:characterId/AddExperience",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     long long characterId = std::stoll(req.path_params.at("characterId"));
                     json payloads = pipeline::read_request_list(req, "UseExperienceItemsPayload");
                     std::map<long long, long long> costs;
                     long long raw = 0;
                     double bonus_gain = 0;
                     for (const json& entry : payloads) {
                         long long item_master_id = jint(entry, "item_master_id");
                         long long quantity = jint(entry, "quantity");
                         std::optional<character_level::json> item =
                             character_level::experience_item(item_master_id);
                         if (!item || quantity < 0) throw game_state::Rejected();
                         costs[item_master_id] += quantity;
                         raw += item->at("acquirable_experience").get<long long>() * quantity;
                         bonus_gain +=
                             item->at("acquirable_experience_bonus").get<double>() * quantity;
                     }
                     if (raw <= 0) throw game_state::Rejected();
                     game_state::State s = game_state::transaction(req);
                     {
                         game_state::json* row = s.one("Character", nlohmann::json{{"id", characterId}});
                         game_state::json* user = s.one("User");
                         game_state::json* bonus = s.one("UserBonus");
                         if (!row || !user || !bonus) throw game_state::Rejected();
                         const character_enhance::json* cm = character_enhance::character_master(
                             row->at("characterMasterId").get<long long>());
                         long long cap = std::min(user->at("playerRank").get<long long>(),
                                                  character_level::released_max_level());
                         if (row->at("level").get<long long>() >= cap) throw game_state::Rejected();
                         long long gained =
                             _actor_item_experience(costs, bonus->at("experienceBonus").get<double>());
                         auto [level, experience] = _actor_experience(
                             row->at("level").get<long long>(),
                             row->at("currentExperience").get<long long>(), gained, cap,
                             cm->at("rarity").get<long long>());
                         long long delta = level - row->at("level").get<long long>();
                         s.pay(costs);
                         s.update("Character", row,
                                  nlohmann::json{{"level", level}, {"currentExperience", experience}});
                         s.update("UserBonus", bonus,
                                  nlohmann::json{{"experienceBonus",
                                                  game_state::f32(bonus->at("experienceBonus").get<double>() +
                                                                  bonus_gain)}});
                         game_state::level_missions(s, *cm, delta, level);
                         s.commit();
                         json result = json::object();
                         result["is_success"] = true;
                         pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
                     }
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Characters/{characterId}/Awaken
    svr.Post("/api/Characters/:characterId/Awaken",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long characterId = std::stoll(req.path_params.at("characterId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id) {
                     pipeline::respond(res, "BooleanResult", json::object());
                     return;
                 }

                 std::map<long long, long long> cost;
                 {
                     auto tx = db::transaction();
                     std::optional<db::json> character = _character(*user_id, characterId);
                     const character_enhance::json* master =
                         character ? character_enhance::character_master(
                                         character->at("characterMasterId").get<long long>())
                                   : nullptr;
                     if (!character || !master) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("InvalidRequest")}));
                         return;
                     }
                     // 42 characters carry no awakening group at all, so their max phase is 0
                     if (character->at("awakeningPhase").get<long long>() >=
                         character_enhance::max_awakening_phase(*master)) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("InvalidRequest")}));
                         return;
                     }
                     std::optional<character_enhance::item_counts> c = character_enhance::awakening_cost(
                         *master, character->at("awakeningPhase").get<long long>());
                     if (!c) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("InvalidRequest")}));
                         return;
                     }
                     cost = *c;
                     if (!_pay_items(*user_id, cost)) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("NotEnoughThing")}));
                         return;
                     }
                     db::execute(db::user::update_character_awakening(
                         *user_id, characterId, character->at("awakeningPhase").get<long long>() + 1));
                     tx.commit();
                 }

                 std::vector<user_data::PresentSpec> updates;
                 updates.emplace_back("Character", std::vector<long long>{characterId});
                 std::vector<long long> item_ids;
                 for (const auto& [i, q] : cost) {
                     (void)q;
                     item_ids.push_back(i);
                 }
                 updates.emplace_back("Item", item_ids);
                 json present = user_data::build_present(user_id, updates);
                 json result = json::object();
                 result["is_success"] = true;
                 pipeline::respond(res, "BooleanResult", result, json::array(), present);
             });

    // /api/Characters/{characterId}/BloomTalent/{stageTo}
    svr.Post("/api/Characters/:characterId/BloomTalent/:stageTo",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     long long characterId = std::stoll(req.path_params.at("characterId"));
                     long long stageTo = std::stoll(req.path_params.at("stageTo"));
                     game_state::State s = game_state::transaction(req);
                     {
                         game_state::json* row = s.one("Character", nlohmann::json{{"id", characterId}});
                         if (!row) throw game_state::Rejected();
                         const character_enhance::json* cm = character_enhance::character_master(
                             row->at("characterMasterId").get<long long>());
                         long long talentStage = row->at("talentStage").get<long long>();
                         if (!cm || !(talentStage < stageTo &&
                                      stageTo <= character_enhance::max_talent_stage(*cm)))
                             throw game_state::Rejected();
                         std::map<long long, long long> stock;
                         for (const game_state::json& r : s.rows("Item"))
                             stock[r.at("itemMasterId").get<long long>()] = r.at("stock").get<long long>();
                         std::map<long long, long long> costs;
                         std::map<long long, long long> remaining = stock;
                         for (long long stage = talentStage; stage < stageTo; ++stage) {
                             const character_enhance::json* step = character_enhance::_bloom_step(
                                 cm->at("rarity").get<long long>(), stage);
                             const character_enhance::json* piece = character_enhance::_piece(
                                 cm->at("id_").get<long long>(),
                                 step->at("talent_bloom_item_type").get<long long>());
                             long long piece_item = piece->at("item_master_id").get<long long>();
                             long long required_piece = step->at("required_piece_amount").get<long long>();
                             long long have = remaining.count(piece_item) ? remaining[piece_item] : 0;
                             long long own = std::min(have, required_piece);
                             costs[piece_item] += own;
                             remaining[piece_item] = have - own;
                             long long shortfall = required_piece - own;
                             if (shortfall) {
                                 auto gp = character_enhance::_generic_piece(*cm, *step, *piece);
                                 std::optional<long long> generic = gp.first;
                                 if (!generic || *generic == 0 ||
                                     cm->value("forbid_generic_item_bloom", false))
                                     throw game_state::Rejected();
                                 costs[*generic] += shortfall;
                             }
                             long long required_item = jint(*step, "required_item_master_id");
                             if (required_item)
                                 costs[required_item] += jint(*step, "required_item_amount");
                         }
                         s.pay(costs);
                         long long delta = stageTo - talentStage;
                         std::vector<std::tuple<long long, long long, long long>> rewards =
                             character_enhance::bloom_rewards(*cm, talentStage, stageTo);
                         s.update("Character", row, nlohmann::json{{"talentStage", stageTo}});
                         game_state::ojson things = game_state::ojson::array();
                         for (const auto& [t, id, q] : rewards)
                             things.push_back(game_state::ojson::array({t, id, q}));
                         s.grant(things);
                         game_state::character_progress(
                             s, cm->at("character_base_master_id").get<long long>(), 6, delta);
                         s.commit();
                         json result = json::object();
                         result["is_success"] = true;
                         pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
                     }
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Characters/BulkLevelUp
    svr.Post("/api/Characters/BulkLevelUp", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        json payloads = pipeline::read_request_list(req, "BulkLevelUpPayload");
        if (!user_id || payloads.empty()) {
            pipeline::respond(res, "BooleanResult", json::object());
            return;
        }

        std::set<long long> leveled;
        std::map<long long, long long> spent;
        {
            auto tx = db::transaction();
            std::optional<db::json> user = db::fetchrow(db::user::get_users(*user_id));
            if (!user) {
                pipeline::respond(res, "BooleanResult", json::object(),
                                  json::array({pipeline::fault("InvalidRequest")}));
                return;
            }
            long long cap = character_level::level_cap(*user);
            std::unordered_map<long long, db::json> characters;
            for (const db::json& c : db::fetch(db::user::get_characters(*user_id)))
                characters[c.at("id").get<long long>()] = c;
            std::map<long long, long long> stock = _experience_pool(_item_stock(*user_id));

            // every entry draws on the one pool, so `order` is who gets first claim on it
            std::vector<json> sorted_payloads(payloads.begin(), payloads.end());
            std::stable_sort(sorted_payloads.begin(), sorted_payloads.end(),
                             [](const json& a, const json& b) {
                                 return jint(a, "order") < jint(b, "order");
                             });
            for (const json& entry : sorted_payloads) {
                auto cit = characters.find(jint(entry, "character_id"));
                if (cit == characters.end() || cit->second.at("level").get<long long>() >= cap)
                    continue;
                const db::json& character = cit->second;
                std::optional<long long> rarity =
                    character_level::character_rarity(character.at("characterMasterId").get<long long>());
                std::map<long long, long long> spend = character_level::spend_from_pool(
                    stock, character_level::experience_to_reach(
                               character.at("level").get<long long>(),
                               character.at("currentExperience").get<long long>(), cap, rarity));
                long long gained = 0;
                for (const auto& [item_master_id, quantity] : spend) {
                    stock[item_master_id] -= quantity;
                    spent[item_master_id] += quantity;
                    std::optional<character_level::json> master =
                        character_level::experience_item(item_master_id);
                    gained += master ? master->at("acquirable_experience").get<long long>() * quantity : 0;
                }
                if (!gained) continue;
                auto [level, experience] = character_level::apply_experience(
                    character.at("level").get<long long>(),
                    character.at("currentExperience").get<long long>(), gained, cap, rarity);
                db::execute(db::user::update_character_level(
                    *user_id, character.at("id").get<long long>(), level, experience));
                leveled.insert(character.at("id").get<long long>());
            }

            if (leveled.empty()) {  // nothing affordable -- no items were touched either
                pipeline::respond(res, "BooleanResult", json::object());
                return;
            }
            std::vector<std::pair<long long, long long>> deltas;
            for (const auto& [i, q] : spent) deltas.emplace_back(i, -q);
            db::user::increment_item_stocks(*user_id, deltas);
            tx.commit();
        }

        std::vector<user_data::PresentSpec> updates;
        updates.emplace_back("Character", std::vector<long long>(leveled.begin(), leveled.end()));
        std::vector<long long> item_ids;
        for (const auto& [i, q] : spent) {
            (void)q;
            item_ids.push_back(i);
        }
        updates.emplace_back("Item", item_ids);
        json present = user_data::build_present(user_id, updates);
        json result = json::object();
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, json::array(), present);
    });

    // /api/Characters/{characterId}/EnhanceSenseLevel/{levelTo}?priority=
    svr.Post("/api/Characters/:characterId/EnhanceSenseLevel/:levelTo",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long levelTo = std::stoll(req.path_params.at("levelTo"));
                 std::optional<long long> priority =
                     req.has_param("priority")
                         ? std::optional<long long>(std::stoll(req.get_param_value("priority")))
                         : std::nullopt;
                 long long pr = priority.value_or(1);
                 try {
                     long long characterId = std::stoll(req.path_params.at("characterId"));
                     game_state::State s = game_state::transaction(req);
                     {
                         game_state::json* row = s.one("Character", nlohmann::json{{"id", characterId}});
                         if (!row || (pr != 1 && pr != 2)) throw game_state::Rejected();
                         const character_enhance::json* cm = character_enhance::character_master(
                             row->at("characterMasterId").get<long long>());
                         std::string field = pr == 1 ? "senseLevel" : "secondarySenseLevel";
                         const char* bkey =
                             pr == 1 ? "character_base_master_id" : "secondary_character_base_master_id";
                         std::optional<long long> base;
                         {
                             auto bit = cm->find(bkey);
                             if (bit != cm->end() && !bit->is_null()) base = bit->get<long long>();
                         }
                         // the cost table starts at level 1, and so does a sense -- a secondary that
                         // was never initialised reads 0 but stands at that same first step, so clamp to 1
                         long long current = std::max<long long>(1, row->at(field).get<long long>());
                         if (!base || *base == 0 ||
                             !(current < levelTo && levelTo <= character_enhance::max_sense_level(*cm)))
                             throw game_state::Rejected();
                         std::optional<character_enhance::item_counts> costs =
                             character_enhance::sense_enhance_cost(*cm, current, levelTo);
                         if (!costs) throw game_state::Rejected();
                         long long delta = levelTo - current;
                         s.pay(*costs);
                         s.update("Character", row, nlohmann::json{{field, levelTo}});
                         game_state::character_progress(s, *base, 5, delta);
                         if (levelTo >= 2) game_state::mission_progress(s, 14, 1, true, 1);
                         s.commit();
                         json result = json::object();
                         result["is_success"] = true;
                         pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
                     }
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Characters/LinkCharacter?mCharacterBaseId=&linkedMCharacterBaseId=
    svr.Post("/api/Characters/LinkCharacter", [](const httplib::Request&, httplib::Response& res) {
        pipeline::respond(res, "", json::array());
    });

    // /api/Characters/ReceiveLinkCharacterReward?mCharacterBaseId=
    svr.Post("/api/Characters/ReceiveLinkCharacterReward",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "", json::array());
             });

    // /api/Characters/{characterMasterId}/ReleaseSideStory?order=
    svr.Post("/api/Characters/:characterMasterId/ReleaseSideStory",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long target =
                     req.has_param("order") ? std::stoll(req.get_param_value("order")) : 1;
                 try {
                     long long characterMasterId = std::stoll(req.path_params.at("characterMasterId"));
                     game_state::State s = game_state::transaction(req);
                     {
                         game_state::json* row = s.one(
                             "Character", nlohmann::json{{"characterMasterId", characterMasterId}});
                         const character_enhance::json* cm =
                             character_enhance::character_master(characterMasterId);
                         const character_enhance::json* episode = nullptr;
                         for (const auto& e : master_data::table("CharacterEpisodeMaster"))
                             if (e.at("character_master_id").get<long long>() == characterMasterId &&
                                 e.at("episode_order").get<long long>() == target) {
                                 episode = &e;
                                 break;
                             }
                         if (!row || !cm || !episode ||
                             row->at("level").get<long long>() <
                                 episode->at("required_character_level").get<long long>() ||
                             target != row->at("releasedEpisodeOrder").get<long long>() + 1)
                             throw game_state::Rejected();
                         long long gid =
                             target == 1 ? cm->at("first_episode_release_item_group_id").get<long long>()
                                         : cm->at("second_episode_release_item_group_id").get<long long>();
                         const game_state::ojson* group =
                             game_state::master("character_episode_release_item_group_master", gid);
                         if (!group) throw game_state::Rejected();
                         std::map<long long, long long> costs;
                         auto items = group->find("items");
                         if (items != group->end() && items->is_array())
                             for (const auto& i : *items)
                                 if (i.at("order").get<long long>() == target)
                                     costs[i.at("item_master_id").get<long long>()] +=
                                         i.at("required_quantity").get<long long>();
                         if (costs.empty()) throw game_state::Rejected();
                         s.pay(costs);
                         s.update("Character", row, nlohmann::json{{"releasedEpisodeOrder", target}});
                         s.commit();
                         json result = json::object();
                         result["is_success"] = true;
                         pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
                     }
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Characters/SetFavorite
    svr.Post("/api/Characters/SetFavorite", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "CharacterFavoritePayload");
        pipeline::respond(res, "BooleanResult", json::object());
    });

    // /api/CharacterBases/{characterBaseMasterId}/SetCostume/{costumeMasterId}
    svr.Post("/api/CharacterBases/:characterBaseMasterId/SetCostume/:costumeMasterId",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     long long characterBaseMasterId =
                         std::stoll(req.path_params.at("characterBaseMasterId"));
                     long long costumeMasterId = std::stoll(req.path_params.at("costumeMasterId"));
                     game_state::State s = game_state::transaction(req);
                     {
                         game_state::costume_owned(s, characterBaseMasterId, costumeMasterId);
                         game_state::json* base = s.one(
                             "CharacterBase",
                             nlohmann::json{{"characterBaseMasterId", characterBaseMasterId}});
                         s.update("CharacterBase", base,
                                  nlohmann::json{{"costumeMasterId", costumeMasterId}});
                         s.commit();
                         json result = json::object();
                         result["is_success"] = true;
                         pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
                     }
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Characters/Portal/SetCharacter
    svr.Post("/api/Characters/Portal/SetCharacter",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     json payload = pipeline::read_request(req, "ActorPortalCharacterPayload");
                     if (payload.is_null()) throw game_state::Rejected();
                     long long character_id = jint(payload, "character_id");
                     long long character_base_id = jint(payload, "character_base_id");
                     bool is_awakening = jbool(payload, "is_awakening");
                     game_state::State s = game_state::transaction(req);
                     {
                         if (!character_enhance::character_master(character_id) ||
                             !s.one("Character", nlohmann::json{{"characterMasterId", character_id}}))
                             throw game_state::Rejected();
                         game_state::json* base = s.one(
                             "CharacterBase",
                             nlohmann::json{{"characterBaseMasterId", character_base_id}});
                         s.update("CharacterBase", base,
                                  nlohmann::json{{"portalCharacterId", character_id},
                                                 {"portalDisplayAwakeningStatus", is_awakening}});
                         s.commit();
                         json result = json::object();
                         result["is_success"] = true;
                         pipeline::respond(res, "BooleanResult", result, json::array(), s.present());
                     }
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Characters/{characterId}/SwitchCharacterDisplayAwakeningStatusAsync
    svr.Post("/api/Characters/:characterId/SwitchCharacterDisplayAwakeningStatusAsync",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    // /api/Characters/UpdateSelectionType?characterId=&selectionType=
    svr.Post("/api/Characters/UpdateSelectionType",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });
}

}  // namespace routes
