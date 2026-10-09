#include "db/user.h"

namespace db {
namespace user {

SelectQuery get_users(long long user_id) {
    // coin/freeJewel/paidJewel come from the "currency" table (single source of truth)
    return SelectQuery("UserModel", "SELECT u.*, COALESCE(c.\"coin\", 0) AS \"coin\", COALESCE(c.\"freeJewel\", 0) AS \"freeJewel\", COALESCE(c.\"paidJewel\", 0) AS \"paidJewel\" FROM \"user\" u LEFT JOIN \"currency\" c ON c.\"userId\" = u.\"userId\" WHERE u.\"userId\" = $1", user_id);
}

SelectQuery get_user_profiles(long long user_id) {
    return SelectQuery("UserProfileModel", "SELECT * FROM \"user_profile\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_user_preferences(long long user_id) {
    return SelectQuery("UserPreferenceModel", "SELECT * FROM \"user_preference\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_home_display_preferences(long long user_id) {
    return SelectQuery("HomeDisplayPreferenceModel", "SELECT * FROM \"home_display_preference\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_characters(long long user_id) {
    return SelectQuery("CharacterModel", "SELECT * FROM \"character\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_bases(long long user_id) {
    return SelectQuery("CharacterBaseModel", "SELECT * FROM \"character_base\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_partys(long long user_id) {
    return SelectQuery("PartyModel", "SELECT * FROM \"party\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_party_slots(long long user_id) {
    return SelectQuery("PartySlotModel", "SELECT * FROM \"party_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_masters(long long user_id) {
    return SelectQuery("CharacterMasterModel", "SELECT * FROM \"character_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_base_masters(long long user_id) {
    return SelectQuery("CharacterBaseMasterModel", "SELECT * FROM \"character_base_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_level_masters(long long user_id) {
    return SelectQuery("CharacterLevelMasterModel", "SELECT * FROM \"character_level_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_posters(long long user_id) {
    return SelectQuery("PosterModel", "SELECT * FROM \"poster\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_accessory_level_pattern_group_masters(long long user_id) {
    return SelectQuery("AccessoryLevelPatternGroupMasterModel", "SELECT * FROM \"accessory_level_pattern_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_accessory_level_pattern_masters(long long user_id) {
    return SelectQuery("AccessoryLevelPatternMasterModel", "SELECT * FROM \"accessory_level_pattern_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_accessory_masters(long long user_id) {
    return SelectQuery("AccessoryMasterModel", "SELECT * FROM \"accessory_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_episode_masters(long long user_id) {
    return SelectQuery("EpisodeMasterModel", "SELECT * FROM \"episode_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_episode_reward_package_masters(long long user_id) {
    return SelectQuery("EpisodeRewardPackageMasterModel", "SELECT * FROM \"episode_reward_package_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_live_masters(long long user_id) {
    return SelectQuery("LiveMasterModel", "SELECT * FROM \"live_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_music_masters(long long user_id) {
    return SelectQuery("MusicMasterModel", "SELECT * FROM \"music_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_sense_masters(long long user_id) {
    return SelectQuery("SenseMasterModel", "SELECT * FROM \"sense_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_masters(long long user_id) {
    return SelectQuery("StoryMasterModel", "SELECT * FROM \"story_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_poster_level_pattern_group_masters(long long user_id) {
    return SelectQuery("PosterLevelPatternGroupMasterModel", "SELECT * FROM \"poster_level_pattern_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_poster_level_pattern_masters(long long user_id) {
    return SelectQuery("PosterLevelPatternMasterModel", "SELECT * FROM \"poster_level_pattern_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_poster_masters(long long user_id) {
    return SelectQuery("PosterMasterModel", "SELECT * FROM \"poster_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_lives(long long user_id) {
    return SelectQuery("LiveModel", "SELECT * FROM \"live\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_musics(long long user_id) {
    return SelectQuery("MusicModel", "SELECT * FROM \"music\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_accessorys(long long user_id) {
    return SelectQuery("AccessoryModel", "SELECT * FROM \"accessory\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_items(long long user_id) {
    return SelectQuery("ItemModel", "SELECT * FROM \"item\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_accessory_effect_masters(long long user_id) {
    return SelectQuery("AccessoryEffectMasterModel", "SELECT * FROM \"accessory_effect_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_company_masters(long long user_id) {
    return SelectQuery("CompanyMasterModel", "SELECT * FROM \"company_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_effect_duration_group_masters(long long user_id) {
    return SelectQuery("EffectDurationGroupMasterModel", "SELECT * FROM \"effect_duration_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_effect_masters(long long user_id) {
    return SelectQuery("EffectMasterModel", "SELECT * FROM \"effect_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_item_masters(long long user_id) {
    return SelectQuery("ItemMasterModel", "SELECT * FROM \"item_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_random_effect_group_masters(long long user_id) {
    return SelectQuery("RandomEffectGroupMasterModel", "SELECT * FROM \"random_effect_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_reward_rule_masters(long long user_id) {
    return SelectQuery("RewardRuleMasterModel", "SELECT * FROM \"reward_rule_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_sense_effect_masters(long long user_id) {
    return SelectQuery("SenseEffectMasterModel", "SELECT * FROM \"sense_effect_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trophy_group_masters(long long user_id) {
    return SelectQuery("TrophyGroupMasterModel", "SELECT * FROM \"trophy_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trophy_masters(long long user_id) {
    return SelectQuery("TrophyMasterModel", "SELECT * FROM \"trophy_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_lessons(long long user_id) {
    return SelectQuery("CharacterLessonModel", "SELECT * FROM \"character_lesson\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_daily_lessons(long long user_id) {
    return SelectQuery("DailyLessonModel", "SELECT * FROM \"daily_lesson\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_inboxs(long long user_id) {
    return SelectQuery("InboxModel", "SELECT * FROM \"inbox\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_unchecked_inboxs(long long user_id) {
    // inbox packages not yet surfaced by checkpackages (its new-package diff)
    return SelectQuery("InboxModel", "SELECT * FROM \"inbox\" WHERE \"userId\" = $1 AND \"checked\" = false", user_id);
}

SelectQuery get_bombs(long long user_id) {
    return SelectQuery("BombModel", "SELECT * FROM \"bomb\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_costumes(long long user_id) {
    return SelectQuery("CostumeModel", "SELECT * FROM \"costume\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_name_colors(long long user_id) {
    return SelectQuery("NameColorModel", "SELECT * FROM \"name_color\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_nameplates(long long user_id) {
    return SelectQuery("NameplateModel", "SELECT * FROM \"nameplate\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_notes(long long user_id) {
    return SelectQuery("NoteModel", "SELECT * FROM \"note\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_stamps(long long user_id) {
    return SelectQuery("StampModel", "SELECT * FROM \"stamp\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_missions(long long user_id) {
    return SelectQuery("MissionModel", "SELECT * FROM \"mission\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_audition_masters(long long user_id) {
    return SelectQuery("AuditionMasterModel", "SELECT * FROM \"audition_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_bomb_masters(long long user_id) {
    return SelectQuery("BombMasterModel", "SELECT * FROM \"bomb_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_star_rank_masters(long long user_id) {
    return SelectQuery("CharacterStarRankMasterModel", "SELECT * FROM \"character_star_rank_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_star_rank_reward_group_masters(long long user_id) {
    return SelectQuery("CharacterStarRankRewardGroupMasterModel", "SELECT * FROM \"character_star_rank_reward_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_costume_masters(long long user_id) {
    return SelectQuery("CostumeMasterModel", "SELECT * FROM \"costume_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_home_character_voice_masters(long long user_id) {
    return SelectQuery("HomeCharacterVoiceMasterModel", "SELECT * FROM \"home_character_voice_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_name_color_masters(long long user_id) {
    return SelectQuery("NameColorMasterModel", "SELECT * FROM \"name_color_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_nameplate_masters(long long user_id) {
    return SelectQuery("NameplateMasterModel", "SELECT * FROM \"nameplate_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_note_masters(long long user_id) {
    return SelectQuery("NoteMasterModel", "SELECT * FROM \"note_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_spot_conversation_masters(long long user_id) {
    return SelectQuery("SpotConversationMasterModel", "SELECT * FROM \"spot_conversation_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_stamp_masters(long long user_id) {
    return SelectQuery("StampMasterModel", "SELECT * FROM \"stamp_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_lesson_slots(long long user_id) {
    return SelectQuery("CharacterLessonSlotModel", "SELECT * FROM \"character_lesson_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trophys(long long user_id) {
    return SelectQuery("TrophyModel", "SELECT * FROM \"trophy\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_markets(long long user_id) {
    return SelectQuery("MarketModel", "SELECT * FROM \"market\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_market_things(long long user_id) {
    return SelectQuery("MarketThingModel", "SELECT * FROM \"market_thing\" WHERE \"userId\" = $1 ORDER BY \"frameNumber\"", user_id);
}

SelectQuery get_viewed_shops(long long user_id) {
    return SelectQuery("ViewedShopModel", "SELECT * FROM \"viewed_shop\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_game_hints(long long user_id) {
    return SelectQuery("GameHintModel", "SELECT * FROM \"game_hint\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_user_bonuss(long long user_id) {
    return SelectQuery("UserBonusModel", "SELECT * FROM \"user_bonus\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_audition_phase_masters(long long user_id) {
    return SelectQuery("AuditionPhaseMasterModel", "SELECT * FROM \"audition_phase_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_audition_reward_package_masters(long long user_id) {
    return SelectQuery("AuditionRewardPackageMasterModel", "SELECT * FROM \"audition_reward_package_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_campaign_masters(long long user_id) {
    return SelectQuery("CampaignMasterModel", "SELECT * FROM \"campaign_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_awakening_item_masters(long long user_id) {
    return SelectQuery("CharacterAwakeningItemMasterModel", "SELECT * FROM \"character_awakening_item_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_bloom_bonus_group_masters(long long user_id) {
    return SelectQuery("CharacterBloomBonusGroupMasterModel", "SELECT * FROM \"character_bloom_bonus_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_bloom_item_masters(long long user_id) {
    return SelectQuery("CharacterBloomItemMasterModel", "SELECT * FROM \"character_bloom_item_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_experience_item_masters(long long user_id) {
    return SelectQuery("CharacterExperienceItemMasterModel", "SELECT * FROM \"character_experience_item_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_mission_masters(long long user_id) {
    return SelectQuery("CharacterMissionMasterModel", "SELECT * FROM \"character_mission_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_mission_stage_masters(long long user_id) {
    return SelectQuery("CharacterMissionStageMasterModel", "SELECT * FROM \"character_mission_stage_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_piece_masters(long long user_id) {
    return SelectQuery("CharacterPieceMasterModel", "SELECT * FROM \"character_piece_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_sense_enhance_item_group_masters(long long user_id) {
    return SelectQuery("CharacterSenseEnhanceItemGroupMasterModel", "SELECT * FROM \"character_sense_enhance_item_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_costume_wearable_character_group_masters(long long user_id) {
    return SelectQuery("CostumeWearableCharacterGroupMasterModel", "SELECT * FROM \"costume_wearable_character_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_exchange_shop_masters(long long user_id) {
    return SelectQuery("ExchangeShopMasterModel", "SELECT * FROM \"exchange_shop_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_live_setting_masters(long long user_id) {
    return SelectQuery("LiveSettingMasterModel", "SELECT * FROM \"live_setting_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_mission_masters(long long user_id) {
    return SelectQuery("MissionMasterModel", "SELECT * FROM \"mission_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_music_vocal_version_masters(long long user_id) {
    return SelectQuery("MusicVocalVersionMasterModel", "SELECT * FROM \"music_vocal_version_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_poster_release_item_group_masters(long long user_id) {
    return SelectQuery("PosterReleaseItemGroupMasterModel", "SELECT * FROM \"poster_release_item_group_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_poster_release_item_masters(long long user_id) {
    return SelectQuery("PosterReleaseItemMasterModel", "SELECT * FROM \"poster_release_item_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_poster_story_masters(long long user_id) {
    return SelectQuery("PosterStoryMasterModel", "SELECT * FROM \"poster_story_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_star_rank_reward_masters(long long user_id) {
    return SelectQuery("StarRankRewardMasterModel", "SELECT * FROM \"star_rank_reward_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_audition_clears(long long user_id) {
    return SelectQuery("AuditionClearModel", "SELECT * FROM \"audition_clear\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_sp_rates(long long user_id) {
    return SelectQuery("SpRateModel", "SELECT * FROM \"sp_rate\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_notifications(long long user_id) {
    return SelectQuery("NotificationModel", "SELECT * FROM \"notification\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_episodes(long long user_id) {
    return SelectQuery("EpisodeModel", "SELECT * FROM \"episode\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_missions(long long user_id) {
    return SelectQuery("CharacterMissionModel", "SELECT * FROM \"character_mission\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_mission_passs(long long user_id) {
    return SelectQuery("MissionPassModel", "SELECT * FROM \"mission_pass\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_mission_pass_detail_masters(long long user_id) {
    return SelectQuery("MissionPassDetailMasterModel", "SELECT * FROM \"mission_pass_detail_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_mission_pass_masters(long long user_id) {
    return SelectQuery("MissionPassMasterModel", "SELECT * FROM \"mission_pass_master\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_basics(long long user_id) {
    return SelectQuery("LeagueBasicModel", "SELECT * FROM \"league_basic\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_events(long long user_id) {
    return SelectQuery("StoryEventModel", "SELECT * FROM \"story_event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_exchange_limits(long long user_id) {
    return SelectQuery("ExchangeLimitModel", "SELECT * FROM \"exchange_limit\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_groups(long long user_id) {
    return SelectQuery("LeagueGroupModel", "SELECT * FROM \"league_group\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_group_members(long long user_id) {
    return SelectQuery("LeagueGroupMemberModel", "SELECT * FROM \"league_group_member\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_historys(long long user_id) {
    return SelectQuery("LeagueHistoryModel", "SELECT * FROM \"league_history\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_jewel_shops(long long user_id) {
    return SelectQuery("JewelShopModel", "SELECT * FROM \"jewel_shop\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_daily_limits(long long user_id) {
    return SelectQuery("DailyLimitModel", "SELECT * FROM \"daily_limit\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_high_score_partys(long long user_id) {
    return SelectQuery("LeagueHighScorePartyModel", "SELECT * FROM \"league_high_score_party\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_high_score_party_slots(long long user_id) {
    return SelectQuery("LeagueHighScorePartySlotModel", "SELECT * FROM \"league_high_score_party_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_circles(long long user_id) {
    return SelectQuery("StoryEventCircleModel", "SELECT * FROM \"story_event_circle\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_circle_missions(long long user_id) {
    return SelectQuery("StoryEventCircleMissionModel", "SELECT * FROM \"story_event_circle_mission\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_circle_mission_rewards(long long user_id) {
    return SelectQuery("StoryEventCircleMissionRewardModel", "SELECT * FROM \"story_event_circle_mission_reward\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_high_score_buff_settings(long long user_id) {
    return SelectQuery("StoryEventHighScoreBuffSettingModel", "SELECT * FROM \"story_event_high_score_buff_setting\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_high_score_partys(long long user_id) {
    return SelectQuery("StoryEventHighScorePartyModel", "SELECT * FROM \"story_event_high_score_party\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_high_score_party_slots(long long user_id) {
    return SelectQuery("StoryEventHighScorePartySlotModel", "SELECT * FROM \"story_event_high_score_party_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_connect_with_accounts(long long user_id) {
    return SelectQuery("ConnectWithAccountModel", "SELECT * FROM \"connect_with_account\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_connect_with_passwords(long long user_id) {
    return SelectQuery("ConnectWithPasswordModel", "SELECT * FROM \"connect_with_password\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_connect_with_password_by_linkage_code(const std::string& linkage_code) {
    // the takeover lookup is by linkage code alone: the caller is unauthenticated and does
    // not know its own userId yet
    return SelectQuery("ConnectWithPasswordModel", "SELECT * FROM \"connect_with_password\" WHERE \"linkageCode\" = $1", linkage_code);
}

SelectQuery get_tournament_details(long long user_id) {
    return SelectQuery("TournamentDetailModel", "SELECT * FROM \"tournament_detail\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_gradual_mission_groups(long long user_id) {
    return SelectQuery("GradualMissionGroupModel", "SELECT * FROM \"gradual_mission_group\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_photos(long long user_id) {
    return SelectQuery("PhotoModel", "SELECT * FROM \"photo\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_albums(long long user_id) {
    return SelectQuery("AlbumModel", "SELECT * FROM \"album\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_circle_supports(long long user_id) {
    return SelectQuery("CircleSupportModel", "SELECT * FROM \"circle_support\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_album_pages(long long user_id) {
    return SelectQuery("AlbumPageModel", "SELECT * FROM \"album_page\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_star_pass_statuss(long long user_id) {
    return SelectQuery("StarPassStatusModel", "SELECT * FROM \"star_pass_status\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_login_pass_statuss(long long user_id) {
    return SelectQuery("LoginPassStatusModel", "SELECT * FROM \"login_pass_status\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_currencys(long long user_id) {
    return SelectQuery("CurrencyModel", "SELECT * FROM \"currency\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_decorations(long long user_id) {
    return SelectQuery("DecorationModel", "SELECT * FROM \"decoration\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_live_achievements(long long user_id) {
    return SelectQuery("LiveAchievementModel", "SELECT * FROM \"live_achievement\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_music_videos(long long user_id) {
    return SelectQuery("MusicVideoModel", "SELECT * FROM \"music_video\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_theater_storys(long long user_id) {
    return SelectQuery("TheaterStoryModel", "SELECT * FROM \"theater_story\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_live_drop_cellings(long long user_id) {
    return SelectQuery("LiveDropCellingModel", "SELECT * FROM \"live_drop_celling\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_story_event_high_scores(long long user_id) {
    return SelectQuery("StoryEventHighScoreModel", "SELECT * FROM \"story_event_high_score\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_comics(long long user_id) {
    return SelectQuery("ComicModel", "SELECT * FROM \"comic\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_comeback_campaigns(long long user_id) {
    return SelectQuery("ComebackCampaignModel", "SELECT * FROM \"comeback_campaign\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_concert_stages(long long user_id) {
    return SelectQuery("ConcertStageModel", "SELECT * FROM \"concert_stage\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_limits(long long user_id) {
    return SelectQuery("LimitModel", "SELECT * FROM \"limit\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_gacha_selected_things(long long user_id) {
    return SelectQuery("GachaSelectedThingModel", "SELECT * FROM \"gacha_selected_thing\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_total_point_events(long long user_id) {
    return SelectQuery("TotalPointEventModel", "SELECT * FROM \"total_point_event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_event_box_gachas(long long user_id) {
    return SelectQuery("EventBoxGachaModel", "SELECT * FROM \"event_box_gacha\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_event_box_gacha_box_things(long long user_id) {
    return SelectQuery("EventBoxGachaBoxThingModel", "SELECT * FROM \"event_box_gacha_box_thing\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_special_events(long long user_id) {
    return SelectQuery("SpecialEventModel", "SELECT * FROM \"special_event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_character_point_events(long long user_id) {
    return SelectQuery("CharacterPointEventModel", "SELECT * FROM \"character_point_event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_another_notations(long long user_id) {
    return SelectQuery("AnotherNotationModel", "SELECT * FROM \"another_notation\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_music_bookmarks(long long user_id) {
    return SelectQuery("MusicBookmarkModel", "SELECT * FROM \"music_bookmark\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_live_drop_limits(long long user_id) {
    return SelectQuery("LiveDropLimitModel", "SELECT * FROM \"live_drop_limit\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_restrictions(long long user_id) {
    return SelectQuery("RestrictionModel", "SELECT * FROM \"restriction\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_permanent_market_things(long long user_id) {
    return SelectQuery("PermanentMarketThingModel", "SELECT * FROM \"permanent_market_thing\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_time_limited_controls(long long user_id) {
    return SelectQuery("TimeLimitedControlModel", "SELECT * FROM \"time_limited_control\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_flash_sale_stages(long long user_id) {
    return SelectQuery("FlashSaleStageModel", "SELECT * FROM \"flash_sale_stage\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_album_themes(long long user_id) {
    return SelectQuery("AlbumThemeModel", "SELECT * FROM \"album_theme\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_circle_event_missions(long long user_id) {
    return SelectQuery("CircleEventMissionModel", "SELECT * FROM \"circle_event_mission\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_pickup_character_missions(long long user_id) {
    return SelectQuery("PickupCharacterMissionModel", "SELECT * FROM \"pickup_character_mission\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_league_season_results(long long user_id) {
    return SelectQuery("LeagueSeasonResultModel", "SELECT * FROM \"league_season_result\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_events(long long user_id) {
    return SelectQuery("EventModel", "SELECT * FROM \"event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_bonus_lives(long long user_id) {
    return SelectQuery("BonusLiveModel", "SELECT * FROM \"bonus_live\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_bonus_live_stages(long long user_id) {
    return SelectQuery("BonusLiveStageModel", "SELECT * FROM \"bonus_live_stage\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_roulette_events(long long user_id) {
    return SelectQuery("RouletteEventModel", "SELECT * FROM \"roulette_event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_roulettes(long long user_id) {
    return SelectQuery("RouletteModel", "SELECT * FROM \"roulette\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_home_b_g_ms(long long user_id) {
    return SelectQuery("HomeBGMModel", "SELECT * FROM \"home_b_g_m\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_link_characters(long long user_id) {
    return SelectQuery("LinkCharacterModel", "SELECT * FROM \"link_character\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_music_courses(long long user_id) {
    return SelectQuery("MusicCourseModel", "SELECT * FROM \"music_course\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_tournament_qualifyings(long long user_id) {
    return SelectQuery("TournamentQualifyingModel", "SELECT * FROM \"tournament_qualifying\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_lotterys(long long user_id) {
    return SelectQuery("LotteryModel", "SELECT * FROM \"lottery\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_partys(long long user_id) {
    return SelectQuery("TripleCastPartyModel", "SELECT * FROM \"triple_cast_party\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_party_slots(long long user_id) {
    return SelectQuery("TripleCastPartySlotModel", "SELECT * FROM \"triple_cast_party_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_basics(long long user_id) {
    return SelectQuery("TripleCastBasicModel", "SELECT * FROM \"triple_cast_basic\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_groups(long long user_id) {
    return SelectQuery("TripleCastGroupModel", "SELECT * FROM \"triple_cast_group\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_group_members(long long user_id) {
    return SelectQuery("TripleCastGroupMemberModel", "SELECT * FROM \"triple_cast_group_member\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_high_score_partys(long long user_id) {
    return SelectQuery("TripleCastHighScorePartyModel", "SELECT * FROM \"triple_cast_high_score_party\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_high_score_party_slots(long long user_id) {
    return SelectQuery("TripleCastHighScorePartySlotModel", "SELECT * FROM \"triple_cast_high_score_party_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_triple_cast_season_results(long long user_id) {
    return SelectQuery("TripleCastSeasonResultModel", "SELECT * FROM \"triple_cast_season_result\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_album_presets(long long user_id) {
    return SelectQuery("AlbumPresetModel", "SELECT * FROM \"album_preset\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_gachas(long long user_id) {
    return SelectQuery("GachaModel", "SELECT * FROM \"gacha\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_gacha_historys(long long user_id, long long card_type, long long limit) {
    // most recent gacha prizes first. the client asks for the character and poster
    // histories separately, hence the cardtype filter.
    return SelectQuery("GachaHistoryModel", "SELECT * FROM \"gacha_history\" WHERE \"userId\" = $1 AND \"cardType\" = $2 ORDER BY \"createdAt\" DESC, \"rowId\" DESC LIMIT $3", user_id, card_type, limit);
}

SelectQuery get_triple_cast_historys(long long user_id) {
    return SelectQuery("TripleCastHistoryModel", "SELECT * FROM \"triple_cast_history\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_dugong_runs(long long user_id) {
    return SelectQuery("DugongRunModel", "SELECT * FROM \"dugong_run\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_music_course_rankings(long long user_id) {
    return SelectQuery("MusicCourseRankingModel", "SELECT * FROM \"music_course_ranking\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_friend_invitations(long long user_id) {
    return SelectQuery("FriendInvitationModel", "SELECT * FROM \"friend_invitation\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_friend_invitation_missions(long long user_id) {
    return SelectQuery("FriendInvitationMissionModel", "SELECT * FROM \"friend_invitation_mission\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_name_base_colors(long long user_id) {
    return SelectQuery("NameBaseColorModel", "SELECT * FROM \"name_base_color\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_icon_frames(long long user_id) {
    return SelectQuery("IconFrameModel", "SELECT * FROM \"icon_frame\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_gacha_re_rolls(long long user_id) {
    return SelectQuery("GachaReRollModel", "SELECT * FROM \"gacha_re_roll\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trial_party_events(long long user_id) {
    return SelectQuery("TrialPartyEventModel", "SELECT * FROM \"trial_party_event\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trial_party_event_stages(long long user_id) {
    return SelectQuery("TrialPartyEventStageModel", "SELECT * FROM \"trial_party_event_stage\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trial_party_event_stage_partys(long long user_id) {
    return SelectQuery("TrialPartyEventStagePartyModel", "SELECT * FROM \"trial_party_event_stage_party\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_trial_party_event_stage_party_slots(long long user_id) {
    return SelectQuery("TrialPartyEventStagePartySlotModel", "SELECT * FROM \"trial_party_event_stage_party_slot\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_user_blocks(long long user_id) {
    return SelectQuery("UserBlockModel", "SELECT * FROM \"user_block\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_home_skins(long long user_id) {
    return SelectQuery("HomeSkinModel", "SELECT * FROM \"home_skin\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_accessory_auto_sells(long long user_id) {
    return SelectQuery("AccessoryAutoSellModel", "SELECT * FROM \"accessory_auto_sell\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_favorite_costumes(long long user_id) {
    return SelectQuery("FavoriteCostumeModel", "SELECT * FROM \"favorite_costume\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_buff_item_statuss(long long user_id) {
    return SelectQuery("BuffItemStatusModel", "SELECT * FROM \"buff_item_status\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_multi_room_basics(long long user_id) {
    return SelectQuery("MultiRoomBasicModel", "SELECT * FROM \"multi_room_basic\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_event_camps(long long user_id) {
    return SelectQuery("EventCampModel", "SELECT * FROM \"event_camp\" WHERE \"userId\" = $1", user_id);
}

}  // namespace user
}  // namespace db
