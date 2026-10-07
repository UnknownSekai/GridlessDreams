#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "db.h"
#include "wire.h"

// Query builders for the db.user package (get/create/update/grant/live), merged into one
// namespace as in Python. Each returns an ExecutableQuery/SelectQuery exactly like its source
// function; Postgres $n placeholders are kept verbatim for the engine to rewrite. dict/model
// args are wire::json (camelCase DB-row shape); nullable args use std::optional.
namespace db {
namespace user {

// get.py
SelectQuery get_users(std::int64_t user_id);
SelectQuery get_user_profiles(std::int64_t user_id);
SelectQuery get_user_preferences(std::int64_t user_id);
SelectQuery get_home_display_preferences(std::int64_t user_id);
SelectQuery get_characters(std::int64_t user_id);
SelectQuery get_character_bases(std::int64_t user_id);
SelectQuery get_partys(std::int64_t user_id);
SelectQuery get_party_slots(std::int64_t user_id);
SelectQuery get_character_masters(std::int64_t user_id);
SelectQuery get_character_base_masters(std::int64_t user_id);
SelectQuery get_character_level_masters(std::int64_t user_id);
SelectQuery get_posters(std::int64_t user_id);
SelectQuery get_accessory_level_pattern_group_masters(std::int64_t user_id);
SelectQuery get_accessory_level_pattern_masters(std::int64_t user_id);
SelectQuery get_accessory_masters(std::int64_t user_id);
SelectQuery get_episode_masters(std::int64_t user_id);
SelectQuery get_episode_reward_package_masters(std::int64_t user_id);
SelectQuery get_live_masters(std::int64_t user_id);
SelectQuery get_music_masters(std::int64_t user_id);
SelectQuery get_sense_masters(std::int64_t user_id);
SelectQuery get_story_masters(std::int64_t user_id);
SelectQuery get_poster_level_pattern_group_masters(std::int64_t user_id);
SelectQuery get_poster_level_pattern_masters(std::int64_t user_id);
SelectQuery get_poster_masters(std::int64_t user_id);
SelectQuery get_lives(std::int64_t user_id);
SelectQuery get_musics(std::int64_t user_id);
SelectQuery get_accessorys(std::int64_t user_id);
SelectQuery get_items(std::int64_t user_id);
SelectQuery get_accessory_effect_masters(std::int64_t user_id);
SelectQuery get_company_masters(std::int64_t user_id);
SelectQuery get_effect_duration_group_masters(std::int64_t user_id);
SelectQuery get_effect_masters(std::int64_t user_id);
SelectQuery get_item_masters(std::int64_t user_id);
SelectQuery get_random_effect_group_masters(std::int64_t user_id);
SelectQuery get_reward_rule_masters(std::int64_t user_id);
SelectQuery get_sense_effect_masters(std::int64_t user_id);
SelectQuery get_trophy_group_masters(std::int64_t user_id);
SelectQuery get_trophy_masters(std::int64_t user_id);
SelectQuery get_character_lessons(std::int64_t user_id);
SelectQuery get_daily_lessons(std::int64_t user_id);
SelectQuery get_inboxs(std::int64_t user_id);
SelectQuery get_unchecked_inboxs(std::int64_t user_id);
SelectQuery get_bombs(std::int64_t user_id);
SelectQuery get_costumes(std::int64_t user_id);
SelectQuery get_name_colors(std::int64_t user_id);
SelectQuery get_nameplates(std::int64_t user_id);
SelectQuery get_notes(std::int64_t user_id);
SelectQuery get_stamps(std::int64_t user_id);
SelectQuery get_missions(std::int64_t user_id);
SelectQuery get_audition_masters(std::int64_t user_id);
SelectQuery get_bomb_masters(std::int64_t user_id);
SelectQuery get_character_star_rank_masters(std::int64_t user_id);
SelectQuery get_character_star_rank_reward_group_masters(std::int64_t user_id);
SelectQuery get_costume_masters(std::int64_t user_id);
SelectQuery get_home_character_voice_masters(std::int64_t user_id);
SelectQuery get_name_color_masters(std::int64_t user_id);
SelectQuery get_nameplate_masters(std::int64_t user_id);
SelectQuery get_note_masters(std::int64_t user_id);
SelectQuery get_spot_conversation_masters(std::int64_t user_id);
SelectQuery get_stamp_masters(std::int64_t user_id);
SelectQuery get_character_lesson_slots(std::int64_t user_id);
SelectQuery get_trophys(std::int64_t user_id);
SelectQuery get_markets(std::int64_t user_id);
SelectQuery get_market_things(std::int64_t user_id);
SelectQuery get_viewed_shops(std::int64_t user_id);
SelectQuery get_game_hints(std::int64_t user_id);
SelectQuery get_user_bonuss(std::int64_t user_id);
SelectQuery get_audition_phase_masters(std::int64_t user_id);
SelectQuery get_audition_reward_package_masters(std::int64_t user_id);
SelectQuery get_campaign_masters(std::int64_t user_id);
SelectQuery get_character_awakening_item_masters(std::int64_t user_id);
SelectQuery get_character_bloom_bonus_group_masters(std::int64_t user_id);
SelectQuery get_character_bloom_item_masters(std::int64_t user_id);
SelectQuery get_character_experience_item_masters(std::int64_t user_id);
SelectQuery get_character_mission_masters(std::int64_t user_id);
SelectQuery get_character_mission_stage_masters(std::int64_t user_id);
SelectQuery get_character_piece_masters(std::int64_t user_id);
SelectQuery get_character_sense_enhance_item_group_masters(std::int64_t user_id);
SelectQuery get_costume_wearable_character_group_masters(std::int64_t user_id);
SelectQuery get_exchange_shop_masters(std::int64_t user_id);
SelectQuery get_live_setting_masters(std::int64_t user_id);
SelectQuery get_mission_masters(std::int64_t user_id);
SelectQuery get_music_vocal_version_masters(std::int64_t user_id);
SelectQuery get_poster_release_item_group_masters(std::int64_t user_id);
SelectQuery get_poster_release_item_masters(std::int64_t user_id);
SelectQuery get_poster_story_masters(std::int64_t user_id);
SelectQuery get_star_rank_reward_masters(std::int64_t user_id);
SelectQuery get_audition_clears(std::int64_t user_id);
SelectQuery get_sp_rates(std::int64_t user_id);
SelectQuery get_notifications(std::int64_t user_id);
SelectQuery get_episodes(std::int64_t user_id);
SelectQuery get_character_missions(std::int64_t user_id);
SelectQuery get_mission_passs(std::int64_t user_id);
SelectQuery get_mission_pass_detail_masters(std::int64_t user_id);
SelectQuery get_mission_pass_masters(std::int64_t user_id);
SelectQuery get_league_basics(std::int64_t user_id);
SelectQuery get_story_events(std::int64_t user_id);
SelectQuery get_exchange_limits(std::int64_t user_id);
SelectQuery get_league_groups(std::int64_t user_id);
SelectQuery get_league_group_members(std::int64_t user_id);
SelectQuery get_league_historys(std::int64_t user_id);
SelectQuery get_jewel_shops(std::int64_t user_id);
SelectQuery get_daily_limits(std::int64_t user_id);
SelectQuery get_league_high_score_partys(std::int64_t user_id);
SelectQuery get_league_high_score_party_slots(std::int64_t user_id);
SelectQuery get_story_event_circles(std::int64_t user_id);
SelectQuery get_story_event_circle_missions(std::int64_t user_id);
SelectQuery get_story_event_circle_mission_rewards(std::int64_t user_id);
SelectQuery get_story_event_high_score_buff_settings(std::int64_t user_id);
SelectQuery get_story_event_high_score_partys(std::int64_t user_id);
SelectQuery get_story_event_high_score_party_slots(std::int64_t user_id);
SelectQuery get_connect_with_accounts(std::int64_t user_id);
SelectQuery get_connect_with_passwords(std::int64_t user_id);
SelectQuery get_connect_with_password_by_linkage_code(const std::string& linkage_code);
SelectQuery get_tournament_details(std::int64_t user_id);
SelectQuery get_gradual_mission_groups(std::int64_t user_id);
SelectQuery get_photos(std::int64_t user_id);
SelectQuery get_albums(std::int64_t user_id);
SelectQuery get_circle_supports(std::int64_t user_id);
SelectQuery get_album_pages(std::int64_t user_id);
SelectQuery get_star_pass_statuss(std::int64_t user_id);
SelectQuery get_login_pass_statuss(std::int64_t user_id);
SelectQuery get_currencys(std::int64_t user_id);
SelectQuery get_decorations(std::int64_t user_id);
SelectQuery get_live_achievements(std::int64_t user_id);
SelectQuery get_music_videos(std::int64_t user_id);
SelectQuery get_theater_storys(std::int64_t user_id);
SelectQuery get_live_drop_cellings(std::int64_t user_id);
SelectQuery get_story_event_high_scores(std::int64_t user_id);
SelectQuery get_comics(std::int64_t user_id);
SelectQuery get_comeback_campaigns(std::int64_t user_id);
SelectQuery get_concert_stages(std::int64_t user_id);
SelectQuery get_limits(std::int64_t user_id);
SelectQuery get_gacha_selected_things(std::int64_t user_id);
SelectQuery get_total_point_events(std::int64_t user_id);
SelectQuery get_event_box_gachas(std::int64_t user_id);
SelectQuery get_event_box_gacha_box_things(std::int64_t user_id);
SelectQuery get_special_events(std::int64_t user_id);
SelectQuery get_character_point_events(std::int64_t user_id);
SelectQuery get_another_notations(std::int64_t user_id);
SelectQuery get_music_bookmarks(std::int64_t user_id);
SelectQuery get_live_drop_limits(std::int64_t user_id);
SelectQuery get_restrictions(std::int64_t user_id);
SelectQuery get_permanent_market_things(std::int64_t user_id);
SelectQuery get_time_limited_controls(std::int64_t user_id);
SelectQuery get_flash_sale_stages(std::int64_t user_id);
SelectQuery get_album_themes(std::int64_t user_id);
SelectQuery get_circle_event_missions(std::int64_t user_id);
SelectQuery get_pickup_character_missions(std::int64_t user_id);
SelectQuery get_league_season_results(std::int64_t user_id);
SelectQuery get_events(std::int64_t user_id);
SelectQuery get_bonus_lives(std::int64_t user_id);
SelectQuery get_bonus_live_stages(std::int64_t user_id);
SelectQuery get_roulette_events(std::int64_t user_id);
SelectQuery get_roulettes(std::int64_t user_id);
SelectQuery get_home_b_g_ms(std::int64_t user_id);
SelectQuery get_link_characters(std::int64_t user_id);
SelectQuery get_music_courses(std::int64_t user_id);
SelectQuery get_tournament_qualifyings(std::int64_t user_id);
SelectQuery get_lotterys(std::int64_t user_id);
SelectQuery get_triple_cast_partys(std::int64_t user_id);
SelectQuery get_triple_cast_party_slots(std::int64_t user_id);
SelectQuery get_triple_cast_basics(std::int64_t user_id);
SelectQuery get_triple_cast_groups(std::int64_t user_id);
SelectQuery get_triple_cast_group_members(std::int64_t user_id);
SelectQuery get_triple_cast_high_score_partys(std::int64_t user_id);
SelectQuery get_triple_cast_high_score_party_slots(std::int64_t user_id);
SelectQuery get_triple_cast_season_results(std::int64_t user_id);
SelectQuery get_album_presets(std::int64_t user_id);
SelectQuery get_gachas(std::int64_t user_id);
SelectQuery get_gacha_historys(std::int64_t user_id, std::int64_t card_type, std::int64_t limit = 100);
SelectQuery get_triple_cast_historys(std::int64_t user_id);
SelectQuery get_dugong_runs(std::int64_t user_id);
SelectQuery get_music_course_rankings(std::int64_t user_id);
SelectQuery get_friend_invitations(std::int64_t user_id);
SelectQuery get_friend_invitation_missions(std::int64_t user_id);
SelectQuery get_name_base_colors(std::int64_t user_id);
SelectQuery get_icon_frames(std::int64_t user_id);
SelectQuery get_gacha_re_rolls(std::int64_t user_id);
SelectQuery get_trial_party_events(std::int64_t user_id);
SelectQuery get_trial_party_event_stages(std::int64_t user_id);
SelectQuery get_trial_party_event_stage_partys(std::int64_t user_id);
SelectQuery get_trial_party_event_stage_party_slots(std::int64_t user_id);
SelectQuery get_user_blocks(std::int64_t user_id);
SelectQuery get_home_skins(std::int64_t user_id);
SelectQuery get_accessory_auto_sells(std::int64_t user_id);
SelectQuery get_favorite_costumes(std::int64_t user_id);
SelectQuery get_buff_item_statuss(std::int64_t user_id);
SelectQuery get_multi_room_basics(std::int64_t user_id);
SelectQuery get_event_camps(std::int64_t user_id);

// create.py
ExecutableQuery update_user_tutorial_status(std::int64_t user_id, std::int64_t status);
ExecutableQuery update_user_splash_last_displayed_at(std::int64_t user_id, std::int64_t ts);
ExecutableQuery upsert_user(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_user_profile(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_user_preference(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_home_display_preference(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_base(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_party(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_party_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_base_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_level_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_accessory_level_pattern_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_accessory_level_pattern_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_accessory_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_episode_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_episode_reward_package_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_live_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_sense_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster_level_pattern_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster_level_pattern_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_live(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_accessory(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_item(std::int64_t user_id, const wire::json& row);
// data-modifying CTEs: SQLite cannot run these as one statement, so they execute via the
// db::composite_* executors and return the command tag (not a deferred ExecutableQuery).
std::string increment_item_stock(std::int64_t user_id, std::int64_t item_master_id, std::int64_t delta);
std::string increment_item_stocks(std::int64_t user_id, const std::vector<std::pair<std::int64_t, std::int64_t>>& items);
ExecutableQuery add_currency(std::int64_t user_id, std::int64_t coin = 0, std::int64_t free_jewel = 0);
ExecutableQuery mark_inboxs_checked(std::int64_t user_id);
ExecutableQuery create_inbox(std::int64_t user_id, std::int64_t inbox_id, std::int64_t thing_type, std::int64_t thing_id, std::int64_t quantity, std::optional<std::string> description, std::int64_t sent_at, std::int64_t receive_limit_at);
ExecutableQuery upsert_accessory_effect_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_company_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_effect_duration_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_effect_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_item_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_random_effect_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_reward_rule_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_sense_effect_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trophy_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trophy_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_lesson(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_daily_lesson(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_inbox(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_bomb(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_costume(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_name_color(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_nameplate(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_note(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_stamp(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_mission(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_audition_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_bomb_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_star_rank_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_star_rank_reward_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_costume_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_home_character_voice_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_name_color_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_nameplate_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_note_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_spot_conversation_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_stamp_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_lesson_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trophy(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_market(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_viewed_shop(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_game_hint(std::int64_t user_id, const wire::json& row);
std::string mark_game_hint_read(std::int64_t user_id, std::int64_t page_category);  // data-modifying CTE -> db::composite_*
ExecutableQuery upsert_user_bonus(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_audition_phase_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_audition_reward_package_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_campaign_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_awakening_item_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_bloom_bonus_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_bloom_item_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_experience_item_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_mission_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_mission_stage_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_piece_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_sense_enhance_item_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_costume_wearable_character_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_exchange_shop_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_live_setting_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_mission_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music_vocal_version_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster_release_item_group_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster_release_item_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_poster_story_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_star_rank_reward_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_audition_clear(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_sp_rate(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_notification(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_episode(std::int64_t user_id, const wire::json& row);
ExecutableQuery update_episode_read_all(std::int64_t user_id, std::int64_t episode_master_id, bool has_read_all);
ExecutableQuery upsert_character_mission(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_mission_pass(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_mission_pass_detail_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_mission_pass_master(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_basic(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_exchange_limit(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_group(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_group_member(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_history(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_jewel_shop(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_daily_limit(std::int64_t user_id, const wire::json& row);
ExecutableQuery update_daily_limit(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_high_score_party(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_high_score_party_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_circle(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_circle_mission(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_circle_mission_reward(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score_buff_setting(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score_party(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score_party_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_connect_with_account(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_connect_with_password(std::int64_t user_id, const wire::json& row);
ExecutableQuery set_connect_with_password_confirmation(std::int64_t user_id, const std::string& confirmation_code, std::int64_t expires_at);
ExecutableQuery upsert_tournament_detail(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_gradual_mission_group(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_photo(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_album(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_album_page(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_star_pass_status(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_login_pass_status(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_currency(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_decoration(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_live_achievement(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music_video(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_theater_story(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_live_drop_celling(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_comic(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_comeback_campaign(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_concert_stage(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_limit(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_gacha_selected_thing(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_total_point_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_event_box_gacha(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_event_box_gacha_box_thing(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_special_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_character_point_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_another_notation(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music_bookmark(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_live_drop_limit(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_restriction(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_permanent_market_thing(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_time_limited_control(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_flash_sale_stage(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_album_theme(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_circle_event_mission(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_pickup_character_mission(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_league_season_result(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_bonus_live(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_bonus_live_stage(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_roulette_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_roulette(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_home_b_g_m(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_link_character(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music_course(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_tournament_qualifying(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_lottery(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_party(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_party_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_basic(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_group(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_group_member(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_high_score_party(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_high_score_party_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_season_result(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_album_preset(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_gacha(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_history(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_dugong_run(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_music_course_ranking(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_friend_invitation(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_friend_invitation_mission(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_name_base_color(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_icon_frame(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_gacha_re_roll(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event_stage(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event_stage_party(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event_stage_party_slot(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_user_block(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_home_skin(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_accessory_auto_sell(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_favorite_costume(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_buff_item_status(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_multi_room_basic(std::int64_t user_id, const wire::json& row);
ExecutableQuery upsert_event_camp(std::int64_t user_id, const wire::json& row);

// update.py
ExecutableQuery update_party_slots(std::int64_t user_id, const std::vector<wire::json>& slots);
ExecutableQuery update_party_slot_positions(std::int64_t user_id, const std::vector<std::pair<std::int64_t, std::int64_t>>& positions);
ExecutableQuery update_party_leader(std::int64_t user_id, std::int64_t party_id, std::int64_t position);
ExecutableQuery update_party_name(std::int64_t user_id, std::int64_t party_id, const std::string& name);
ExecutableQuery update_user_profile_edit(std::int64_t user_id, const wire::json& fields);
ExecutableQuery update_home_display_preference(std::int64_t user_id, const wire::json& fields);
// data-modifying CTEs (DELETE/UPDATE/INSERT inside WITH) have no single-statement SQLite form,
// so they execute via db::composite_* and return the result (tag / rows), not a deferred query.
std::vector<std::string> set_home_bgm(std::int64_t user_id, std::int64_t master_id, std::int64_t selection_type, std::optional<std::int64_t> detail_master_id);
ExecutableQuery update_character_base_costume(std::int64_t user_id, std::int64_t character_base_master_id, std::int64_t costume_master_id);
ExecutableQuery update_character_base_portal(std::int64_t user_id, std::int64_t character_base_master_id, std::int64_t portal_character_id, bool display_awakening);
ExecutableQuery update_accessory_level(std::int64_t user_id, std::int64_t accessory_id, std::int64_t level);
ExecutableQuery update_poster_released_episode(std::int64_t user_id, std::int64_t poster_id, std::int64_t released_episode);
ExecutableQuery set_stamp_favorites(std::int64_t user_id, std::int64_t stamp_id, const wire::json& favorite_ids);
std::string set_favorite_costumes(std::int64_t user_id, std::int64_t character_base_master_id, const wire::json& favorite_ids, std::int64_t new_id);  // data-modifying CTE -> db::composite_*
ExecutableQuery add_watch_record(std::int64_t user_id, const std::string& table, const std::string& column, std::int64_t new_id, std::int64_t master_id);
ExecutableQuery update_sp_rate_point(std::int64_t user_id, std::int64_t sp_rate_id, std::int64_t point);
ExecutableQuery breakthrough_poster(std::int64_t user_id, std::int64_t poster_id, std::int64_t max_phase);
std::string add_gacha_rolls(std::int64_t user_id, std::int64_t gacha_master_id, std::int64_t delta, std::int64_t new_id);  // data-modifying CTE -> db::composite_*
ExecutableQuery add_gacha_historys(std::int64_t user_id, std::int64_t card_type, const std::vector<std::int64_t>& master_ids, std::int64_t created_at);
std::vector<std::string> set_gacha_selected_things(std::int64_t user_id, std::int64_t gacha_master_id, const std::vector<std::int64_t>& thing_ids);  // DELETE+INSERT -> db::composite_*
ExecutableQuery update_multi_party(std::int64_t user_id, std::int64_t party_id);
ExecutableQuery update_character_level(std::int64_t user_id, std::int64_t character_id, std::int64_t level, std::int64_t current_experience);
ExecutableQuery update_character_awakening(std::int64_t user_id, std::int64_t character_id, std::int64_t phase);
ExecutableQuery update_character_talent_stage(std::int64_t user_id, std::int64_t character_id, std::int64_t stage);
ExecutableQuery update_character_sense_level(std::int64_t user_id, std::int64_t character_id, std::int64_t level, bool secondary = false);
ExecutableQuery update_birth_date(std::int64_t user_id, std::optional<std::int64_t> birth_date);
SelectQuery adjust_user_stamina_atomic(std::int64_t user_id, std::int64_t delta, std::int64_t max_stamina, std::int64_t interval_micros, std::int64_t now_micros, bool auto_max_clamp = false);
std::vector<std::string> replace_market_things(std::int64_t user_id, const std::vector<std::tuple<std::int64_t, std::int64_t, std::optional<std::int64_t>>>& frames);  // DELETE+INSERT -> db::composite_*
SelectQuery purchase_market_thing(std::int64_t user_id, std::int64_t frame_number);
std::optional<json> roll_over_market(std::int64_t user_id, std::int64_t now, std::int64_t new_id, std::int64_t reset_at);  // upd UNION ALL ins RETURNING * -> db::composite_upsert_returning
std::optional<json> consume_market_refresh(std::int64_t user_id, std::int64_t now, std::int64_t new_id, std::int64_t reset_at, std::int64_t max_refreshes);  // -> db::composite_upsert_returning
std::optional<json> consume_exchange_limit(std::int64_t user_id, std::int64_t exchange_shop_thing_id, std::int64_t quantity, std::int64_t limit, std::int64_t replace_type, std::optional<std::int64_t> until, std::int64_t now, std::int64_t new_id);  // -> db::composite_upsert_returning
std::optional<json> consume_permanent_market_limit(std::int64_t user_id, std::int64_t master_id, std::int64_t quantity, std::int64_t limit);  // -> db::composite_upsert_returning
SelectQuery release_music_olivier(std::int64_t user_id, std::int64_t music_master_id, std::int64_t purchasable, std::int64_t released);
std::string record_jewel_shop_purchase(std::int64_t user_id, std::int64_t jewel_shop_item_master_id, std::int64_t new_id, std::optional<std::int64_t> re_purchase_date);  // data-modifying CTE -> db::composite_*
std::string touch_viewed_shop(std::int64_t user_id, std::int64_t category, std::optional<std::int64_t> exchange_shop_master_id, std::int64_t now, std::int64_t new_id);  // data-modifying CTE -> db::composite_*

// grant.py
SelectQuery get_character_base(std::int64_t user_id, std::int64_t base_master_id);
ExecutableQuery create_character_base(std::int64_t user_id, std::int64_t row_id, std::int64_t base_master_id, std::optional<std::int64_t> costume_master_id, std::int64_t portal_character_id);
ExecutableQuery create_character(std::int64_t user_id, std::int64_t row_id, std::int64_t master_id, std::int64_t character_base_id);
ExecutableQuery create_accessory(std::int64_t user_id, std::int64_t row_id, std::int64_t master_id, const wire::json& effects);
ExecutableQuery add_stamina(std::int64_t user_id, std::int64_t amount);
ExecutableQuery grant_possession(const std::string& table, const std::string& master_col, std::int64_t user_id, std::int64_t row_id, std::int64_t master_id, const wire::json& extra);
// executes directly (not a builder): the pg data-modifying CTE has no single-statement SQLite
// form (no unique index on "userId"), so it runs the update-or-insert composite and returns the tag.
std::string grant_collection(const std::string& table, const std::string& array_col, std::int64_t user_id, std::int64_t master_id);
SelectQuery get_inboxes_by_ids(std::int64_t user_id, const std::vector<std::int64_t>& ids);
ExecutableQuery receive_inbox(std::int64_t user_id, std::int64_t inbox_id, std::int64_t now);

// live.py
SelectQuery next_live_id();
ExecutableQuery update_player_rate(std::int64_t user_id, double rate);
ExecutableQuery update_music_releases(std::int64_t user_id, const std::vector<std::tuple<std::int64_t, bool, std::int64_t>>& changes);
ExecutableQuery delete_active_lives(std::int64_t user_id);
ExecutableQuery create_active_live(std::int64_t user_id, std::int64_t live_id, std::int64_t live_master_id, std::int64_t party_id, std::int64_t live_setting_master_id = 0, bool stamina_spent = false);
SelectQuery get_active_live(std::int64_t user_id);
ExecutableQuery update_live_result(std::int64_t user_id, std::int64_t live_master_id, std::int64_t times_completed, double achievement_rate, double notation_rate, std::int64_t clear_lamp, std::int64_t rate_grade);

}  // namespace user
}  // namespace db
