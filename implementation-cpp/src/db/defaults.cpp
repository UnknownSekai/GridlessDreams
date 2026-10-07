#include "db/defaults.h"

#include <cctype>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "config.h"
#include "db.h"
#include "db/user.h"
#include "helpers/shops.h"
#include "platform.h"
#include "user_hash.h"
#include "wire.h"

// Seed a brand-new account's default data (ports db/defaults.py). default_account.json holds a
// fresh account's initial entities as camelCase DB rows with no identity baked in; each row is
// replayed through the matching db::user upsert_* builder for the new userId.

namespace db::defaults {

namespace {

using UpsertFn = ExecutableQuery (*)(std::int64_t, const wire::json&);

// entities that are 1:1 with the account -- their id is the userId
const std::unordered_set<std::string> ACCOUNT_ENTITIES = {
    "User",       "UserProfile", "HomeDisplayPreference", "UserPreference",
    "UserBonus",  "Currency",    "Restriction",           "Notification",
};

constexpr std::int64_t INBOX_TTL_MICROS = 30LL * 86400 * 1000000;  // claimable for 30 days

// getattr(db_user, f"upsert_{table}", None): the whole db.user upsert namespace keyed by table.
const std::unordered_map<std::string, UpsertFn> UPSERTS = {
    {"user", &user::upsert_user},
    {"user_profile", &user::upsert_user_profile},
    {"user_preference", &user::upsert_user_preference},
    {"home_display_preference", &user::upsert_home_display_preference},
    {"character", &user::upsert_character},
    {"character_base", &user::upsert_character_base},
    {"party", &user::upsert_party},
    {"party_slot", &user::upsert_party_slot},
    {"character_master", &user::upsert_character_master},
    {"character_base_master", &user::upsert_character_base_master},
    {"character_level_master", &user::upsert_character_level_master},
    {"poster", &user::upsert_poster},
    {"accessory_level_pattern_group_master", &user::upsert_accessory_level_pattern_group_master},
    {"accessory_level_pattern_master", &user::upsert_accessory_level_pattern_master},
    {"accessory_master", &user::upsert_accessory_master},
    {"episode_master", &user::upsert_episode_master},
    {"episode_reward_package_master", &user::upsert_episode_reward_package_master},
    {"live_master", &user::upsert_live_master},
    {"music_master", &user::upsert_music_master},
    {"sense_master", &user::upsert_sense_master},
    {"story_master", &user::upsert_story_master},
    {"poster_level_pattern_group_master", &user::upsert_poster_level_pattern_group_master},
    {"poster_level_pattern_master", &user::upsert_poster_level_pattern_master},
    {"poster_master", &user::upsert_poster_master},
    {"live", &user::upsert_live},
    {"music", &user::upsert_music},
    {"accessory", &user::upsert_accessory},
    {"item", &user::upsert_item},
    {"accessory_effect_master", &user::upsert_accessory_effect_master},
    {"company_master", &user::upsert_company_master},
    {"effect_duration_group_master", &user::upsert_effect_duration_group_master},
    {"effect_master", &user::upsert_effect_master},
    {"item_master", &user::upsert_item_master},
    {"random_effect_group_master", &user::upsert_random_effect_group_master},
    {"reward_rule_master", &user::upsert_reward_rule_master},
    {"sense_effect_master", &user::upsert_sense_effect_master},
    {"trophy_group_master", &user::upsert_trophy_group_master},
    {"trophy_master", &user::upsert_trophy_master},
    {"character_lesson", &user::upsert_character_lesson},
    {"daily_lesson", &user::upsert_daily_lesson},
    {"inbox", &user::upsert_inbox},
    {"bomb", &user::upsert_bomb},
    {"costume", &user::upsert_costume},
    {"name_color", &user::upsert_name_color},
    {"nameplate", &user::upsert_nameplate},
    {"note", &user::upsert_note},
    {"stamp", &user::upsert_stamp},
    {"mission", &user::upsert_mission},
    {"audition_master", &user::upsert_audition_master},
    {"bomb_master", &user::upsert_bomb_master},
    {"character_star_rank_master", &user::upsert_character_star_rank_master},
    {"character_star_rank_reward_group_master", &user::upsert_character_star_rank_reward_group_master},
    {"costume_master", &user::upsert_costume_master},
    {"home_character_voice_master", &user::upsert_home_character_voice_master},
    {"name_color_master", &user::upsert_name_color_master},
    {"nameplate_master", &user::upsert_nameplate_master},
    {"note_master", &user::upsert_note_master},
    {"spot_conversation_master", &user::upsert_spot_conversation_master},
    {"stamp_master", &user::upsert_stamp_master},
    {"character_lesson_slot", &user::upsert_character_lesson_slot},
    {"trophy", &user::upsert_trophy},
    {"market", &user::upsert_market},
    {"viewed_shop", &user::upsert_viewed_shop},
    {"game_hint", &user::upsert_game_hint},
    {"user_bonus", &user::upsert_user_bonus},
    {"audition_phase_master", &user::upsert_audition_phase_master},
    {"audition_reward_package_master", &user::upsert_audition_reward_package_master},
    {"campaign_master", &user::upsert_campaign_master},
    {"character_awakening_item_master", &user::upsert_character_awakening_item_master},
    {"character_bloom_bonus_group_master", &user::upsert_character_bloom_bonus_group_master},
    {"character_bloom_item_master", &user::upsert_character_bloom_item_master},
    {"character_experience_item_master", &user::upsert_character_experience_item_master},
    {"character_mission_master", &user::upsert_character_mission_master},
    {"character_mission_stage_master", &user::upsert_character_mission_stage_master},
    {"character_piece_master", &user::upsert_character_piece_master},
    {"character_sense_enhance_item_group_master", &user::upsert_character_sense_enhance_item_group_master},
    {"costume_wearable_character_group_master", &user::upsert_costume_wearable_character_group_master},
    {"exchange_shop_master", &user::upsert_exchange_shop_master},
    {"live_setting_master", &user::upsert_live_setting_master},
    {"mission_master", &user::upsert_mission_master},
    {"music_vocal_version_master", &user::upsert_music_vocal_version_master},
    {"poster_release_item_group_master", &user::upsert_poster_release_item_group_master},
    {"poster_release_item_master", &user::upsert_poster_release_item_master},
    {"poster_story_master", &user::upsert_poster_story_master},
    {"star_rank_reward_master", &user::upsert_star_rank_reward_master},
    {"audition_clear", &user::upsert_audition_clear},
    {"sp_rate", &user::upsert_sp_rate},
    {"notification", &user::upsert_notification},
    {"episode", &user::upsert_episode},
    {"character_mission", &user::upsert_character_mission},
    {"mission_pass", &user::upsert_mission_pass},
    {"mission_pass_detail_master", &user::upsert_mission_pass_detail_master},
    {"mission_pass_master", &user::upsert_mission_pass_master},
    {"league_basic", &user::upsert_league_basic},
    {"story_event", &user::upsert_story_event},
    {"exchange_limit", &user::upsert_exchange_limit},
    {"league_group", &user::upsert_league_group},
    {"league_group_member", &user::upsert_league_group_member},
    {"league_history", &user::upsert_league_history},
    {"jewel_shop", &user::upsert_jewel_shop},
    {"daily_limit", &user::upsert_daily_limit},
    {"league_high_score_party", &user::upsert_league_high_score_party},
    {"league_high_score_party_slot", &user::upsert_league_high_score_party_slot},
    {"story_event_circle", &user::upsert_story_event_circle},
    {"story_event_circle_mission", &user::upsert_story_event_circle_mission},
    {"story_event_circle_mission_reward", &user::upsert_story_event_circle_mission_reward},
    {"story_event_high_score_buff_setting", &user::upsert_story_event_high_score_buff_setting},
    {"story_event_high_score_party", &user::upsert_story_event_high_score_party},
    {"story_event_high_score_party_slot", &user::upsert_story_event_high_score_party_slot},
    {"connect_with_account", &user::upsert_connect_with_account},
    {"connect_with_password", &user::upsert_connect_with_password},
    {"tournament_detail", &user::upsert_tournament_detail},
    {"gradual_mission_group", &user::upsert_gradual_mission_group},
    {"photo", &user::upsert_photo},
    {"album", &user::upsert_album},
    {"album_page", &user::upsert_album_page},
    {"star_pass_status", &user::upsert_star_pass_status},
    {"login_pass_status", &user::upsert_login_pass_status},
    {"currency", &user::upsert_currency},
    {"decoration", &user::upsert_decoration},
    {"live_achievement", &user::upsert_live_achievement},
    {"music_video", &user::upsert_music_video},
    {"theater_story", &user::upsert_theater_story},
    {"live_drop_celling", &user::upsert_live_drop_celling},
    {"story_event_high_score", &user::upsert_story_event_high_score},
    {"comic", &user::upsert_comic},
    {"comeback_campaign", &user::upsert_comeback_campaign},
    {"concert_stage", &user::upsert_concert_stage},
    {"limit", &user::upsert_limit},
    {"gacha_selected_thing", &user::upsert_gacha_selected_thing},
    {"total_point_event", &user::upsert_total_point_event},
    {"event_box_gacha", &user::upsert_event_box_gacha},
    {"event_box_gacha_box_thing", &user::upsert_event_box_gacha_box_thing},
    {"special_event", &user::upsert_special_event},
    {"character_point_event", &user::upsert_character_point_event},
    {"another_notation", &user::upsert_another_notation},
    {"music_bookmark", &user::upsert_music_bookmark},
    {"live_drop_limit", &user::upsert_live_drop_limit},
    {"restriction", &user::upsert_restriction},
    {"permanent_market_thing", &user::upsert_permanent_market_thing},
    {"time_limited_control", &user::upsert_time_limited_control},
    {"flash_sale_stage", &user::upsert_flash_sale_stage},
    {"album_theme", &user::upsert_album_theme},
    {"circle_event_mission", &user::upsert_circle_event_mission},
    {"pickup_character_mission", &user::upsert_pickup_character_mission},
    {"league_season_result", &user::upsert_league_season_result},
    {"event", &user::upsert_event},
    {"bonus_live", &user::upsert_bonus_live},
    {"bonus_live_stage", &user::upsert_bonus_live_stage},
    {"roulette_event", &user::upsert_roulette_event},
    {"roulette", &user::upsert_roulette},
    {"home_b_g_m", &user::upsert_home_b_g_m},
    {"link_character", &user::upsert_link_character},
    {"music_course", &user::upsert_music_course},
    {"tournament_qualifying", &user::upsert_tournament_qualifying},
    {"lottery", &user::upsert_lottery},
    {"triple_cast_party", &user::upsert_triple_cast_party},
    {"triple_cast_party_slot", &user::upsert_triple_cast_party_slot},
    {"triple_cast_basic", &user::upsert_triple_cast_basic},
    {"triple_cast_group", &user::upsert_triple_cast_group},
    {"triple_cast_group_member", &user::upsert_triple_cast_group_member},
    {"triple_cast_high_score_party", &user::upsert_triple_cast_high_score_party},
    {"triple_cast_high_score_party_slot", &user::upsert_triple_cast_high_score_party_slot},
    {"triple_cast_season_result", &user::upsert_triple_cast_season_result},
    {"album_preset", &user::upsert_album_preset},
    {"gacha", &user::upsert_gacha},
    {"triple_cast_history", &user::upsert_triple_cast_history},
    {"dugong_run", &user::upsert_dugong_run},
    {"music_course_ranking", &user::upsert_music_course_ranking},
    {"friend_invitation", &user::upsert_friend_invitation},
    {"friend_invitation_mission", &user::upsert_friend_invitation_mission},
    {"name_base_color", &user::upsert_name_base_color},
    {"icon_frame", &user::upsert_icon_frame},
    {"gacha_re_roll", &user::upsert_gacha_re_roll},
    {"trial_party_event", &user::upsert_trial_party_event},
    {"trial_party_event_stage", &user::upsert_trial_party_event_stage},
    {"trial_party_event_stage_party", &user::upsert_trial_party_event_stage_party},
    {"trial_party_event_stage_party_slot", &user::upsert_trial_party_event_stage_party_slot},
    {"user_block", &user::upsert_user_block},
    {"home_skin", &user::upsert_home_skin},
    {"accessory_auto_sell", &user::upsert_accessory_auto_sell},
    {"favorite_costume", &user::upsert_favorite_costume},
    {"buff_item_status", &user::upsert_buff_item_status},
    {"multi_room_basic", &user::upsert_multi_room_basic},
    {"event_camp", &user::upsert_event_camp},
};

// initial entities of a fresh account; loaded once, mirroring the module-level _SEED.
const wire::json& seed() {
    static const wire::json data =
        wire::json::parse(platform::read_file("db/default_account.json"));
    return data;
}

// re.sub(r"(?<!^)(?=[A-Z])", "_", type_name).lower(): insert "_" before each non-leading capital.
std::string table_name(const std::string& type_name) {
    std::string out;
    for (std::size_t i = 0; i < type_name.size(); ++i) {
        char c = type_name[i];
        if (i > 0 && c >= 'A' && c <= 'Z') out += '_';
        out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

// "".join(random.choices(ascii_uppercase + digits, k=10))
std::string invitation_code() {
    static const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<std::size_t> pick(0, alphabet.size() - 1);
    std::string out;
    out.reserve(10);
    for (int i = 0; i < 10; ++i) out += alphabet[pick(rng())];
    return out;
}

std::int64_t now_micros() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

}  // namespace

void create_default_user_data(long long user_id, const std::string& name) {
    const std::int64_t now = now_micros();  // epoch microseconds
    for (auto it = seed().begin(); it != seed().end(); ++it) {
        const std::string& type_name = it.key();
        const wire::json& rows = it.value();
        auto found = UPSERTS.find(table_name(type_name));
        if (found == UPSERTS.end()) continue;  // entity has no table yet (e.g. PartySlot)
        UpsertFn upsert = found->second;
        std::string sql;
        bool have_sql = false;
        std::vector<std::vector<db::json>> args_seq;
        for (const wire::json& seed_row : rows) {
            wire::json row = seed_row;
            if (ACCOUNT_ENTITIES.count(type_name)) row["id"] = user_id;
            if (type_name == "User") {
                row["hashUserId"] = user_hash::hash_id(user_id);
                row["gameStartAt"] = now;            // account created now
                row["maxStaminaRestoredAt"] = now;   // starts at full stamina
            }
            if (type_name == "UserProfile") row["name"] = name;
            if (type_name == "FriendInvitation") row["invitationCode"] = invitation_code();
            if (type_name == "MissionPass") {
                std::uniform_int_distribution<std::int64_t> pick(1000000, 999999999);
                row["id"] = pick(rng());
            }
            if (type_name == "DailyLimit") row["lastRefreshedAt"] = now;
            if (type_name == "Inbox") {  // anchor the claim window to signup, not the capture
                row["sentAt"] = now;
                row["receiveLimitAt"] = now + INBOX_TTL_MICROS;
            }
            ExecutableQuery query = upsert(user_id, row);
            sql = query.sql;  // identical for every row of a table
            have_sql = true;
            args_seq.push_back(query.args);
        }
        if (have_sql)  // one batched round-trip per table instead of one per row
            db::execute_batch(sql, args_seq);
    }

    if (config::get_bool("grant_music_tickets"))
        // increment_item_stock is a data-modifying CTE: in the SQLite port it executes itself
        // (returning the command tag), so it is not wrapped in db::execute.
        user::increment_item_stock(user_id, shops::MUSIC_UNLOCK_ITEM_ID, 100000);
}

}  // namespace db::defaults
