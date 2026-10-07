#pragma once
// Generated from server-of-dreams models/master_data + models/keys.py + models/enums.py.
// Do not edit by hand. Table list (header alias -> pydantic row model) and the per-model
// non-zero pydantic defaults injected when a JSON key is absent (wire packs the default, not 0).
// Only alias remap across all reachable models is id_ <- "id" (loader hardcodes it).
namespace master_spec {
struct TableSpec { const char* header_name; const char* row_model; };
static const TableSpec TABLES[229] = {
  {"AccessoryEffectFilterMaster","AccessoryEffectFilterMaster"},
  {"AccessoryEffectMaster","AccessoryEffectMaster"},
  {"AccessoryLevelPatternGroupMaster","AccessoryLevelPatternGroupMaster"},
  {"AccessoryMaster","AccessoryMaster"},
  {"ActivityLogMessageTemplateMaster","ActivityLogMessageTemplateMaster"},
  {"AdditionalRewardPackageMaster","AdditionalRewardPackageMaster"},
  {"AlbumEffectMaster","AlbumEffectMaster"},
  {"AlbumThemeMaster","AlbumThemeMaster"},
  {"AnotherNotationMaster","AnotherNotationMaster"},
  {"AuditionMaster","AuditionMaster"},
  {"AuditionPhaseMaster","AuditionPhaseMaster"},
  {"AuditionRewardPackageMaster","AuditionRewardPackageMaster"},
  {"BannerMaster","BannerMaster"},
  {"BodyMotionMaster","BodyMotionMaster"},
  {"BombMaster","BombMaster"},
  {"BonusLiveMaster","BonusLiveMaster"},
  {"BonusLiveStageMaster","BonusLiveStageMaster"},
  {"BuffItemMaster","BuffItemMaster"},
  {"CampaignMaster","CampaignMaster"},
  {"CategoryGroupMaster","CategoryGroupMaster"},
  {"CategoryMaster","CategoryMaster"},
  {"ChangeBodyMotionMaster","ChangeBodyMotionMaster"},
  {"CharacterAwakeningItemGroupMaster","CharacterAwakeningItemGroupMaster"},
  {"CharacterAwakeningItemMaster","CharacterAwakeningItemMaster"},
  {"CharacterBaseBloomGenericItemMaster","CharacterBaseBloomGenericItemMaster"},
  {"CharacterBaseMaster","CharacterBaseMaster"},
  {"CharacterBloomBonusGroupMaster","CharacterBloomBonusGroupMaster"},
  {"CharacterBloomDetailMaster","CharacterBloomDetailMaster"},
  {"CharacterBloomItemMaster","CharacterBloomItemMaster"},
  {"CharacterEpisodeMaster","CharacterEpisodeMaster"},
  {"CharacterEpisodeRelationMaster","CharacterEpisodeRelationMaster"},
  {"CharacterEpisodeReleaseItemGroupMaster","CharacterEpisodeReleaseItemGroupMaster"},
  {"CharacterExperienceItemMaster","CharacterExperienceItemMaster"},
  {"CharacterKeyMissionMaster","CharacterKeyMissionMaster"},
  {"CharacterLessonScoreRewardMaster","CharacterLessonScoreRewardMaster"},
  {"CharacterLevelMaster","CharacterLevelMaster"},
  {"CharacterMaster","CharacterMaster"},
  {"CharacterMissionCategoryLevelMaster","CharacterMissionCategoryLevelMaster"},
  {"CharacterMissionItemMaster","CharacterMissionItemMaster"},
  {"CharacterMissionMaster","CharacterMissionMaster"},
  {"CharacterMissionStageMaster","CharacterMissionStageMaster"},
  {"CharacterPieceMaster","CharacterPieceMaster"},
  {"CharacterPointEventCharacterRankingRewardItemPackageMaster","CharacterPointEventCharacterRankingRewardItemPackageMaster"},
  {"CharacterPointEventCharacterRankingRewardMaster","CharacterPointEventCharacterRankingRewardMaster"},
  {"CharacterPointEventMaster","CharacterPointEventMaster"},
  {"CharacterProfileRestrictionMaster","CharacterProfileRestrictionMaster"},
  {"CharacterSenseEnhanceItemGroupMaster","CharacterSenseEnhanceItemGroupMaster"},
  {"CharacterStarRankMaster","CharacterStarRankMaster"},
  {"CharacterStarRankRewardGroupMaster","CharacterStarRankRewardGroupMaster"},
  {"CircleEventCirclePointRewardMaster","CircleEventCirclePointRewardMaster"},
  {"CircleEventCircleRankingRewardItemPackageMaster","CircleEventCircleRankingRewardItemPackageMaster"},
  {"CircleEventCircleRankingRewardMaster","CircleEventCircleRankingRewardMaster"},
  {"CircleEventMaster","CircleEventMaster"},
  {"CircleEventMissionMaster","CircleEventMissionMaster"},
  {"CircleEventMissionRefreshSettingGroupMaster","CircleEventMissionRefreshSettingGroupMaster"},
  {"CircleSupportCompanyLevelDetailMaster","CircleSupportCompanyLevelDetailMaster"},
  {"CircleSupportCompanyLevelLimitMaster","CircleSupportCompanyLevelLimitMaster"},
  {"CircleTheaterLevelMaster","CircleTheaterLevelMaster"},
  {"ComebackCampaignMaster","ComebackCampaignMaster"},
  {"ComicMaster","ComicMaster"},
  {"CompanyMaster","CompanyMaster"},
  {"ConcertMaster","ConcertMaster"},
  {"ConcertStageMaster","ConcertStageMaster"},
  {"ConcoursMaster","ConcoursMaster"},
  {"ConcoursPointRewardMaster","ConcoursPointRewardMaster"},
  {"CostumeGroupMaster","CostumeGroupMaster"},
  {"CostumeMaster","CostumeMaster"},
  {"CostumeWearableCharacterGroupMaster","CostumeWearableCharacterGroupMaster"},
  {"CourseRankingRewardMaster","CourseRankingRewardMaster"},
  {"DecorationMaster","DecorationMaster"},
  {"DugongRunCourseMaster","DugongRunCourseMaster"},
  {"DugongRunRewardMaster","DugongRunRewardMaster"},
  {"EffectDurationGroupMaster","EffectDurationGroupMaster"},
  {"EffectMaster","EffectMaster"},
  {"EffectTriggerCharacterBaseGroupMaster","EffectTriggerCharacterBaseGroupMaster"},
  {"EpisodeMaster","EpisodeMaster"},
  {"EpisodeRewardPackageMaster","EpisodeRewardPackageMaster"},
  {"EventBoxGachaBoxMaster","EventBoxGachaBoxMaster"},
  {"EventBoxGachaMaster","EventBoxGachaMaster"},
  {"EventBoxGachaTextTemplateMaster","EventBoxGachaTextTemplateMaster"},
  {"EventCampClassMaster","EventCampClassMaster"},
  {"EventCampMaster","EventCampMaster"},
  {"EventCampSupportPointRewardMaster","EventCampSupportPointRewardMaster"},
  {"EventMaster","EventMaster"},
  {"ExchangeShopMaster","ExchangeShopMaster"},
  {"FacialExpressionMaster","FacialExpressionMaster"},
  {"FilmItemMaster","FilmItemMaster"},
  {"FriendInvitationMissionMaster","FriendInvitationMissionMaster"},
  {"GachaMaster","GachaMaster"},
  {"GachaTextTemplateMaster","GachaTextTemplateMaster"},
  {"GameHintMaster","GameHintMaster"},
  {"GhostLiveMaster","GhostLiveMaster"},
  {"GradualMissionGroupMaster","GradualMissionGroupMaster"},
  {"HeadDirectionMaster","HeadDirectionMaster"},
  {"HeadMotionMaster","HeadMotionMaster"},
  {"HomeBGMDetailMaster","HomeBGMDetailMaster"},
  {"HomeBGMMaster","HomeBGMMaster"},
  {"HomeBackgroundMaster","HomeBackgroundMaster"},
  {"HomeCharacterMaster","HomeCharacterMaster"},
  {"HomeCharacterVoiceMaster","HomeCharacterVoiceMaster"},
  {"HomeCharacterVoicePeriodMaster","HomeCharacterVoicePeriodMaster"},
  {"HomePosterMaster","HomePosterMaster"},
  {"HomeSkinMaster","HomeSkinMaster"},
  {"IconFrameMaster","IconFrameMaster"},
  {"ItemMaster","ItemMaster"},
  {"JewelShopCategoryMaster","JewelShopCategoryMaster"},
  {"JewelShopItemMaster","JewelShopItemMaster"},
  {"LeaderSenseMaster","LeaderSenseMaster"},
  {"LeagueAllClassGlobalRankingRewardThingMaster","LeagueAllClassGlobalRankingRewardPackageMaster"},
  {"LeagueClassGroupMaster","LeagueClassGroupMaster"},
  {"LeagueGroupRankingRewardPackageMaster","LeagueGroupRankingRewardPackageMaster"},
  {"LeagueMaster","LeagueMaster"},
  {"LeaguePlayRewardPackageMaster","LeaguePlayRewardPackageMaster"},
  {"LeagueRewardPackageMaster","LeagueRewardPackageMaster"},
  {"LessonScoreRewardGroupMaster","LessonScoreRewardGroupMaster"},
  {"LessonScoreRewardMaster","LessonScoreRewardMaster"},
  {"LightLoadSplitEffectMaster","LightLoadSplitEffectMaster"},
  {"LipSyncMaster","LipSyncMaster"},
  {"LiveDropFrameGroupMaster","LiveDropFrameGroupMaster"},
  {"LiveMaster","LiveMaster"},
  {"LiveSettingMaster","LiveSettingMaster"},
  {"LoginBonusSpineCostumeMaster","LoginBonusSpineCostumeMaster"},
  {"LoopMotionMaster","LoopMotionMaster"},
  {"MarketFrameThingMaster","MarketFrameThingMaster"},
  {"MissionMaster","MissionMaster"},
  {"MissionPassDetailMaster","MissionPassDetailMaster"},
  {"MissionPassLoopRewardMaster","MissionPassLoopRewardMaster"},
  {"MissionPassLoopRewardThingMaster","MissionPassLoopRewardThingMaster"},
  {"MissionPassMaster","MissionPassMaster"},
  {"MultiLiveScheduleMaster","MultiLiveScheduleMaster"},
  {"MusicCourseMaster","MusicCourseMaster"},
  {"MusicCourseRewardGroupMaster","MusicCourseRewardGroupMaster"},
  {"MusicGroupDetailMaster","MusicGroupDetailMaster"},
  {"MusicGroupMaster","MusicGroupMaster"},
  {"MusicMaster","MusicMaster"},
  {"MusicVideoDefaultCostumeGroupMaster","MusicVideoDefaultCostumeGroupMaster"},
  {"MusicVideoMaster","MusicVideoMaster"},
  {"MusicVocalVersionMaster","MusicVocalVersionMaster"},
  {"NameBaseColorMaster","NameBaseColorMaster"},
  {"NameColorMaster","NameColorMaster"},
  {"NameplateMaster","NameplateMaster"},
  {"NgWordMaster","NgWordMaster"},
  {"NoteMaster","NoteMaster"},
  {"PermanentMarketThingMaster","PermanentMarketThingMaster"},
  {"PhotoEffectChangeItemMaster","PhotoEffectChangeItemMaster"},
  {"PhotoEffectMaster","PhotoEffectMaster"},
  {"PhotoEffectTypeGroupMaster","PhotoEffectTypeGroupMaster"},
  {"PhotoEffectVarietyChangeDetailMaster","PhotoEffectVarietyChangeDetailMaster"},
  {"PhotoEffectVarietyUpDetailMaster","PhotoEffectVarietyUpDetailMaster"},
  {"PhotoLevelUpItemGroupMaster","PhotoLevelUpItemGroupMaster"},
  {"PhotoSpotMaster","PhotoSpotMaster"},
  {"PickupCharacterMissionDetailGroupMaster","PickupCharacterMissionDetailGroupMaster"},
  {"PickupCharacterMissionDetailMaster","PickupCharacterMissionDetailMaster"},
  {"PickupCharacterMissionMaster","PickupCharacterMissionMaster"},
  {"PickupSelectionGachaMaster","PickupSelectionGachaMaster"},
  {"PlayerRankCapMaster","PlayerRankCapMaster"},
  {"PlayerRankMaster","PlayerRankMaster"},
  {"PosterAbilityMaster","PosterAbilityMaster"},
  {"PosterLevelPatternGroupMaster","PosterLevelPatternGroupMaster"},
  {"PosterMaster","PosterMaster"},
  {"PosterReleaseItemGroupMaster","PosterReleaseItemGroupMaster"},
  {"PosterStoryMaster","PosterStoryMaster"},
  {"RandomEffectGroupMaster","RandomEffectGroupMaster"},
  {"ResultVoiceMaster","ResultVoiceMaster"},
  {"RewardRuleMaster","RewardRuleMaster"},
  {"RouletteEventMaster","RouletteEventMaster"},
  {"RouletteMaster","RouletteMaster"},
  {"RoulettePrizeMaster","RoulettePrizeMaster"},
  {"SceneCameraMaster","SceneCameraMaster"},
  {"SenseBranchMaster","SenseBranchMaster"},
  {"SenseMaster","SenseMaster"},
  {"SenseNotationMaster","SenseNotationMaster"},
  {"SensePerformanceMaster","SensePerformanceMaster"},
  {"SignMaster","SignMaster"},
  {"SpecialEpisodeMaster","SpecialEpisodeMaster"},
  {"SpecialEventMaster","SpecialEventMaster"},
  {"SpecialStoryMaster","SpecialStoryMaster"},
  {"SplashMaster","SplashMaster"},
  {"SpotConversationMaster","SpotConversationMaster"},
  {"StaminaRecoveryItemMaster","StaminaRecoveryItemMaster"},
  {"StampMaster","StampMaster"},
  {"StarActBranchMaster","StarActBranchMaster"},
  {"StarActConditionMaster","StarActConditionMaster"},
  {"StarActMaster","StarActMaster"},
  {"StarRankRewardMaster","StarRankRewardMaster"},
  {"StepupGachaGroupMaster","StepupGachaGroupMaster"},
  {"StoryEventBonusCharacterBaseMaster","StoryEventBonusCharacterBaseMaster"},
  {"StoryEventCircleHighScoreRewardMaster","StoryEventCircleHighScoreRewardMaster"},
  {"StoryEventCircleMaster","StoryEventCircleMaster"},
  {"StoryEventCircleMissionMaster","StoryEventCircleMissionMaster"},
  {"StoryEventCircleMissionRewardMaster","StoryEventCircleMissionRewardMaster"},
  {"StoryEventEpisodeMaster","StoryEventEpisodeMaster"},
  {"StoryEventHighScoreBuffMaster","StoryEventHighScoreBuffMaster"},
  {"StoryEventHighScoreBuffPatternGroupMaster","StoryEventHighScoreBuffPatternGroupMaster"},
  {"StoryEventHighScoreBuffSettingMaster","StoryEventHighScoreBuffSettingMaster"},
  {"StoryEventHighScoreMaster","StoryEventHighScoreMaster"},
  {"StoryEventHighScoreRewardMaster","StoryEventHighScoreRewardMaster"},
  {"StoryEventMaster","StoryEventMaster"},
  {"StoryEventRewardItemPackage","StoryEventRewardItemPackage"},
  {"StoryEventRewardMaster","StoryEventRewardMaster"},
  {"StoryEventStoryBgmGroupMaster","StoryEventStoryBgmGroupMaster"},
  {"StoryEventStoryMaster","StoryEventStoryMaster"},
  {"StoryEventTotalPointRewardMaster","StoryEventTotalPointRewardMaster"},
  {"StoryMaster","StoryMaster"},
  {"StoryRelationMaster","StoryRelationMaster"},
  {"TeamChallengeMaster","TeamChallengeMaster"},
  {"TheaterChapterMaster","TheaterChapterMaster"},
  {"TheaterStoryMaster","TheaterStoryMaster"},
  {"TimeLimitedControlMaster","TimeLimitedControlMaster"},
  {"TipMaster","TipMaster"},
  {"TitleBackgroundMaster","TitleBackgroundMaster"},
  {"TitleCallVoiceMaster","TitleCallVoiceMaster"},
  {"TitleDecorationMaster","TitleDecorationMaster"},
  {"TotalPointEventMaster","TotalPointEventMaster"},
  {"TotalPointEventRewardMaster","TotalPointEventRewardMaster"},
  {"TournamentMaster","TournamentMaster"},
  {"TournamentQualifyingMaster","TournamentQualifyingMaster"},
  {"TrialPartyAccessoryMaster","TrialPartyAccessoryMaster"},
  {"TrialPartyCharacterMaster","TrialPartyCharacterMaster"},
  {"TrialPartyEventMaster","TrialPartyEventMaster"},
  {"TrialPartyEventStageMaster","TrialPartyEventStageMaster"},
  {"TrialPartyMaster","TrialPartyMaster"},
  {"TrialPartyPosterMaster","TrialPartyPosterMaster"},
  {"TripleCastAllClassGlobalRankingRewardThingMaster","TripleCastAllClassGlobalRankingRewardPackageMaster"},
  {"TripleCastGroupRankingRewardPackageMaster","TripleCastGroupRankingRewardPackageMaster"},
  {"TripleCastMaster","TripleCastMaster"},
  {"TrophyGroupMaster","TrophyGroupMaster"},
  {"TrophyMaster","TrophyMaster"},
  {"UnlockConditionMaster","UnlockConditionMaster"},
};

struct DefaultSpec { const char* fn; long long ival; };
struct ModelDefaults { const char* model; const DefaultSpec* defs; int count; };

static const DefaultSpec _d_AccessoryEffectFilterMaster[] = { {"effect_type",1LL} };
static const DefaultSpec _d_AccessoryMaster[] = { {"rarity",1LL} };
static const DefaultSpec _d_AchivementRateRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_AdditionalRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_AnotherNotationMaster[] = { {"another_notation_type",1LL} };
static const DefaultSpec _d_AuditionRewardThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_BannerMaster[] = { {"delete_condition_type",1LL} };
static const DefaultSpec _d_BonusLiveMaster[] = { {"unlock_condition_type",1LL} };
static const DefaultSpec _d_BonusLiveStageRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_BuffItemMaster[] = { {"effect_type",1LL} };
static const DefaultSpec _d_CampaignMaster[] = { {"campaign_effect_type",1LL} };
static const DefaultSpec _d_CharacterBaseBloomGenericItemMaster[] = { {"talent_bloom_item_type",1LL} };
static const DefaultSpec _d_CharacterBaseMaster[] = { {"character_base_type",1LL} };
static const DefaultSpec _d_CharacterBloomItemMaster[] = { {"rarity",1LL}, {"talent_bloom_item_type",1LL} };
static const DefaultSpec _d_CharacterBloomRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_CharacterMaster[] = { {"rarity",1LL}, {"attribute",1LL} };
static const DefaultSpec _d_CharacterMissionCategoryLevelMaster[] = { {"category_type",1LL} };
static const DefaultSpec _d_CharacterPieceMaster[] = { {"talent_bloom_item_type",1LL} };
static const DefaultSpec _d_CharacterPointEventCharacterRankingRewardItemMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_CharacterProfileRestrictionMaster[] = { {"profile_restriction_item",1LL} };
static const DefaultSpec _d_CharacterStarRankRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_CircleEventCirclePointRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_CircleEventCircleRankingRewardItem[] = { {"thing_type",1LL} };
static const DefaultSpec _d_CircleEventMissionMaster[] = { {"mission_category",1LL} };
static const DefaultSpec _d_ConcertRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_ConcoursPointRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_CourseRankingRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_DecorationMaster[] = { {"category",3LL} };
static const DefaultSpec _d_DugongRunCourseMaster[] = { {"difficulty",1LL} };
static const DefaultSpec _d_DugongRunRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_EffectConditionMaster[] = { {"condition",1LL} };
static const DefaultSpec _d_EffectMaster[] = { {"type",1LL}, {"calculation_type",1LL}, {"fire_timing_type",1LL} };
static const DefaultSpec _d_EffectTriggerMaster[] = { {"trigger",1LL} };
static const DefaultSpec _d_EpisodeReleaseCondition[] = { {"condition_type",1LL} };
static const DefaultSpec _d_EpisodeRewardThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_EventBonusMaster[] = { {"bonus_target_type",1LL} };
static const DefaultSpec _d_EventBoxGachaBoxThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_EventMaster[] = { {"event_type",20LL} };
static const DefaultSpec _d_EventPointRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_EventRankingRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_ExchangeShopMaster[] = { {"display_thing_type",1LL} };
static const DefaultSpec _d_ExchangeShopThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_FriendInvitationMissionRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_GachaBonusThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_GachaDetailBonusThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_GachaDetailMaster[] = { {"button_type",1LL} };
static const DefaultSpec _d_GachaMaster[] = { {"card_type",1LL}, {"gacha_type",1LL} };
static const DefaultSpec _d_GachaRollBonus[] = { {"thing_type",1LL} };
static const DefaultSpec _d_GachaThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_GameHintMaster[] = { {"page_category",1LL} };
static const DefaultSpec _d_HomeBGMMaster[] = { {"selection_type",1LL} };
static const DefaultSpec _d_ItemMaster[] = { {"category",11LL}, {"rarity",1LL} };
static const DefaultSpec _d_JewelShopItemMaster[] = { {"purchase_type",1LL} };
static const DefaultSpec _d_JewelShopThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LeagueAllClassGlobalRankingRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LeagueGroupRankingRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LeaguePlayRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LeagueRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LessonScoreRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LiveDropThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_LiveSettingMaster[] = { {"live_type",1LL} };
static const DefaultSpec _d_LoopMotionMaster[] = { {"size",1LL} };
static const DefaultSpec _d_MarketFrameThingMaster[] = { {"thing_type",1LL}, {"required_thing_type",1LL} };
static const DefaultSpec _d_MissionMaster[] = { {"mission_category",1LL} };
static const DefaultSpec _d_MissionPassLoopRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_MissionPassRewardThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_MissionRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_MusicCourseMaster[] = { {"music_course_type",1LL} };
static const DefaultSpec _d_MusicCourseRewardThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_MusicMaster[] = { {"unlock_condition_type",1LL}, {"music_cover_type",1LL} };
static const DefaultSpec _d_PermanentMarketThingMaster[] = { {"thing_type",1LL}, {"required_thing_type",1LL} };
static const DefaultSpec _d_PhotoLevelUpItemGroupMaster[] = { {"rarity",1LL} };
static const DefaultSpec _d_PickupCharacterMissionDetailMaster[] = { {"check_condition",1LL} };
static const DefaultSpec _d_PickupCharacterMissionDetailRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_PosterAbilityMaster[] = { {"type",1LL} };
static const DefaultSpec _d_PosterMaster[] = { {"rarity",1LL}, {"orientation",1LL} };
static const DefaultSpec _d_ResultVoiceMaster[] = { {"condition_type",1LL} };
static const DefaultSpec _d_RouletteMaster[] = { {"roulette_type",1LL} };
static const DefaultSpec _d_RoulettePrizeThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_RouletteRollRewardThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_SenseMaster[] = { {"type",1LL} };
static const DefaultSpec _d_SpecialEventMaster[] = { {"category_type",1LL} };
static const DefaultSpec _d_SpecialStoryMaster[] = { {"event_story_list_flags",1LL} };
static const DefaultSpec _d_SplashMaster[] = { {"splash_type",1LL} };
static const DefaultSpec _d_SpotConversationMaster[] = { {"spot",1LL} };
static const DefaultSpec _d_StampMaster[] = { {"type",1LL} };
static const DefaultSpec _d_StoryEventBonusCharacterBaseMaster[] = { {"bonus_target_type",1LL} };
static const DefaultSpec _d_StoryEventCircleHighScoreRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_StoryEventCircleMissionRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_StoryEventHighScoreRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_StoryEventMaster[] = { {"category",1LL} };
static const DefaultSpec _d_StoryEventRewardThing[] = { {"thing_type",1LL} };
static const DefaultSpec _d_StoryEventStoryMaster[] = { {"event_story_list_flags",1LL} };
static const DefaultSpec _d_StoryEventTotalPointRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_TeamChallengeMaster[] = { {"difficulty",1LL}, {"goal_type",1LL} };
static const DefaultSpec _d_TitleBackgroundDetailMaster[] = { {"path_type",1LL} };
static const DefaultSpec _d_TitleDecorationMaster[] = { {"decoration_type",1LL} };
static const DefaultSpec _d_TotalPointEventRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_TrialPartyEventStageRewardMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_TripleCastAllClassGlobalRankingRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_TripleCastGroupRankingRewardThingMaster[] = { {"thing_type",1LL} };
static const DefaultSpec _d_TrophyGroupMaster[] = { {"category",1LL} };
static const DefaultSpec _d_TrophyMaster[] = { {"rarity",1LL} };
static const DefaultSpec _d_UnlockConditionMaster[] = { {"condition_usable_table_filter",1LL}, {"unlock_condition_type",1LL} };

static const ModelDefaults MODEL_DEFAULTS[104] = {
  {"AccessoryEffectFilterMaster",_d_AccessoryEffectFilterMaster,1},
  {"AccessoryMaster",_d_AccessoryMaster,1},
  {"AchivementRateRewardMaster",_d_AchivementRateRewardMaster,1},
  {"AdditionalRewardThingMaster",_d_AdditionalRewardThingMaster,1},
  {"AnotherNotationMaster",_d_AnotherNotationMaster,1},
  {"AuditionRewardThing",_d_AuditionRewardThing,1},
  {"BannerMaster",_d_BannerMaster,1},
  {"BonusLiveMaster",_d_BonusLiveMaster,1},
  {"BonusLiveStageRewardThingMaster",_d_BonusLiveStageRewardThingMaster,1},
  {"BuffItemMaster",_d_BuffItemMaster,1},
  {"CampaignMaster",_d_CampaignMaster,1},
  {"CharacterBaseBloomGenericItemMaster",_d_CharacterBaseBloomGenericItemMaster,1},
  {"CharacterBaseMaster",_d_CharacterBaseMaster,1},
  {"CharacterBloomItemMaster",_d_CharacterBloomItemMaster,2},
  {"CharacterBloomRewardMaster",_d_CharacterBloomRewardMaster,1},
  {"CharacterMaster",_d_CharacterMaster,2},
  {"CharacterMissionCategoryLevelMaster",_d_CharacterMissionCategoryLevelMaster,1},
  {"CharacterPieceMaster",_d_CharacterPieceMaster,1},
  {"CharacterPointEventCharacterRankingRewardItemMaster",_d_CharacterPointEventCharacterRankingRewardItemMaster,1},
  {"CharacterProfileRestrictionMaster",_d_CharacterProfileRestrictionMaster,1},
  {"CharacterStarRankRewardMaster",_d_CharacterStarRankRewardMaster,1},
  {"CircleEventCirclePointRewardMaster",_d_CircleEventCirclePointRewardMaster,1},
  {"CircleEventCircleRankingRewardItem",_d_CircleEventCircleRankingRewardItem,1},
  {"CircleEventMissionMaster",_d_CircleEventMissionMaster,1},
  {"ConcertRewardMaster",_d_ConcertRewardMaster,1},
  {"ConcoursPointRewardThingMaster",_d_ConcoursPointRewardThingMaster,1},
  {"CourseRankingRewardThingMaster",_d_CourseRankingRewardThingMaster,1},
  {"DecorationMaster",_d_DecorationMaster,1},
  {"DugongRunCourseMaster",_d_DugongRunCourseMaster,1},
  {"DugongRunRewardMaster",_d_DugongRunRewardMaster,1},
  {"EffectConditionMaster",_d_EffectConditionMaster,1},
  {"EffectMaster",_d_EffectMaster,3},
  {"EffectTriggerMaster",_d_EffectTriggerMaster,1},
  {"EpisodeReleaseCondition",_d_EpisodeReleaseCondition,1},
  {"EpisodeRewardThing",_d_EpisodeRewardThing,1},
  {"EventBonusMaster",_d_EventBonusMaster,1},
  {"EventBoxGachaBoxThingMaster",_d_EventBoxGachaBoxThingMaster,1},
  {"EventMaster",_d_EventMaster,1},
  {"EventPointRewardMaster",_d_EventPointRewardMaster,1},
  {"EventRankingRewardThingMaster",_d_EventRankingRewardThingMaster,1},
  {"ExchangeShopMaster",_d_ExchangeShopMaster,1},
  {"ExchangeShopThing",_d_ExchangeShopThing,1},
  {"FriendInvitationMissionRewardMaster",_d_FriendInvitationMissionRewardMaster,1},
  {"GachaBonusThing",_d_GachaBonusThing,1},
  {"GachaDetailBonusThing",_d_GachaDetailBonusThing,1},
  {"GachaDetailMaster",_d_GachaDetailMaster,1},
  {"GachaMaster",_d_GachaMaster,2},
  {"GachaRollBonus",_d_GachaRollBonus,1},
  {"GachaThing",_d_GachaThing,1},
  {"GameHintMaster",_d_GameHintMaster,1},
  {"HomeBGMMaster",_d_HomeBGMMaster,1},
  {"ItemMaster",_d_ItemMaster,2},
  {"JewelShopItemMaster",_d_JewelShopItemMaster,1},
  {"JewelShopThing",_d_JewelShopThing,1},
  {"LeagueAllClassGlobalRankingRewardThingMaster",_d_LeagueAllClassGlobalRankingRewardThingMaster,1},
  {"LeagueGroupRankingRewardThingMaster",_d_LeagueGroupRankingRewardThingMaster,1},
  {"LeaguePlayRewardThingMaster",_d_LeaguePlayRewardThingMaster,1},
  {"LeagueRewardThingMaster",_d_LeagueRewardThingMaster,1},
  {"LessonScoreRewardMaster",_d_LessonScoreRewardMaster,1},
  {"LiveDropThingMaster",_d_LiveDropThingMaster,1},
  {"LiveSettingMaster",_d_LiveSettingMaster,1},
  {"LoopMotionMaster",_d_LoopMotionMaster,1},
  {"MarketFrameThingMaster",_d_MarketFrameThingMaster,2},
  {"MissionMaster",_d_MissionMaster,1},
  {"MissionPassLoopRewardThingMaster",_d_MissionPassLoopRewardThingMaster,1},
  {"MissionPassRewardThing",_d_MissionPassRewardThing,1},
  {"MissionRewardMaster",_d_MissionRewardMaster,1},
  {"MusicCourseMaster",_d_MusicCourseMaster,1},
  {"MusicCourseRewardThing",_d_MusicCourseRewardThing,1},
  {"MusicMaster",_d_MusicMaster,2},
  {"PermanentMarketThingMaster",_d_PermanentMarketThingMaster,2},
  {"PhotoLevelUpItemGroupMaster",_d_PhotoLevelUpItemGroupMaster,1},
  {"PickupCharacterMissionDetailMaster",_d_PickupCharacterMissionDetailMaster,1},
  {"PickupCharacterMissionDetailRewardMaster",_d_PickupCharacterMissionDetailRewardMaster,1},
  {"PosterAbilityMaster",_d_PosterAbilityMaster,1},
  {"PosterMaster",_d_PosterMaster,2},
  {"ResultVoiceMaster",_d_ResultVoiceMaster,1},
  {"RouletteMaster",_d_RouletteMaster,1},
  {"RoulettePrizeThingMaster",_d_RoulettePrizeThingMaster,1},
  {"RouletteRollRewardThing",_d_RouletteRollRewardThing,1},
  {"SenseMaster",_d_SenseMaster,1},
  {"SpecialEventMaster",_d_SpecialEventMaster,1},
  {"SpecialStoryMaster",_d_SpecialStoryMaster,1},
  {"SplashMaster",_d_SplashMaster,1},
  {"SpotConversationMaster",_d_SpotConversationMaster,1},
  {"StampMaster",_d_StampMaster,1},
  {"StoryEventBonusCharacterBaseMaster",_d_StoryEventBonusCharacterBaseMaster,1},
  {"StoryEventCircleHighScoreRewardMaster",_d_StoryEventCircleHighScoreRewardMaster,1},
  {"StoryEventCircleMissionRewardMaster",_d_StoryEventCircleMissionRewardMaster,1},
  {"StoryEventHighScoreRewardMaster",_d_StoryEventHighScoreRewardMaster,1},
  {"StoryEventMaster",_d_StoryEventMaster,1},
  {"StoryEventRewardThing",_d_StoryEventRewardThing,1},
  {"StoryEventStoryMaster",_d_StoryEventStoryMaster,1},
  {"StoryEventTotalPointRewardMaster",_d_StoryEventTotalPointRewardMaster,1},
  {"TeamChallengeMaster",_d_TeamChallengeMaster,2},
  {"TitleBackgroundDetailMaster",_d_TitleBackgroundDetailMaster,1},
  {"TitleDecorationMaster",_d_TitleDecorationMaster,1},
  {"TotalPointEventRewardMaster",_d_TotalPointEventRewardMaster,1},
  {"TrialPartyEventStageRewardMaster",_d_TrialPartyEventStageRewardMaster,1},
  {"TripleCastAllClassGlobalRankingRewardThingMaster",_d_TripleCastAllClassGlobalRankingRewardThingMaster,1},
  {"TripleCastGroupRankingRewardThingMaster",_d_TripleCastGroupRankingRewardThingMaster,1},
  {"TrophyGroupMaster",_d_TrophyGroupMaster,1},
  {"TrophyMaster",_d_TrophyMaster,1},
  {"UnlockConditionMaster",_d_UnlockConditionMaster,2},
};
static const int MODEL_DEFAULTS_COUNT = 104;
}
