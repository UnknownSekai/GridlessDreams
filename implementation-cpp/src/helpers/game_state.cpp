#include "game_state.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>
#include <vector>

#include "db/user.h"
#include "master_data.h"
#include "things.h"
#include "user_data.h"

namespace game_state {

namespace {

using UpsertFn = db::ExecutableQuery (*)(long long, const wire::json&);

// snake_case cache attribute name -> PascalCase on-disk header name
std::string to_pascal(const std::string& snake) {
    std::string out;
    bool up = true;
    for (char ch : snake) {
        if (ch == '_') {
            up = true;
            continue;
        }
        out += up ? static_cast<char>(std::toupper(static_cast<unsigned char>(ch))) : ch;
        up = false;
    }
    return out;
}

// getattr(db.user, "upsert_" + table) -- the per-table upsert builder, or nullptr
UpsertFn resolve_upsert(const std::string& table) {
    static const std::unordered_map<std::string, UpsertFn> kMap = {
        {"accessory", &db::user::upsert_accessory},
        {"accessory_auto_sell", &db::user::upsert_accessory_auto_sell},
        {"accessory_effect_master", &db::user::upsert_accessory_effect_master},
        {"accessory_level_pattern_group_master", &db::user::upsert_accessory_level_pattern_group_master},
        {"accessory_level_pattern_master", &db::user::upsert_accessory_level_pattern_master},
        {"accessory_master", &db::user::upsert_accessory_master},
        {"album", &db::user::upsert_album},
        {"album_page", &db::user::upsert_album_page},
        {"album_preset", &db::user::upsert_album_preset},
        {"album_theme", &db::user::upsert_album_theme},
        {"another_notation", &db::user::upsert_another_notation},
        {"audition_clear", &db::user::upsert_audition_clear},
        {"audition_master", &db::user::upsert_audition_master},
        {"audition_phase_master", &db::user::upsert_audition_phase_master},
        {"audition_reward_package_master", &db::user::upsert_audition_reward_package_master},
        {"bomb", &db::user::upsert_bomb},
        {"bomb_master", &db::user::upsert_bomb_master},
        {"bonus_live", &db::user::upsert_bonus_live},
        {"bonus_live_stage", &db::user::upsert_bonus_live_stage},
        {"buff_item_status", &db::user::upsert_buff_item_status},
        {"campaign_master", &db::user::upsert_campaign_master},
        {"character", &db::user::upsert_character},
        {"character_awakening_item_master", &db::user::upsert_character_awakening_item_master},
        {"character_base", &db::user::upsert_character_base},
        {"character_base_master", &db::user::upsert_character_base_master},
        {"character_bloom_bonus_group_master", &db::user::upsert_character_bloom_bonus_group_master},
        {"character_bloom_item_master", &db::user::upsert_character_bloom_item_master},
        {"character_experience_item_master", &db::user::upsert_character_experience_item_master},
        {"character_lesson", &db::user::upsert_character_lesson},
        {"character_lesson_slot", &db::user::upsert_character_lesson_slot},
        {"character_level_master", &db::user::upsert_character_level_master},
        {"character_master", &db::user::upsert_character_master},
        {"character_mission", &db::user::upsert_character_mission},
        {"character_mission_master", &db::user::upsert_character_mission_master},
        {"character_mission_stage_master", &db::user::upsert_character_mission_stage_master},
        {"character_piece_master", &db::user::upsert_character_piece_master},
        {"character_point_event", &db::user::upsert_character_point_event},
        {"character_sense_enhance_item_group_master", &db::user::upsert_character_sense_enhance_item_group_master},
        {"character_star_rank_master", &db::user::upsert_character_star_rank_master},
        {"character_star_rank_reward_group_master", &db::user::upsert_character_star_rank_reward_group_master},
        {"circle_event_mission", &db::user::upsert_circle_event_mission},
        {"comeback_campaign", &db::user::upsert_comeback_campaign},
        {"comic", &db::user::upsert_comic},
        {"company_master", &db::user::upsert_company_master},
        {"concert_stage", &db::user::upsert_concert_stage},
        {"connect_with_account", &db::user::upsert_connect_with_account},
        {"connect_with_password", &db::user::upsert_connect_with_password},
        {"costume", &db::user::upsert_costume},
        {"costume_master", &db::user::upsert_costume_master},
        {"costume_wearable_character_group_master", &db::user::upsert_costume_wearable_character_group_master},
        {"currency", &db::user::upsert_currency},
        {"daily_lesson", &db::user::upsert_daily_lesson},
        {"daily_limit", &db::user::upsert_daily_limit},
        {"decoration", &db::user::upsert_decoration},
        {"dugong_run", &db::user::upsert_dugong_run},
        {"effect_duration_group_master", &db::user::upsert_effect_duration_group_master},
        {"effect_master", &db::user::upsert_effect_master},
        {"episode", &db::user::upsert_episode},
        {"episode_master", &db::user::upsert_episode_master},
        {"episode_reward_package_master", &db::user::upsert_episode_reward_package_master},
        {"event", &db::user::upsert_event},
        {"event_box_gacha", &db::user::upsert_event_box_gacha},
        {"event_box_gacha_box_thing", &db::user::upsert_event_box_gacha_box_thing},
        {"event_camp", &db::user::upsert_event_camp},
        {"exchange_limit", &db::user::upsert_exchange_limit},
        {"exchange_shop_master", &db::user::upsert_exchange_shop_master},
        {"favorite_costume", &db::user::upsert_favorite_costume},
        {"flash_sale_stage", &db::user::upsert_flash_sale_stage},
        {"friend_invitation", &db::user::upsert_friend_invitation},
        {"friend_invitation_mission", &db::user::upsert_friend_invitation_mission},
        {"gacha", &db::user::upsert_gacha},
        {"gacha_re_roll", &db::user::upsert_gacha_re_roll},
        {"gacha_selected_thing", &db::user::upsert_gacha_selected_thing},
        {"game_hint", &db::user::upsert_game_hint},
        {"gradual_mission_group", &db::user::upsert_gradual_mission_group},
        {"home_b_g_m", &db::user::upsert_home_b_g_m},
        {"home_character_voice_master", &db::user::upsert_home_character_voice_master},
        {"home_display_preference", &db::user::upsert_home_display_preference},
        {"home_skin", &db::user::upsert_home_skin},
        {"icon_frame", &db::user::upsert_icon_frame},
        {"inbox", &db::user::upsert_inbox},
        {"item", &db::user::upsert_item},
        {"item_master", &db::user::upsert_item_master},
        {"jewel_shop", &db::user::upsert_jewel_shop},
        {"league_basic", &db::user::upsert_league_basic},
        {"league_group", &db::user::upsert_league_group},
        {"league_group_member", &db::user::upsert_league_group_member},
        {"league_high_score_party", &db::user::upsert_league_high_score_party},
        {"league_high_score_party_slot", &db::user::upsert_league_high_score_party_slot},
        {"league_history", &db::user::upsert_league_history},
        {"league_season_result", &db::user::upsert_league_season_result},
        {"limit", &db::user::upsert_limit},
        {"link_character", &db::user::upsert_link_character},
        {"live", &db::user::upsert_live},
        {"live_achievement", &db::user::upsert_live_achievement},
        {"live_drop_celling", &db::user::upsert_live_drop_celling},
        {"live_drop_limit", &db::user::upsert_live_drop_limit},
        {"live_master", &db::user::upsert_live_master},
        {"live_setting_master", &db::user::upsert_live_setting_master},
        {"login_pass_status", &db::user::upsert_login_pass_status},
        {"lottery", &db::user::upsert_lottery},
        {"market", &db::user::upsert_market},
        {"mission", &db::user::upsert_mission},
        {"mission_master", &db::user::upsert_mission_master},
        {"mission_pass", &db::user::upsert_mission_pass},
        {"mission_pass_detail_master", &db::user::upsert_mission_pass_detail_master},
        {"mission_pass_master", &db::user::upsert_mission_pass_master},
        {"multi_room_basic", &db::user::upsert_multi_room_basic},
        {"music", &db::user::upsert_music},
        {"music_bookmark", &db::user::upsert_music_bookmark},
        {"music_course", &db::user::upsert_music_course},
        {"music_course_ranking", &db::user::upsert_music_course_ranking},
        {"music_master", &db::user::upsert_music_master},
        {"music_video", &db::user::upsert_music_video},
        {"music_vocal_version_master", &db::user::upsert_music_vocal_version_master},
        {"name_base_color", &db::user::upsert_name_base_color},
        {"name_color", &db::user::upsert_name_color},
        {"name_color_master", &db::user::upsert_name_color_master},
        {"nameplate", &db::user::upsert_nameplate},
        {"nameplate_master", &db::user::upsert_nameplate_master},
        {"note", &db::user::upsert_note},
        {"note_master", &db::user::upsert_note_master},
        {"notification", &db::user::upsert_notification},
        {"party", &db::user::upsert_party},
        {"party_slot", &db::user::upsert_party_slot},
        {"permanent_market_thing", &db::user::upsert_permanent_market_thing},
        {"photo", &db::user::upsert_photo},
        {"pickup_character_mission", &db::user::upsert_pickup_character_mission},
        {"poster", &db::user::upsert_poster},
        {"poster_level_pattern_group_master", &db::user::upsert_poster_level_pattern_group_master},
        {"poster_level_pattern_master", &db::user::upsert_poster_level_pattern_master},
        {"poster_master", &db::user::upsert_poster_master},
        {"poster_release_item_group_master", &db::user::upsert_poster_release_item_group_master},
        {"poster_release_item_master", &db::user::upsert_poster_release_item_master},
        {"poster_story_master", &db::user::upsert_poster_story_master},
        {"random_effect_group_master", &db::user::upsert_random_effect_group_master},
        {"restriction", &db::user::upsert_restriction},
        {"reward_rule_master", &db::user::upsert_reward_rule_master},
        {"roulette", &db::user::upsert_roulette},
        {"roulette_event", &db::user::upsert_roulette_event},
        {"sense_effect_master", &db::user::upsert_sense_effect_master},
        {"sense_master", &db::user::upsert_sense_master},
        {"sp_rate", &db::user::upsert_sp_rate},
        {"special_event", &db::user::upsert_special_event},
        {"spot_conversation_master", &db::user::upsert_spot_conversation_master},
        {"stamp", &db::user::upsert_stamp},
        {"stamp_master", &db::user::upsert_stamp_master},
        {"star_pass_status", &db::user::upsert_star_pass_status},
        {"star_rank_reward_master", &db::user::upsert_star_rank_reward_master},
        {"story_event", &db::user::upsert_story_event},
        {"story_event_circle", &db::user::upsert_story_event_circle},
        {"story_event_circle_mission", &db::user::upsert_story_event_circle_mission},
        {"story_event_circle_mission_reward", &db::user::upsert_story_event_circle_mission_reward},
        {"story_event_high_score", &db::user::upsert_story_event_high_score},
        {"story_event_high_score_buff_setting", &db::user::upsert_story_event_high_score_buff_setting},
        {"story_event_high_score_party", &db::user::upsert_story_event_high_score_party},
        {"story_event_high_score_party_slot", &db::user::upsert_story_event_high_score_party_slot},
        {"story_master", &db::user::upsert_story_master},
        {"theater_story", &db::user::upsert_theater_story},
        {"time_limited_control", &db::user::upsert_time_limited_control},
        {"total_point_event", &db::user::upsert_total_point_event},
        {"tournament_detail", &db::user::upsert_tournament_detail},
        {"tournament_qualifying", &db::user::upsert_tournament_qualifying},
        {"trial_party_event", &db::user::upsert_trial_party_event},
        {"trial_party_event_stage", &db::user::upsert_trial_party_event_stage},
        {"trial_party_event_stage_party", &db::user::upsert_trial_party_event_stage_party},
        {"trial_party_event_stage_party_slot", &db::user::upsert_trial_party_event_stage_party_slot},
        {"triple_cast_basic", &db::user::upsert_triple_cast_basic},
        {"triple_cast_group", &db::user::upsert_triple_cast_group},
        {"triple_cast_group_member", &db::user::upsert_triple_cast_group_member},
        {"triple_cast_high_score_party", &db::user::upsert_triple_cast_high_score_party},
        {"triple_cast_high_score_party_slot", &db::user::upsert_triple_cast_high_score_party_slot},
        {"triple_cast_history", &db::user::upsert_triple_cast_history},
        {"triple_cast_party", &db::user::upsert_triple_cast_party},
        {"triple_cast_party_slot", &db::user::upsert_triple_cast_party_slot},
        {"triple_cast_season_result", &db::user::upsert_triple_cast_season_result},
        {"trophy", &db::user::upsert_trophy},
        {"trophy_group_master", &db::user::upsert_trophy_group_master},
        {"trophy_master", &db::user::upsert_trophy_master},
        {"user", &db::user::upsert_user},
        {"user_block", &db::user::upsert_user_block},
        {"user_bonus", &db::user::upsert_user_bonus},
        {"user_preference", &db::user::upsert_user_preference},
        {"user_profile", &db::user::upsert_user_profile},
        {"viewed_shop", &db::user::upsert_viewed_shop},
    };
    auto it = kMap.find(table);
    return it == kMap.end() ? nullptr : it->second;
}

// self.dirty[(entity, pk)] = row -- replace the value in place if present (python dicts keep
// first-insertion position on reassignment), else append in first-touch order
void record_dirty(std::vector<std::pair<std::pair<std::string, long long>, json>>& dirty,
                  const std::string& entity, long long pk, const json& row) {
    for (auto& e : dirty)
        if (e.first.first == entity && e.first.second == pk) {
            e.second = row;
            return;
        }
    dirty.emplace_back(std::make_pair(entity, pk), row);
}

}  // namespace

const std::unordered_map<std::string, std::string> State::KEY_FIELD = {
    {"Episode", "episodeMasterId"},
    {"ConcertStage", "concertStageMasterId"},
    {"HomeBGM", "homeBGMMasterId"},
    {"CharacterLesson", "characterBaseMasterId"},
};

const ojson* master(const std::string& table, long long ident, const std::string& field) {
    for (const ojson& x : master_data::table(to_pascal(table))) {
        auto it = x.find(field);
        if (it != x.end() && *it == ident) return &x;
    }
    return nullptr;
}

double f32(double value) {
    return static_cast<double>(static_cast<float>(value));
}

State::State(long long uid) : uid(uid), tx_(db::transaction()) {}

std::vector<json>& State::rows(const std::string& name) {
    auto it = tables.find(name);
    if (it == tables.end()) {
        // table names come only from the implementation, never the request
        std::string sql = "SELECT * FROM \"" + user_data::_table(name) + "\" WHERE \"userId\"=$1";
        std::vector<json> fetched = db::fetch(db::SelectQuery(nullptr, sql, uid));
        it = tables.emplace(name, std::move(fetched)).first;
    }
    return it->second;
}

json* State::one(const std::string& name, const json& where) {
    for (json& r : rows(name)) {
        bool match = true;
        for (auto it = where.begin(); it != where.end(); ++it)
            if (r.value(it.key(), json(nullptr)) != it.value()) {
                match = false;
                break;
            }
        if (match) return &r;
    }
    return nullptr;
}

std::string State::key_field(const std::string& name) {
    auto it = KEY_FIELD.find(name);
    return it != KEY_FIELD.end() ? it->second : "id";
}

void State::update(const std::string& entity, json* row, const json& values) {
    if (row == nullptr) throw Rejected("Missing account record");
    if (values.empty()) return;
    const std::string pk = key_field(entity);
    std::string columns;
    int i = 0;
    for (auto it = values.begin(); it != values.end(); ++it) {
        if (i) columns += ", ";
        columns += "\"" + it.key() + "\"=$" + std::to_string(i + 3);
        ++i;
    }
    db::ExecutableQuery q;
    q.sql = "UPDATE \"" + user_data::_table(entity) + "\" SET " + columns +
            " WHERE \"userId\"=$1 AND \"" + pk + "\"=$2";
    q.args.push_back(json(uid));
    q.args.push_back(row->at(pk));
    for (auto it = values.begin(); it != values.end(); ++it) q.args.push_back(it.value());
    db::execute(q);
    row->update(values);
    record_dirty(dirty, entity, row->at(pk).get<long long>(), *row);
}

json State::insert(const std::string& entity, json values) {
    std::vector<json>& rws = rows(entity);
    const std::string pk = key_field(entity);
    if (pk == "id" && !values.contains("id")) {
        long long mx = 0;
        for (const json& r : rws) mx = std::max(mx, r.at("id").get<long long>());
        values["id"] = mx + 1;
    }
    UpsertFn fn = resolve_upsert(user_data::_table(entity));
    if (fn == nullptr) throw std::runtime_error("no upsert query for " + entity);
    db::execute(fn(uid, values));
    std::string sql = "SELECT * FROM \"" + user_data::_table(entity) +
                      "\" WHERE \"userId\"=$1 AND \"" + pk + "\"=$2";
    json row = db::fetchrow(db::SelectQuery(nullptr, sql, uid, values.at(pk))).value();
    rws.push_back(row);
    record_dirty(dirty, entity, row.at(pk).get<long long>(), row);
    return row;
}

void State::pay(const std::map<long long, long long>& costs, long long coin) {
    bool any_negative = false;
    for (const auto& kv : costs)
        if (kv.second < 0) {
            any_negative = true;
            break;
        }
    if (coin < 0 || any_negative) throw Rejected("Invalid cost");
    std::unordered_map<long long, json*> items;
    for (json& r : rows("Item")) items[r.at("itemMasterId").get<long long>()] = &r;
    json* currency = one("Currency");
    bool insufficient = false;
    for (const auto& kv : costs) {
        long long stock = 0;
        auto it = items.find(kv.first);
        if (it != items.end()) stock = it->second->value("stock", static_cast<long long>(0));
        if (stock < kv.second) {
            insufficient = true;
            break;
        }
    }
    if (!insufficient && coin != 0 &&
        (currency == nullptr || currency->at("coin").get<long long>() < coin))
        insufficient = true;
    if (insufficient) throw Rejected("Insufficient resources");
    for (const auto& kv : costs)
        if (kv.second != 0) {
            json* row = items.at(kv.first);
            update("Item", row, json{{"stock", row->at("stock").get<long long>() - kv.second}});
        }
    if (coin != 0)
        update("Currency", currency, json{{"coin", currency->at("coin").get<long long>() - coin}});
}

ojson State::grant(const ojson& things) {
    if (!things.is_array() || things.empty()) return ojson::array();
    // the per-user types this grant can touch (python set, deduped; first-seen order here)
    std::vector<std::string> names;
    for (const ojson& triple : things) {
        std::optional<std::string> name = things::present_type(triple[0].get<long long>());
        if (name && std::find(names.begin(), names.end(), *name) == names.end())
            names.push_back(*name);
    }
    std::unordered_map<std::string, std::unordered_map<long long, json>> snapshots;
    for (const std::string& name : names) {
        std::unordered_map<long long, json>& snap = snapshots[name];
        for (json& r : rows(name)) snap[r.at("id").get<long long>()] = r;
    }
    std::vector<things::ThingTriple> triples;
    triples.reserve(things.size());
    for (const ojson& triple : things)
        triples.emplace_back(triple[0].get<long long>(), triple[1].get<long long>(),
                             triple[2].get<long long>());
    std::vector<things::json> result = things::grant_things_consolidated(uid, triples);
    // grant helpers can create several resource types; reload just the affected rows
    for (const std::string& name : names) {
        const std::unordered_map<long long, json>& before = snapshots[name];
        tables.erase(name);
        for (json& r : rows(name)) {
            long long id = r.at("id").get<long long>();
            auto bit = before.find(id);
            if (bit == before.end() || bit->second != r)
                record_dirty(dirty, name, r.at(key_field(name)).get<long long>(), r);
        }
    }
    ojson out = ojson::array();
    for (const things::json& rt : result) out.push_back(rt);
    return out;
}

ojson State::present() const {
    ojson out = ojson::array();
    for (const auto& e : dirty)
        out.push_back(user_data::data_object(e.first.first, e.second));
    return out;
}

void State::commit() {
    tx_.commit();
}

State transaction(const httplib::Request& request) {
    std::optional<long long> uid = user_data::current_user_id(request);
    if (!uid.has_value()) throw Rejected("Authentication required");
    return State(*uid);
}

void character_progress(State& s, long long base, long long mission_id, long long delta) {
    if (delta <= 0) return;
    const ojson* m = master("character_mission_master", mission_id);
    if (m == nullptr) return;
    std::vector<const ojson*> stages;
    const ojson& stages_arr = m->at("stages");
    if (stages_arr.is_array())
        for (const ojson& st : stages_arr) stages.push_back(&st);
    std::stable_sort(stages.begin(), stages.end(), [](const ojson* a, const ojson* b) {
        return a->at("stage_order").get<long long>() < b->at("stage_order").get<long long>();
    });
    if (stages.empty()) return;
    json* row = s.one("CharacterMission", json{{"characterBaseMasterId", base},
                                               {"characterMissionMasterId", mission_id}});
    long long count = (row != nullptr ? row->at("currentCount").get<long long>() : 0) + delta;
    long long cleared = 0;
    for (const ojson* st : stages)
        if (st->at("goal_count").get<long long>() <= count)
            cleared = std::max(cleared, st->at("stage_order").get<long long>());
    if (row != nullptr) {
        long long cleared_now = std::max(cleared, row->at("clearedStageOrder").get<long long>());
        s.update("CharacterMission", row,
                 json{{"currentCount", count}, {"clearedStageOrder", cleared_now}});
    } else {
        s.insert("CharacterMission", json{{"characterBaseMasterId", base},
                                          {"characterMissionMasterId", mission_id},
                                          {"currentStageMasterId", stages.front()->at("id_").get<long long>()},
                                          {"currentCount", count},
                                          {"clearedStageOrder", cleared},
                                          {"rewardReceivedStageOrder", 0},
                                          {"completedLevel", 0}});
    }
}

void mission_progress(State& s, long long ident, long long delta, bool create,
                      std::optional<long long> absolute) {
    const ojson* m = master("mission_master", ident);
    if (m == nullptr) return;
    const ojson& stages = m->at("stages");
    if (!stages.is_array() || stages.empty()) return;
    json* row = s.one("Mission", json{{"missionMasterId", ident}});
    if (row == nullptr && !create) return;
    const ojson* stage = &stages[0];
    if (row != nullptr) {
        long long current = row->at("currentMissionStageMasterId").get<long long>();
        for (const ojson& x : stages)
            if (x.at("id_").get<long long>() == current) {
                stage = &x;
                break;
            }
    }
    long long old = row != nullptr ? row->at("missionCurrentCount").get<long long>() : 0;
    long long count = absolute.has_value() ? std::max(old, *absolute) : old + delta;
    bool cleared = count >= stage->at("stage_goal_value").get<long long>();
    bool newly = cleared && (row == nullptr || !row->at("isCleared").get<bool>());
    if (row != nullptr) {
        bool was_cleared = row->at("isCleared").get<bool>();
        s.update("Mission", row,
                 json{{"missionCurrentCount", count}, {"isCleared", was_cleared || cleared}});
    } else {
        s.insert("Mission", json{{"missionMasterId", ident},
                                 {"currentMissionStageMasterId", stage->at("id_").get<long long>()},
                                 {"missionCurrentCount", count},
                                 {"isCleared", cleared},
                                 {"isRewardReceived", false}});
    }
    if (newly && 1 <= ident && ident < 42) {
        mission_progress(s, 42);
        if (ident % 6) mission_progress(s, ((ident - 1) / 6 + 1) * 6);
    }
}

void costume_owned(State& s, long long base, long long ident) {
    const ojson* b = master("character_base_master", base);
    const ojson* c = master("costume_master", ident);
    if (b == nullptr || c == nullptr ||
        s.one("CharacterBase", json{{"characterBaseMasterId", base}}) == nullptr)
        throw Rejected();
    const ojson* group = master("costume_group_master", c->at("costume_group_master_id").get<long long>());
    const ojson* wearable =
        group != nullptr
            ? master("costume_wearable_character_group_master",
                     group->at("costume_wearable_character_group_master_id").get<long long>())
            : nullptr;
    if (wearable != nullptr) {
        bool allowed = false;
        const ojson& ids = wearable->at("character_base_master_ids");
        if (ids.is_array())
            for (const ojson& v : ids)
                if (v.get<long long>() == base) {
                    allowed = true;
                    break;
                }
        if (!allowed) throw Rejected();
    }
    if (!c->at("is_default").get<bool>() &&
        b->at("default_costume_master_id").get<long long>() != ident &&
        s.one("Costume", json{{"costumeMasterId", ident}}) == nullptr)
        throw Rejected();
}

ojson star_points(State& s, long long base, long long points) {
    json* row = s.one("CharacterBase", json{{"characterBaseMasterId", base}});
    if (row == nullptr) throw Rejected();
    long long before = row->at("starRank").get<long long>();
    long long rank = before;
    long long old = row->at("totalStarPoint").get<long long>();
    long long balance = old + points;
    while (true) {
        const ojson* m = master("character_star_rank_master", rank, "rank");
        if (m == nullptr || m->at("next_rank_point").get<long long>() <= 0 ||
            balance < m->at("next_rank_point").get<long long>() ||
            master("character_star_rank_master", rank + 1, "rank") == nullptr)
            break;
        balance -= m->at("next_rank_point").get<long long>();
        rank += 1;
    }
    s.update("CharacterBase", row, json{{"starRank", rank}, {"totalStarPoint", balance}});
    ojson things = ojson::array();
    for (const ojson& r : master_data::table("StarRankRewardMaster")) {
        long long reward_rank = r.at("rank").get<long long>();
        if (r.at("character_base_master_id").get<long long>() == base && before < reward_rank &&
            reward_rank <= rank) {
            const ojson* group =
                master("character_star_rank_reward_group_master",
                       r.at("character_star_rank_reward_group_master_id").get<long long>());
            if (group != nullptr) {
                const ojson& rewards = group->at("rewards");
                if (rewards.is_array())
                    for (const ojson& x : rewards)
                        things.push_back(ojson::array({x.at("thing_type").get<long long>(),
                                                       x.at("thing_id").get<long long>(),
                                                       x.at("thing_quantity").get<long long>()}));
            }
        }
    }
    ojson received = s.grant(things);
    ojson result;
    result["rank_before"] = before;
    result["rank_after"] = rank;
    result["star_point_before"] = old;
    result["star_point_after"] = balance;
    result["star_point_acquired"] = points;
    result["received_reward"] = received;
    return result;
}

void level_missions(State& s, const ojson& cm, long long delta, long long level) {
    if (delta <= 0) return;
    long long base_master_id = cm.at("character_base_master_id").get<long long>();
    const ojson* base = master("character_base_master", base_master_id);
    character_progress(s, base_master_id, 3, delta);
    mission_progress(s, 1200, delta);
    if (base != nullptr)
        mission_progress(s, base->at("company_master_id").get<long long>() * 100 + 20, delta);
    for (long long mid : {static_cast<long long>(300030), static_cast<long long>(300100)})
        mission_progress(s, mid, delta);
    if (level >= 10) mission_progress(s, 8, 1, true, 1);
}

}  // namespace game_state
