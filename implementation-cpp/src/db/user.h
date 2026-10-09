#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "db.h"
#include "wire.h"

// query builders for the db.user package (get/create/update/grant/live), merged into one
// namespace as in Python. each returns an ExecutableQuery/SelectQuery exactly like its source
// function; Postgres $n placeholders are kept verbatim for the engine to rewrite. dict/model
// args are wire::json (camelCase DB-row shape); nullable args use std::optional.
namespace db {
namespace user {

// get.py
SelectQuery get_users(long long user_id);
SelectQuery get_user_profiles(long long user_id);
SelectQuery get_user_preferences(long long user_id);
SelectQuery get_home_display_preferences(long long user_id);
SelectQuery get_characters(long long user_id);
SelectQuery get_character_bases(long long user_id);
SelectQuery get_partys(long long user_id);
SelectQuery get_party_slots(long long user_id);
SelectQuery get_character_masters(long long user_id);
SelectQuery get_character_base_masters(long long user_id);
SelectQuery get_character_level_masters(long long user_id);
SelectQuery get_posters(long long user_id);
SelectQuery get_accessory_level_pattern_group_masters(long long user_id);
SelectQuery get_accessory_level_pattern_masters(long long user_id);
SelectQuery get_accessory_masters(long long user_id);
SelectQuery get_episode_masters(long long user_id);
SelectQuery get_episode_reward_package_masters(long long user_id);
SelectQuery get_live_masters(long long user_id);
SelectQuery get_music_masters(long long user_id);
SelectQuery get_sense_masters(long long user_id);
SelectQuery get_story_masters(long long user_id);
SelectQuery get_poster_level_pattern_group_masters(long long user_id);
SelectQuery get_poster_level_pattern_masters(long long user_id);
SelectQuery get_poster_masters(long long user_id);
SelectQuery get_lives(long long user_id);
SelectQuery get_musics(long long user_id);
SelectQuery get_accessorys(long long user_id);
SelectQuery get_items(long long user_id);
SelectQuery get_accessory_effect_masters(long long user_id);
SelectQuery get_company_masters(long long user_id);
SelectQuery get_effect_duration_group_masters(long long user_id);
SelectQuery get_effect_masters(long long user_id);
SelectQuery get_item_masters(long long user_id);
SelectQuery get_random_effect_group_masters(long long user_id);
SelectQuery get_reward_rule_masters(long long user_id);
SelectQuery get_sense_effect_masters(long long user_id);
SelectQuery get_trophy_group_masters(long long user_id);
SelectQuery get_trophy_masters(long long user_id);
SelectQuery get_character_lessons(long long user_id);
SelectQuery get_daily_lessons(long long user_id);
SelectQuery get_inboxs(long long user_id);
SelectQuery get_unchecked_inboxs(long long user_id);
SelectQuery get_bombs(long long user_id);
SelectQuery get_costumes(long long user_id);
SelectQuery get_name_colors(long long user_id);
SelectQuery get_nameplates(long long user_id);
SelectQuery get_notes(long long user_id);
SelectQuery get_stamps(long long user_id);
SelectQuery get_missions(long long user_id);
SelectQuery get_audition_masters(long long user_id);
SelectQuery get_bomb_masters(long long user_id);
SelectQuery get_character_star_rank_masters(long long user_id);
SelectQuery get_character_star_rank_reward_group_masters(long long user_id);
SelectQuery get_costume_masters(long long user_id);
SelectQuery get_home_character_voice_masters(long long user_id);
SelectQuery get_name_color_masters(long long user_id);
SelectQuery get_nameplate_masters(long long user_id);
SelectQuery get_note_masters(long long user_id);
SelectQuery get_spot_conversation_masters(long long user_id);
SelectQuery get_stamp_masters(long long user_id);
SelectQuery get_character_lesson_slots(long long user_id);
SelectQuery get_trophys(long long user_id);
SelectQuery get_markets(long long user_id);
SelectQuery get_market_things(long long user_id);
SelectQuery get_viewed_shops(long long user_id);
SelectQuery get_game_hints(long long user_id);
SelectQuery get_user_bonuss(long long user_id);
SelectQuery get_audition_phase_masters(long long user_id);
SelectQuery get_audition_reward_package_masters(long long user_id);
SelectQuery get_campaign_masters(long long user_id);
SelectQuery get_character_awakening_item_masters(long long user_id);
SelectQuery get_character_bloom_bonus_group_masters(long long user_id);
SelectQuery get_character_bloom_item_masters(long long user_id);
SelectQuery get_character_experience_item_masters(long long user_id);
SelectQuery get_character_mission_masters(long long user_id);
SelectQuery get_character_mission_stage_masters(long long user_id);
SelectQuery get_character_piece_masters(long long user_id);
SelectQuery get_character_sense_enhance_item_group_masters(long long user_id);
SelectQuery get_costume_wearable_character_group_masters(long long user_id);
SelectQuery get_exchange_shop_masters(long long user_id);
SelectQuery get_live_setting_masters(long long user_id);
SelectQuery get_mission_masters(long long user_id);
SelectQuery get_music_vocal_version_masters(long long user_id);
SelectQuery get_poster_release_item_group_masters(long long user_id);
SelectQuery get_poster_release_item_masters(long long user_id);
SelectQuery get_poster_story_masters(long long user_id);
SelectQuery get_star_rank_reward_masters(long long user_id);
SelectQuery get_audition_clears(long long user_id);
SelectQuery get_sp_rates(long long user_id);
SelectQuery get_notifications(long long user_id);
SelectQuery get_episodes(long long user_id);
SelectQuery get_character_missions(long long user_id);
SelectQuery get_mission_passs(long long user_id);
SelectQuery get_mission_pass_detail_masters(long long user_id);
SelectQuery get_mission_pass_masters(long long user_id);
SelectQuery get_league_basics(long long user_id);
SelectQuery get_story_events(long long user_id);
SelectQuery get_exchange_limits(long long user_id);
SelectQuery get_league_groups(long long user_id);
SelectQuery get_league_group_members(long long user_id);
SelectQuery get_league_historys(long long user_id);
SelectQuery get_jewel_shops(long long user_id);
SelectQuery get_daily_limits(long long user_id);
SelectQuery get_league_high_score_partys(long long user_id);
SelectQuery get_league_high_score_party_slots(long long user_id);
SelectQuery get_story_event_circles(long long user_id);
SelectQuery get_story_event_circle_missions(long long user_id);
SelectQuery get_story_event_circle_mission_rewards(long long user_id);
SelectQuery get_story_event_high_score_buff_settings(long long user_id);
SelectQuery get_story_event_high_score_partys(long long user_id);
SelectQuery get_story_event_high_score_party_slots(long long user_id);
SelectQuery get_connect_with_accounts(long long user_id);
SelectQuery get_connect_with_passwords(long long user_id);
SelectQuery get_connect_with_password_by_linkage_code(const std::string& linkage_code);
SelectQuery get_tournament_details(long long user_id);
SelectQuery get_gradual_mission_groups(long long user_id);
SelectQuery get_photos(long long user_id);
SelectQuery get_albums(long long user_id);
SelectQuery get_circle_supports(long long user_id);
SelectQuery get_album_pages(long long user_id);
SelectQuery get_star_pass_statuss(long long user_id);
SelectQuery get_login_pass_statuss(long long user_id);
SelectQuery get_currencys(long long user_id);
SelectQuery get_decorations(long long user_id);
SelectQuery get_live_achievements(long long user_id);
SelectQuery get_music_videos(long long user_id);
SelectQuery get_theater_storys(long long user_id);
SelectQuery get_live_drop_cellings(long long user_id);
SelectQuery get_story_event_high_scores(long long user_id);
SelectQuery get_comics(long long user_id);
SelectQuery get_comeback_campaigns(long long user_id);
SelectQuery get_concert_stages(long long user_id);
SelectQuery get_limits(long long user_id);
SelectQuery get_gacha_selected_things(long long user_id);
SelectQuery get_total_point_events(long long user_id);
SelectQuery get_event_box_gachas(long long user_id);
SelectQuery get_event_box_gacha_box_things(long long user_id);
SelectQuery get_special_events(long long user_id);
SelectQuery get_character_point_events(long long user_id);
SelectQuery get_another_notations(long long user_id);
SelectQuery get_music_bookmarks(long long user_id);
SelectQuery get_live_drop_limits(long long user_id);
SelectQuery get_restrictions(long long user_id);
SelectQuery get_permanent_market_things(long long user_id);
SelectQuery get_time_limited_controls(long long user_id);
SelectQuery get_flash_sale_stages(long long user_id);
SelectQuery get_album_themes(long long user_id);
SelectQuery get_circle_event_missions(long long user_id);
SelectQuery get_pickup_character_missions(long long user_id);
SelectQuery get_league_season_results(long long user_id);
SelectQuery get_events(long long user_id);
SelectQuery get_bonus_lives(long long user_id);
SelectQuery get_bonus_live_stages(long long user_id);
SelectQuery get_roulette_events(long long user_id);
SelectQuery get_roulettes(long long user_id);
SelectQuery get_home_b_g_ms(long long user_id);
SelectQuery get_link_characters(long long user_id);
SelectQuery get_music_courses(long long user_id);
SelectQuery get_tournament_qualifyings(long long user_id);
SelectQuery get_lotterys(long long user_id);
SelectQuery get_triple_cast_partys(long long user_id);
SelectQuery get_triple_cast_party_slots(long long user_id);
SelectQuery get_triple_cast_basics(long long user_id);
SelectQuery get_triple_cast_groups(long long user_id);
SelectQuery get_triple_cast_group_members(long long user_id);
SelectQuery get_triple_cast_high_score_partys(long long user_id);
SelectQuery get_triple_cast_high_score_party_slots(long long user_id);
SelectQuery get_triple_cast_season_results(long long user_id);
SelectQuery get_album_presets(long long user_id);
SelectQuery get_gachas(long long user_id);
SelectQuery get_gacha_historys(long long user_id, long long card_type, long long limit = 100);
SelectQuery get_triple_cast_historys(long long user_id);
SelectQuery get_dugong_runs(long long user_id);
SelectQuery get_music_course_rankings(long long user_id);
SelectQuery get_friend_invitations(long long user_id);
SelectQuery get_friend_invitation_missions(long long user_id);
SelectQuery get_name_base_colors(long long user_id);
SelectQuery get_icon_frames(long long user_id);
SelectQuery get_gacha_re_rolls(long long user_id);
SelectQuery get_trial_party_events(long long user_id);
SelectQuery get_trial_party_event_stages(long long user_id);
SelectQuery get_trial_party_event_stage_partys(long long user_id);
SelectQuery get_trial_party_event_stage_party_slots(long long user_id);
SelectQuery get_user_blocks(long long user_id);
SelectQuery get_home_skins(long long user_id);
SelectQuery get_accessory_auto_sells(long long user_id);
SelectQuery get_favorite_costumes(long long user_id);
SelectQuery get_buff_item_statuss(long long user_id);
SelectQuery get_multi_room_basics(long long user_id);
SelectQuery get_event_camps(long long user_id);

// create.py
ExecutableQuery update_user_tutorial_status(long long user_id, long long status);
ExecutableQuery update_user_splash_last_displayed_at(long long user_id, long long ts);
ExecutableQuery upsert_user(long long user_id, const wire::json& row);
ExecutableQuery upsert_user_profile(long long user_id, const wire::json& row);
ExecutableQuery upsert_user_preference(long long user_id, const wire::json& row);
ExecutableQuery upsert_home_display_preference(long long user_id, const wire::json& row);
ExecutableQuery upsert_character(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_base(long long user_id, const wire::json& row);
ExecutableQuery upsert_party(long long user_id, const wire::json& row);
ExecutableQuery upsert_party_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_base_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_level_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster(long long user_id, const wire::json& row);
ExecutableQuery upsert_accessory_level_pattern_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_accessory_level_pattern_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_accessory_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_episode_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_episode_reward_package_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_live_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_music_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_sense_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster_level_pattern_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster_level_pattern_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_live(long long user_id, const wire::json& row);
ExecutableQuery upsert_music(long long user_id, const wire::json& row);
ExecutableQuery upsert_accessory(long long user_id, const wire::json& row);
ExecutableQuery upsert_item(long long user_id, const wire::json& row);
// data-modifying CTEs: SQLite cannot run these as one statement, so they execute via the
// db::composite_* executors and return the command tag (not a deferred ExecutableQuery)
std::string increment_item_stock(long long user_id, long long item_master_id, long long delta);
std::string increment_item_stocks(long long user_id, const std::vector<std::pair<long long, long long>>& items);
ExecutableQuery add_currency(long long user_id, long long coin = 0, long long free_jewel = 0);
ExecutableQuery mark_inboxs_checked(long long user_id);
ExecutableQuery create_inbox(long long user_id, long long inbox_id, long long thing_type, long long thing_id, long long quantity, std::optional<std::string> description, long long sent_at, long long receive_limit_at);
ExecutableQuery upsert_accessory_effect_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_company_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_effect_duration_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_effect_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_item_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_random_effect_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_reward_rule_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_sense_effect_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_trophy_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_trophy_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_lesson(long long user_id, const wire::json& row);
ExecutableQuery upsert_daily_lesson(long long user_id, const wire::json& row);
ExecutableQuery upsert_inbox(long long user_id, const wire::json& row);
ExecutableQuery upsert_bomb(long long user_id, const wire::json& row);
ExecutableQuery upsert_costume(long long user_id, const wire::json& row);
ExecutableQuery upsert_name_color(long long user_id, const wire::json& row);
ExecutableQuery upsert_nameplate(long long user_id, const wire::json& row);
ExecutableQuery upsert_note(long long user_id, const wire::json& row);
ExecutableQuery upsert_stamp(long long user_id, const wire::json& row);
ExecutableQuery upsert_mission(long long user_id, const wire::json& row);
ExecutableQuery upsert_audition_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_bomb_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_star_rank_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_star_rank_reward_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_costume_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_home_character_voice_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_name_color_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_nameplate_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_note_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_spot_conversation_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_stamp_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_lesson_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_trophy(long long user_id, const wire::json& row);
ExecutableQuery upsert_market(long long user_id, const wire::json& row);
ExecutableQuery upsert_viewed_shop(long long user_id, const wire::json& row);
ExecutableQuery upsert_game_hint(long long user_id, const wire::json& row);
std::string mark_game_hint_read(long long user_id, long long page_category);  // data-modifying CTE -> db::composite_*
ExecutableQuery upsert_user_bonus(long long user_id, const wire::json& row);
ExecutableQuery upsert_audition_phase_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_audition_reward_package_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_campaign_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_awakening_item_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_bloom_bonus_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_bloom_item_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_experience_item_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_mission_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_mission_stage_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_piece_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_sense_enhance_item_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_costume_wearable_character_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_exchange_shop_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_live_setting_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_mission_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_music_vocal_version_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster_release_item_group_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster_release_item_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_poster_story_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_star_rank_reward_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_audition_clear(long long user_id, const wire::json& row);
ExecutableQuery upsert_sp_rate(long long user_id, const wire::json& row);
ExecutableQuery upsert_notification(long long user_id, const wire::json& row);
ExecutableQuery upsert_episode(long long user_id, const wire::json& row);
ExecutableQuery update_episode_read_all(long long user_id, long long episode_master_id, bool has_read_all);
ExecutableQuery upsert_character_mission(long long user_id, const wire::json& row);
ExecutableQuery upsert_mission_pass(long long user_id, const wire::json& row);
ExecutableQuery upsert_mission_pass_detail_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_mission_pass_master(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_basic(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_exchange_limit(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_group(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_group_member(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_history(long long user_id, const wire::json& row);
ExecutableQuery upsert_jewel_shop(long long user_id, const wire::json& row);
ExecutableQuery upsert_daily_limit(long long user_id, const wire::json& row);
ExecutableQuery update_daily_limit(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_high_score_party(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_high_score_party_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_circle(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_circle_mission(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_circle_mission_reward(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score_buff_setting(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score_party(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score_party_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_connect_with_account(long long user_id, const wire::json& row);
ExecutableQuery upsert_connect_with_password(long long user_id, const wire::json& row);
ExecutableQuery set_connect_with_password_confirmation(long long user_id, const std::string& confirmation_code, long long expires_at);
ExecutableQuery upsert_tournament_detail(long long user_id, const wire::json& row);
ExecutableQuery upsert_gradual_mission_group(long long user_id, const wire::json& row);
ExecutableQuery upsert_photo(long long user_id, const wire::json& row);
ExecutableQuery upsert_album(long long user_id, const wire::json& row);
ExecutableQuery upsert_album_page(long long user_id, const wire::json& row);
ExecutableQuery upsert_star_pass_status(long long user_id, const wire::json& row);
ExecutableQuery upsert_login_pass_status(long long user_id, const wire::json& row);
ExecutableQuery upsert_currency(long long user_id, const wire::json& row);
ExecutableQuery upsert_decoration(long long user_id, const wire::json& row);
ExecutableQuery upsert_live_achievement(long long user_id, const wire::json& row);
ExecutableQuery upsert_music_video(long long user_id, const wire::json& row);
ExecutableQuery upsert_theater_story(long long user_id, const wire::json& row);
ExecutableQuery upsert_live_drop_celling(long long user_id, const wire::json& row);
ExecutableQuery upsert_story_event_high_score(long long user_id, const wire::json& row);
ExecutableQuery upsert_comic(long long user_id, const wire::json& row);
ExecutableQuery upsert_comeback_campaign(long long user_id, const wire::json& row);
ExecutableQuery upsert_concert_stage(long long user_id, const wire::json& row);
ExecutableQuery upsert_limit(long long user_id, const wire::json& row);
ExecutableQuery upsert_gacha_selected_thing(long long user_id, const wire::json& row);
ExecutableQuery upsert_total_point_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_event_box_gacha(long long user_id, const wire::json& row);
ExecutableQuery upsert_event_box_gacha_box_thing(long long user_id, const wire::json& row);
ExecutableQuery upsert_special_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_character_point_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_another_notation(long long user_id, const wire::json& row);
ExecutableQuery upsert_music_bookmark(long long user_id, const wire::json& row);
ExecutableQuery upsert_live_drop_limit(long long user_id, const wire::json& row);
ExecutableQuery upsert_restriction(long long user_id, const wire::json& row);
ExecutableQuery upsert_permanent_market_thing(long long user_id, const wire::json& row);
ExecutableQuery upsert_time_limited_control(long long user_id, const wire::json& row);
ExecutableQuery upsert_flash_sale_stage(long long user_id, const wire::json& row);
ExecutableQuery upsert_album_theme(long long user_id, const wire::json& row);
ExecutableQuery upsert_circle_event_mission(long long user_id, const wire::json& row);
ExecutableQuery upsert_pickup_character_mission(long long user_id, const wire::json& row);
ExecutableQuery upsert_league_season_result(long long user_id, const wire::json& row);
ExecutableQuery upsert_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_bonus_live(long long user_id, const wire::json& row);
ExecutableQuery upsert_bonus_live_stage(long long user_id, const wire::json& row);
ExecutableQuery upsert_roulette_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_roulette(long long user_id, const wire::json& row);
ExecutableQuery upsert_home_b_g_m(long long user_id, const wire::json& row);
ExecutableQuery upsert_link_character(long long user_id, const wire::json& row);
ExecutableQuery upsert_music_course(long long user_id, const wire::json& row);
ExecutableQuery upsert_tournament_qualifying(long long user_id, const wire::json& row);
ExecutableQuery upsert_lottery(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_party(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_party_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_basic(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_group(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_group_member(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_high_score_party(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_high_score_party_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_season_result(long long user_id, const wire::json& row);
ExecutableQuery upsert_album_preset(long long user_id, const wire::json& row);
ExecutableQuery upsert_gacha(long long user_id, const wire::json& row);
ExecutableQuery upsert_triple_cast_history(long long user_id, const wire::json& row);
ExecutableQuery upsert_dugong_run(long long user_id, const wire::json& row);
ExecutableQuery upsert_music_course_ranking(long long user_id, const wire::json& row);
ExecutableQuery upsert_friend_invitation(long long user_id, const wire::json& row);
ExecutableQuery upsert_friend_invitation_mission(long long user_id, const wire::json& row);
ExecutableQuery upsert_name_base_color(long long user_id, const wire::json& row);
ExecutableQuery upsert_icon_frame(long long user_id, const wire::json& row);
ExecutableQuery upsert_gacha_re_roll(long long user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event(long long user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event_stage(long long user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event_stage_party(long long user_id, const wire::json& row);
ExecutableQuery upsert_trial_party_event_stage_party_slot(long long user_id, const wire::json& row);
ExecutableQuery upsert_user_block(long long user_id, const wire::json& row);
ExecutableQuery upsert_home_skin(long long user_id, const wire::json& row);
ExecutableQuery upsert_accessory_auto_sell(long long user_id, const wire::json& row);
ExecutableQuery upsert_favorite_costume(long long user_id, const wire::json& row);
ExecutableQuery upsert_buff_item_status(long long user_id, const wire::json& row);
ExecutableQuery upsert_multi_room_basic(long long user_id, const wire::json& row);
ExecutableQuery upsert_event_camp(long long user_id, const wire::json& row);

// update.py
ExecutableQuery update_party_slots(long long user_id, const std::vector<wire::json>& slots);
ExecutableQuery update_party_slot_positions(long long user_id, const std::vector<std::pair<long long, long long>>& positions);
ExecutableQuery update_party_leader(long long user_id, long long party_id, long long position);
ExecutableQuery update_party_name(long long user_id, long long party_id, const std::string& name);
ExecutableQuery update_user_profile_edit(long long user_id, const wire::json& fields);
ExecutableQuery update_home_display_preference(long long user_id, const wire::json& fields);
// data-modifying CTEs (DELETE/UPDATE/INSERT inside WITH) have no single-statement SQLite form,
// so they execute via db::composite_* and return the result (tag / rows), not a deferred query
std::vector<std::string> set_home_bgm(long long user_id, long long master_id, long long selection_type, std::optional<long long> detail_master_id);
ExecutableQuery update_character_base_costume(long long user_id, long long character_base_master_id, long long costume_master_id);
ExecutableQuery update_character_base_portal(long long user_id, long long character_base_master_id, long long portal_character_id, bool display_awakening);
ExecutableQuery update_accessory_level(long long user_id, long long accessory_id, long long level);
ExecutableQuery update_poster_released_episode(long long user_id, long long poster_id, long long released_episode);
ExecutableQuery set_stamp_favorites(long long user_id, long long stamp_id, const wire::json& favorite_ids);
std::string set_favorite_costumes(long long user_id, long long character_base_master_id, const wire::json& favorite_ids, long long new_id);  // data-modifying CTE -> db::composite_*
ExecutableQuery add_watch_record(long long user_id, const std::string& table, const std::string& column, long long new_id, long long master_id);
ExecutableQuery update_sp_rate_point(long long user_id, long long sp_rate_id, long long point);
ExecutableQuery breakthrough_poster(long long user_id, long long poster_id, long long max_phase);
std::string add_gacha_rolls(long long user_id, long long gacha_master_id, long long delta, long long new_id);  // data-modifying CTE -> db::composite_*
ExecutableQuery add_gacha_historys(long long user_id, long long card_type, const std::vector<long long>& master_ids, long long created_at);
std::vector<std::string> set_gacha_selected_things(long long user_id, long long gacha_master_id, const std::vector<long long>& thing_ids);  // DELETE+INSERT -> db::composite_*
ExecutableQuery update_multi_party(long long user_id, long long party_id);
ExecutableQuery update_character_level(long long user_id, long long character_id, long long level, long long current_experience);
ExecutableQuery update_character_awakening(long long user_id, long long character_id, long long phase);
ExecutableQuery update_character_talent_stage(long long user_id, long long character_id, long long stage);
ExecutableQuery update_character_sense_level(long long user_id, long long character_id, long long level, bool secondary = false);
ExecutableQuery update_birth_date(long long user_id, std::optional<long long> birth_date);
SelectQuery adjust_user_stamina_atomic(long long user_id, long long delta, long long max_stamina, long long interval_micros, long long now_micros, bool auto_max_clamp = false);
std::vector<std::string> replace_market_things(long long user_id, const std::vector<std::tuple<long long, long long, std::optional<long long>>>& frames);  // DELETE+INSERT -> db::composite_*
SelectQuery purchase_market_thing(long long user_id, long long frame_number);
std::optional<json> roll_over_market(long long user_id, long long now, long long new_id, long long reset_at);  // upd UNION ALL ins RETURNING * -> db::composite_upsert_returning
std::optional<json> consume_market_refresh(long long user_id, long long now, long long new_id, long long reset_at, long long max_refreshes);  // -> db::composite_upsert_returning
std::optional<json> consume_exchange_limit(long long user_id, long long exchange_shop_thing_id, long long quantity, long long limit, long long replace_type, std::optional<long long> until, long long now, long long new_id);  // -> db::composite_upsert_returning
std::optional<json> consume_permanent_market_limit(long long user_id, long long master_id, long long quantity, long long limit);  // -> db::composite_upsert_returning
SelectQuery release_music_olivier(long long user_id, long long music_master_id, long long purchasable, long long released);
std::string record_jewel_shop_purchase(long long user_id, long long jewel_shop_item_master_id, long long new_id, std::optional<long long> re_purchase_date);  // data-modifying CTE -> db::composite_*
std::string touch_viewed_shop(long long user_id, long long category, std::optional<long long> exchange_shop_master_id, long long now, long long new_id);  // data-modifying CTE -> db::composite_*

// grant.py
SelectQuery get_character_base(long long user_id, long long base_master_id);
ExecutableQuery create_character_base(long long user_id, long long row_id, long long base_master_id, std::optional<long long> costume_master_id, long long portal_character_id);
ExecutableQuery create_character(long long user_id, long long row_id, long long master_id, long long character_base_id);
ExecutableQuery create_accessory(long long user_id, long long row_id, long long master_id, const wire::json& effects);
ExecutableQuery add_stamina(long long user_id, long long amount);
ExecutableQuery grant_possession(const std::string& table, const std::string& master_col, long long user_id, long long row_id, long long master_id, const wire::json& extra);
// executes directly (not a builder): the pg data-modifying CTE has no single-statement SQLite
// form (no unique index on "userId"), so it runs the update-or-insert composite and returns the tag
std::string grant_collection(const std::string& table, const std::string& array_col, long long user_id, long long master_id);
SelectQuery get_inboxes_by_ids(long long user_id, const std::vector<long long>& ids);
ExecutableQuery receive_inbox(long long user_id, long long inbox_id, long long now);

// live.py
SelectQuery next_live_id();
ExecutableQuery update_player_rate(long long user_id, double rate);
ExecutableQuery update_music_releases(long long user_id, const std::vector<std::tuple<long long, bool, long long>>& changes);
ExecutableQuery delete_active_lives(long long user_id);
ExecutableQuery create_active_live(long long user_id, long long live_id, long long live_master_id, long long party_id, long long live_setting_master_id = 0, bool stamina_spent = false, bool is_auto_play = false);
SelectQuery get_active_live(long long user_id);
ExecutableQuery update_live_result(long long user_id, long long live_master_id, long long times_completed, double achievement_rate, double notation_rate, long long clear_lamp, long long rate_grade);

}  // namespace user
}  // namespace db
