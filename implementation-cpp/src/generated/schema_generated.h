#pragma once
// Generated from server-of-dreams scripts/database_setup.py (peewee models),
// scripts/database_migration/migrate_2_to_3.py and app.py. Do not edit by hand.
// Postgres schema translated to bundled SQLite; DB_VERSION=3.
namespace schema {

static constexpr int DB_VERSION = 3;

enum ColType { INT, BIGINT, REAL, BOOL, TEXT, JSON };

// Ordered, idempotent CREATE/seed statements (FK parents first).
static const char* const DDL[] = {
  "CREATE TABLE IF NOT EXISTS \"sequences\" (\"name\" TEXT PRIMARY KEY, \"value\" INTEGER)",
  "INSERT OR IGNORE INTO \"sequences\" (\"name\", \"value\") VALUES ('user_id_seq', 0)",
  "INSERT OR IGNORE INTO \"sequences\" (\"name\", \"value\") VALUES ('live_id_seq', 0)",
  "CREATE TABLE IF NOT EXISTS \"databaseinfo\" (\"version\" INTEGER PRIMARY KEY)",
  "CREATE TABLE IF NOT EXISTS \"accounts\" (\"userId\" INTEGER PRIMARY KEY, \"credential\" TEXT NOT NULL UNIQUE, \"apiToken\" TEXT, \"platform\" TEXT NOT NULL DEFAULT 'Android', \"banLevel\" INTEGER NOT NULL DEFAULT 0, \"registeredAt\" INTEGER NOT NULL DEFAULT 0, \"lastLoginAt\" INTEGER NOT NULL DEFAULT 0)",
  "CREATE TABLE IF NOT EXISTS \"user\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"playerRank\" INTEGER NOT NULL DEFAULT 0, \"currentRankPoint\" INTEGER NOT NULL DEFAULT 0, \"currentStamina\" INTEGER NOT NULL DEFAULT 0, \"maxStaminaRestoredAt\" INTEGER NOT NULL DEFAULT 0, \"playerRankLimit\" INTEGER NOT NULL DEFAULT 0, \"staminaRecoverTimesWithJewel\" INTEGER NOT NULL DEFAULT 0, \"circleUsageRestrictionsEndTime\" INTEGER NOT NULL DEFAULT 0, \"circleId\" TEXT, \"gameStartAt\" INTEGER NOT NULL DEFAULT 0, \"hashUserId\" TEXT, \"banLevel\" INTEGER NOT NULL DEFAULT 0, \"tutorialStatus\" INTEGER NOT NULL DEFAULT 0, \"monthlyPayment\" INTEGER NOT NULL DEFAULT 0, \"splashLastDisplayedAt\" INTEGER NOT NULL DEFAULT 0, \"isCapedPlayerRank\" BOOLEAN NOT NULL DEFAULT false, \"requireCapedPlayerRankAnnounce\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"user_profile\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"introduction\" TEXT, \"mainUCharacterId\" INTEGER NOT NULL DEFAULT 0, \"mNameplateId\" INTEGER, \"mNameColorId\" INTEGER NOT NULL DEFAULT 0, \"mTrophyId1\" INTEGER, \"mTrophyId2\" INTEGER, \"mTrophyId3\" INTEGER, \"playerRate\" REAL NOT NULL DEFAULT 0, \"isPublicPlayerRate\" BOOLEAN NOT NULL DEFAULT false, \"leagueClass\" INTEGER NOT NULL DEFAULT 0, \"totalSpCount\" INTEGER NOT NULL DEFAULT 0, \"isPublicAlbumMainPage\" BOOLEAN NOT NULL DEFAULT false, \"mNameplateDetailId\" INTEGER, \"mainCharacterMasterId\" INTEGER NOT NULL DEFAULT 0, \"displayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, \"isPublicActivityLog\" BOOLEAN NOT NULL DEFAULT false, \"nameBaseColorMasterId\" INTEGER NOT NULL DEFAULT 0, \"iconFrameMasterId\" INTEGER NOT NULL DEFAULT 0, \"homeSkinMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"user_preference\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"multiPartyId\" INTEGER NOT NULL DEFAULT 0, \"birthDate\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"home_display_preference\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"homeCharacterBaseMasterId\" INTEGER, \"memberCharacterBaseMasterId\" INTEGER, \"storyCharacterBaseMasterId\" INTEGER, \"shopCharacterBaseMasterId\" INTEGER, \"homeCostumeMasterId\" INTEGER, \"memberCostumeMasterId\" INTEGER, \"storyCostumeMasterId\" INTEGER, \"shopCostumeMasterId\" INTEGER, \"illustCharacterMasterId\" INTEGER NOT NULL DEFAULT 0, \"displayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, \"homeCharacterDisplayType\" INTEGER NOT NULL DEFAULT 0, \"loginBonusCharacterBaseMasterId\" INTEGER, \"loginBonusCostumeMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterMasterId\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"currentExperience\" INTEGER NOT NULL DEFAULT 0, \"talentStage\" INTEGER NOT NULL DEFAULT 0, \"awakeningPhase\" INTEGER NOT NULL DEFAULT 0, \"characterBaseId\" INTEGER NOT NULL DEFAULT 0, \"senseLevel\" INTEGER NOT NULL DEFAULT 0, \"readEpisodeOrder\" INTEGER NOT NULL DEFAULT 0, \"releasedEpisodeOrder\" INTEGER NOT NULL DEFAULT 0, \"displayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, \"secondaryCharacterBaseId\" INTEGER, \"secondarySenseLevel\" INTEGER NOT NULL DEFAULT 0, \"selectionType\" INTEGER NOT NULL DEFAULT 0, \"isFavorite\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_base\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"starRank\" INTEGER NOT NULL DEFAULT 0, \"totalStarPoint\" INTEGER NOT NULL DEFAULT 0, \"costumeMasterId\" INTEGER, \"keyMissionLevel\" INTEGER NOT NULL DEFAULT 0, \"portalCharacterId\" INTEGER NOT NULL DEFAULT 0, \"portalDisplayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"party\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"order\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"party_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"partyId\" INTEGER NOT NULL DEFAULT 0, \"position\" INTEGER NOT NULL DEFAULT 0, \"characterId\" INTEGER NOT NULL DEFAULT 0, \"posterId\" INTEGER, \"accessoryId\" INTEGER, \"bonusAbilityEnableFlags\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"assetId\" TEXT, \"rarity\" INTEGER NOT NULL DEFAULT 0, \"attribute\" INTEGER NOT NULL DEFAULT 0, \"minLevelStatus\" JSONTEXT, \"starActMasterId\" INTEGER NOT NULL DEFAULT 0, \"awakenStarActMasterId\" INTEGER, \"senseMasterId\" INTEGER NOT NULL DEFAULT 0, \"forbidGenericItemBloom\" BOOLEAN NOT NULL DEFAULT false, \"bloomBonusGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"senseEnhanceItemGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"firstEpisodeReleaseItemGroupId\" INTEGER NOT NULL DEFAULT 0, \"secondEpisodeReleaseItemGroupId\" INTEGER NOT NULL DEFAULT 0, \"characterAwakeningItemGroupMasterId\" INTEGER, \"displayStartAt\" INTEGER NOT NULL DEFAULT 0, \"displayEndAt\" INTEGER NOT NULL DEFAULT 0, \"unlockText\" TEXT, \"categories\" JSONTEXT, \"leaderSenseMasterId\" INTEGER, \"maxTalentStage\" INTEGER NOT NULL DEFAULT 0, \"maxTalentStageReleaseDate\" INTEGER, \"secondaryCharacterBaseMasterId\" INTEGER, \"secondarySenseMasterId\" INTEGER, \"secondaryAttribute\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_base_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"school\" TEXT, \"grade\" INTEGER, \"birthMonth\" INTEGER, \"birthDay\" INTEGER, \"height\" INTEGER NOT NULL DEFAULT 0, \"hobby\" TEXT, \"companyMasterId\" INTEGER NOT NULL DEFAULT 0, \"nameRomanization\" TEXT, \"senseName\" TEXT, \"senseEffect\" TEXT, \"characterVoice\" TEXT, \"profileImageAssetId\" TEXT, \"age\" INTEGER, \"familyNameRomanization\" TEXT, \"firstNameRomanization\" TEXT, \"pronounceFamilyName\" TEXT, \"pronounceFirstName\" TEXT, \"familyName\" TEXT, \"firstName\" TEXT, \"evoSenseName\" TEXT, \"evoSenseEffect\" TEXT, \"defaultCostumeMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterBaseType\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_level_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"level\" INTEGER NOT NULL DEFAULT 0, \"experienceToLevelUp\" INTEGER NOT NULL DEFAULT 0, \"characterStatusLevel\" INTEGER NOT NULL DEFAULT 0, \"startDate\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"posterMasterId\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"breakthroughPhase\" INTEGER NOT NULL DEFAULT 0, \"releasedEpisode\" INTEGER NOT NULL DEFAULT 0, \"itemConsumeBreakThroughCount\" INTEGER NOT NULL DEFAULT 0, \"isFavorite\" BOOLEAN NOT NULL DEFAULT false, \"alternativeImagePattern\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"accessory_level_pattern_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"patterns\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"accessory_level_pattern_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"accessoryLevelPatternGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"requiredCoin\" INTEGER NOT NULL DEFAULT 0, \"items\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"accessory_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"rarity\" INTEGER NOT NULL DEFAULT 0, \"accessoryLevelPatternGroupId\" INTEGER NOT NULL DEFAULT 0, \"fixedAccessoryEffects\" JSONTEXT, \"randomEffectGroups\" JSONTEXT, \"pronounceName\" TEXT, \"series\" INTEGER NOT NULL DEFAULT 0, \"maxLevel\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"episode_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyMasterId\" INTEGER NOT NULL DEFAULT 0, \"title\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"episodeRewardPackageMasterId\" INTEGER NOT NULL DEFAULT 0, \"conditions\" JSONTEXT, \"preEpisodeMasterId\" INTEGER, \"displayStartDate\" INTEGER, \"displayEndDate\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"episode_reward_package_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"rewards\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"live_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"difficulty\" INTEGER NOT NULL DEFAULT 0, \"musicMasterId\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"noteCount\" INTEGER NOT NULL DEFAULT 0, \"unlockCondition\" INTEGER NOT NULL DEFAULT 0, \"unlockValue\" INTEGER, \"startDate\" INTEGER NOT NULL DEFAULT 0, \"endDate\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"rewardRuleMasterId\" INTEGER NOT NULL DEFAULT 0, \"pronounceName\" TEXT, \"lyricWriter\" TEXT, \"composer\" TEXT, \"arranger\" TEXT, \"unlockText\" TEXT, \"isLongVersion\" BOOLEAN NOT NULL DEFAULT false, \"releasedAt\" INTEGER, \"staminaConsumption\" INTEGER NOT NULL DEFAULT 0, \"musicTimeSecond\" INTEGER NOT NULL DEFAULT 0, \"invisible\" BOOLEAN NOT NULL DEFAULT false, \"sampleStartSeconds\" REAL NOT NULL DEFAULT 0, \"sampleEndSeconds\" REAL NOT NULL DEFAULT 0, \"delaySeconds\" REAL NOT NULL DEFAULT 0, \"vocalVersions\" JSONTEXT, \"unlockConditionType\" INTEGER NOT NULL DEFAULT 0, \"unlockConditionValue\" INTEGER, \"musicVideoType\" INTEGER NOT NULL DEFAULT 0, \"musicCoverType\" INTEGER NOT NULL DEFAULT 0, \"storyEventMasterId\" INTEGER, \"storyMasterId\" INTEGER, \"eventMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"sense_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"type\" INTEGER NOT NULL DEFAULT 0, \"preEffects\" JSONTEXT, \"branches\" JSONTEXT, \"acquirableGauge\" INTEGER NOT NULL DEFAULT 0, \"acquirableScorePercent\" INTEGER NOT NULL DEFAULT 0, \"scoreUpPerLevel\" INTEGER NOT NULL DEFAULT 0, \"lightCount\" INTEGER NOT NULL DEFAULT 0, \"coolTime\" INTEGER NOT NULL DEFAULT 0, \"branchCondition1\" INTEGER NOT NULL DEFAULT 0, \"conditionValue1\" INTEGER, \"branchCondition2\" INTEGER NOT NULL DEFAULT 0, \"conditionValue2\" INTEGER, \"subTypes\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"type\" INTEGER NOT NULL DEFAULT 0, \"companyMasterId\" INTEGER, \"eventMasterId\" INTEGER, \"chapterOrder\" INTEGER NOT NULL DEFAULT 0, \"displayStartAt\" INTEGER NOT NULL DEFAULT 0, \"displayEndAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster_level_pattern_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"patterns\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster_level_pattern_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"levelPatternGroupId\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"quantity\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"organizeRestrictGroupId\" INTEGER, \"rarity\" INTEGER NOT NULL DEFAULT 0, \"levelPatternGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"subTitlePositionX1\" REAL, \"subTitlePositionY1\" REAL, \"subTitlePositionX2\" REAL, \"subTitlePositionY2\" REAL, \"subTitlePositionX3\" REAL, \"subTitlePositionY3\" REAL, \"releaseItemGroupId\" INTEGER NOT NULL DEFAULT 0, \"pronounceName\" TEXT, \"costumes\" JSONTEXT, \"appearanceCharacterBaseMasterIds\" JSONTEXT, \"isRestrictItemBreakThrough\" BOOLEAN NOT NULL DEFAULT false, \"displayStartAt\" INTEGER NOT NULL DEFAULT 0, \"displayEndAt\" INTEGER NOT NULL DEFAULT 0, \"unlockText\" TEXT, \"orientation\" INTEGER NOT NULL DEFAULT 0, \"subTitleDisplayCondition\" INTEGER NOT NULL DEFAULT 0, \"subTitleDisplayConditionValue\" INTEGER, \"posterBreakthroughMaxPhase\" INTEGER, \"posterBreakthroughMaxPhaseReleaseDate\" INTEGER, \"secondarySubTitleDisplayCondition\" INTEGER NOT NULL DEFAULT 0, \"secondarySubTitleDisplayConditionValue\" INTEGER, \"alternateImagePositionX1\" REAL, \"alternateImagePositionY1\" REAL, \"alternateImageReleasePhase1\" INTEGER, \"alternateImagePositionX2\" REAL, \"alternateImagePositionY2\" REAL, \"alternateImageReleasePhase2\" INTEGER, \"alternateImagePositionX3\" REAL, \"alternateImagePositionY3\" REAL, \"alternateImageReleasePhase3\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"live\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"liveMasterId\" INTEGER NOT NULL DEFAULT 0, \"timesCompleted\" INTEGER NOT NULL DEFAULT 0, \"achievementRate\" REAL NOT NULL DEFAULT 0, \"notationRate\" REAL NOT NULL DEFAULT 0, \"clearLamp\" INTEGER NOT NULL DEFAULT 0, \"status\" INTEGER NOT NULL DEFAULT 0, \"rateGrade\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"musicMasterId\" INTEGER NOT NULL DEFAULT 0, \"stellaReleased\" BOOLEAN NOT NULL DEFAULT false, \"vocalVersion\" INTEGER NOT NULL DEFAULT 0, \"olivierReleaseStatus\" INTEGER NOT NULL DEFAULT 0, \"isPossession\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"accessory\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"accessoryMasterId\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"locked\" BOOLEAN NOT NULL DEFAULT false, \"accessoryEffects\" JSONTEXT, \"referenceCounting\" INTEGER NOT NULL DEFAULT 0, \"isFavorite\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"item\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"stock\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"accessory_effect_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"effectMasterId\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"variety\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"company_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"companies\" INTEGER NOT NULL DEFAULT 0, \"description\" TEXT, \"isOther\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"effect_duration_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"durations\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"effect_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"type\" INTEGER NOT NULL DEFAULT 0, \"range\" INTEGER NOT NULL DEFAULT 0, \"calculationType\" INTEGER NOT NULL DEFAULT 0, \"details\" JSONTEXT, \"conditions\" JSONTEXT, \"durationSecond\" INTEGER NOT NULL DEFAULT 0, \"triggers\" JSONTEXT, \"fireTimingType\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"item_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"displayOrder\" INTEGER NOT NULL DEFAULT 0, \"displayEndDate\" INTEGER, \"maxStock\" INTEGER NOT NULL DEFAULT 0, \"category\" INTEGER NOT NULL DEFAULT 0, \"consumable\" BOOLEAN NOT NULL DEFAULT false, \"jumpType\" INTEGER, \"jumpTargetId\" INTEGER, \"tabCategory\" INTEGER NOT NULL DEFAULT 0, \"rarity\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"random_effect_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"accessoryEffects\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"reward_rule_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"achivementRateRewards\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"sense_effect_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"order\" INTEGER NOT NULL DEFAULT 0, \"effectMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trophy_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"category\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trophy_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"rarity\" INTEGER NOT NULL DEFAULT 0, \"order\" INTEGER NOT NULL DEFAULT 0, \"trophyGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"hidden\" BOOLEAN NOT NULL DEFAULT false, \"unlockText\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_lesson\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"setCharacters\" JSONTEXT, \"bestScore\" INTEGER NOT NULL DEFAULT 0, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, \"rewardReceivedHighScore\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"daily_lesson\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"timesLeft\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"inbox\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"thingType\" INTEGER NOT NULL DEFAULT 0, \"thingId\" INTEGER NOT NULL DEFAULT 0, \"thingQuantity\" INTEGER NOT NULL DEFAULT 0, \"isTimeLimited\" BOOLEAN NOT NULL DEFAULT false, \"hasReceived\" BOOLEAN NOT NULL DEFAULT false, \"title\" TEXT, \"description\" TEXT, \"sentAt\" INTEGER NOT NULL DEFAULT 0, \"receivedAt\" INTEGER, \"receiveLimitAt\" INTEGER NOT NULL DEFAULT 0, \"checked\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"bomb\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"bombMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"costume\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"costumeMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"name_color\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"nameColorMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"nameplate\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"namePlateMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"note\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"noteMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"stamp\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"stampMasterIds\" JSONTEXT, \"favoriteStampMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"mission\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"isCleared\" BOOLEAN NOT NULL DEFAULT false, \"isRewardReceived\" BOOLEAN NOT NULL DEFAULT false, \"missionCurrentCount\" INTEGER NOT NULL DEFAULT 0, \"missionMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentMissionStageMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"audition_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"musicMasterId\" INTEGER NOT NULL DEFAULT 0, \"recommendedCompany\" INTEGER NOT NULL DEFAULT 0, \"canSkip\" BOOLEAN NOT NULL DEFAULT false, \"senseNotationMasterId\" INTEGER NOT NULL DEFAULT 0, \"maxPhase\" TEXT, \"displayStartAt\" INTEGER NOT NULL DEFAULT 0, \"displayEndAt\" INTEGER NOT NULL DEFAULT 0, \"vocalVersion\" INTEGER NOT NULL DEFAULT 0, \"auditionGroupNumber\" INTEGER NOT NULL DEFAULT 0, \"skipStartAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"bomb_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"hidden\" BOOLEAN NOT NULL DEFAULT false, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_star_rank_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"rank\" INTEGER NOT NULL DEFAULT 0, \"nextRankPoint\" INTEGER NOT NULL DEFAULT 0, \"requiredLessonScore\" INTEGER NOT NULL DEFAULT 0, \"statusBonus\" REAL NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_star_rank_reward_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"rewards\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"costume_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, \"costumeGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"description\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"home_character_voice_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"text\" TEXT, \"weight\" INTEGER NOT NULL DEFAULT 0, \"characterVoicePeriodMasterId\" INTEGER, \"isPlayerBirthDateVoice\" BOOLEAN NOT NULL DEFAULT false, \"voiceFileName1\" TEXT, \"voiceFileName2\" TEXT, \"voiceFileName3\" TEXT, \"voiceFileName4\" TEXT, \"voiceInterval1\" REAL NOT NULL DEFAULT 0, \"voiceInterval2\" REAL NOT NULL DEFAULT 0, \"voiceInterval3\" REAL NOT NULL DEFAULT 0, \"mouthMotionId1\" TEXT, \"mouthMotionId2\" TEXT, \"mouthMotionId3\" TEXT, \"mouthMotionId4\" TEXT, \"bodyMotionId1\" TEXT, \"bodyMotionId2\" TEXT, \"bodyMotionId3\" TEXT, \"bodyMotionId4\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"name_color_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"hidden\" BOOLEAN NOT NULL DEFAULT false, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, \"unlockText\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"nameplate_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"hidden\" BOOLEAN NOT NULL DEFAULT false, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, \"details\" JSONTEXT, \"unlockText\" TEXT, \"changeType\" INTEGER NOT NULL DEFAULT 0, \"changeValue1\" INTEGER, \"changeValue2\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"note_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"description\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"hidden\" BOOLEAN NOT NULL DEFAULT false, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"spot_conversation_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"spot\" INTEGER NOT NULL DEFAULT 0, \"characterId1\" INTEGER, \"characterId2\" INTEGER, \"characterId3\" INTEGER, \"characterId4\" INTEGER, \"characterId5\" INTEGER, \"episodeMasterId\" INTEGER NOT NULL DEFAULT 0, \"costumeId1\" INTEGER, \"costumeId2\" INTEGER, \"costumeId3\" INTEGER, \"costumeId4\" INTEGER, \"costumeId5\" INTEGER, \"title\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"stamp_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"order\" INTEGER NOT NULL DEFAULT 0, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"type\" INTEGER NOT NULL DEFAULT 0, \"assetId\" TEXT, \"voiceAssetId\" TEXT, \"name\" TEXT, \"characterBaseMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_lesson_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"position\" INTEGER NOT NULL DEFAULT 0, \"setCharacterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trophy\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"trophyMasterId\" INTEGER NOT NULL DEFAULT 0, \"trophyGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentOrder\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"market\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"lastRefreshedAt\" INTEGER NOT NULL DEFAULT 0, \"refreshTimes\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"market_thing\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"frameNumber\" INTEGER NOT NULL DEFAULT 0, \"marketFrameThingMasterId\" INTEGER NOT NULL DEFAULT 0, \"hasPurchased\" BOOLEAN NOT NULL DEFAULT FALSE, \"discountPercent\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"viewed_shop\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"exchangeShopMasterId\" INTEGER, \"lastViewedAt\" INTEGER NOT NULL DEFAULT 0, \"viewedShopCategory\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"game_hint\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"pageCategory\" INTEGER NOT NULL DEFAULT 0, \"hasAlreadyRead\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"user_bonus\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"experienceBonus\" REAL NOT NULL DEFAULT 0, \"lessonStarRankBonus\" REAL NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"audition_phase_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"auditionasterId\" INTEGER NOT NULL DEFAULT 0, \"phase\" INTEGER NOT NULL DEFAULT 0, \"recommendedPlayerRank\" INTEGER NOT NULL DEFAULT 0, \"clearScore\" INTEGER NOT NULL DEFAULT 0, \"starActCount\" INTEGER, \"auditionRewardPackageMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"audition_reward_package_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"rewards\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"campaign_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"title\" TEXT, \"description\" TEXT, \"iconImagePath\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"startDate\" INTEGER NOT NULL DEFAULT 0, \"endDate\" INTEGER NOT NULL DEFAULT 0, \"comebackCampaignMasterId\" INTEGER, \"campaignEffectType\" INTEGER NOT NULL DEFAULT 0, \"campaignEffectValue\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_awakening_item_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"awakeningPhase\" INTEGER NOT NULL DEFAULT 0, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"requiredQuantity\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_bloom_bonus_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"bloomBonuses\" JSONTEXT, \"bloomRewards\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_bloom_item_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"rarity\" INTEGER NOT NULL DEFAULT 0, \"currentStage\" INTEGER NOT NULL DEFAULT 0, \"requiredPieceAmount\" INTEGER NOT NULL DEFAULT 0, \"talentBloomItemType\" INTEGER NOT NULL DEFAULT 0, \"genericBloomItemMasterId\" INTEGER, \"requiredItemMasterId\" INTEGER, \"requiredItemAmount\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_experience_item_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"acquirableExperience\" INTEGER NOT NULL DEFAULT 0, \"acquirableExperienceBonus\" REAL NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_mission_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"title\" TEXT, \"jumpType\" INTEGER, \"jumpValue\" INTEGER, \"stages\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_mission_stage_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterMissionCategoryLevelMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterMissionMasterId\" INTEGER NOT NULL DEFAULT 0, \"exclusionNoSenseCharacter\" BOOLEAN NOT NULL DEFAULT false, \"order\" INTEGER NOT NULL DEFAULT 0, \"stageOrder\" INTEGER NOT NULL DEFAULT 0, \"goalCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_piece_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterMasterId\" INTEGER NOT NULL DEFAULT 0, \"dugongRequiredAmount\" INTEGER NOT NULL DEFAULT 0, \"talentBloomItemType\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_sense_enhance_item_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"items\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"costume_wearable_character_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"exchange_shop_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"isDisplayRequiredHavingItem\" BOOLEAN NOT NULL DEFAULT false, \"category\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"displayThingType\" INTEGER NOT NULL DEFAULT 0, \"displayItemMasterId\" INTEGER, \"bannerPath\" TEXT, \"startDate\" INTEGER, \"endDate\" INTEGER, \"lastRefreshedAt\" INTEGER NOT NULL DEFAULT 0, \"lineup\" JSONTEXT, \"order\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"live_setting_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"liveType\" INTEGER NOT NULL DEFAULT 0, \"liveDropFrameGroupMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"mission_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"missionCategory\" INTEGER NOT NULL DEFAULT 0, \"missionViewOrder\" INTEGER NOT NULL DEFAULT 0, \"title\" TEXT, \"description\" TEXT, \"eventMasterId\" INTEGER, \"jumpType\" INTEGER NOT NULL DEFAULT 0, \"jumpTargetId\" INTEGER, \"startDate\" INTEGER NOT NULL DEFAULT 0, \"endDate\" INTEGER NOT NULL DEFAULT 0, \"stages\" JSONTEXT, \"comebackCampaignMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music_vocal_version_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"musicMasterId\" INTEGER NOT NULL DEFAULT 0, \"vocalVersion\" INTEGER NOT NULL DEFAULT 0, \"singer\" TEXT, \"name\" TEXT, \"musicTimeSecond\" INTEGER NOT NULL DEFAULT 0, \"sampleStartSeconds\" REAL NOT NULL DEFAULT 0, \"sampleEndSeconds\" REAL NOT NULL DEFAULT 0, \"musicVideoType\" INTEGER NOT NULL DEFAULT 0, \"characters\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster_release_item_group_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"items\" JSONTEXT, \"itemConsumeApplyFlag\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster_release_item_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"currentPhase\" INTEGER NOT NULL DEFAULT 0, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"requiredQuantity\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"poster_story_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"posterMasterId\" INTEGER NOT NULL DEFAULT 0, \"episodeType\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER, \"description\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, \"characterIconId\" INTEGER, \"characterName\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"star_rank_reward_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"rank\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterStarRankRewardGroupMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"audition_clear\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"auditionMasterId\" INTEGER NOT NULL DEFAULT 0, \"clearPhase\" INTEGER NOT NULL DEFAULT 0, \"auditionClearPartyId\" INTEGER NOT NULL DEFAULT 0, \"skipClearPhase\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"sp_rate\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"liveMasterId\" INTEGER NOT NULL DEFAULT 0, \"point\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"notification\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"importantReadAt\" INTEGER NOT NULL DEFAULT 0, \"updateReadAt\" INTEGER NOT NULL DEFAULT 0, \"bugReadAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"episode\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"episodeMasterId\" INTEGER NOT NULL DEFAULT 0, \"hasReadAll\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_mission\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterMissionMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentStageMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentCount\" INTEGER NOT NULL DEFAULT 0, \"clearedStageOrder\" INTEGER NOT NULL DEFAULT 0, \"rewardReceivedStageOrder\" INTEGER NOT NULL DEFAULT 0, \"completedLevel\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"mission_pass\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"missionPassMasterId\" INTEGER NOT NULL DEFAULT 0, \"paid\" BOOLEAN NOT NULL DEFAULT false, \"freeRewardReceivedPhase\" INTEGER, \"spRewardReceivedPhase\" INTEGER, \"terminated\" BOOLEAN NOT NULL DEFAULT false, \"freeRewardLoopCount\" INTEGER, \"freeRewardLoopReceivedPhase\" INTEGER, \"paidRewardLoopCount\" INTEGER, \"paidRewardLoopReceivedPhase\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"mission_pass_detail_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"phase\" INTEGER NOT NULL DEFAULT 0, \"missionPassMasterId\" INTEGER, \"clearPoint\" INTEGER NOT NULL DEFAULT 0, \"startDate\" INTEGER NOT NULL DEFAULT 0, \"endDate\" INTEGER NOT NULL DEFAULT 0, \"rewards\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"mission_pass_master\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"itemMasterId\" INTEGER NOT NULL DEFAULT 0, \"startDate\" INTEGER NOT NULL DEFAULT 0, \"endDate\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_basic\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"myProperty\" INTEGER NOT NULL DEFAULT 0, \"starEnrollCount\" INTEGER NOT NULL DEFAULT 0, \"daiStarEnrollCount\" INTEGER NOT NULL DEFAULT 0, \"currentClassType\" INTEGER NOT NULL DEFAULT 0, \"bestClassType\" INTEGER NOT NULL DEFAULT 0, \"lastJoinedLeagueSeasonMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"totalAcquiredPoint\" INTEGER NOT NULL DEFAULT 0, \"acquiredPointUpdatedDate\" INTEGER NOT NULL DEFAULT 0, \"lastRank\" INTEGER, \"readTips\" BOOLEAN NOT NULL DEFAULT false, \"loginDays\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"exchange_limit\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"exchangeShopThingId\" INTEGER NOT NULL DEFAULT 0, \"replaceType\" INTEGER NOT NULL DEFAULT 0, \"specifiedNumberOfDaysLimit\" INTEGER, \"exchangedCount\" INTEGER NOT NULL DEFAULT 0, \"until\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_group\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"leagueMasterId\" INTEGER NOT NULL DEFAULT 0, \"classType\" INTEGER NOT NULL DEFAULT 0, \"classOrder\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_group_member\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"leagueGroupId\" INTEGER NOT NULL DEFAULT 0, \"leagueMasterId\" INTEGER NOT NULL DEFAULT 0, \"bestScore\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_history\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"leagueMasterId\" INTEGER NOT NULL DEFAULT 0, \"classType\" INTEGER NOT NULL DEFAULT 0, \"historyCount\" INTEGER NOT NULL DEFAULT 0, \"isSendedReward\" BOOLEAN NOT NULL DEFAULT false, \"isPlayed\" BOOLEAN NOT NULL DEFAULT false, \"classChangeType\" INTEGER NOT NULL DEFAULT 0, \"groupRank\" INTEGER NOT NULL DEFAULT 0, \"globalRank\" INTEGER NOT NULL DEFAULT 0, \"allClassGlobalRank\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"jewel_shop\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"jewelShopItemMasterId\" INTEGER NOT NULL DEFAULT 0, \"purchaseCount\" INTEGER NOT NULL DEFAULT 0, \"totalPurchaseCount\" INTEGER NOT NULL DEFAULT 0, \"rePurchaseDate\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"daily_limit\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"autoPlayTimes\" INTEGER NOT NULL DEFAULT 0, \"dailyLessonTimes\" INTEGER NOT NULL DEFAULT 0, \"lastRefreshedAt\" INTEGER, \"musicCourseFreeChallengeTimes\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_high_score_party\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"leagueMasterId\" INTEGER NOT NULL DEFAULT 0, \"highScore\" INTEGER NOT NULL DEFAULT 0, \"classType\" INTEGER NOT NULL DEFAULT 0, \"difficulty\" INTEGER NOT NULL DEFAULT 0, \"musicMasterId\" INTEGER NOT NULL DEFAULT 0, \"leagueGroupId\" INTEGER NOT NULL DEFAULT 0, \"slots\" JSONTEXT, \"userName\" TEXT, \"actingAbility\" INTEGER NOT NULL DEFAULT 0, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_high_score_party_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"leagueHighScorePartyId\" INTEGER NOT NULL DEFAULT 0, \"position\" INTEGER NOT NULL DEFAULT 0, \"characterMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterLevel\" INTEGER NOT NULL DEFAULT 0, \"posterMasterId\" INTEGER, \"posterLevel\" INTEGER, \"posterBreakthroughPhase\" INTEGER, \"accessoryMasterId\" INTEGER, \"accessoryLevel\" INTEGER, \"currentStatus\" JSONTEXT, \"characterTalentStage\" INTEGER NOT NULL DEFAULT 0, \"characterAwakeningPhase\" INTEGER NOT NULL DEFAULT 0, \"characterDisplayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_circle\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentPoint\" INTEGER NOT NULL DEFAULT 0, \"highScore\" INTEGER NOT NULL DEFAULT 0, \"circleId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_circle_mission\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventCircleMissionMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_circle_mission_reward\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"storyEventCircleMissionRewardMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_high_score_buff_setting\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"storyEventHighScoreBuffSettingMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentLevel\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_high_score_party\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"highScore\" INTEGER NOT NULL DEFAULT 0, \"rateGrade\" INTEGER NOT NULL DEFAULT 0, \"difficulty\" INTEGER NOT NULL DEFAULT 0, \"highScoreType\" INTEGER NOT NULL DEFAULT 0, \"liveSettingMasterId\" INTEGER NOT NULL DEFAULT 0, \"slots\" JSONTEXT, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_high_score_party_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventHighScoreClearPartyId\" INTEGER NOT NULL DEFAULT 0, \"position\" INTEGER NOT NULL DEFAULT 0, \"characterMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterLevel\" INTEGER NOT NULL DEFAULT 0, \"characterTalentStage\" INTEGER NOT NULL DEFAULT 0, \"characterAwakeningPhase\" INTEGER NOT NULL DEFAULT 0, \"posterMasterId\" INTEGER, \"posterLevel\" INTEGER, \"posterBreakthroughPhase\" INTEGER, \"accessoryMasterId\" INTEGER, \"accessoryLevel\" INTEGER, \"currentStatus\" JSONTEXT, \"characterDisplayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"connect_with_account\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"provider\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"connect_with_password\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL UNIQUE, \"id\" INTEGER NOT NULL DEFAULT 0, \"passwordHash\" TEXT, \"linkageCode\" TEXT, \"confirmationCode\" TEXT, \"confirmationExpiresAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"tournament_detail\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"tournamentDetailMasterId\" INTEGER NOT NULL DEFAULT 0, \"bestUniqueScore\" INTEGER NOT NULL DEFAULT 0, \"perfectStar\" INTEGER NOT NULL DEFAULT 0, \"perfect\" INTEGER NOT NULL DEFAULT 0, \"great\" INTEGER NOT NULL DEFAULT 0, \"good\" INTEGER NOT NULL DEFAULT 0, \"bad\" INTEGER NOT NULL DEFAULT 0, \"miss\" INTEGER NOT NULL DEFAULT 0, \"recordedAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"gradual_mission_group\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"gradualMissionGroupMasterId\" INTEGER NOT NULL DEFAULT 0, \"startAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"photo\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"fileName\" TEXT, \"sasToken\" TEXT, \"photoEffectMasterId\" INTEGER, \"lock\" BOOLEAN NOT NULL DEFAULT false, \"useAlbumPage\" INTEGER, \"level\" INTEGER NOT NULL DEFAULT 0, \"rarity\" INTEGER NOT NULL DEFAULT 0, \"signMasterId\" INTEGER, \"generatedAt\" INTEGER NOT NULL DEFAULT 0, \"thumbnailSasToken\" TEXT, \"appearedCharacterBaseMasterIds\" JSONTEXT, \"taggedCharacterBaseMasterIds\" JSONTEXT, \"useDecoPage\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"album\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"publishPageNumber\" INTEGER NOT NULL DEFAULT 0, \"currentPresetOrder\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"circle_support\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"company\" INTEGER NOT NULL DEFAULT 0, \"level\" INTEGER NOT NULL DEFAULT 0, \"currentSupportPoint\" INTEGER NOT NULL DEFAULT 0, \"lastLevelUppedAt\" INTEGER, \"levelLimit\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"album_page\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"page\" INTEGER NOT NULL DEFAULT 0, \"editType\" INTEGER NOT NULL DEFAULT 0, \"publishing\" BOOLEAN NOT NULL DEFAULT false, \"items\" JSONTEXT, \"albumThemeMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"star_pass_status\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"type\" INTEGER NOT NULL DEFAULT 0, \"totalPurchasedCount\" INTEGER NOT NULL DEFAULT 0, \"validUntil\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"login_pass_status\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"validUntil\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"currency\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"coin\" INTEGER NOT NULL DEFAULT 0, \"freeJewel\" INTEGER NOT NULL DEFAULT 0, \"paidJewel\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"decoration\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"decorationMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"live_achievement\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"olivierReleasedCount\" INTEGER NOT NULL DEFAULT 0, \"olivierClearedLevel\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music_video\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"musicVideoMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"theater_story\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"theaterStoryMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"live_drop_celling\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"multiLiveScheduleMasterId\" INTEGER NOT NULL DEFAULT 0, \"count\" INTEGER NOT NULL DEFAULT 0, \"totalCellingCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"story_event_high_score\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"storyEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentEnhancementPoint\" INTEGER NOT NULL DEFAULT 0, \"totalAcquiredEnhancementPoint\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"comic\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"comicEpisodeMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"comeback_campaign\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"comebackCampaignMasterId\" INTEGER NOT NULL DEFAULT 0, \"activatedAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"concert_stage\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"concertStageMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"limit\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"additionalAcquirablePhotoLimit\" INTEGER NOT NULL DEFAULT 0, \"acquirablePhotoLimitIncreasedTimes\" INTEGER NOT NULL DEFAULT 0, \"additionalAcquirableAccessoryLimit\" INTEGER NOT NULL DEFAULT 0, \"acquirableAccessoryLimitIncreasedTimes\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"gacha_selected_thing\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"gachaMasterId\" INTEGER NOT NULL DEFAULT 0, \"gachaThingIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"total_point_event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"totalPointEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"totalAcquiredPoint\" INTEGER NOT NULL DEFAULT 0, \"receivedRewardOrder\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"event_box_gacha\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"eventBoxGachaMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentBoxCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"event_box_gacha_box_thing\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"eventBoxGachaBoxThingMasterId\" INTEGER NOT NULL DEFAULT 0, \"hitCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"special_event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"specialEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"readTips\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"character_point_event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterPointEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER, \"totalAcquiredPoint\" INTEGER NOT NULL DEFAULT 0, \"lastRank\" INTEGER NOT NULL DEFAULT 0, \"readTips\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"another_notation\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"anotherNotationMasterId\" INTEGER NOT NULL DEFAULT 0, \"clearLamp\" INTEGER NOT NULL DEFAULT 0, \"rateGrade\" INTEGER NOT NULL DEFAULT 0, \"achievementRatePercentRecord\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music_bookmark\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"musicMasterId\" INTEGER NOT NULL DEFAULT 0, \"musicBookmarkFlag\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"live_drop_limit\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"multiLiveScheduleMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentCount\" INTEGER NOT NULL DEFAULT 0, \"countLimit\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"restriction\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"multiLiveRestrictionFinishedAt\" INTEGER, \"readMultiLiveRestrictionDialog\" BOOLEAN, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"permanent_market_thing\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"permanentMarketThingMasterId\" INTEGER NOT NULL DEFAULT 0, \"purchaseCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"time_limited_control\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"timeLimitedControlMasterId\" INTEGER NOT NULL DEFAULT 0, \"expiredAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"flash_sale_stage\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"flashSaleStageMasterId\" INTEGER NOT NULL DEFAULT 0, \"purchaseLimitedAt\" INTEGER, \"isDefault\" BOOLEAN NOT NULL DEFAULT false, \"isCompleted\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"album_theme\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"albumThemeMasterId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"circle_event_mission\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"circleEventMissionMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentCount\" INTEGER NOT NULL DEFAULT 0, \"isActive\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"pickup_character_mission\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"pickupCharacterMissionMasterId\" INTEGER NOT NULL DEFAULT 0, \"receivedDetailMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"league_season_result\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"leagueSeasonMasterId\" INTEGER NOT NULL DEFAULT 0, \"daiStarMaxEnrollCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"eventMasterId\" INTEGER NOT NULL DEFAULT 0, \"totalAcquiredPoint\" INTEGER NOT NULL DEFAULT 0, \"acquiredPointUpdatedDate\" INTEGER NOT NULL DEFAULT 0, \"lastRank\" INTEGER, \"readTips\" BOOLEAN NOT NULL DEFAULT false, \"loginDays\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"bonus_live\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"bonusLiveMasterId\" INTEGER NOT NULL DEFAULT 0, \"clearedStageOrder\" INTEGER NOT NULL DEFAULT 0, \"readTips\" BOOLEAN NOT NULL DEFAULT false, \"dailyClearTimes\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"bonus_live_stage\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"bonusLiveMasterStageId\" INTEGER NOT NULL DEFAULT 0, \"clearTimes\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"roulette_event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"rouletteEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"totalAcquiredPoint\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"roulette\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"rouletteMasterId\" INTEGER NOT NULL DEFAULT 0, \"rollCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"home_b_g_m\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"homeBGMMasterId\" INTEGER NOT NULL DEFAULT 0, \"selectionType\" INTEGER NOT NULL DEFAULT 0, \"homeBGMDetailMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"link_character\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"companyMasterId\" INTEGER NOT NULL DEFAULT 0, \"linkedCharacterBaseMasterId\" INTEGER, \"rewardReceivedMaxRank\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music_course\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"musicCourseMasterId\" INTEGER NOT NULL DEFAULT 0, \"clearLamp\" INTEGER NOT NULL DEFAULT 0, \"certificationGrade\" INTEGER NOT NULL DEFAULT 0, \"totalAchievementRatePercentRecord\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"tournament_qualifying\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"tournamentQualifyingMasterId\" INTEGER NOT NULL DEFAULT 0, \"musicCourseMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentChallengeCount\" INTEGER NOT NULL DEFAULT 0, \"perfectStar\" INTEGER NOT NULL DEFAULT 0, \"perfect\" INTEGER NOT NULL DEFAULT 0, \"great\" INTEGER NOT NULL DEFAULT 0, \"good\" INTEGER NOT NULL DEFAULT 0, \"bad\" INTEGER NOT NULL DEFAULT 0, \"miss\" INTEGER NOT NULL DEFAULT 0, \"totalAchievementRatePercentRecord\" JSONTEXT, \"bestRecordChallengeCount\" INTEGER NOT NULL DEFAULT 0, \"bestRecordDate\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"lottery\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"lotteryMasterId\" INTEGER NOT NULL DEFAULT 0, \"results\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_party\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"order\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_party_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"partyId\" INTEGER NOT NULL DEFAULT 0, \"position\" INTEGER NOT NULL DEFAULT 0, \"characterId\" INTEGER NOT NULL DEFAULT 0, \"posterId\" INTEGER, \"accessoryId\" INTEGER, \"bonusAbilityEnableFlags\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_basic\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"starEnrollCount\" INTEGER NOT NULL DEFAULT 0, \"daiStarEnrollCount\" INTEGER NOT NULL DEFAULT 0, \"currentClassType\" INTEGER NOT NULL DEFAULT 0, \"bestClassType\" INTEGER NOT NULL DEFAULT 0, \"lastJoinedTripleCastSeasonMasterId\" INTEGER, \"partyOrder1\" INTEGER, \"partyOrder2\" INTEGER, \"partyOrder3\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_group\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"tripleCastMasterId\" INTEGER NOT NULL DEFAULT 0, \"classType\" INTEGER NOT NULL DEFAULT 0, \"classOrder\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_group_member\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"tripleCastGroupId\" INTEGER NOT NULL DEFAULT 0, \"tripleCastMasterId\" INTEGER NOT NULL DEFAULT 0, \"bestScore\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_high_score_party\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"tripleCastMasterId\" INTEGER NOT NULL DEFAULT 0, \"order\" INTEGER NOT NULL DEFAULT 0, \"highScore\" INTEGER NOT NULL DEFAULT 0, \"slots\" JSONTEXT, \"actingAbility\" INTEGER NOT NULL DEFAULT 0, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, \"difficulty\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_high_score_party_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"tripleCastHighScorePartyId\" INTEGER NOT NULL DEFAULT 0, \"position\" INTEGER NOT NULL DEFAULT 0, \"characterMasterId\" INTEGER NOT NULL DEFAULT 0, \"characterLevel\" INTEGER NOT NULL DEFAULT 0, \"posterMasterId\" INTEGER, \"posterLevel\" INTEGER, \"posterBreakthroughPhase\" INTEGER, \"accessoryMasterId\" INTEGER, \"accessoryLevel\" INTEGER, \"currentStatus\" JSONTEXT, \"characterTalentStage\" INTEGER NOT NULL DEFAULT 0, \"characterAwakeningPhase\" INTEGER NOT NULL DEFAULT 0, \"characterDisplayAwakeningStatus\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_season_result\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"tripleCastSeasonMasterId\" INTEGER NOT NULL DEFAULT 0, \"daiStarMaxEnrollCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"album_preset\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"name\" TEXT, \"order\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"gacha\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"gachaMasterId\" INTEGER NOT NULL DEFAULT 0, \"rollCount\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"triple_cast_history\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"tripleCastMasterId\" INTEGER NOT NULL DEFAULT 0, \"classType\" INTEGER NOT NULL DEFAULT 0, \"historyCount\" INTEGER NOT NULL DEFAULT 0, \"isSendedReward\" BOOLEAN NOT NULL DEFAULT false, \"isPlayed\" BOOLEAN NOT NULL DEFAULT false, \"classChangeType\" INTEGER NOT NULL DEFAULT 0, \"groupRank\" INTEGER NOT NULL DEFAULT 0, \"globalRank\" INTEGER NOT NULL DEFAULT 0, \"allClassGlobalRank\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"dugong_run\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"clearedCourseIds\" JSONTEXT, \"noMistakeCourseIds\" JSONTEXT, \"dugongRunCourseGroupId\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"music_course_ranking\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"musicCourseMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentChallengeCount\" INTEGER NOT NULL DEFAULT 0, \"perfectStar\" INTEGER NOT NULL DEFAULT 0, \"perfect\" INTEGER NOT NULL DEFAULT 0, \"great\" INTEGER NOT NULL DEFAULT 0, \"good\" INTEGER NOT NULL DEFAULT 0, \"bad\" INTEGER NOT NULL DEFAULT 0, \"miss\" INTEGER NOT NULL DEFAULT 0, \"totalAchievementRatePercentRecord\" JSONTEXT, \"bestRecordChallengeCount\" INTEGER NOT NULL DEFAULT 0, \"bestRecordDate\" INTEGER, \"hasReceivedReward\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"friend_invitation\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"invitationCode\" TEXT, \"hasInputOtherInvitationCode\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"friend_invitation_mission\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"friendInvitationMissionMasterId\" INTEGER NOT NULL DEFAULT 0, \"friendInvitationMissionStageMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentCount\" INTEGER NOT NULL DEFAULT 0, \"isCleared\" BOOLEAN NOT NULL DEFAULT false, \"isRewardReceived\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"name_base_color\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"nameBaseColorMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"icon_frame\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"iconFrameMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"gacha_re_roll\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"gachaMasterId\" INTEGER NOT NULL DEFAULT 0, \"rollCount\" INTEGER NOT NULL DEFAULT 0, \"isDecided\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trial_party_event\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"trialPartyEventMasterId\" INTEGER NOT NULL DEFAULT 0, \"currentStageOrder\" INTEGER NOT NULL DEFAULT 0, \"isCompleted\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trial_party_event_stage\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"trialPartyEventStageMasterId\" INTEGER NOT NULL DEFAULT 0, \"isCleared\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trial_party_event_stage_party\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"trialPartyEventStageMasterId\" INTEGER NOT NULL DEFAULT 0, \"leaderPosition\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"trial_party_event_stage_party_slot\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"trialPartyEventStagePartyId\" INTEGER NOT NULL DEFAULT 0, \"position\" INTEGER NOT NULL DEFAULT 0, \"trialPartyCharacterMasterId\" INTEGER NOT NULL DEFAULT 0, \"trialPartyPosterMasterId\" INTEGER, \"trialPartyAccessoryMasterId\" INTEGER, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"user_block\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"blockUserId\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"friend\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"friendUserId\" INTEGER NOT NULL, \"isFavorite\" BOOLEAN NOT NULL DEFAULT false, \"createdAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"friend_request\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"fromUserId\" INTEGER NOT NULL, \"toUserId\" INTEGER NOT NULL, \"createdAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"fromUserId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"home_skin\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"homeSkinMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"accessory_auto_sell\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"autoSellRarity\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"favorite_costume\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"characterBaseMasterId\" INTEGER NOT NULL DEFAULT 0, \"favoriteCostumeMasterIds\" JSONTEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"buff_item_status\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"effectType\" INTEGER NOT NULL DEFAULT 0, \"buffItemMasterId\" INTEGER NOT NULL DEFAULT 0, \"validUntil\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"multi_room_basic\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"ownerMultiRoomId\" TEXT, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"event_camp\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"id\" INTEGER NOT NULL DEFAULT 0, \"eventMasterId\" INTEGER NOT NULL DEFAULT 0, \"campType\" INTEGER NOT NULL DEFAULT 0, \"totalSupportPoint\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"hash_user_id\" (\"hashUserId\" INTEGER PRIMARY KEY, \"userId\" INTEGER NOT NULL UNIQUE, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"active_live\" (\"userId\" INTEGER PRIMARY KEY, \"id\" INTEGER NOT NULL, \"liveMasterId\" INTEGER NOT NULL DEFAULT 0, \"partyId\" INTEGER NOT NULL DEFAULT 0, \"liveSettingMasterId\" INTEGER NOT NULL DEFAULT 0, \"staminaSpent\" BOOLEAN NOT NULL DEFAULT false, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE TABLE IF NOT EXISTS \"gacha_history\" (\"rowId\" INTEGER PRIMARY KEY AUTOINCREMENT, \"userId\" INTEGER NOT NULL, \"cardType\" INTEGER NOT NULL DEFAULT 0, \"masterId\" INTEGER NOT NULL DEFAULT 0, \"createdAt\" INTEGER NOT NULL DEFAULT 0, FOREIGN KEY (\"userId\") REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE)",
  "CREATE INDEX IF NOT EXISTS \"user_userId\" ON \"user\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"user_profile_userId\" ON \"user_profile\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"user_preference_userId\" ON \"user_preference\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"home_display_preference_userId\" ON \"home_display_preference\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_userId\" ON \"character\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_base_userId\" ON \"character_base\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"party_userId\" ON \"party\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"party_slot_userId\" ON \"party_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_master_userId\" ON \"character_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_base_master_userId\" ON \"character_base_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_level_master_userId\" ON \"character_level_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_userId\" ON \"poster\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"accessory_level_pattern_group_master_userId\" ON \"accessory_level_pattern_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"accessory_level_pattern_master_userId\" ON \"accessory_level_pattern_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"accessory_master_userId\" ON \"accessory_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"episode_master_userId\" ON \"episode_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"episode_reward_package_master_userId\" ON \"episode_reward_package_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"live_master_userId\" ON \"live_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_master_userId\" ON \"music_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"sense_master_userId\" ON \"sense_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_master_userId\" ON \"story_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_level_pattern_group_master_userId\" ON \"poster_level_pattern_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_level_pattern_master_userId\" ON \"poster_level_pattern_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_master_userId\" ON \"poster_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"live_userId\" ON \"live\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_userId\" ON \"music\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"accessory_userId\" ON \"accessory\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"item_userId\" ON \"item\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"accessory_effect_master_userId\" ON \"accessory_effect_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"company_master_userId\" ON \"company_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"effect_duration_group_master_userId\" ON \"effect_duration_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"effect_master_userId\" ON \"effect_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"item_master_userId\" ON \"item_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"random_effect_group_master_userId\" ON \"random_effect_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"reward_rule_master_userId\" ON \"reward_rule_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"sense_effect_master_userId\" ON \"sense_effect_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trophy_group_master_userId\" ON \"trophy_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trophy_master_userId\" ON \"trophy_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_lesson_userId\" ON \"character_lesson\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"daily_lesson_userId\" ON \"daily_lesson\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"inbox_userId\" ON \"inbox\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"bomb_userId\" ON \"bomb\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"costume_userId\" ON \"costume\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"name_color_userId\" ON \"name_color\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"nameplate_userId\" ON \"nameplate\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"note_userId\" ON \"note\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"stamp_userId\" ON \"stamp\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"mission_userId\" ON \"mission\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"audition_master_userId\" ON \"audition_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"bomb_master_userId\" ON \"bomb_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_star_rank_master_userId\" ON \"character_star_rank_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_star_rank_reward_group_master_userId\" ON \"character_star_rank_reward_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"costume_master_userId\" ON \"costume_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"home_character_voice_master_userId\" ON \"home_character_voice_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"name_color_master_userId\" ON \"name_color_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"nameplate_master_userId\" ON \"nameplate_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"note_master_userId\" ON \"note_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"spot_conversation_master_userId\" ON \"spot_conversation_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"stamp_master_userId\" ON \"stamp_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_lesson_slot_userId\" ON \"character_lesson_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trophy_userId\" ON \"trophy\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"market_userId\" ON \"market\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"market_thing_userId\" ON \"market_thing\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"viewed_shop_userId\" ON \"viewed_shop\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"game_hint_userId\" ON \"game_hint\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"user_bonus_userId\" ON \"user_bonus\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"audition_phase_master_userId\" ON \"audition_phase_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"audition_reward_package_master_userId\" ON \"audition_reward_package_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"campaign_master_userId\" ON \"campaign_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_awakening_item_master_userId\" ON \"character_awakening_item_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_bloom_bonus_group_master_userId\" ON \"character_bloom_bonus_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_bloom_item_master_userId\" ON \"character_bloom_item_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_experience_item_master_userId\" ON \"character_experience_item_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_mission_master_userId\" ON \"character_mission_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_mission_stage_master_userId\" ON \"character_mission_stage_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_piece_master_userId\" ON \"character_piece_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_sense_enhance_item_group_master_userId\" ON \"character_sense_enhance_item_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"costume_wearable_character_group_master_userId\" ON \"costume_wearable_character_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"exchange_shop_master_userId\" ON \"exchange_shop_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"live_setting_master_userId\" ON \"live_setting_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"mission_master_userId\" ON \"mission_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_vocal_version_master_userId\" ON \"music_vocal_version_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_release_item_group_master_userId\" ON \"poster_release_item_group_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_release_item_master_userId\" ON \"poster_release_item_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"poster_story_master_userId\" ON \"poster_story_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"star_rank_reward_master_userId\" ON \"star_rank_reward_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"audition_clear_userId\" ON \"audition_clear\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"sp_rate_userId\" ON \"sp_rate\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"notification_userId\" ON \"notification\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"episode_userId\" ON \"episode\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_mission_userId\" ON \"character_mission\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"mission_pass_userId\" ON \"mission_pass\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"mission_pass_detail_master_userId\" ON \"mission_pass_detail_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"mission_pass_master_userId\" ON \"mission_pass_master\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_basic_userId\" ON \"league_basic\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_userId\" ON \"story_event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"exchange_limit_userId\" ON \"exchange_limit\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_group_userId\" ON \"league_group\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_group_member_userId\" ON \"league_group_member\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_history_userId\" ON \"league_history\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"jewel_shop_userId\" ON \"jewel_shop\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"daily_limit_userId\" ON \"daily_limit\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_high_score_party_userId\" ON \"league_high_score_party\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_high_score_party_slot_userId\" ON \"league_high_score_party_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_circle_userId\" ON \"story_event_circle\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_circle_mission_userId\" ON \"story_event_circle_mission\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_circle_mission_reward_userId\" ON \"story_event_circle_mission_reward\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_high_score_buff_setting_userId\" ON \"story_event_high_score_buff_setting\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_high_score_party_userId\" ON \"story_event_high_score_party\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_high_score_party_slot_userId\" ON \"story_event_high_score_party_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"connect_with_account_userId\" ON \"connect_with_account\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"tournament_detail_userId\" ON \"tournament_detail\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"gradual_mission_group_userId\" ON \"gradual_mission_group\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"photo_userId\" ON \"photo\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"album_userId\" ON \"album\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"circle_support_userId\" ON \"circle_support\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"album_page_userId\" ON \"album_page\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"star_pass_status_userId\" ON \"star_pass_status\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"login_pass_status_userId\" ON \"login_pass_status\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"currency_userId\" ON \"currency\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"decoration_userId\" ON \"decoration\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"live_achievement_userId\" ON \"live_achievement\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_video_userId\" ON \"music_video\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"theater_story_userId\" ON \"theater_story\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"live_drop_celling_userId\" ON \"live_drop_celling\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"story_event_high_score_userId\" ON \"story_event_high_score\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"comic_userId\" ON \"comic\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"comeback_campaign_userId\" ON \"comeback_campaign\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"concert_stage_userId\" ON \"concert_stage\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"limit_userId\" ON \"limit\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"gacha_selected_thing_userId\" ON \"gacha_selected_thing\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"total_point_event_userId\" ON \"total_point_event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"event_box_gacha_userId\" ON \"event_box_gacha\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"event_box_gacha_box_thing_userId\" ON \"event_box_gacha_box_thing\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"special_event_userId\" ON \"special_event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"character_point_event_userId\" ON \"character_point_event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"another_notation_userId\" ON \"another_notation\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_bookmark_userId\" ON \"music_bookmark\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"live_drop_limit_userId\" ON \"live_drop_limit\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"restriction_userId\" ON \"restriction\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"permanent_market_thing_userId\" ON \"permanent_market_thing\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"time_limited_control_userId\" ON \"time_limited_control\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"flash_sale_stage_userId\" ON \"flash_sale_stage\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"album_theme_userId\" ON \"album_theme\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"circle_event_mission_userId\" ON \"circle_event_mission\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"pickup_character_mission_userId\" ON \"pickup_character_mission\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"league_season_result_userId\" ON \"league_season_result\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"event_userId\" ON \"event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"bonus_live_userId\" ON \"bonus_live\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"bonus_live_stage_userId\" ON \"bonus_live_stage\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"roulette_event_userId\" ON \"roulette_event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"roulette_userId\" ON \"roulette\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"home_b_g_m_userId\" ON \"home_b_g_m\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"link_character_userId\" ON \"link_character\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_course_userId\" ON \"music_course\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"tournament_qualifying_userId\" ON \"tournament_qualifying\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"lottery_userId\" ON \"lottery\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_party_userId\" ON \"triple_cast_party\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_party_slot_userId\" ON \"triple_cast_party_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_basic_userId\" ON \"triple_cast_basic\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_group_userId\" ON \"triple_cast_group\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_group_member_userId\" ON \"triple_cast_group_member\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_high_score_party_userId\" ON \"triple_cast_high_score_party\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_high_score_party_slot_userId\" ON \"triple_cast_high_score_party_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_season_result_userId\" ON \"triple_cast_season_result\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"album_preset_userId\" ON \"album_preset\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"gacha_userId\" ON \"gacha\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"triple_cast_history_userId\" ON \"triple_cast_history\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"dugong_run_userId\" ON \"dugong_run\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"music_course_ranking_userId\" ON \"music_course_ranking\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"friend_invitation_userId\" ON \"friend_invitation\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"friend_invitation_mission_userId\" ON \"friend_invitation_mission\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"name_base_color_userId\" ON \"name_base_color\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"icon_frame_userId\" ON \"icon_frame\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"gacha_re_roll_userId\" ON \"gacha_re_roll\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trial_party_event_userId\" ON \"trial_party_event\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trial_party_event_stage_userId\" ON \"trial_party_event_stage\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trial_party_event_stage_party_userId\" ON \"trial_party_event_stage_party\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"trial_party_event_stage_party_slot_userId\" ON \"trial_party_event_stage_party_slot\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"user_block_userId\" ON \"user_block\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"friend_userId\" ON \"friend\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"friend_request_fromUserId\" ON \"friend_request\" (\"fromUserId\")",
  "CREATE INDEX IF NOT EXISTS \"friend_request_toUserId\" ON \"friend_request\" (\"toUserId\")",
  "CREATE INDEX IF NOT EXISTS \"home_skin_userId\" ON \"home_skin\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"accessory_auto_sell_userId\" ON \"accessory_auto_sell\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"favorite_costume_userId\" ON \"favorite_costume\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"buff_item_status_userId\" ON \"buff_item_status\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"multi_room_basic_userId\" ON \"multi_room_basic\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"event_camp_userId\" ON \"event_camp\" (\"userId\")",
  "CREATE INDEX IF NOT EXISTS \"gacha_history_userId\" ON \"gacha_history\" (\"userId\")",
  "CREATE UNIQUE INDEX IF NOT EXISTS \"circle_support_userId_company\" ON \"circle_support\" (\"userId\", \"company\")",
  "CREATE UNIQUE INDEX IF NOT EXISTS \"friend_userId_friendUserId\" ON \"friend\" (\"userId\", \"friendUserId\")",
  "CREATE UNIQUE INDEX IF NOT EXISTS \"friend_request_fromUserId_toUserId\" ON \"friend_request\" (\"fromUserId\", \"toUserId\")",
  "CREATE UNIQUE INDEX IF NOT EXISTS \"connectwithpassword_linkageCode_unique\" ON \"connect_with_password\" (\"linkageCode\") WHERE \"linkageCode\" IS NOT NULL",
  "CREATE TABLE IF NOT EXISTS \"preservation_live_context\" (\"userId\" INTEGER PRIMARY KEY REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE, \"mode\" TEXT NOT NULL, \"masterId\" INTEGER NOT NULL, \"extra\" JSONTEXT NOT NULL DEFAULT '{}')",
  "CREATE TABLE IF NOT EXISTS \"preservation_course_run\" (\"userId\" INTEGER PRIMARY KEY REFERENCES \"accounts\"(\"userId\") ON DELETE CASCADE, \"data\" JSONTEXT NOT NULL)",
};
static const int DDL_COUNT = 393;

struct ColEntry { const char* col; ColType type; };
struct TableCols { const char* table; const ColEntry* cols; int count; };

static const ColEntry _c_sequences[] = {
  {"name",TEXT}, {"value",BIGINT}
};
static const ColEntry _c_databaseinfo[] = {
  {"version",INT}
};
static const ColEntry _c_accounts[] = {
  {"userId",BIGINT}, {"credential",TEXT}, {"apiToken",TEXT}, {"platform",TEXT}, {"banLevel",INT}, {"registeredAt",BIGINT}, {"lastLoginAt",BIGINT}
};
static const ColEntry _c_user[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"playerRank",BIGINT}, {"currentRankPoint",BIGINT}, {"currentStamina",BIGINT}, {"maxStaminaRestoredAt",BIGINT}, {"playerRankLimit",BIGINT}, {"staminaRecoverTimesWithJewel",BIGINT}, {"circleUsageRestrictionsEndTime",BIGINT}, {"circleId",TEXT}, {"gameStartAt",BIGINT}, {"hashUserId",TEXT}, {"banLevel",BIGINT}, {"tutorialStatus",BIGINT}, {"monthlyPayment",BIGINT}, {"splashLastDisplayedAt",BIGINT}, {"isCapedPlayerRank",BOOL}, {"requireCapedPlayerRankAnnounce",BOOL}
};
static const ColEntry _c_user_profile[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"introduction",TEXT}, {"mainUCharacterId",BIGINT}, {"mNameplateId",BIGINT}, {"mNameColorId",BIGINT}, {"mTrophyId1",BIGINT}, {"mTrophyId2",BIGINT}, {"mTrophyId3",BIGINT}, {"playerRate",REAL}, {"isPublicPlayerRate",BOOL}, {"leagueClass",BIGINT}, {"totalSpCount",BIGINT}, {"isPublicAlbumMainPage",BOOL}, {"mNameplateDetailId",BIGINT}, {"mainCharacterMasterId",BIGINT}, {"displayAwakeningStatus",BOOL}, {"isPublicActivityLog",BOOL}, {"nameBaseColorMasterId",BIGINT}, {"iconFrameMasterId",BIGINT}, {"homeSkinMasterId",BIGINT}
};
static const ColEntry _c_user_preference[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"multiPartyId",BIGINT}, {"birthDate",BIGINT}
};
static const ColEntry _c_home_display_preference[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"homeCharacterBaseMasterId",BIGINT}, {"memberCharacterBaseMasterId",BIGINT}, {"storyCharacterBaseMasterId",BIGINT}, {"shopCharacterBaseMasterId",BIGINT}, {"homeCostumeMasterId",BIGINT}, {"memberCostumeMasterId",BIGINT}, {"storyCostumeMasterId",BIGINT}, {"shopCostumeMasterId",BIGINT}, {"illustCharacterMasterId",BIGINT}, {"displayAwakeningStatus",BOOL}, {"homeCharacterDisplayType",BIGINT}, {"loginBonusCharacterBaseMasterId",BIGINT}, {"loginBonusCostumeMasterId",BIGINT}
};
static const ColEntry _c_character[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterMasterId",BIGINT}, {"level",BIGINT}, {"currentExperience",BIGINT}, {"talentStage",BIGINT}, {"awakeningPhase",BIGINT}, {"characterBaseId",BIGINT}, {"senseLevel",BIGINT}, {"readEpisodeOrder",BIGINT}, {"releasedEpisodeOrder",BIGINT}, {"displayAwakeningStatus",BOOL}, {"secondaryCharacterBaseId",BIGINT}, {"secondarySenseLevel",BIGINT}, {"selectionType",BIGINT}, {"isFavorite",BOOL}
};
static const ColEntry _c_character_base[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterId",BIGINT}, {"starRank",BIGINT}, {"totalStarPoint",BIGINT}, {"costumeMasterId",BIGINT}, {"keyMissionLevel",BIGINT}, {"portalCharacterId",BIGINT}, {"portalDisplayAwakeningStatus",BOOL}
};
static const ColEntry _c_party[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"order",BIGINT}, {"name",TEXT}, {"leaderPosition",BIGINT}
};
static const ColEntry _c_party_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"partyId",BIGINT}, {"position",BIGINT}, {"characterId",BIGINT}, {"posterId",BIGINT}, {"accessoryId",BIGINT}, {"bonusAbilityEnableFlags",BIGINT}
};
static const ColEntry _c_character_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterId",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"assetId",TEXT}, {"rarity",BIGINT}, {"attribute",BIGINT}, {"minLevelStatus",JSON}, {"starActMasterId",BIGINT}, {"awakenStarActMasterId",BIGINT}, {"senseMasterId",BIGINT}, {"forbidGenericItemBloom",BOOL}, {"bloomBonusGroupMasterId",BIGINT}, {"senseEnhanceItemGroupMasterId",BIGINT}, {"firstEpisodeReleaseItemGroupId",BIGINT}, {"secondEpisodeReleaseItemGroupId",BIGINT}, {"characterAwakeningItemGroupMasterId",BIGINT}, {"displayStartAt",BIGINT}, {"displayEndAt",BIGINT}, {"unlockText",TEXT}, {"categories",JSON}, {"leaderSenseMasterId",BIGINT}, {"maxTalentStage",BIGINT}, {"maxTalentStageReleaseDate",BIGINT}, {"secondaryCharacterBaseMasterId",BIGINT}, {"secondarySenseMasterId",BIGINT}, {"secondaryAttribute",BIGINT}
};
static const ColEntry _c_character_base_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"school",TEXT}, {"grade",BIGINT}, {"birthMonth",BIGINT}, {"birthDay",BIGINT}, {"height",BIGINT}, {"hobby",TEXT}, {"companyMasterId",BIGINT}, {"nameRomanization",TEXT}, {"senseName",TEXT}, {"senseEffect",TEXT}, {"characterVoice",TEXT}, {"profileImageAssetId",TEXT}, {"age",BIGINT}, {"familyNameRomanization",TEXT}, {"firstNameRomanization",TEXT}, {"pronounceFamilyName",TEXT}, {"pronounceFirstName",TEXT}, {"familyName",TEXT}, {"firstName",TEXT}, {"evoSenseName",TEXT}, {"evoSenseEffect",TEXT}, {"defaultCostumeMasterId",BIGINT}, {"characterBaseType",BIGINT}
};
static const ColEntry _c_character_level_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"level",BIGINT}, {"experienceToLevelUp",BIGINT}, {"characterStatusLevel",BIGINT}, {"startDate",BIGINT}
};
static const ColEntry _c_poster[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"posterMasterId",BIGINT}, {"level",BIGINT}, {"breakthroughPhase",BIGINT}, {"releasedEpisode",BIGINT}, {"itemConsumeBreakThroughCount",BIGINT}, {"isFavorite",BOOL}, {"alternativeImagePattern",BIGINT}
};
static const ColEntry _c_accessory_level_pattern_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"patterns",JSON}
};
static const ColEntry _c_accessory_level_pattern_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"accessoryLevelPatternGroupMasterId",BIGINT}, {"level",BIGINT}, {"requiredCoin",BIGINT}, {"items",JSON}
};
static const ColEntry _c_accessory_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"rarity",BIGINT}, {"accessoryLevelPatternGroupId",BIGINT}, {"fixedAccessoryEffects",JSON}, {"randomEffectGroups",JSON}, {"pronounceName",TEXT}, {"series",BIGINT}, {"maxLevel",BIGINT}
};
static const ColEntry _c_episode_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyMasterId",BIGINT}, {"title",TEXT}, {"order",BIGINT}, {"episodeRewardPackageMasterId",BIGINT}, {"conditions",JSON}, {"preEpisodeMasterId",BIGINT}, {"displayStartDate",BIGINT}, {"displayEndDate",BIGINT}
};
static const ColEntry _c_episode_reward_package_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"rewards",JSON}
};
static const ColEntry _c_live_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"difficulty",BIGINT}, {"musicMasterId",BIGINT}, {"level",BIGINT}, {"noteCount",BIGINT}, {"unlockCondition",BIGINT}, {"unlockValue",BIGINT}, {"startDate",BIGINT}, {"endDate",BIGINT}
};
static const ColEntry _c_music_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"rewardRuleMasterId",BIGINT}, {"pronounceName",TEXT}, {"lyricWriter",TEXT}, {"composer",TEXT}, {"arranger",TEXT}, {"unlockText",TEXT}, {"isLongVersion",BOOL}, {"releasedAt",BIGINT}, {"staminaConsumption",BIGINT}, {"musicTimeSecond",BIGINT}, {"invisible",BOOL}, {"sampleStartSeconds",REAL}, {"sampleEndSeconds",REAL}, {"delaySeconds",REAL}, {"vocalVersions",JSON}, {"unlockConditionType",BIGINT}, {"unlockConditionValue",BIGINT}, {"musicVideoType",BIGINT}, {"musicCoverType",BIGINT}, {"storyEventMasterId",BIGINT}, {"storyMasterId",BIGINT}, {"eventMasterId",BIGINT}
};
static const ColEntry _c_sense_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"type",BIGINT}, {"preEffects",JSON}, {"branches",JSON}, {"acquirableGauge",BIGINT}, {"acquirableScorePercent",BIGINT}, {"scoreUpPerLevel",BIGINT}, {"lightCount",BIGINT}, {"coolTime",BIGINT}, {"branchCondition1",BIGINT}, {"conditionValue1",BIGINT}, {"branchCondition2",BIGINT}, {"conditionValue2",BIGINT}, {"subTypes",JSON}
};
static const ColEntry _c_story_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"type",BIGINT}, {"companyMasterId",BIGINT}, {"eventMasterId",BIGINT}, {"chapterOrder",BIGINT}, {"displayStartAt",BIGINT}, {"displayEndAt",BIGINT}
};
static const ColEntry _c_poster_level_pattern_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"patterns",JSON}
};
static const ColEntry _c_poster_level_pattern_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"levelPatternGroupId",BIGINT}, {"level",BIGINT}, {"itemMasterId",BIGINT}, {"quantity",BIGINT}
};
static const ColEntry _c_poster_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"organizeRestrictGroupId",BIGINT}, {"rarity",BIGINT}, {"levelPatternGroupMasterId",BIGINT}, {"subTitlePositionX1",REAL}, {"subTitlePositionY1",REAL}, {"subTitlePositionX2",REAL}, {"subTitlePositionY2",REAL}, {"subTitlePositionX3",REAL}, {"subTitlePositionY3",REAL}, {"releaseItemGroupId",BIGINT}, {"pronounceName",TEXT}, {"costumes",JSON}, {"appearanceCharacterBaseMasterIds",JSON}, {"isRestrictItemBreakThrough",BOOL}, {"displayStartAt",BIGINT}, {"displayEndAt",BIGINT}, {"unlockText",TEXT}, {"orientation",BIGINT}, {"subTitleDisplayCondition",BIGINT}, {"subTitleDisplayConditionValue",BIGINT}, {"posterBreakthroughMaxPhase",BIGINT}, {"posterBreakthroughMaxPhaseReleaseDate",BIGINT}, {"secondarySubTitleDisplayCondition",BIGINT}, {"secondarySubTitleDisplayConditionValue",BIGINT}, {"alternateImagePositionX1",REAL}, {"alternateImagePositionY1",REAL}, {"alternateImageReleasePhase1",BIGINT}, {"alternateImagePositionX2",REAL}, {"alternateImagePositionY2",REAL}, {"alternateImageReleasePhase2",BIGINT}, {"alternateImagePositionX3",REAL}, {"alternateImagePositionY3",REAL}, {"alternateImageReleasePhase3",BIGINT}
};
static const ColEntry _c_live[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"liveMasterId",BIGINT}, {"timesCompleted",BIGINT}, {"achievementRate",REAL}, {"notationRate",REAL}, {"clearLamp",BIGINT}, {"status",BIGINT}, {"rateGrade",BIGINT}
};
static const ColEntry _c_music[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"musicMasterId",BIGINT}, {"stellaReleased",BOOL}, {"vocalVersion",BIGINT}, {"olivierReleaseStatus",BIGINT}, {"isPossession",BOOL}
};
static const ColEntry _c_accessory[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"accessoryMasterId",BIGINT}, {"level",BIGINT}, {"locked",BOOL}, {"accessoryEffects",JSON}, {"referenceCounting",BIGINT}, {"isFavorite",BOOL}
};
static const ColEntry _c_item[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"itemMasterId",BIGINT}, {"stock",BIGINT}
};
static const ColEntry _c_accessory_effect_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"effectMasterId",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"variety",BIGINT}
};
static const ColEntry _c_company_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"companies",BIGINT}, {"description",TEXT}, {"isOther",BOOL}
};
static const ColEntry _c_effect_duration_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"durations",JSON}
};
static const ColEntry _c_effect_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"type",BIGINT}, {"range",BIGINT}, {"calculationType",BIGINT}, {"details",JSON}, {"conditions",JSON}, {"durationSecond",BIGINT}, {"triggers",JSON}, {"fireTimingType",BIGINT}
};
static const ColEntry _c_item_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"displayOrder",BIGINT}, {"displayEndDate",BIGINT}, {"maxStock",BIGINT}, {"category",BIGINT}, {"consumable",BOOL}, {"jumpType",BIGINT}, {"jumpTargetId",BIGINT}, {"tabCategory",BIGINT}, {"rarity",BIGINT}
};
static const ColEntry _c_random_effect_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"accessoryEffects",JSON}
};
static const ColEntry _c_reward_rule_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"achivementRateRewards",JSON}
};
static const ColEntry _c_sense_effect_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"order",BIGINT}, {"effectMasterId",BIGINT}
};
static const ColEntry _c_trophy_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"category",BIGINT}
};
static const ColEntry _c_trophy_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"rarity",BIGINT}, {"order",BIGINT}, {"trophyGroupMasterId",BIGINT}, {"hidden",BOOL}, {"unlockText",TEXT}
};
static const ColEntry _c_character_lesson[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"characterBaseMasterId",BIGINT}, {"setCharacters",JSON}, {"bestScore",BIGINT}, {"leaderPosition",BIGINT}, {"rewardReceivedHighScore",BIGINT}
};
static const ColEntry _c_daily_lesson[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"timesLeft",BIGINT}
};
static const ColEntry _c_inbox[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"thingType",BIGINT}, {"thingId",BIGINT}, {"thingQuantity",BIGINT}, {"isTimeLimited",BOOL}, {"hasReceived",BOOL}, {"title",TEXT}, {"description",TEXT}, {"sentAt",BIGINT}, {"receivedAt",BIGINT}, {"receiveLimitAt",BIGINT}, {"checked",BOOL}
};
static const ColEntry _c_bomb[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"bombMasterIds",JSON}
};
static const ColEntry _c_costume[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"costumeMasterId",BIGINT}
};
static const ColEntry _c_name_color[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"nameColorMasterIds",JSON}
};
static const ColEntry _c_nameplate[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"namePlateMasterId",BIGINT}
};
static const ColEntry _c_note[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"noteMasterIds",JSON}
};
static const ColEntry _c_stamp[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"stampMasterIds",JSON}, {"favoriteStampMasterIds",JSON}
};
static const ColEntry _c_mission[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"isCleared",BOOL}, {"isRewardReceived",BOOL}, {"missionCurrentCount",BIGINT}, {"missionMasterId",BIGINT}, {"currentMissionStageMasterId",BIGINT}
};
static const ColEntry _c_audition_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"musicMasterId",BIGINT}, {"recommendedCompany",BIGINT}, {"canSkip",BOOL}, {"senseNotationMasterId",BIGINT}, {"maxPhase",TEXT}, {"displayStartAt",BIGINT}, {"displayEndAt",BIGINT}, {"vocalVersion",BIGINT}, {"auditionGroupNumber",BIGINT}, {"skipStartAt",BIGINT}
};
static const ColEntry _c_bomb_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"order",BIGINT}, {"hidden",BOOL}, {"isDefault",BOOL}
};
static const ColEntry _c_character_star_rank_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"rank",BIGINT}, {"nextRankPoint",BIGINT}, {"requiredLessonScore",BIGINT}, {"statusBonus",REAL}
};
static const ColEntry _c_character_star_rank_reward_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"rewards",JSON}
};
static const ColEntry _c_costume_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"order",BIGINT}, {"isDefault",BOOL}, {"costumeGroupMasterId",BIGINT}, {"description",TEXT}
};
static const ColEntry _c_home_character_voice_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterId",BIGINT}, {"text",TEXT}, {"weight",BIGINT}, {"characterVoicePeriodMasterId",BIGINT}, {"isPlayerBirthDateVoice",BOOL}, {"voiceFileName1",TEXT}, {"voiceFileName2",TEXT}, {"voiceFileName3",TEXT}, {"voiceFileName4",TEXT}, {"voiceInterval1",REAL}, {"voiceInterval2",REAL}, {"voiceInterval3",REAL}, {"mouthMotionId1",TEXT}, {"mouthMotionId2",TEXT}, {"mouthMotionId3",TEXT}, {"mouthMotionId4",TEXT}, {"bodyMotionId1",TEXT}, {"bodyMotionId2",TEXT}, {"bodyMotionId3",TEXT}, {"bodyMotionId4",TEXT}
};
static const ColEntry _c_name_color_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"order",BIGINT}, {"hidden",BOOL}, {"isDefault",BOOL}, {"unlockText",TEXT}
};
static const ColEntry _c_nameplate_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"order",BIGINT}, {"hidden",BOOL}, {"isDefault",BOOL}, {"details",JSON}, {"unlockText",TEXT}, {"changeType",BIGINT}, {"changeValue1",BIGINT}, {"changeValue2",BIGINT}
};
static const ColEntry _c_note_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"description",TEXT}, {"order",BIGINT}, {"hidden",BOOL}, {"isDefault",BOOL}
};
static const ColEntry _c_spot_conversation_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"spot",BIGINT}, {"characterId1",BIGINT}, {"characterId2",BIGINT}, {"characterId3",BIGINT}, {"characterId4",BIGINT}, {"characterId5",BIGINT}, {"episodeMasterId",BIGINT}, {"costumeId1",BIGINT}, {"costumeId2",BIGINT}, {"costumeId3",BIGINT}, {"costumeId4",BIGINT}, {"costumeId5",BIGINT}, {"title",TEXT}
};
static const ColEntry _c_stamp_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"order",BIGINT}, {"isDefault",BOOL}, {"characterBaseMasterId",BIGINT}, {"type",BIGINT}, {"assetId",TEXT}, {"voiceAssetId",TEXT}, {"name",TEXT}, {"characterBaseMasterIds",JSON}
};
static const ColEntry _c_character_lesson_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"position",BIGINT}, {"setCharacterId",BIGINT}
};
static const ColEntry _c_trophy[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"trophyMasterId",BIGINT}, {"trophyGroupMasterId",BIGINT}, {"currentOrder",BIGINT}
};
static const ColEntry _c_market[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"lastRefreshedAt",BIGINT}, {"refreshTimes",BIGINT}
};
static const ColEntry _c_market_thing[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"frameNumber",BIGINT}, {"marketFrameThingMasterId",BIGINT}, {"hasPurchased",BOOL}, {"discountPercent",BIGINT}
};
static const ColEntry _c_viewed_shop[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"exchangeShopMasterId",BIGINT}, {"lastViewedAt",BIGINT}, {"viewedShopCategory",BIGINT}
};
static const ColEntry _c_game_hint[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"pageCategory",BIGINT}, {"hasAlreadyRead",BOOL}
};
static const ColEntry _c_user_bonus[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"experienceBonus",REAL}, {"lessonStarRankBonus",REAL}
};
static const ColEntry _c_audition_phase_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"auditionasterId",BIGINT}, {"phase",BIGINT}, {"recommendedPlayerRank",BIGINT}, {"clearScore",BIGINT}, {"starActCount",BIGINT}, {"auditionRewardPackageMasterId",BIGINT}
};
static const ColEntry _c_audition_reward_package_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"rewards",JSON}
};
static const ColEntry _c_campaign_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"title",TEXT}, {"description",TEXT}, {"iconImagePath",TEXT}, {"order",BIGINT}, {"startDate",BIGINT}, {"endDate",BIGINT}, {"comebackCampaignMasterId",BIGINT}, {"campaignEffectType",BIGINT}, {"campaignEffectValue",BIGINT}
};
static const ColEntry _c_character_awakening_item_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"awakeningPhase",BIGINT}, {"itemMasterId",BIGINT}, {"requiredQuantity",BIGINT}
};
static const ColEntry _c_character_bloom_bonus_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"bloomBonuses",JSON}, {"bloomRewards",JSON}
};
static const ColEntry _c_character_bloom_item_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"rarity",BIGINT}, {"currentStage",BIGINT}, {"requiredPieceAmount",BIGINT}, {"talentBloomItemType",BIGINT}, {"genericBloomItemMasterId",BIGINT}, {"requiredItemMasterId",BIGINT}, {"requiredItemAmount",BIGINT}
};
static const ColEntry _c_character_experience_item_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"itemMasterId",BIGINT}, {"acquirableExperience",BIGINT}, {"acquirableExperienceBonus",REAL}
};
static const ColEntry _c_character_mission_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"title",TEXT}, {"jumpType",BIGINT}, {"jumpValue",BIGINT}, {"stages",JSON}
};
static const ColEntry _c_character_mission_stage_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterMissionCategoryLevelMasterId",BIGINT}, {"characterMissionMasterId",BIGINT}, {"exclusionNoSenseCharacter",BOOL}, {"order",BIGINT}, {"stageOrder",BIGINT}, {"goalCount",BIGINT}
};
static const ColEntry _c_character_piece_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"itemMasterId",BIGINT}, {"characterMasterId",BIGINT}, {"dugongRequiredAmount",BIGINT}, {"talentBloomItemType",BIGINT}
};
static const ColEntry _c_character_sense_enhance_item_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"items",JSON}
};
static const ColEntry _c_costume_wearable_character_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterIds",JSON}
};
static const ColEntry _c_exchange_shop_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"isDisplayRequiredHavingItem",BOOL}, {"category",BIGINT}, {"name",TEXT}, {"displayThingType",BIGINT}, {"displayItemMasterId",BIGINT}, {"bannerPath",TEXT}, {"startDate",BIGINT}, {"endDate",BIGINT}, {"lastRefreshedAt",BIGINT}, {"lineup",JSON}, {"order",BIGINT}
};
static const ColEntry _c_live_setting_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"liveType",BIGINT}, {"liveDropFrameGroupMasterId",BIGINT}
};
static const ColEntry _c_mission_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"missionCategory",BIGINT}, {"missionViewOrder",BIGINT}, {"title",TEXT}, {"description",TEXT}, {"eventMasterId",BIGINT}, {"jumpType",BIGINT}, {"jumpTargetId",BIGINT}, {"startDate",BIGINT}, {"endDate",BIGINT}, {"stages",JSON}, {"comebackCampaignMasterId",BIGINT}
};
static const ColEntry _c_music_vocal_version_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"musicMasterId",BIGINT}, {"vocalVersion",BIGINT}, {"singer",TEXT}, {"name",TEXT}, {"musicTimeSecond",BIGINT}, {"sampleStartSeconds",REAL}, {"sampleEndSeconds",REAL}, {"musicVideoType",BIGINT}, {"characters",JSON}
};
static const ColEntry _c_poster_release_item_group_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"items",JSON}, {"itemConsumeApplyFlag",BOOL}
};
static const ColEntry _c_poster_release_item_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"currentPhase",BIGINT}, {"itemMasterId",BIGINT}, {"requiredQuantity",BIGINT}
};
static const ColEntry _c_poster_story_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"posterMasterId",BIGINT}, {"episodeType",BIGINT}, {"characterBaseMasterId",BIGINT}, {"description",TEXT}, {"order",BIGINT}, {"characterIconId",BIGINT}, {"characterName",TEXT}
};
static const ColEntry _c_star_rank_reward_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"rank",BIGINT}, {"characterBaseMasterId",BIGINT}, {"characterStarRankRewardGroupMasterId",BIGINT}
};
static const ColEntry _c_audition_clear[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"auditionMasterId",BIGINT}, {"clearPhase",BIGINT}, {"auditionClearPartyId",BIGINT}, {"skipClearPhase",BIGINT}
};
static const ColEntry _c_sp_rate[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"liveMasterId",BIGINT}, {"point",BIGINT}
};
static const ColEntry _c_notification[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"importantReadAt",BIGINT}, {"updateReadAt",BIGINT}, {"bugReadAt",BIGINT}
};
static const ColEntry _c_episode[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"episodeMasterId",BIGINT}, {"hasReadAll",BOOL}
};
static const ColEntry _c_character_mission[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterId",BIGINT}, {"characterMissionMasterId",BIGINT}, {"currentStageMasterId",BIGINT}, {"currentCount",BIGINT}, {"clearedStageOrder",BIGINT}, {"rewardReceivedStageOrder",BIGINT}, {"completedLevel",BIGINT}
};
static const ColEntry _c_mission_pass[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"missionPassMasterId",BIGINT}, {"paid",BOOL}, {"freeRewardReceivedPhase",BIGINT}, {"spRewardReceivedPhase",BIGINT}, {"terminated",BOOL}, {"freeRewardLoopCount",BIGINT}, {"freeRewardLoopReceivedPhase",BIGINT}, {"paidRewardLoopCount",BIGINT}, {"paidRewardLoopReceivedPhase",BIGINT}
};
static const ColEntry _c_mission_pass_detail_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"phase",BIGINT}, {"missionPassMasterId",BIGINT}, {"clearPoint",BIGINT}, {"startDate",BIGINT}, {"endDate",BIGINT}, {"rewards",JSON}
};
static const ColEntry _c_mission_pass_master[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"itemMasterId",BIGINT}, {"startDate",BIGINT}, {"endDate",BIGINT}
};
static const ColEntry _c_league_basic[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"myProperty",BIGINT}, {"starEnrollCount",BIGINT}, {"daiStarEnrollCount",BIGINT}, {"currentClassType",BIGINT}, {"bestClassType",BIGINT}, {"lastJoinedLeagueSeasonMasterId",BIGINT}
};
static const ColEntry _c_story_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventMasterId",BIGINT}, {"totalAcquiredPoint",BIGINT}, {"acquiredPointUpdatedDate",BIGINT}, {"lastRank",BIGINT}, {"readTips",BOOL}, {"loginDays",BIGINT}
};
static const ColEntry _c_exchange_limit[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"exchangeShopThingId",BIGINT}, {"replaceType",BIGINT}, {"specifiedNumberOfDaysLimit",BIGINT}, {"exchangedCount",BIGINT}, {"until",BIGINT}
};
static const ColEntry _c_league_group[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"leagueMasterId",BIGINT}, {"classType",BIGINT}, {"classOrder",BIGINT}
};
static const ColEntry _c_league_group_member[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"leagueGroupId",BIGINT}, {"leagueMasterId",BIGINT}, {"bestScore",BIGINT}
};
static const ColEntry _c_league_history[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"leagueMasterId",BIGINT}, {"classType",BIGINT}, {"historyCount",BIGINT}, {"isSendedReward",BOOL}, {"isPlayed",BOOL}, {"classChangeType",BIGINT}, {"groupRank",BIGINT}, {"globalRank",BIGINT}, {"allClassGlobalRank",BIGINT}
};
static const ColEntry _c_jewel_shop[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"jewelShopItemMasterId",BIGINT}, {"purchaseCount",BIGINT}, {"totalPurchaseCount",BIGINT}, {"rePurchaseDate",BIGINT}
};
static const ColEntry _c_daily_limit[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"autoPlayTimes",BIGINT}, {"dailyLessonTimes",BIGINT}, {"lastRefreshedAt",BIGINT}, {"musicCourseFreeChallengeTimes",BIGINT}
};
static const ColEntry _c_league_high_score_party[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"leagueMasterId",BIGINT}, {"highScore",BIGINT}, {"classType",BIGINT}, {"difficulty",BIGINT}, {"musicMasterId",BIGINT}, {"leagueGroupId",BIGINT}, {"slots",JSON}, {"userName",TEXT}, {"actingAbility",BIGINT}, {"leaderPosition",BIGINT}
};
static const ColEntry _c_league_high_score_party_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"leagueHighScorePartyId",BIGINT}, {"position",BIGINT}, {"characterMasterId",BIGINT}, {"characterLevel",BIGINT}, {"posterMasterId",BIGINT}, {"posterLevel",BIGINT}, {"posterBreakthroughPhase",BIGINT}, {"accessoryMasterId",BIGINT}, {"accessoryLevel",BIGINT}, {"currentStatus",JSON}, {"characterTalentStage",BIGINT}, {"characterAwakeningPhase",BIGINT}, {"characterDisplayAwakeningStatus",BOOL}
};
static const ColEntry _c_story_event_circle[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventMasterId",BIGINT}, {"currentPoint",BIGINT}, {"highScore",BIGINT}, {"circleId",BIGINT}
};
static const ColEntry _c_story_event_circle_mission[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventCircleMissionMasterId",BIGINT}, {"currentCount",BIGINT}
};
static const ColEntry _c_story_event_circle_mission_reward[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventMasterId",BIGINT}, {"storyEventCircleMissionRewardMasterId",BIGINT}
};
static const ColEntry _c_story_event_high_score_buff_setting[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"storyEventHighScoreBuffSettingMasterId",BIGINT}, {"currentLevel",BIGINT}
};
static const ColEntry _c_story_event_high_score_party[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventMasterId",BIGINT}, {"highScore",BIGINT}, {"rateGrade",BIGINT}, {"difficulty",BIGINT}, {"highScoreType",BIGINT}, {"liveSettingMasterId",BIGINT}, {"slots",JSON}, {"leaderPosition",BIGINT}
};
static const ColEntry _c_story_event_high_score_party_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventHighScoreClearPartyId",BIGINT}, {"position",BIGINT}, {"characterMasterId",BIGINT}, {"characterLevel",BIGINT}, {"characterTalentStage",BIGINT}, {"characterAwakeningPhase",BIGINT}, {"posterMasterId",BIGINT}, {"posterLevel",BIGINT}, {"posterBreakthroughPhase",BIGINT}, {"accessoryMasterId",BIGINT}, {"accessoryLevel",BIGINT}, {"currentStatus",JSON}, {"characterDisplayAwakeningStatus",BOOL}
};
static const ColEntry _c_connect_with_account[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"provider",BIGINT}
};
static const ColEntry _c_connect_with_password[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"passwordHash",TEXT}, {"linkageCode",TEXT}, {"confirmationCode",TEXT}, {"confirmationExpiresAt",BIGINT}
};
static const ColEntry _c_tournament_detail[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"tournamentDetailMasterId",BIGINT}, {"bestUniqueScore",BIGINT}, {"perfectStar",BIGINT}, {"perfect",BIGINT}, {"great",BIGINT}, {"good",BIGINT}, {"bad",BIGINT}, {"miss",BIGINT}, {"recordedAt",BIGINT}
};
static const ColEntry _c_gradual_mission_group[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"gradualMissionGroupMasterId",BIGINT}, {"startAt",BIGINT}
};
static const ColEntry _c_photo[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"fileName",TEXT}, {"sasToken",TEXT}, {"photoEffectMasterId",BIGINT}, {"lock",BOOL}, {"useAlbumPage",BIGINT}, {"level",BIGINT}, {"rarity",BIGINT}, {"signMasterId",BIGINT}, {"generatedAt",BIGINT}, {"thumbnailSasToken",TEXT}, {"appearedCharacterBaseMasterIds",JSON}, {"taggedCharacterBaseMasterIds",JSON}, {"useDecoPage",BIGINT}
};
static const ColEntry _c_album[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"level",BIGINT}, {"publishPageNumber",BIGINT}, {"currentPresetOrder",BIGINT}
};
static const ColEntry _c_circle_support[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"company",INT}, {"level",INT}, {"currentSupportPoint",INT}, {"lastLevelUppedAt",BIGINT}, {"levelLimit",INT}
};
static const ColEntry _c_album_page[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"page",BIGINT}, {"editType",BIGINT}, {"publishing",BOOL}, {"items",JSON}, {"albumThemeMasterId",BIGINT}
};
static const ColEntry _c_star_pass_status[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"type",BIGINT}, {"totalPurchasedCount",BIGINT}, {"validUntil",BIGINT}
};
static const ColEntry _c_login_pass_status[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"validUntil",BIGINT}
};
static const ColEntry _c_currency[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"coin",BIGINT}, {"freeJewel",BIGINT}, {"paidJewel",BIGINT}
};
static const ColEntry _c_decoration[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"decorationMasterId",BIGINT}
};
static const ColEntry _c_live_achievement[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"olivierReleasedCount",BIGINT}, {"olivierClearedLevel",BIGINT}
};
static const ColEntry _c_music_video[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"musicVideoMasterId",BIGINT}
};
static const ColEntry _c_theater_story[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"theaterStoryMasterId",BIGINT}
};
static const ColEntry _c_live_drop_celling[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"multiLiveScheduleMasterId",BIGINT}, {"count",BIGINT}, {"totalCellingCount",BIGINT}
};
static const ColEntry _c_story_event_high_score[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"storyEventMasterId",BIGINT}, {"currentEnhancementPoint",BIGINT}, {"totalAcquiredEnhancementPoint",BIGINT}
};
static const ColEntry _c_comic[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"comicEpisodeMasterId",BIGINT}
};
static const ColEntry _c_comeback_campaign[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"comebackCampaignMasterId",BIGINT}, {"activatedAt",BIGINT}
};
static const ColEntry _c_concert_stage[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"concertStageMasterId",BIGINT}
};
static const ColEntry _c_limit[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"additionalAcquirablePhotoLimit",BIGINT}, {"acquirablePhotoLimitIncreasedTimes",BIGINT}, {"additionalAcquirableAccessoryLimit",BIGINT}, {"acquirableAccessoryLimitIncreasedTimes",BIGINT}
};
static const ColEntry _c_gacha_selected_thing[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"gachaMasterId",BIGINT}, {"gachaThingIds",JSON}
};
static const ColEntry _c_total_point_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"totalPointEventMasterId",BIGINT}, {"totalAcquiredPoint",BIGINT}, {"receivedRewardOrder",BIGINT}
};
static const ColEntry _c_event_box_gacha[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"eventBoxGachaMasterId",BIGINT}, {"currentBoxCount",BIGINT}
};
static const ColEntry _c_event_box_gacha_box_thing[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"eventBoxGachaBoxThingMasterId",BIGINT}, {"hitCount",BIGINT}
};
static const ColEntry _c_special_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"specialEventMasterId",BIGINT}, {"readTips",BOOL}
};
static const ColEntry _c_character_point_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterPointEventMasterId",BIGINT}, {"characterBaseMasterId",BIGINT}, {"totalAcquiredPoint",BIGINT}, {"lastRank",BIGINT}, {"readTips",BOOL}
};
static const ColEntry _c_another_notation[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"anotherNotationMasterId",BIGINT}, {"clearLamp",BIGINT}, {"rateGrade",BIGINT}, {"achievementRatePercentRecord",JSON}
};
static const ColEntry _c_music_bookmark[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"musicMasterId",BIGINT}, {"musicBookmarkFlag",BIGINT}
};
static const ColEntry _c_live_drop_limit[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"multiLiveScheduleMasterId",BIGINT}, {"currentCount",BIGINT}, {"countLimit",BIGINT}
};
static const ColEntry _c_restriction[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"multiLiveRestrictionFinishedAt",BIGINT}, {"readMultiLiveRestrictionDialog",BOOL}
};
static const ColEntry _c_permanent_market_thing[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"permanentMarketThingMasterId",BIGINT}, {"purchaseCount",BIGINT}
};
static const ColEntry _c_time_limited_control[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"timeLimitedControlMasterId",BIGINT}, {"expiredAt",BIGINT}
};
static const ColEntry _c_flash_sale_stage[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"flashSaleStageMasterId",BIGINT}, {"purchaseLimitedAt",BIGINT}, {"isDefault",BOOL}, {"isCompleted",BOOL}
};
static const ColEntry _c_album_theme[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"albumThemeMasterId",BIGINT}
};
static const ColEntry _c_circle_event_mission[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"circleEventMissionMasterId",BIGINT}, {"currentCount",BIGINT}, {"isActive",BOOL}
};
static const ColEntry _c_pickup_character_mission[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"pickupCharacterMissionMasterId",BIGINT}, {"receivedDetailMasterIds",JSON}
};
static const ColEntry _c_league_season_result[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"leagueSeasonMasterId",BIGINT}, {"daiStarMaxEnrollCount",BIGINT}
};
static const ColEntry _c_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"eventMasterId",BIGINT}, {"totalAcquiredPoint",BIGINT}, {"acquiredPointUpdatedDate",BIGINT}, {"lastRank",BIGINT}, {"readTips",BOOL}, {"loginDays",BIGINT}
};
static const ColEntry _c_bonus_live[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"bonusLiveMasterId",BIGINT}, {"clearedStageOrder",BIGINT}, {"readTips",BOOL}, {"dailyClearTimes",BIGINT}
};
static const ColEntry _c_bonus_live_stage[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"bonusLiveMasterStageId",BIGINT}, {"clearTimes",BIGINT}
};
static const ColEntry _c_roulette_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"rouletteEventMasterId",BIGINT}, {"totalAcquiredPoint",BIGINT}
};
static const ColEntry _c_roulette[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"rouletteMasterId",BIGINT}, {"rollCount",BIGINT}
};
static const ColEntry _c_home_b_g_m[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"homeBGMMasterId",BIGINT}, {"selectionType",BIGINT}, {"homeBGMDetailMasterId",BIGINT}
};
static const ColEntry _c_link_character[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterId",BIGINT}, {"companyMasterId",BIGINT}, {"linkedCharacterBaseMasterId",BIGINT}, {"rewardReceivedMaxRank",BIGINT}
};
static const ColEntry _c_music_course[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"musicCourseMasterId",BIGINT}, {"clearLamp",BIGINT}, {"certificationGrade",BIGINT}, {"totalAchievementRatePercentRecord",JSON}
};
static const ColEntry _c_tournament_qualifying[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"tournamentQualifyingMasterId",BIGINT}, {"musicCourseMasterId",BIGINT}, {"currentChallengeCount",BIGINT}, {"perfectStar",BIGINT}, {"perfect",BIGINT}, {"great",BIGINT}, {"good",BIGINT}, {"bad",BIGINT}, {"miss",BIGINT}, {"totalAchievementRatePercentRecord",JSON}, {"bestRecordChallengeCount",BIGINT}, {"bestRecordDate",BIGINT}
};
static const ColEntry _c_lottery[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"lotteryMasterId",BIGINT}, {"results",JSON}
};
static const ColEntry _c_triple_cast_party[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"order",BIGINT}, {"name",TEXT}, {"leaderPosition",BIGINT}
};
static const ColEntry _c_triple_cast_party_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"partyId",BIGINT}, {"position",BIGINT}, {"characterId",BIGINT}, {"posterId",BIGINT}, {"accessoryId",BIGINT}, {"bonusAbilityEnableFlags",BIGINT}
};
static const ColEntry _c_triple_cast_basic[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"starEnrollCount",BIGINT}, {"daiStarEnrollCount",BIGINT}, {"currentClassType",BIGINT}, {"bestClassType",BIGINT}, {"lastJoinedTripleCastSeasonMasterId",BIGINT}, {"partyOrder1",BIGINT}, {"partyOrder2",BIGINT}, {"partyOrder3",BIGINT}
};
static const ColEntry _c_triple_cast_group[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"tripleCastMasterId",BIGINT}, {"classType",BIGINT}, {"classOrder",BIGINT}
};
static const ColEntry _c_triple_cast_group_member[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"tripleCastGroupId",BIGINT}, {"tripleCastMasterId",BIGINT}, {"bestScore",BIGINT}
};
static const ColEntry _c_triple_cast_high_score_party[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"tripleCastMasterId",BIGINT}, {"order",BIGINT}, {"highScore",BIGINT}, {"slots",JSON}, {"actingAbility",BIGINT}, {"leaderPosition",BIGINT}, {"difficulty",BIGINT}
};
static const ColEntry _c_triple_cast_high_score_party_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"tripleCastHighScorePartyId",BIGINT}, {"position",BIGINT}, {"characterMasterId",BIGINT}, {"characterLevel",BIGINT}, {"posterMasterId",BIGINT}, {"posterLevel",BIGINT}, {"posterBreakthroughPhase",BIGINT}, {"accessoryMasterId",BIGINT}, {"accessoryLevel",BIGINT}, {"currentStatus",JSON}, {"characterTalentStage",BIGINT}, {"characterAwakeningPhase",BIGINT}, {"characterDisplayAwakeningStatus",BOOL}
};
static const ColEntry _c_triple_cast_season_result[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"tripleCastSeasonMasterId",BIGINT}, {"daiStarMaxEnrollCount",BIGINT}
};
static const ColEntry _c_album_preset[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"name",TEXT}, {"order",BIGINT}
};
static const ColEntry _c_gacha[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"gachaMasterId",BIGINT}, {"rollCount",BIGINT}
};
static const ColEntry _c_triple_cast_history[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"tripleCastMasterId",BIGINT}, {"classType",BIGINT}, {"historyCount",BIGINT}, {"isSendedReward",BOOL}, {"isPlayed",BOOL}, {"classChangeType",BIGINT}, {"groupRank",BIGINT}, {"globalRank",BIGINT}, {"allClassGlobalRank",BIGINT}
};
static const ColEntry _c_dugong_run[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"clearedCourseIds",JSON}, {"noMistakeCourseIds",JSON}, {"dugongRunCourseGroupId",BIGINT}
};
static const ColEntry _c_music_course_ranking[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"musicCourseMasterId",BIGINT}, {"currentChallengeCount",BIGINT}, {"perfectStar",BIGINT}, {"perfect",BIGINT}, {"great",BIGINT}, {"good",BIGINT}, {"bad",BIGINT}, {"miss",BIGINT}, {"totalAchievementRatePercentRecord",JSON}, {"bestRecordChallengeCount",BIGINT}, {"bestRecordDate",BIGINT}, {"hasReceivedReward",BOOL}
};
static const ColEntry _c_friend_invitation[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"invitationCode",TEXT}, {"hasInputOtherInvitationCode",BOOL}
};
static const ColEntry _c_friend_invitation_mission[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"friendInvitationMissionMasterId",BIGINT}, {"friendInvitationMissionStageMasterId",BIGINT}, {"currentCount",BIGINT}, {"isCleared",BOOL}, {"isRewardReceived",BOOL}
};
static const ColEntry _c_name_base_color[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"nameBaseColorMasterIds",JSON}
};
static const ColEntry _c_icon_frame[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"iconFrameMasterIds",JSON}
};
static const ColEntry _c_gacha_re_roll[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"gachaMasterId",BIGINT}, {"rollCount",BIGINT}, {"isDecided",BOOL}
};
static const ColEntry _c_trial_party_event[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"trialPartyEventMasterId",BIGINT}, {"currentStageOrder",BIGINT}, {"isCompleted",BOOL}
};
static const ColEntry _c_trial_party_event_stage[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"trialPartyEventStageMasterId",BIGINT}, {"isCleared",BOOL}
};
static const ColEntry _c_trial_party_event_stage_party[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"trialPartyEventStageMasterId",BIGINT}, {"leaderPosition",BIGINT}
};
static const ColEntry _c_trial_party_event_stage_party_slot[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"trialPartyEventStagePartyId",BIGINT}, {"position",BIGINT}, {"trialPartyCharacterMasterId",BIGINT}, {"trialPartyPosterMasterId",BIGINT}, {"trialPartyAccessoryMasterId",BIGINT}
};
static const ColEntry _c_user_block[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"blockUserId",TEXT}
};
static const ColEntry _c_friend[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"friendUserId",BIGINT}, {"isFavorite",BOOL}, {"createdAt",BIGINT}
};
static const ColEntry _c_friend_request[] = {
  {"rowId",INT}, {"fromUserId",BIGINT}, {"toUserId",BIGINT}, {"createdAt",BIGINT}
};
static const ColEntry _c_home_skin[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"homeSkinMasterIds",JSON}
};
static const ColEntry _c_accessory_auto_sell[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"autoSellRarity",BIGINT}
};
static const ColEntry _c_favorite_costume[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"characterBaseMasterId",BIGINT}, {"favoriteCostumeMasterIds",JSON}
};
static const ColEntry _c_buff_item_status[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"effectType",BIGINT}, {"buffItemMasterId",BIGINT}, {"validUntil",BIGINT}
};
static const ColEntry _c_multi_room_basic[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"ownerMultiRoomId",TEXT}
};
static const ColEntry _c_event_camp[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"id",BIGINT}, {"eventMasterId",BIGINT}, {"campType",BIGINT}, {"totalSupportPoint",BIGINT}
};
static const ColEntry _c_hash_user_id[] = {
  {"hashUserId",TEXT}, {"userId",BIGINT}
};
static const ColEntry _c_active_live[] = {
  {"userId",BIGINT}, {"id",BIGINT}, {"liveMasterId",BIGINT}, {"partyId",BIGINT}, {"liveSettingMasterId",BIGINT}, {"staminaSpent",BOOL}
};
static const ColEntry _c_gacha_history[] = {
  {"rowId",INT}, {"userId",BIGINT}, {"cardType",BIGINT}, {"masterId",BIGINT}, {"createdAt",BIGINT}
};
static const ColEntry _c_preservation_live_context[] = {
  {"userId",BIGINT}, {"mode",TEXT}, {"masterId",INT}, {"extra",JSON}
};
static const ColEntry _c_preservation_course_run[] = {
  {"userId",BIGINT}, {"data",JSON}
};

// Per-table (col -> ColType) lookup.
static const TableCols TABLE_COLS[] = {
  {"sequences", _c_sequences, 2},
  {"databaseinfo", _c_databaseinfo, 1},
  {"accounts", _c_accounts, 7},
  {"user", _c_user, 19},
  {"user_profile", _c_user_profile, 23},
  {"user_preference", _c_user_preference, 5},
  {"home_display_preference", _c_home_display_preference, 16},
  {"character", _c_character, 17},
  {"character_base", _c_character_base, 10},
  {"party", _c_party, 6},
  {"party_slot", _c_party_slot, 9},
  {"character_master", _c_character_master, 29},
  {"character_base_master", _c_character_base_master, 28},
  {"character_level_master", _c_character_level_master, 6},
  {"poster", _c_poster, 10},
  {"accessory_level_pattern_group_master", _c_accessory_level_pattern_group_master, 4},
  {"accessory_level_pattern_master", _c_accessory_level_pattern_master, 7},
  {"accessory_master", _c_accessory_master, 12},
  {"episode_master", _c_episode_master, 11},
  {"episode_reward_package_master", _c_episode_reward_package_master, 4},
  {"live_master", _c_live_master, 11},
  {"music_master", _c_music_master, 27},
  {"sense_master", _c_sense_master, 18},
  {"story_master", _c_story_master, 9},
  {"poster_level_pattern_group_master", _c_poster_level_pattern_group_master, 4},
  {"poster_level_pattern_master", _c_poster_level_pattern_master, 7},
  {"poster_master", _c_poster_master, 37},
  {"live", _c_live, 10},
  {"music", _c_music, 8},
  {"accessory", _c_accessory, 9},
  {"item", _c_item, 5},
  {"accessory_effect_master", _c_accessory_effect_master, 7},
  {"company_master", _c_company_master, 7},
  {"effect_duration_group_master", _c_effect_duration_group_master, 4},
  {"effect_master", _c_effect_master, 11},
  {"item_master", _c_item_master, 14},
  {"random_effect_group_master", _c_random_effect_group_master, 4},
  {"reward_rule_master", _c_reward_rule_master, 4},
  {"sense_effect_master", _c_sense_effect_master, 4},
  {"trophy_group_master", _c_trophy_group_master, 4},
  {"trophy_master", _c_trophy_master, 10},
  {"character_lesson", _c_character_lesson, 7},
  {"daily_lesson", _c_daily_lesson, 3},
  {"inbox", _c_inbox, 14},
  {"bomb", _c_bomb, 4},
  {"costume", _c_costume, 4},
  {"name_color", _c_name_color, 4},
  {"nameplate", _c_nameplate, 4},
  {"note", _c_note, 4},
  {"stamp", _c_stamp, 5},
  {"mission", _c_mission, 8},
  {"audition_master", _c_audition_master, 13},
  {"bomb_master", _c_bomb_master, 8},
  {"character_star_rank_master", _c_character_star_rank_master, 6},
  {"character_star_rank_reward_group_master", _c_character_star_rank_reward_group_master, 4},
  {"costume_master", _c_costume_master, 8},
  {"home_character_voice_master", _c_home_character_voice_master, 23},
  {"name_color_master", _c_name_color_master, 9},
  {"nameplate_master", _c_nameplate_master, 13},
  {"note_master", _c_note_master, 8},
  {"spot_conversation_master", _c_spot_conversation_master, 16},
  {"stamp_master", _c_stamp_master, 11},
  {"character_lesson_slot", _c_character_lesson_slot, 4},
  {"trophy", _c_trophy, 6},
  {"market", _c_market, 5},
  {"market_thing", _c_market_thing, 6},
  {"viewed_shop", _c_viewed_shop, 6},
  {"game_hint", _c_game_hint, 5},
  {"user_bonus", _c_user_bonus, 5},
  {"audition_phase_master", _c_audition_phase_master, 9},
  {"audition_reward_package_master", _c_audition_reward_package_master, 4},
  {"campaign_master", _c_campaign_master, 12},
  {"character_awakening_item_master", _c_character_awakening_item_master, 6},
  {"character_bloom_bonus_group_master", _c_character_bloom_bonus_group_master, 5},
  {"character_bloom_item_master", _c_character_bloom_item_master, 9},
  {"character_experience_item_master", _c_character_experience_item_master, 6},
  {"character_mission_master", _c_character_mission_master, 7},
  {"character_mission_stage_master", _c_character_mission_stage_master, 9},
  {"character_piece_master", _c_character_piece_master, 6},
  {"character_sense_enhance_item_group_master", _c_character_sense_enhance_item_group_master, 4},
  {"costume_wearable_character_group_master", _c_costume_wearable_character_group_master, 4},
  {"exchange_shop_master", _c_exchange_shop_master, 14},
  {"live_setting_master", _c_live_setting_master, 6},
  {"mission_master", _c_mission_master, 14},
  {"music_vocal_version_master", _c_music_vocal_version_master, 12},
  {"poster_release_item_group_master", _c_poster_release_item_group_master, 5},
  {"poster_release_item_master", _c_poster_release_item_master, 5},
  {"poster_story_master", _c_poster_story_master, 10},
  {"star_rank_reward_master", _c_star_rank_reward_master, 5},
  {"audition_clear", _c_audition_clear, 7},
  {"sp_rate", _c_sp_rate, 5},
  {"notification", _c_notification, 6},
  {"episode", _c_episode, 4},
  {"character_mission", _c_character_mission, 10},
  {"mission_pass", _c_mission_pass, 12},
  {"mission_pass_detail_master", _c_mission_pass_detail_master, 9},
  {"mission_pass_master", _c_mission_pass_master, 6},
  {"league_basic", _c_league_basic, 8},
  {"story_event", _c_story_event, 9},
  {"exchange_limit", _c_exchange_limit, 8},
  {"league_group", _c_league_group, 5},
  {"league_group_member", _c_league_group_member, 5},
  {"league_history", _c_league_history, 11},
  {"jewel_shop", _c_jewel_shop, 7},
  {"daily_limit", _c_daily_limit, 7},
  {"league_high_score_party", _c_league_high_score_party, 13},
  {"league_high_score_party_slot", _c_league_high_score_party_slot, 16},
  {"story_event_circle", _c_story_event_circle, 7},
  {"story_event_circle_mission", _c_story_event_circle_mission, 5},
  {"story_event_circle_mission_reward", _c_story_event_circle_mission_reward, 5},
  {"story_event_high_score_buff_setting", _c_story_event_high_score_buff_setting, 4},
  {"story_event_high_score_party", _c_story_event_high_score_party, 11},
  {"story_event_high_score_party_slot", _c_story_event_high_score_party_slot, 16},
  {"connect_with_account", _c_connect_with_account, 4},
  {"connect_with_password", _c_connect_with_password, 7},
  {"tournament_detail", _c_tournament_detail, 12},
  {"gradual_mission_group", _c_gradual_mission_group, 5},
  {"photo", _c_photo, 16},
  {"album", _c_album, 6},
  {"circle_support", _c_circle_support, 7},
  {"album_page", _c_album_page, 8},
  {"star_pass_status", _c_star_pass_status, 6},
  {"login_pass_status", _c_login_pass_status, 4},
  {"currency", _c_currency, 6},
  {"decoration", _c_decoration, 4},
  {"live_achievement", _c_live_achievement, 5},
  {"music_video", _c_music_video, 4},
  {"theater_story", _c_theater_story, 4},
  {"live_drop_celling", _c_live_drop_celling, 6},
  {"story_event_high_score", _c_story_event_high_score, 6},
  {"comic", _c_comic, 3},
  {"comeback_campaign", _c_comeback_campaign, 4},
  {"concert_stage", _c_concert_stage, 3},
  {"limit", _c_limit, 7},
  {"gacha_selected_thing", _c_gacha_selected_thing, 4},
  {"total_point_event", _c_total_point_event, 6},
  {"event_box_gacha", _c_event_box_gacha, 5},
  {"event_box_gacha_box_thing", _c_event_box_gacha_box_thing, 5},
  {"special_event", _c_special_event, 5},
  {"character_point_event", _c_character_point_event, 8},
  {"another_notation", _c_another_notation, 7},
  {"music_bookmark", _c_music_bookmark, 4},
  {"live_drop_limit", _c_live_drop_limit, 5},
  {"restriction", _c_restriction, 5},
  {"permanent_market_thing", _c_permanent_market_thing, 4},
  {"time_limited_control", _c_time_limited_control, 5},
  {"flash_sale_stage", _c_flash_sale_stage, 7},
  {"album_theme", _c_album_theme, 4},
  {"circle_event_mission", _c_circle_event_mission, 5},
  {"pickup_character_mission", _c_pickup_character_mission, 4},
  {"league_season_result", _c_league_season_result, 4},
  {"event", _c_event, 9},
  {"bonus_live", _c_bonus_live, 7},
  {"bonus_live_stage", _c_bonus_live_stage, 5},
  {"roulette_event", _c_roulette_event, 5},
  {"roulette", _c_roulette, 5},
  {"home_b_g_m", _c_home_b_g_m, 5},
  {"link_character", _c_link_character, 7},
  {"music_course", _c_music_course, 7},
  {"tournament_qualifying", _c_tournament_qualifying, 15},
  {"lottery", _c_lottery, 4},
  {"triple_cast_party", _c_triple_cast_party, 6},
  {"triple_cast_party_slot", _c_triple_cast_party_slot, 9},
  {"triple_cast_basic", _c_triple_cast_basic, 11},
  {"triple_cast_group", _c_triple_cast_group, 5},
  {"triple_cast_group_member", _c_triple_cast_group_member, 5},
  {"triple_cast_high_score_party", _c_triple_cast_high_score_party, 10},
  {"triple_cast_high_score_party_slot", _c_triple_cast_high_score_party_slot, 16},
  {"triple_cast_season_result", _c_triple_cast_season_result, 4},
  {"album_preset", _c_album_preset, 5},
  {"gacha", _c_gacha, 5},
  {"triple_cast_history", _c_triple_cast_history, 11},
  {"dugong_run", _c_dugong_run, 6},
  {"music_course_ranking", _c_music_course_ranking, 15},
  {"friend_invitation", _c_friend_invitation, 5},
  {"friend_invitation_mission", _c_friend_invitation_mission, 8},
  {"name_base_color", _c_name_base_color, 4},
  {"icon_frame", _c_icon_frame, 4},
  {"gacha_re_roll", _c_gacha_re_roll, 5},
  {"trial_party_event", _c_trial_party_event, 6},
  {"trial_party_event_stage", _c_trial_party_event_stage, 5},
  {"trial_party_event_stage_party", _c_trial_party_event_stage_party, 5},
  {"trial_party_event_stage_party_slot", _c_trial_party_event_stage_party_slot, 8},
  {"user_block", _c_user_block, 4},
  {"friend", _c_friend, 5},
  {"friend_request", _c_friend_request, 4},
  {"home_skin", _c_home_skin, 4},
  {"accessory_auto_sell", _c_accessory_auto_sell, 4},
  {"favorite_costume", _c_favorite_costume, 5},
  {"buff_item_status", _c_buff_item_status, 6},
  {"multi_room_basic", _c_multi_room_basic, 4},
  {"event_camp", _c_event_camp, 6},
  {"hash_user_id", _c_hash_user_id, 2},
  {"active_live", _c_active_live, 6},
  {"gacha_history", _c_gacha_history, 5},
  {"preservation_live_context", _c_preservation_live_context, 4},
  {"preservation_course_run", _c_preservation_course_run, 2},
};
static const int TABLE_COLS_COUNT = 197;

// Flattened (table, col) -> ColType lookup.
struct FlatColEntry { const char* table; const char* col; ColType type; };
static const FlatColEntry COL_TYPES[] = {
  {"sequences","name",TEXT},
  {"sequences","value",BIGINT},
  {"databaseinfo","version",INT},
  {"accounts","userId",BIGINT},
  {"accounts","credential",TEXT},
  {"accounts","apiToken",TEXT},
  {"accounts","platform",TEXT},
  {"accounts","banLevel",INT},
  {"accounts","registeredAt",BIGINT},
  {"accounts","lastLoginAt",BIGINT},
  {"user","rowId",INT},
  {"user","userId",BIGINT},
  {"user","id",BIGINT},
  {"user","playerRank",BIGINT},
  {"user","currentRankPoint",BIGINT},
  {"user","currentStamina",BIGINT},
  {"user","maxStaminaRestoredAt",BIGINT},
  {"user","playerRankLimit",BIGINT},
  {"user","staminaRecoverTimesWithJewel",BIGINT},
  {"user","circleUsageRestrictionsEndTime",BIGINT},
  {"user","circleId",TEXT},
  {"user","gameStartAt",BIGINT},
  {"user","hashUserId",TEXT},
  {"user","banLevel",BIGINT},
  {"user","tutorialStatus",BIGINT},
  {"user","monthlyPayment",BIGINT},
  {"user","splashLastDisplayedAt",BIGINT},
  {"user","isCapedPlayerRank",BOOL},
  {"user","requireCapedPlayerRankAnnounce",BOOL},
  {"user_profile","rowId",INT},
  {"user_profile","userId",BIGINT},
  {"user_profile","id",BIGINT},
  {"user_profile","name",TEXT},
  {"user_profile","introduction",TEXT},
  {"user_profile","mainUCharacterId",BIGINT},
  {"user_profile","mNameplateId",BIGINT},
  {"user_profile","mNameColorId",BIGINT},
  {"user_profile","mTrophyId1",BIGINT},
  {"user_profile","mTrophyId2",BIGINT},
  {"user_profile","mTrophyId3",BIGINT},
  {"user_profile","playerRate",REAL},
  {"user_profile","isPublicPlayerRate",BOOL},
  {"user_profile","leagueClass",BIGINT},
  {"user_profile","totalSpCount",BIGINT},
  {"user_profile","isPublicAlbumMainPage",BOOL},
  {"user_profile","mNameplateDetailId",BIGINT},
  {"user_profile","mainCharacterMasterId",BIGINT},
  {"user_profile","displayAwakeningStatus",BOOL},
  {"user_profile","isPublicActivityLog",BOOL},
  {"user_profile","nameBaseColorMasterId",BIGINT},
  {"user_profile","iconFrameMasterId",BIGINT},
  {"user_profile","homeSkinMasterId",BIGINT},
  {"user_preference","rowId",INT},
  {"user_preference","userId",BIGINT},
  {"user_preference","id",BIGINT},
  {"user_preference","multiPartyId",BIGINT},
  {"user_preference","birthDate",BIGINT},
  {"home_display_preference","rowId",INT},
  {"home_display_preference","userId",BIGINT},
  {"home_display_preference","id",BIGINT},
  {"home_display_preference","homeCharacterBaseMasterId",BIGINT},
  {"home_display_preference","memberCharacterBaseMasterId",BIGINT},
  {"home_display_preference","storyCharacterBaseMasterId",BIGINT},
  {"home_display_preference","shopCharacterBaseMasterId",BIGINT},
  {"home_display_preference","homeCostumeMasterId",BIGINT},
  {"home_display_preference","memberCostumeMasterId",BIGINT},
  {"home_display_preference","storyCostumeMasterId",BIGINT},
  {"home_display_preference","shopCostumeMasterId",BIGINT},
  {"home_display_preference","illustCharacterMasterId",BIGINT},
  {"home_display_preference","displayAwakeningStatus",BOOL},
  {"home_display_preference","homeCharacterDisplayType",BIGINT},
  {"home_display_preference","loginBonusCharacterBaseMasterId",BIGINT},
  {"home_display_preference","loginBonusCostumeMasterId",BIGINT},
  {"character","rowId",INT},
  {"character","userId",BIGINT},
  {"character","id",BIGINT},
  {"character","characterMasterId",BIGINT},
  {"character","level",BIGINT},
  {"character","currentExperience",BIGINT},
  {"character","talentStage",BIGINT},
  {"character","awakeningPhase",BIGINT},
  {"character","characterBaseId",BIGINT},
  {"character","senseLevel",BIGINT},
  {"character","readEpisodeOrder",BIGINT},
  {"character","releasedEpisodeOrder",BIGINT},
  {"character","displayAwakeningStatus",BOOL},
  {"character","secondaryCharacterBaseId",BIGINT},
  {"character","secondarySenseLevel",BIGINT},
  {"character","selectionType",BIGINT},
  {"character","isFavorite",BOOL},
  {"character_base","rowId",INT},
  {"character_base","userId",BIGINT},
  {"character_base","id",BIGINT},
  {"character_base","characterBaseMasterId",BIGINT},
  {"character_base","starRank",BIGINT},
  {"character_base","totalStarPoint",BIGINT},
  {"character_base","costumeMasterId",BIGINT},
  {"character_base","keyMissionLevel",BIGINT},
  {"character_base","portalCharacterId",BIGINT},
  {"character_base","portalDisplayAwakeningStatus",BOOL},
  {"party","rowId",INT},
  {"party","userId",BIGINT},
  {"party","id",BIGINT},
  {"party","order",BIGINT},
  {"party","name",TEXT},
  {"party","leaderPosition",BIGINT},
  {"party_slot","rowId",INT},
  {"party_slot","userId",BIGINT},
  {"party_slot","id",BIGINT},
  {"party_slot","partyId",BIGINT},
  {"party_slot","position",BIGINT},
  {"party_slot","characterId",BIGINT},
  {"party_slot","posterId",BIGINT},
  {"party_slot","accessoryId",BIGINT},
  {"party_slot","bonusAbilityEnableFlags",BIGINT},
  {"character_master","rowId",INT},
  {"character_master","userId",BIGINT},
  {"character_master","id",BIGINT},
  {"character_master","characterBaseMasterId",BIGINT},
  {"character_master","name",TEXT},
  {"character_master","description",TEXT},
  {"character_master","assetId",TEXT},
  {"character_master","rarity",BIGINT},
  {"character_master","attribute",BIGINT},
  {"character_master","minLevelStatus",JSON},
  {"character_master","starActMasterId",BIGINT},
  {"character_master","awakenStarActMasterId",BIGINT},
  {"character_master","senseMasterId",BIGINT},
  {"character_master","forbidGenericItemBloom",BOOL},
  {"character_master","bloomBonusGroupMasterId",BIGINT},
  {"character_master","senseEnhanceItemGroupMasterId",BIGINT},
  {"character_master","firstEpisodeReleaseItemGroupId",BIGINT},
  {"character_master","secondEpisodeReleaseItemGroupId",BIGINT},
  {"character_master","characterAwakeningItemGroupMasterId",BIGINT},
  {"character_master","displayStartAt",BIGINT},
  {"character_master","displayEndAt",BIGINT},
  {"character_master","unlockText",TEXT},
  {"character_master","categories",JSON},
  {"character_master","leaderSenseMasterId",BIGINT},
  {"character_master","maxTalentStage",BIGINT},
  {"character_master","maxTalentStageReleaseDate",BIGINT},
  {"character_master","secondaryCharacterBaseMasterId",BIGINT},
  {"character_master","secondarySenseMasterId",BIGINT},
  {"character_master","secondaryAttribute",BIGINT},
  {"character_base_master","rowId",INT},
  {"character_base_master","userId",BIGINT},
  {"character_base_master","id",BIGINT},
  {"character_base_master","name",TEXT},
  {"character_base_master","description",TEXT},
  {"character_base_master","school",TEXT},
  {"character_base_master","grade",BIGINT},
  {"character_base_master","birthMonth",BIGINT},
  {"character_base_master","birthDay",BIGINT},
  {"character_base_master","height",BIGINT},
  {"character_base_master","hobby",TEXT},
  {"character_base_master","companyMasterId",BIGINT},
  {"character_base_master","nameRomanization",TEXT},
  {"character_base_master","senseName",TEXT},
  {"character_base_master","senseEffect",TEXT},
  {"character_base_master","characterVoice",TEXT},
  {"character_base_master","profileImageAssetId",TEXT},
  {"character_base_master","age",BIGINT},
  {"character_base_master","familyNameRomanization",TEXT},
  {"character_base_master","firstNameRomanization",TEXT},
  {"character_base_master","pronounceFamilyName",TEXT},
  {"character_base_master","pronounceFirstName",TEXT},
  {"character_base_master","familyName",TEXT},
  {"character_base_master","firstName",TEXT},
  {"character_base_master","evoSenseName",TEXT},
  {"character_base_master","evoSenseEffect",TEXT},
  {"character_base_master","defaultCostumeMasterId",BIGINT},
  {"character_base_master","characterBaseType",BIGINT},
  {"character_level_master","rowId",INT},
  {"character_level_master","userId",BIGINT},
  {"character_level_master","level",BIGINT},
  {"character_level_master","experienceToLevelUp",BIGINT},
  {"character_level_master","characterStatusLevel",BIGINT},
  {"character_level_master","startDate",BIGINT},
  {"poster","rowId",INT},
  {"poster","userId",BIGINT},
  {"poster","id",BIGINT},
  {"poster","posterMasterId",BIGINT},
  {"poster","level",BIGINT},
  {"poster","breakthroughPhase",BIGINT},
  {"poster","releasedEpisode",BIGINT},
  {"poster","itemConsumeBreakThroughCount",BIGINT},
  {"poster","isFavorite",BOOL},
  {"poster","alternativeImagePattern",BIGINT},
  {"accessory_level_pattern_group_master","rowId",INT},
  {"accessory_level_pattern_group_master","userId",BIGINT},
  {"accessory_level_pattern_group_master","id",BIGINT},
  {"accessory_level_pattern_group_master","patterns",JSON},
  {"accessory_level_pattern_master","rowId",INT},
  {"accessory_level_pattern_master","userId",BIGINT},
  {"accessory_level_pattern_master","id",BIGINT},
  {"accessory_level_pattern_master","accessoryLevelPatternGroupMasterId",BIGINT},
  {"accessory_level_pattern_master","level",BIGINT},
  {"accessory_level_pattern_master","requiredCoin",BIGINT},
  {"accessory_level_pattern_master","items",JSON},
  {"accessory_master","rowId",INT},
  {"accessory_master","userId",BIGINT},
  {"accessory_master","id",BIGINT},
  {"accessory_master","name",TEXT},
  {"accessory_master","description",TEXT},
  {"accessory_master","rarity",BIGINT},
  {"accessory_master","accessoryLevelPatternGroupId",BIGINT},
  {"accessory_master","fixedAccessoryEffects",JSON},
  {"accessory_master","randomEffectGroups",JSON},
  {"accessory_master","pronounceName",TEXT},
  {"accessory_master","series",BIGINT},
  {"accessory_master","maxLevel",BIGINT},
  {"episode_master","rowId",INT},
  {"episode_master","userId",BIGINT},
  {"episode_master","id",BIGINT},
  {"episode_master","storyMasterId",BIGINT},
  {"episode_master","title",TEXT},
  {"episode_master","order",BIGINT},
  {"episode_master","episodeRewardPackageMasterId",BIGINT},
  {"episode_master","conditions",JSON},
  {"episode_master","preEpisodeMasterId",BIGINT},
  {"episode_master","displayStartDate",BIGINT},
  {"episode_master","displayEndDate",BIGINT},
  {"episode_reward_package_master","rowId",INT},
  {"episode_reward_package_master","userId",BIGINT},
  {"episode_reward_package_master","id",BIGINT},
  {"episode_reward_package_master","rewards",JSON},
  {"live_master","rowId",INT},
  {"live_master","userId",BIGINT},
  {"live_master","id",BIGINT},
  {"live_master","difficulty",BIGINT},
  {"live_master","musicMasterId",BIGINT},
  {"live_master","level",BIGINT},
  {"live_master","noteCount",BIGINT},
  {"live_master","unlockCondition",BIGINT},
  {"live_master","unlockValue",BIGINT},
  {"live_master","startDate",BIGINT},
  {"live_master","endDate",BIGINT},
  {"music_master","rowId",INT},
  {"music_master","userId",BIGINT},
  {"music_master","id",BIGINT},
  {"music_master","name",TEXT},
  {"music_master","description",TEXT},
  {"music_master","rewardRuleMasterId",BIGINT},
  {"music_master","pronounceName",TEXT},
  {"music_master","lyricWriter",TEXT},
  {"music_master","composer",TEXT},
  {"music_master","arranger",TEXT},
  {"music_master","unlockText",TEXT},
  {"music_master","isLongVersion",BOOL},
  {"music_master","releasedAt",BIGINT},
  {"music_master","staminaConsumption",BIGINT},
  {"music_master","musicTimeSecond",BIGINT},
  {"music_master","invisible",BOOL},
  {"music_master","sampleStartSeconds",REAL},
  {"music_master","sampleEndSeconds",REAL},
  {"music_master","delaySeconds",REAL},
  {"music_master","vocalVersions",JSON},
  {"music_master","unlockConditionType",BIGINT},
  {"music_master","unlockConditionValue",BIGINT},
  {"music_master","musicVideoType",BIGINT},
  {"music_master","musicCoverType",BIGINT},
  {"music_master","storyEventMasterId",BIGINT},
  {"music_master","storyMasterId",BIGINT},
  {"music_master","eventMasterId",BIGINT},
  {"sense_master","rowId",INT},
  {"sense_master","userId",BIGINT},
  {"sense_master","id",BIGINT},
  {"sense_master","name",TEXT},
  {"sense_master","description",TEXT},
  {"sense_master","type",BIGINT},
  {"sense_master","preEffects",JSON},
  {"sense_master","branches",JSON},
  {"sense_master","acquirableGauge",BIGINT},
  {"sense_master","acquirableScorePercent",BIGINT},
  {"sense_master","scoreUpPerLevel",BIGINT},
  {"sense_master","lightCount",BIGINT},
  {"sense_master","coolTime",BIGINT},
  {"sense_master","branchCondition1",BIGINT},
  {"sense_master","conditionValue1",BIGINT},
  {"sense_master","branchCondition2",BIGINT},
  {"sense_master","conditionValue2",BIGINT},
  {"sense_master","subTypes",JSON},
  {"story_master","rowId",INT},
  {"story_master","userId",BIGINT},
  {"story_master","id",BIGINT},
  {"story_master","type",BIGINT},
  {"story_master","companyMasterId",BIGINT},
  {"story_master","eventMasterId",BIGINT},
  {"story_master","chapterOrder",BIGINT},
  {"story_master","displayStartAt",BIGINT},
  {"story_master","displayEndAt",BIGINT},
  {"poster_level_pattern_group_master","rowId",INT},
  {"poster_level_pattern_group_master","userId",BIGINT},
  {"poster_level_pattern_group_master","id",BIGINT},
  {"poster_level_pattern_group_master","patterns",JSON},
  {"poster_level_pattern_master","rowId",INT},
  {"poster_level_pattern_master","userId",BIGINT},
  {"poster_level_pattern_master","id",BIGINT},
  {"poster_level_pattern_master","levelPatternGroupId",BIGINT},
  {"poster_level_pattern_master","level",BIGINT},
  {"poster_level_pattern_master","itemMasterId",BIGINT},
  {"poster_level_pattern_master","quantity",BIGINT},
  {"poster_master","rowId",INT},
  {"poster_master","userId",BIGINT},
  {"poster_master","id",BIGINT},
  {"poster_master","name",TEXT},
  {"poster_master","organizeRestrictGroupId",BIGINT},
  {"poster_master","rarity",BIGINT},
  {"poster_master","levelPatternGroupMasterId",BIGINT},
  {"poster_master","subTitlePositionX1",REAL},
  {"poster_master","subTitlePositionY1",REAL},
  {"poster_master","subTitlePositionX2",REAL},
  {"poster_master","subTitlePositionY2",REAL},
  {"poster_master","subTitlePositionX3",REAL},
  {"poster_master","subTitlePositionY3",REAL},
  {"poster_master","releaseItemGroupId",BIGINT},
  {"poster_master","pronounceName",TEXT},
  {"poster_master","costumes",JSON},
  {"poster_master","appearanceCharacterBaseMasterIds",JSON},
  {"poster_master","isRestrictItemBreakThrough",BOOL},
  {"poster_master","displayStartAt",BIGINT},
  {"poster_master","displayEndAt",BIGINT},
  {"poster_master","unlockText",TEXT},
  {"poster_master","orientation",BIGINT},
  {"poster_master","subTitleDisplayCondition",BIGINT},
  {"poster_master","subTitleDisplayConditionValue",BIGINT},
  {"poster_master","posterBreakthroughMaxPhase",BIGINT},
  {"poster_master","posterBreakthroughMaxPhaseReleaseDate",BIGINT},
  {"poster_master","secondarySubTitleDisplayCondition",BIGINT},
  {"poster_master","secondarySubTitleDisplayConditionValue",BIGINT},
  {"poster_master","alternateImagePositionX1",REAL},
  {"poster_master","alternateImagePositionY1",REAL},
  {"poster_master","alternateImageReleasePhase1",BIGINT},
  {"poster_master","alternateImagePositionX2",REAL},
  {"poster_master","alternateImagePositionY2",REAL},
  {"poster_master","alternateImageReleasePhase2",BIGINT},
  {"poster_master","alternateImagePositionX3",REAL},
  {"poster_master","alternateImagePositionY3",REAL},
  {"poster_master","alternateImageReleasePhase3",BIGINT},
  {"live","rowId",INT},
  {"live","userId",BIGINT},
  {"live","id",BIGINT},
  {"live","liveMasterId",BIGINT},
  {"live","timesCompleted",BIGINT},
  {"live","achievementRate",REAL},
  {"live","notationRate",REAL},
  {"live","clearLamp",BIGINT},
  {"live","status",BIGINT},
  {"live","rateGrade",BIGINT},
  {"music","rowId",INT},
  {"music","userId",BIGINT},
  {"music","id",BIGINT},
  {"music","musicMasterId",BIGINT},
  {"music","stellaReleased",BOOL},
  {"music","vocalVersion",BIGINT},
  {"music","olivierReleaseStatus",BIGINT},
  {"music","isPossession",BOOL},
  {"accessory","rowId",INT},
  {"accessory","userId",BIGINT},
  {"accessory","id",BIGINT},
  {"accessory","accessoryMasterId",BIGINT},
  {"accessory","level",BIGINT},
  {"accessory","locked",BOOL},
  {"accessory","accessoryEffects",JSON},
  {"accessory","referenceCounting",BIGINT},
  {"accessory","isFavorite",BOOL},
  {"item","rowId",INT},
  {"item","userId",BIGINT},
  {"item","id",BIGINT},
  {"item","itemMasterId",BIGINT},
  {"item","stock",BIGINT},
  {"accessory_effect_master","rowId",INT},
  {"accessory_effect_master","userId",BIGINT},
  {"accessory_effect_master","id",BIGINT},
  {"accessory_effect_master","effectMasterId",BIGINT},
  {"accessory_effect_master","name",TEXT},
  {"accessory_effect_master","description",TEXT},
  {"accessory_effect_master","variety",BIGINT},
  {"company_master","rowId",INT},
  {"company_master","userId",BIGINT},
  {"company_master","id",BIGINT},
  {"company_master","name",TEXT},
  {"company_master","companies",BIGINT},
  {"company_master","description",TEXT},
  {"company_master","isOther",BOOL},
  {"effect_duration_group_master","rowId",INT},
  {"effect_duration_group_master","userId",BIGINT},
  {"effect_duration_group_master","id",BIGINT},
  {"effect_duration_group_master","durations",JSON},
  {"effect_master","rowId",INT},
  {"effect_master","userId",BIGINT},
  {"effect_master","id",BIGINT},
  {"effect_master","type",BIGINT},
  {"effect_master","range",BIGINT},
  {"effect_master","calculationType",BIGINT},
  {"effect_master","details",JSON},
  {"effect_master","conditions",JSON},
  {"effect_master","durationSecond",BIGINT},
  {"effect_master","triggers",JSON},
  {"effect_master","fireTimingType",BIGINT},
  {"item_master","rowId",INT},
  {"item_master","userId",BIGINT},
  {"item_master","id",BIGINT},
  {"item_master","name",TEXT},
  {"item_master","description",TEXT},
  {"item_master","displayOrder",BIGINT},
  {"item_master","displayEndDate",BIGINT},
  {"item_master","maxStock",BIGINT},
  {"item_master","category",BIGINT},
  {"item_master","consumable",BOOL},
  {"item_master","jumpType",BIGINT},
  {"item_master","jumpTargetId",BIGINT},
  {"item_master","tabCategory",BIGINT},
  {"item_master","rarity",BIGINT},
  {"random_effect_group_master","rowId",INT},
  {"random_effect_group_master","userId",BIGINT},
  {"random_effect_group_master","id",BIGINT},
  {"random_effect_group_master","accessoryEffects",JSON},
  {"reward_rule_master","rowId",INT},
  {"reward_rule_master","userId",BIGINT},
  {"reward_rule_master","id",BIGINT},
  {"reward_rule_master","achivementRateRewards",JSON},
  {"sense_effect_master","rowId",INT},
  {"sense_effect_master","userId",BIGINT},
  {"sense_effect_master","order",BIGINT},
  {"sense_effect_master","effectMasterId",BIGINT},
  {"trophy_group_master","rowId",INT},
  {"trophy_group_master","userId",BIGINT},
  {"trophy_group_master","id",BIGINT},
  {"trophy_group_master","category",BIGINT},
  {"trophy_master","rowId",INT},
  {"trophy_master","userId",BIGINT},
  {"trophy_master","id",BIGINT},
  {"trophy_master","name",TEXT},
  {"trophy_master","description",TEXT},
  {"trophy_master","rarity",BIGINT},
  {"trophy_master","order",BIGINT},
  {"trophy_master","trophyGroupMasterId",BIGINT},
  {"trophy_master","hidden",BOOL},
  {"trophy_master","unlockText",TEXT},
  {"character_lesson","rowId",INT},
  {"character_lesson","userId",BIGINT},
  {"character_lesson","characterBaseMasterId",BIGINT},
  {"character_lesson","setCharacters",JSON},
  {"character_lesson","bestScore",BIGINT},
  {"character_lesson","leaderPosition",BIGINT},
  {"character_lesson","rewardReceivedHighScore",BIGINT},
  {"daily_lesson","rowId",INT},
  {"daily_lesson","userId",BIGINT},
  {"daily_lesson","timesLeft",BIGINT},
  {"inbox","rowId",INT},
  {"inbox","userId",BIGINT},
  {"inbox","id",BIGINT},
  {"inbox","thingType",BIGINT},
  {"inbox","thingId",BIGINT},
  {"inbox","thingQuantity",BIGINT},
  {"inbox","isTimeLimited",BOOL},
  {"inbox","hasReceived",BOOL},
  {"inbox","title",TEXT},
  {"inbox","description",TEXT},
  {"inbox","sentAt",BIGINT},
  {"inbox","receivedAt",BIGINT},
  {"inbox","receiveLimitAt",BIGINT},
  {"inbox","checked",BOOL},
  {"bomb","rowId",INT},
  {"bomb","userId",BIGINT},
  {"bomb","id",BIGINT},
  {"bomb","bombMasterIds",JSON},
  {"costume","rowId",INT},
  {"costume","userId",BIGINT},
  {"costume","id",BIGINT},
  {"costume","costumeMasterId",BIGINT},
  {"name_color","rowId",INT},
  {"name_color","userId",BIGINT},
  {"name_color","id",BIGINT},
  {"name_color","nameColorMasterIds",JSON},
  {"nameplate","rowId",INT},
  {"nameplate","userId",BIGINT},
  {"nameplate","id",BIGINT},
  {"nameplate","namePlateMasterId",BIGINT},
  {"note","rowId",INT},
  {"note","userId",BIGINT},
  {"note","id",BIGINT},
  {"note","noteMasterIds",JSON},
  {"stamp","rowId",INT},
  {"stamp","userId",BIGINT},
  {"stamp","id",BIGINT},
  {"stamp","stampMasterIds",JSON},
  {"stamp","favoriteStampMasterIds",JSON},
  {"mission","rowId",INT},
  {"mission","userId",BIGINT},
  {"mission","id",BIGINT},
  {"mission","isCleared",BOOL},
  {"mission","isRewardReceived",BOOL},
  {"mission","missionCurrentCount",BIGINT},
  {"mission","missionMasterId",BIGINT},
  {"mission","currentMissionStageMasterId",BIGINT},
  {"audition_master","rowId",INT},
  {"audition_master","userId",BIGINT},
  {"audition_master","id",BIGINT},
  {"audition_master","musicMasterId",BIGINT},
  {"audition_master","recommendedCompany",BIGINT},
  {"audition_master","canSkip",BOOL},
  {"audition_master","senseNotationMasterId",BIGINT},
  {"audition_master","maxPhase",TEXT},
  {"audition_master","displayStartAt",BIGINT},
  {"audition_master","displayEndAt",BIGINT},
  {"audition_master","vocalVersion",BIGINT},
  {"audition_master","auditionGroupNumber",BIGINT},
  {"audition_master","skipStartAt",BIGINT},
  {"bomb_master","rowId",INT},
  {"bomb_master","userId",BIGINT},
  {"bomb_master","id",BIGINT},
  {"bomb_master","name",TEXT},
  {"bomb_master","description",TEXT},
  {"bomb_master","order",BIGINT},
  {"bomb_master","hidden",BOOL},
  {"bomb_master","isDefault",BOOL},
  {"character_star_rank_master","rowId",INT},
  {"character_star_rank_master","userId",BIGINT},
  {"character_star_rank_master","rank",BIGINT},
  {"character_star_rank_master","nextRankPoint",BIGINT},
  {"character_star_rank_master","requiredLessonScore",BIGINT},
  {"character_star_rank_master","statusBonus",REAL},
  {"character_star_rank_reward_group_master","rowId",INT},
  {"character_star_rank_reward_group_master","userId",BIGINT},
  {"character_star_rank_reward_group_master","id",BIGINT},
  {"character_star_rank_reward_group_master","rewards",JSON},
  {"costume_master","rowId",INT},
  {"costume_master","userId",BIGINT},
  {"costume_master","id",BIGINT},
  {"costume_master","name",TEXT},
  {"costume_master","order",BIGINT},
  {"costume_master","isDefault",BOOL},
  {"costume_master","costumeGroupMasterId",BIGINT},
  {"costume_master","description",TEXT},
  {"home_character_voice_master","rowId",INT},
  {"home_character_voice_master","userId",BIGINT},
  {"home_character_voice_master","id",BIGINT},
  {"home_character_voice_master","characterBaseMasterId",BIGINT},
  {"home_character_voice_master","text",TEXT},
  {"home_character_voice_master","weight",BIGINT},
  {"home_character_voice_master","characterVoicePeriodMasterId",BIGINT},
  {"home_character_voice_master","isPlayerBirthDateVoice",BOOL},
  {"home_character_voice_master","voiceFileName1",TEXT},
  {"home_character_voice_master","voiceFileName2",TEXT},
  {"home_character_voice_master","voiceFileName3",TEXT},
  {"home_character_voice_master","voiceFileName4",TEXT},
  {"home_character_voice_master","voiceInterval1",REAL},
  {"home_character_voice_master","voiceInterval2",REAL},
  {"home_character_voice_master","voiceInterval3",REAL},
  {"home_character_voice_master","mouthMotionId1",TEXT},
  {"home_character_voice_master","mouthMotionId2",TEXT},
  {"home_character_voice_master","mouthMotionId3",TEXT},
  {"home_character_voice_master","mouthMotionId4",TEXT},
  {"home_character_voice_master","bodyMotionId1",TEXT},
  {"home_character_voice_master","bodyMotionId2",TEXT},
  {"home_character_voice_master","bodyMotionId3",TEXT},
  {"home_character_voice_master","bodyMotionId4",TEXT},
  {"name_color_master","rowId",INT},
  {"name_color_master","userId",BIGINT},
  {"name_color_master","id",BIGINT},
  {"name_color_master","name",TEXT},
  {"name_color_master","description",TEXT},
  {"name_color_master","order",BIGINT},
  {"name_color_master","hidden",BOOL},
  {"name_color_master","isDefault",BOOL},
  {"name_color_master","unlockText",TEXT},
  {"nameplate_master","rowId",INT},
  {"nameplate_master","userId",BIGINT},
  {"nameplate_master","id",BIGINT},
  {"nameplate_master","name",TEXT},
  {"nameplate_master","description",TEXT},
  {"nameplate_master","order",BIGINT},
  {"nameplate_master","hidden",BOOL},
  {"nameplate_master","isDefault",BOOL},
  {"nameplate_master","details",JSON},
  {"nameplate_master","unlockText",TEXT},
  {"nameplate_master","changeType",BIGINT},
  {"nameplate_master","changeValue1",BIGINT},
  {"nameplate_master","changeValue2",BIGINT},
  {"note_master","rowId",INT},
  {"note_master","userId",BIGINT},
  {"note_master","id",BIGINT},
  {"note_master","name",TEXT},
  {"note_master","description",TEXT},
  {"note_master","order",BIGINT},
  {"note_master","hidden",BOOL},
  {"note_master","isDefault",BOOL},
  {"spot_conversation_master","rowId",INT},
  {"spot_conversation_master","userId",BIGINT},
  {"spot_conversation_master","id",BIGINT},
  {"spot_conversation_master","spot",BIGINT},
  {"spot_conversation_master","characterId1",BIGINT},
  {"spot_conversation_master","characterId2",BIGINT},
  {"spot_conversation_master","characterId3",BIGINT},
  {"spot_conversation_master","characterId4",BIGINT},
  {"spot_conversation_master","characterId5",BIGINT},
  {"spot_conversation_master","episodeMasterId",BIGINT},
  {"spot_conversation_master","costumeId1",BIGINT},
  {"spot_conversation_master","costumeId2",BIGINT},
  {"spot_conversation_master","costumeId3",BIGINT},
  {"spot_conversation_master","costumeId4",BIGINT},
  {"spot_conversation_master","costumeId5",BIGINT},
  {"spot_conversation_master","title",TEXT},
  {"stamp_master","rowId",INT},
  {"stamp_master","userId",BIGINT},
  {"stamp_master","id",BIGINT},
  {"stamp_master","order",BIGINT},
  {"stamp_master","isDefault",BOOL},
  {"stamp_master","characterBaseMasterId",BIGINT},
  {"stamp_master","type",BIGINT},
  {"stamp_master","assetId",TEXT},
  {"stamp_master","voiceAssetId",TEXT},
  {"stamp_master","name",TEXT},
  {"stamp_master","characterBaseMasterIds",JSON},
  {"character_lesson_slot","rowId",INT},
  {"character_lesson_slot","userId",BIGINT},
  {"character_lesson_slot","position",BIGINT},
  {"character_lesson_slot","setCharacterId",BIGINT},
  {"trophy","rowId",INT},
  {"trophy","userId",BIGINT},
  {"trophy","id",BIGINT},
  {"trophy","trophyMasterId",BIGINT},
  {"trophy","trophyGroupMasterId",BIGINT},
  {"trophy","currentOrder",BIGINT},
  {"market","rowId",INT},
  {"market","userId",BIGINT},
  {"market","id",BIGINT},
  {"market","lastRefreshedAt",BIGINT},
  {"market","refreshTimes",BIGINT},
  {"market_thing","rowId",INT},
  {"market_thing","userId",BIGINT},
  {"market_thing","frameNumber",BIGINT},
  {"market_thing","marketFrameThingMasterId",BIGINT},
  {"market_thing","hasPurchased",BOOL},
  {"market_thing","discountPercent",BIGINT},
  {"viewed_shop","rowId",INT},
  {"viewed_shop","userId",BIGINT},
  {"viewed_shop","id",BIGINT},
  {"viewed_shop","exchangeShopMasterId",BIGINT},
  {"viewed_shop","lastViewedAt",BIGINT},
  {"viewed_shop","viewedShopCategory",BIGINT},
  {"game_hint","rowId",INT},
  {"game_hint","userId",BIGINT},
  {"game_hint","id",BIGINT},
  {"game_hint","pageCategory",BIGINT},
  {"game_hint","hasAlreadyRead",BOOL},
  {"user_bonus","rowId",INT},
  {"user_bonus","userId",BIGINT},
  {"user_bonus","id",BIGINT},
  {"user_bonus","experienceBonus",REAL},
  {"user_bonus","lessonStarRankBonus",REAL},
  {"audition_phase_master","rowId",INT},
  {"audition_phase_master","userId",BIGINT},
  {"audition_phase_master","id",BIGINT},
  {"audition_phase_master","auditionasterId",BIGINT},
  {"audition_phase_master","phase",BIGINT},
  {"audition_phase_master","recommendedPlayerRank",BIGINT},
  {"audition_phase_master","clearScore",BIGINT},
  {"audition_phase_master","starActCount",BIGINT},
  {"audition_phase_master","auditionRewardPackageMasterId",BIGINT},
  {"audition_reward_package_master","rowId",INT},
  {"audition_reward_package_master","userId",BIGINT},
  {"audition_reward_package_master","id",BIGINT},
  {"audition_reward_package_master","rewards",JSON},
  {"campaign_master","rowId",INT},
  {"campaign_master","userId",BIGINT},
  {"campaign_master","id",BIGINT},
  {"campaign_master","title",TEXT},
  {"campaign_master","description",TEXT},
  {"campaign_master","iconImagePath",TEXT},
  {"campaign_master","order",BIGINT},
  {"campaign_master","startDate",BIGINT},
  {"campaign_master","endDate",BIGINT},
  {"campaign_master","comebackCampaignMasterId",BIGINT},
  {"campaign_master","campaignEffectType",BIGINT},
  {"campaign_master","campaignEffectValue",BIGINT},
  {"character_awakening_item_master","rowId",INT},
  {"character_awakening_item_master","userId",BIGINT},
  {"character_awakening_item_master","id",BIGINT},
  {"character_awakening_item_master","awakeningPhase",BIGINT},
  {"character_awakening_item_master","itemMasterId",BIGINT},
  {"character_awakening_item_master","requiredQuantity",BIGINT},
  {"character_bloom_bonus_group_master","rowId",INT},
  {"character_bloom_bonus_group_master","userId",BIGINT},
  {"character_bloom_bonus_group_master","id",BIGINT},
  {"character_bloom_bonus_group_master","bloomBonuses",JSON},
  {"character_bloom_bonus_group_master","bloomRewards",JSON},
  {"character_bloom_item_master","rowId",INT},
  {"character_bloom_item_master","userId",BIGINT},
  {"character_bloom_item_master","rarity",BIGINT},
  {"character_bloom_item_master","currentStage",BIGINT},
  {"character_bloom_item_master","requiredPieceAmount",BIGINT},
  {"character_bloom_item_master","talentBloomItemType",BIGINT},
  {"character_bloom_item_master","genericBloomItemMasterId",BIGINT},
  {"character_bloom_item_master","requiredItemMasterId",BIGINT},
  {"character_bloom_item_master","requiredItemAmount",BIGINT},
  {"character_experience_item_master","rowId",INT},
  {"character_experience_item_master","userId",BIGINT},
  {"character_experience_item_master","id",BIGINT},
  {"character_experience_item_master","itemMasterId",BIGINT},
  {"character_experience_item_master","acquirableExperience",BIGINT},
  {"character_experience_item_master","acquirableExperienceBonus",REAL},
  {"character_mission_master","rowId",INT},
  {"character_mission_master","userId",BIGINT},
  {"character_mission_master","id",BIGINT},
  {"character_mission_master","title",TEXT},
  {"character_mission_master","jumpType",BIGINT},
  {"character_mission_master","jumpValue",BIGINT},
  {"character_mission_master","stages",JSON},
  {"character_mission_stage_master","rowId",INT},
  {"character_mission_stage_master","userId",BIGINT},
  {"character_mission_stage_master","id",BIGINT},
  {"character_mission_stage_master","characterMissionCategoryLevelMasterId",BIGINT},
  {"character_mission_stage_master","characterMissionMasterId",BIGINT},
  {"character_mission_stage_master","exclusionNoSenseCharacter",BOOL},
  {"character_mission_stage_master","order",BIGINT},
  {"character_mission_stage_master","stageOrder",BIGINT},
  {"character_mission_stage_master","goalCount",BIGINT},
  {"character_piece_master","rowId",INT},
  {"character_piece_master","userId",BIGINT},
  {"character_piece_master","itemMasterId",BIGINT},
  {"character_piece_master","characterMasterId",BIGINT},
  {"character_piece_master","dugongRequiredAmount",BIGINT},
  {"character_piece_master","talentBloomItemType",BIGINT},
  {"character_sense_enhance_item_group_master","rowId",INT},
  {"character_sense_enhance_item_group_master","userId",BIGINT},
  {"character_sense_enhance_item_group_master","id",BIGINT},
  {"character_sense_enhance_item_group_master","items",JSON},
  {"costume_wearable_character_group_master","rowId",INT},
  {"costume_wearable_character_group_master","userId",BIGINT},
  {"costume_wearable_character_group_master","id",BIGINT},
  {"costume_wearable_character_group_master","characterBaseMasterIds",JSON},
  {"exchange_shop_master","rowId",INT},
  {"exchange_shop_master","userId",BIGINT},
  {"exchange_shop_master","id",BIGINT},
  {"exchange_shop_master","isDisplayRequiredHavingItem",BOOL},
  {"exchange_shop_master","category",BIGINT},
  {"exchange_shop_master","name",TEXT},
  {"exchange_shop_master","displayThingType",BIGINT},
  {"exchange_shop_master","displayItemMasterId",BIGINT},
  {"exchange_shop_master","bannerPath",TEXT},
  {"exchange_shop_master","startDate",BIGINT},
  {"exchange_shop_master","endDate",BIGINT},
  {"exchange_shop_master","lastRefreshedAt",BIGINT},
  {"exchange_shop_master","lineup",JSON},
  {"exchange_shop_master","order",BIGINT},
  {"live_setting_master","rowId",INT},
  {"live_setting_master","userId",BIGINT},
  {"live_setting_master","id",BIGINT},
  {"live_setting_master","name",TEXT},
  {"live_setting_master","liveType",BIGINT},
  {"live_setting_master","liveDropFrameGroupMasterId",BIGINT},
  {"mission_master","rowId",INT},
  {"mission_master","userId",BIGINT},
  {"mission_master","id",BIGINT},
  {"mission_master","missionCategory",BIGINT},
  {"mission_master","missionViewOrder",BIGINT},
  {"mission_master","title",TEXT},
  {"mission_master","description",TEXT},
  {"mission_master","eventMasterId",BIGINT},
  {"mission_master","jumpType",BIGINT},
  {"mission_master","jumpTargetId",BIGINT},
  {"mission_master","startDate",BIGINT},
  {"mission_master","endDate",BIGINT},
  {"mission_master","stages",JSON},
  {"mission_master","comebackCampaignMasterId",BIGINT},
  {"music_vocal_version_master","rowId",INT},
  {"music_vocal_version_master","userId",BIGINT},
  {"music_vocal_version_master","id",BIGINT},
  {"music_vocal_version_master","musicMasterId",BIGINT},
  {"music_vocal_version_master","vocalVersion",BIGINT},
  {"music_vocal_version_master","singer",TEXT},
  {"music_vocal_version_master","name",TEXT},
  {"music_vocal_version_master","musicTimeSecond",BIGINT},
  {"music_vocal_version_master","sampleStartSeconds",REAL},
  {"music_vocal_version_master","sampleEndSeconds",REAL},
  {"music_vocal_version_master","musicVideoType",BIGINT},
  {"music_vocal_version_master","characters",JSON},
  {"poster_release_item_group_master","rowId",INT},
  {"poster_release_item_group_master","userId",BIGINT},
  {"poster_release_item_group_master","id",BIGINT},
  {"poster_release_item_group_master","items",JSON},
  {"poster_release_item_group_master","itemConsumeApplyFlag",BOOL},
  {"poster_release_item_master","rowId",INT},
  {"poster_release_item_master","userId",BIGINT},
  {"poster_release_item_master","currentPhase",BIGINT},
  {"poster_release_item_master","itemMasterId",BIGINT},
  {"poster_release_item_master","requiredQuantity",BIGINT},
  {"poster_story_master","rowId",INT},
  {"poster_story_master","userId",BIGINT},
  {"poster_story_master","id",BIGINT},
  {"poster_story_master","posterMasterId",BIGINT},
  {"poster_story_master","episodeType",BIGINT},
  {"poster_story_master","characterBaseMasterId",BIGINT},
  {"poster_story_master","description",TEXT},
  {"poster_story_master","order",BIGINT},
  {"poster_story_master","characterIconId",BIGINT},
  {"poster_story_master","characterName",TEXT},
  {"star_rank_reward_master","rowId",INT},
  {"star_rank_reward_master","userId",BIGINT},
  {"star_rank_reward_master","rank",BIGINT},
  {"star_rank_reward_master","characterBaseMasterId",BIGINT},
  {"star_rank_reward_master","characterStarRankRewardGroupMasterId",BIGINT},
  {"audition_clear","rowId",INT},
  {"audition_clear","userId",BIGINT},
  {"audition_clear","id",BIGINT},
  {"audition_clear","auditionMasterId",BIGINT},
  {"audition_clear","clearPhase",BIGINT},
  {"audition_clear","auditionClearPartyId",BIGINT},
  {"audition_clear","skipClearPhase",BIGINT},
  {"sp_rate","rowId",INT},
  {"sp_rate","userId",BIGINT},
  {"sp_rate","id",BIGINT},
  {"sp_rate","liveMasterId",BIGINT},
  {"sp_rate","point",BIGINT},
  {"notification","rowId",INT},
  {"notification","userId",BIGINT},
  {"notification","id",BIGINT},
  {"notification","importantReadAt",BIGINT},
  {"notification","updateReadAt",BIGINT},
  {"notification","bugReadAt",BIGINT},
  {"episode","rowId",INT},
  {"episode","userId",BIGINT},
  {"episode","episodeMasterId",BIGINT},
  {"episode","hasReadAll",BOOL},
  {"character_mission","rowId",INT},
  {"character_mission","userId",BIGINT},
  {"character_mission","id",BIGINT},
  {"character_mission","characterBaseMasterId",BIGINT},
  {"character_mission","characterMissionMasterId",BIGINT},
  {"character_mission","currentStageMasterId",BIGINT},
  {"character_mission","currentCount",BIGINT},
  {"character_mission","clearedStageOrder",BIGINT},
  {"character_mission","rewardReceivedStageOrder",BIGINT},
  {"character_mission","completedLevel",BIGINT},
  {"mission_pass","rowId",INT},
  {"mission_pass","userId",BIGINT},
  {"mission_pass","id",BIGINT},
  {"mission_pass","missionPassMasterId",BIGINT},
  {"mission_pass","paid",BOOL},
  {"mission_pass","freeRewardReceivedPhase",BIGINT},
  {"mission_pass","spRewardReceivedPhase",BIGINT},
  {"mission_pass","terminated",BOOL},
  {"mission_pass","freeRewardLoopCount",BIGINT},
  {"mission_pass","freeRewardLoopReceivedPhase",BIGINT},
  {"mission_pass","paidRewardLoopCount",BIGINT},
  {"mission_pass","paidRewardLoopReceivedPhase",BIGINT},
  {"mission_pass_detail_master","rowId",INT},
  {"mission_pass_detail_master","userId",BIGINT},
  {"mission_pass_detail_master","id",BIGINT},
  {"mission_pass_detail_master","phase",BIGINT},
  {"mission_pass_detail_master","missionPassMasterId",BIGINT},
  {"mission_pass_detail_master","clearPoint",BIGINT},
  {"mission_pass_detail_master","startDate",BIGINT},
  {"mission_pass_detail_master","endDate",BIGINT},
  {"mission_pass_detail_master","rewards",JSON},
  {"mission_pass_master","rowId",INT},
  {"mission_pass_master","userId",BIGINT},
  {"mission_pass_master","id",BIGINT},
  {"mission_pass_master","itemMasterId",BIGINT},
  {"mission_pass_master","startDate",BIGINT},
  {"mission_pass_master","endDate",BIGINT},
  {"league_basic","rowId",INT},
  {"league_basic","userId",BIGINT},
  {"league_basic","myProperty",BIGINT},
  {"league_basic","starEnrollCount",BIGINT},
  {"league_basic","daiStarEnrollCount",BIGINT},
  {"league_basic","currentClassType",BIGINT},
  {"league_basic","bestClassType",BIGINT},
  {"league_basic","lastJoinedLeagueSeasonMasterId",BIGINT},
  {"story_event","rowId",INT},
  {"story_event","userId",BIGINT},
  {"story_event","id",BIGINT},
  {"story_event","storyEventMasterId",BIGINT},
  {"story_event","totalAcquiredPoint",BIGINT},
  {"story_event","acquiredPointUpdatedDate",BIGINT},
  {"story_event","lastRank",BIGINT},
  {"story_event","readTips",BOOL},
  {"story_event","loginDays",BIGINT},
  {"exchange_limit","rowId",INT},
  {"exchange_limit","userId",BIGINT},
  {"exchange_limit","id",BIGINT},
  {"exchange_limit","exchangeShopThingId",BIGINT},
  {"exchange_limit","replaceType",BIGINT},
  {"exchange_limit","specifiedNumberOfDaysLimit",BIGINT},
  {"exchange_limit","exchangedCount",BIGINT},
  {"exchange_limit","until",BIGINT},
  {"league_group","rowId",INT},
  {"league_group","userId",BIGINT},
  {"league_group","leagueMasterId",BIGINT},
  {"league_group","classType",BIGINT},
  {"league_group","classOrder",BIGINT},
  {"league_group_member","rowId",INT},
  {"league_group_member","userId",BIGINT},
  {"league_group_member","leagueGroupId",BIGINT},
  {"league_group_member","leagueMasterId",BIGINT},
  {"league_group_member","bestScore",BIGINT},
  {"league_history","rowId",INT},
  {"league_history","userId",BIGINT},
  {"league_history","leagueMasterId",BIGINT},
  {"league_history","classType",BIGINT},
  {"league_history","historyCount",BIGINT},
  {"league_history","isSendedReward",BOOL},
  {"league_history","isPlayed",BOOL},
  {"league_history","classChangeType",BIGINT},
  {"league_history","groupRank",BIGINT},
  {"league_history","globalRank",BIGINT},
  {"league_history","allClassGlobalRank",BIGINT},
  {"jewel_shop","rowId",INT},
  {"jewel_shop","userId",BIGINT},
  {"jewel_shop","id",BIGINT},
  {"jewel_shop","jewelShopItemMasterId",BIGINT},
  {"jewel_shop","purchaseCount",BIGINT},
  {"jewel_shop","totalPurchaseCount",BIGINT},
  {"jewel_shop","rePurchaseDate",BIGINT},
  {"daily_limit","rowId",INT},
  {"daily_limit","userId",BIGINT},
  {"daily_limit","id",BIGINT},
  {"daily_limit","autoPlayTimes",BIGINT},
  {"daily_limit","dailyLessonTimes",BIGINT},
  {"daily_limit","lastRefreshedAt",BIGINT},
  {"daily_limit","musicCourseFreeChallengeTimes",BIGINT},
  {"league_high_score_party","rowId",INT},
  {"league_high_score_party","userId",BIGINT},
  {"league_high_score_party","id",BIGINT},
  {"league_high_score_party","leagueMasterId",BIGINT},
  {"league_high_score_party","highScore",BIGINT},
  {"league_high_score_party","classType",BIGINT},
  {"league_high_score_party","difficulty",BIGINT},
  {"league_high_score_party","musicMasterId",BIGINT},
  {"league_high_score_party","leagueGroupId",BIGINT},
  {"league_high_score_party","slots",JSON},
  {"league_high_score_party","userName",TEXT},
  {"league_high_score_party","actingAbility",BIGINT},
  {"league_high_score_party","leaderPosition",BIGINT},
  {"league_high_score_party_slot","rowId",INT},
  {"league_high_score_party_slot","userId",BIGINT},
  {"league_high_score_party_slot","id",BIGINT},
  {"league_high_score_party_slot","leagueHighScorePartyId",BIGINT},
  {"league_high_score_party_slot","position",BIGINT},
  {"league_high_score_party_slot","characterMasterId",BIGINT},
  {"league_high_score_party_slot","characterLevel",BIGINT},
  {"league_high_score_party_slot","posterMasterId",BIGINT},
  {"league_high_score_party_slot","posterLevel",BIGINT},
  {"league_high_score_party_slot","posterBreakthroughPhase",BIGINT},
  {"league_high_score_party_slot","accessoryMasterId",BIGINT},
  {"league_high_score_party_slot","accessoryLevel",BIGINT},
  {"league_high_score_party_slot","currentStatus",JSON},
  {"league_high_score_party_slot","characterTalentStage",BIGINT},
  {"league_high_score_party_slot","characterAwakeningPhase",BIGINT},
  {"league_high_score_party_slot","characterDisplayAwakeningStatus",BOOL},
  {"story_event_circle","rowId",INT},
  {"story_event_circle","userId",BIGINT},
  {"story_event_circle","id",BIGINT},
  {"story_event_circle","storyEventMasterId",BIGINT},
  {"story_event_circle","currentPoint",BIGINT},
  {"story_event_circle","highScore",BIGINT},
  {"story_event_circle","circleId",BIGINT},
  {"story_event_circle_mission","rowId",INT},
  {"story_event_circle_mission","userId",BIGINT},
  {"story_event_circle_mission","id",BIGINT},
  {"story_event_circle_mission","storyEventCircleMissionMasterId",BIGINT},
  {"story_event_circle_mission","currentCount",BIGINT},
  {"story_event_circle_mission_reward","rowId",INT},
  {"story_event_circle_mission_reward","userId",BIGINT},
  {"story_event_circle_mission_reward","id",BIGINT},
  {"story_event_circle_mission_reward","storyEventMasterId",BIGINT},
  {"story_event_circle_mission_reward","storyEventCircleMissionRewardMasterId",BIGINT},
  {"story_event_high_score_buff_setting","rowId",INT},
  {"story_event_high_score_buff_setting","userId",BIGINT},
  {"story_event_high_score_buff_setting","storyEventHighScoreBuffSettingMasterId",BIGINT},
  {"story_event_high_score_buff_setting","currentLevel",BIGINT},
  {"story_event_high_score_party","rowId",INT},
  {"story_event_high_score_party","userId",BIGINT},
  {"story_event_high_score_party","id",BIGINT},
  {"story_event_high_score_party","storyEventMasterId",BIGINT},
  {"story_event_high_score_party","highScore",BIGINT},
  {"story_event_high_score_party","rateGrade",BIGINT},
  {"story_event_high_score_party","difficulty",BIGINT},
  {"story_event_high_score_party","highScoreType",BIGINT},
  {"story_event_high_score_party","liveSettingMasterId",BIGINT},
  {"story_event_high_score_party","slots",JSON},
  {"story_event_high_score_party","leaderPosition",BIGINT},
  {"story_event_high_score_party_slot","rowId",INT},
  {"story_event_high_score_party_slot","userId",BIGINT},
  {"story_event_high_score_party_slot","id",BIGINT},
  {"story_event_high_score_party_slot","storyEventHighScoreClearPartyId",BIGINT},
  {"story_event_high_score_party_slot","position",BIGINT},
  {"story_event_high_score_party_slot","characterMasterId",BIGINT},
  {"story_event_high_score_party_slot","characterLevel",BIGINT},
  {"story_event_high_score_party_slot","characterTalentStage",BIGINT},
  {"story_event_high_score_party_slot","characterAwakeningPhase",BIGINT},
  {"story_event_high_score_party_slot","posterMasterId",BIGINT},
  {"story_event_high_score_party_slot","posterLevel",BIGINT},
  {"story_event_high_score_party_slot","posterBreakthroughPhase",BIGINT},
  {"story_event_high_score_party_slot","accessoryMasterId",BIGINT},
  {"story_event_high_score_party_slot","accessoryLevel",BIGINT},
  {"story_event_high_score_party_slot","currentStatus",JSON},
  {"story_event_high_score_party_slot","characterDisplayAwakeningStatus",BOOL},
  {"connect_with_account","rowId",INT},
  {"connect_with_account","userId",BIGINT},
  {"connect_with_account","id",BIGINT},
  {"connect_with_account","provider",BIGINT},
  {"connect_with_password","rowId",INT},
  {"connect_with_password","userId",BIGINT},
  {"connect_with_password","id",BIGINT},
  {"connect_with_password","passwordHash",TEXT},
  {"connect_with_password","linkageCode",TEXT},
  {"connect_with_password","confirmationCode",TEXT},
  {"connect_with_password","confirmationExpiresAt",BIGINT},
  {"tournament_detail","rowId",INT},
  {"tournament_detail","userId",BIGINT},
  {"tournament_detail","id",BIGINT},
  {"tournament_detail","tournamentDetailMasterId",BIGINT},
  {"tournament_detail","bestUniqueScore",BIGINT},
  {"tournament_detail","perfectStar",BIGINT},
  {"tournament_detail","perfect",BIGINT},
  {"tournament_detail","great",BIGINT},
  {"tournament_detail","good",BIGINT},
  {"tournament_detail","bad",BIGINT},
  {"tournament_detail","miss",BIGINT},
  {"tournament_detail","recordedAt",BIGINT},
  {"gradual_mission_group","rowId",INT},
  {"gradual_mission_group","userId",BIGINT},
  {"gradual_mission_group","id",BIGINT},
  {"gradual_mission_group","gradualMissionGroupMasterId",BIGINT},
  {"gradual_mission_group","startAt",BIGINT},
  {"photo","rowId",INT},
  {"photo","userId",BIGINT},
  {"photo","id",BIGINT},
  {"photo","fileName",TEXT},
  {"photo","sasToken",TEXT},
  {"photo","photoEffectMasterId",BIGINT},
  {"photo","lock",BOOL},
  {"photo","useAlbumPage",BIGINT},
  {"photo","level",BIGINT},
  {"photo","rarity",BIGINT},
  {"photo","signMasterId",BIGINT},
  {"photo","generatedAt",BIGINT},
  {"photo","thumbnailSasToken",TEXT},
  {"photo","appearedCharacterBaseMasterIds",JSON},
  {"photo","taggedCharacterBaseMasterIds",JSON},
  {"photo","useDecoPage",BIGINT},
  {"album","rowId",INT},
  {"album","userId",BIGINT},
  {"album","id",BIGINT},
  {"album","level",BIGINT},
  {"album","publishPageNumber",BIGINT},
  {"album","currentPresetOrder",BIGINT},
  {"circle_support","rowId",INT},
  {"circle_support","userId",BIGINT},
  {"circle_support","company",INT},
  {"circle_support","level",INT},
  {"circle_support","currentSupportPoint",INT},
  {"circle_support","lastLevelUppedAt",BIGINT},
  {"circle_support","levelLimit",INT},
  {"album_page","rowId",INT},
  {"album_page","userId",BIGINT},
  {"album_page","id",BIGINT},
  {"album_page","page",BIGINT},
  {"album_page","editType",BIGINT},
  {"album_page","publishing",BOOL},
  {"album_page","items",JSON},
  {"album_page","albumThemeMasterId",BIGINT},
  {"star_pass_status","rowId",INT},
  {"star_pass_status","userId",BIGINT},
  {"star_pass_status","id",BIGINT},
  {"star_pass_status","type",BIGINT},
  {"star_pass_status","totalPurchasedCount",BIGINT},
  {"star_pass_status","validUntil",BIGINT},
  {"login_pass_status","rowId",INT},
  {"login_pass_status","userId",BIGINT},
  {"login_pass_status","id",BIGINT},
  {"login_pass_status","validUntil",BIGINT},
  {"currency","rowId",INT},
  {"currency","userId",BIGINT},
  {"currency","id",BIGINT},
  {"currency","coin",BIGINT},
  {"currency","freeJewel",BIGINT},
  {"currency","paidJewel",BIGINT},
  {"decoration","rowId",INT},
  {"decoration","userId",BIGINT},
  {"decoration","id",BIGINT},
  {"decoration","decorationMasterId",BIGINT},
  {"live_achievement","rowId",INT},
  {"live_achievement","userId",BIGINT},
  {"live_achievement","id",BIGINT},
  {"live_achievement","olivierReleasedCount",BIGINT},
  {"live_achievement","olivierClearedLevel",BIGINT},
  {"music_video","rowId",INT},
  {"music_video","userId",BIGINT},
  {"music_video","id",BIGINT},
  {"music_video","musicVideoMasterId",BIGINT},
  {"theater_story","rowId",INT},
  {"theater_story","userId",BIGINT},
  {"theater_story","id",BIGINT},
  {"theater_story","theaterStoryMasterId",BIGINT},
  {"live_drop_celling","rowId",INT},
  {"live_drop_celling","userId",BIGINT},
  {"live_drop_celling","id",BIGINT},
  {"live_drop_celling","multiLiveScheduleMasterId",BIGINT},
  {"live_drop_celling","count",BIGINT},
  {"live_drop_celling","totalCellingCount",BIGINT},
  {"story_event_high_score","rowId",INT},
  {"story_event_high_score","userId",BIGINT},
  {"story_event_high_score","id",BIGINT},
  {"story_event_high_score","storyEventMasterId",BIGINT},
  {"story_event_high_score","currentEnhancementPoint",BIGINT},
  {"story_event_high_score","totalAcquiredEnhancementPoint",BIGINT},
  {"comic","rowId",INT},
  {"comic","userId",BIGINT},
  {"comic","comicEpisodeMasterId",BIGINT},
  {"comeback_campaign","rowId",INT},
  {"comeback_campaign","userId",BIGINT},
  {"comeback_campaign","comebackCampaignMasterId",BIGINT},
  {"comeback_campaign","activatedAt",BIGINT},
  {"concert_stage","rowId",INT},
  {"concert_stage","userId",BIGINT},
  {"concert_stage","concertStageMasterId",BIGINT},
  {"limit","rowId",INT},
  {"limit","userId",BIGINT},
  {"limit","id",BIGINT},
  {"limit","additionalAcquirablePhotoLimit",BIGINT},
  {"limit","acquirablePhotoLimitIncreasedTimes",BIGINT},
  {"limit","additionalAcquirableAccessoryLimit",BIGINT},
  {"limit","acquirableAccessoryLimitIncreasedTimes",BIGINT},
  {"gacha_selected_thing","rowId",INT},
  {"gacha_selected_thing","userId",BIGINT},
  {"gacha_selected_thing","gachaMasterId",BIGINT},
  {"gacha_selected_thing","gachaThingIds",JSON},
  {"total_point_event","rowId",INT},
  {"total_point_event","userId",BIGINT},
  {"total_point_event","id",BIGINT},
  {"total_point_event","totalPointEventMasterId",BIGINT},
  {"total_point_event","totalAcquiredPoint",BIGINT},
  {"total_point_event","receivedRewardOrder",BIGINT},
  {"event_box_gacha","rowId",INT},
  {"event_box_gacha","userId",BIGINT},
  {"event_box_gacha","id",BIGINT},
  {"event_box_gacha","eventBoxGachaMasterId",BIGINT},
  {"event_box_gacha","currentBoxCount",BIGINT},
  {"event_box_gacha_box_thing","rowId",INT},
  {"event_box_gacha_box_thing","userId",BIGINT},
  {"event_box_gacha_box_thing","id",BIGINT},
  {"event_box_gacha_box_thing","eventBoxGachaBoxThingMasterId",BIGINT},
  {"event_box_gacha_box_thing","hitCount",BIGINT},
  {"special_event","rowId",INT},
  {"special_event","userId",BIGINT},
  {"special_event","id",BIGINT},
  {"special_event","specialEventMasterId",BIGINT},
  {"special_event","readTips",BOOL},
  {"character_point_event","rowId",INT},
  {"character_point_event","userId",BIGINT},
  {"character_point_event","id",BIGINT},
  {"character_point_event","characterPointEventMasterId",BIGINT},
  {"character_point_event","characterBaseMasterId",BIGINT},
  {"character_point_event","totalAcquiredPoint",BIGINT},
  {"character_point_event","lastRank",BIGINT},
  {"character_point_event","readTips",BOOL},
  {"another_notation","rowId",INT},
  {"another_notation","userId",BIGINT},
  {"another_notation","id",BIGINT},
  {"another_notation","anotherNotationMasterId",BIGINT},
  {"another_notation","clearLamp",BIGINT},
  {"another_notation","rateGrade",BIGINT},
  {"another_notation","achievementRatePercentRecord",JSON},
  {"music_bookmark","rowId",INT},
  {"music_bookmark","userId",BIGINT},
  {"music_bookmark","musicMasterId",BIGINT},
  {"music_bookmark","musicBookmarkFlag",BIGINT},
  {"live_drop_limit","rowId",INT},
  {"live_drop_limit","userId",BIGINT},
  {"live_drop_limit","multiLiveScheduleMasterId",BIGINT},
  {"live_drop_limit","currentCount",BIGINT},
  {"live_drop_limit","countLimit",BIGINT},
  {"restriction","rowId",INT},
  {"restriction","userId",BIGINT},
  {"restriction","id",BIGINT},
  {"restriction","multiLiveRestrictionFinishedAt",BIGINT},
  {"restriction","readMultiLiveRestrictionDialog",BOOL},
  {"permanent_market_thing","rowId",INT},
  {"permanent_market_thing","userId",BIGINT},
  {"permanent_market_thing","permanentMarketThingMasterId",BIGINT},
  {"permanent_market_thing","purchaseCount",BIGINT},
  {"time_limited_control","rowId",INT},
  {"time_limited_control","userId",BIGINT},
  {"time_limited_control","id",BIGINT},
  {"time_limited_control","timeLimitedControlMasterId",BIGINT},
  {"time_limited_control","expiredAt",BIGINT},
  {"flash_sale_stage","rowId",INT},
  {"flash_sale_stage","userId",BIGINT},
  {"flash_sale_stage","id",BIGINT},
  {"flash_sale_stage","flashSaleStageMasterId",BIGINT},
  {"flash_sale_stage","purchaseLimitedAt",BIGINT},
  {"flash_sale_stage","isDefault",BOOL},
  {"flash_sale_stage","isCompleted",BOOL},
  {"album_theme","rowId",INT},
  {"album_theme","userId",BIGINT},
  {"album_theme","id",BIGINT},
  {"album_theme","albumThemeMasterId",BIGINT},
  {"circle_event_mission","rowId",INT},
  {"circle_event_mission","userId",BIGINT},
  {"circle_event_mission","circleEventMissionMasterId",BIGINT},
  {"circle_event_mission","currentCount",BIGINT},
  {"circle_event_mission","isActive",BOOL},
  {"pickup_character_mission","rowId",INT},
  {"pickup_character_mission","userId",BIGINT},
  {"pickup_character_mission","pickupCharacterMissionMasterId",BIGINT},
  {"pickup_character_mission","receivedDetailMasterIds",JSON},
  {"league_season_result","rowId",INT},
  {"league_season_result","userId",BIGINT},
  {"league_season_result","leagueSeasonMasterId",BIGINT},
  {"league_season_result","daiStarMaxEnrollCount",BIGINT},
  {"event","rowId",INT},
  {"event","userId",BIGINT},
  {"event","id",BIGINT},
  {"event","eventMasterId",BIGINT},
  {"event","totalAcquiredPoint",BIGINT},
  {"event","acquiredPointUpdatedDate",BIGINT},
  {"event","lastRank",BIGINT},
  {"event","readTips",BOOL},
  {"event","loginDays",BIGINT},
  {"bonus_live","rowId",INT},
  {"bonus_live","userId",BIGINT},
  {"bonus_live","id",BIGINT},
  {"bonus_live","bonusLiveMasterId",BIGINT},
  {"bonus_live","clearedStageOrder",BIGINT},
  {"bonus_live","readTips",BOOL},
  {"bonus_live","dailyClearTimes",BIGINT},
  {"bonus_live_stage","rowId",INT},
  {"bonus_live_stage","userId",BIGINT},
  {"bonus_live_stage","id",BIGINT},
  {"bonus_live_stage","bonusLiveMasterStageId",BIGINT},
  {"bonus_live_stage","clearTimes",BIGINT},
  {"roulette_event","rowId",INT},
  {"roulette_event","userId",BIGINT},
  {"roulette_event","id",BIGINT},
  {"roulette_event","rouletteEventMasterId",BIGINT},
  {"roulette_event","totalAcquiredPoint",BIGINT},
  {"roulette","rowId",INT},
  {"roulette","userId",BIGINT},
  {"roulette","id",BIGINT},
  {"roulette","rouletteMasterId",BIGINT},
  {"roulette","rollCount",BIGINT},
  {"home_b_g_m","rowId",INT},
  {"home_b_g_m","userId",BIGINT},
  {"home_b_g_m","homeBGMMasterId",BIGINT},
  {"home_b_g_m","selectionType",BIGINT},
  {"home_b_g_m","homeBGMDetailMasterId",BIGINT},
  {"link_character","rowId",INT},
  {"link_character","userId",BIGINT},
  {"link_character","id",BIGINT},
  {"link_character","characterBaseMasterId",BIGINT},
  {"link_character","companyMasterId",BIGINT},
  {"link_character","linkedCharacterBaseMasterId",BIGINT},
  {"link_character","rewardReceivedMaxRank",BIGINT},
  {"music_course","rowId",INT},
  {"music_course","userId",BIGINT},
  {"music_course","id",BIGINT},
  {"music_course","musicCourseMasterId",BIGINT},
  {"music_course","clearLamp",BIGINT},
  {"music_course","certificationGrade",BIGINT},
  {"music_course","totalAchievementRatePercentRecord",JSON},
  {"tournament_qualifying","rowId",INT},
  {"tournament_qualifying","userId",BIGINT},
  {"tournament_qualifying","id",BIGINT},
  {"tournament_qualifying","tournamentQualifyingMasterId",BIGINT},
  {"tournament_qualifying","musicCourseMasterId",BIGINT},
  {"tournament_qualifying","currentChallengeCount",BIGINT},
  {"tournament_qualifying","perfectStar",BIGINT},
  {"tournament_qualifying","perfect",BIGINT},
  {"tournament_qualifying","great",BIGINT},
  {"tournament_qualifying","good",BIGINT},
  {"tournament_qualifying","bad",BIGINT},
  {"tournament_qualifying","miss",BIGINT},
  {"tournament_qualifying","totalAchievementRatePercentRecord",JSON},
  {"tournament_qualifying","bestRecordChallengeCount",BIGINT},
  {"tournament_qualifying","bestRecordDate",BIGINT},
  {"lottery","rowId",INT},
  {"lottery","userId",BIGINT},
  {"lottery","lotteryMasterId",BIGINT},
  {"lottery","results",JSON},
  {"triple_cast_party","rowId",INT},
  {"triple_cast_party","userId",BIGINT},
  {"triple_cast_party","id",BIGINT},
  {"triple_cast_party","order",BIGINT},
  {"triple_cast_party","name",TEXT},
  {"triple_cast_party","leaderPosition",BIGINT},
  {"triple_cast_party_slot","rowId",INT},
  {"triple_cast_party_slot","userId",BIGINT},
  {"triple_cast_party_slot","id",BIGINT},
  {"triple_cast_party_slot","partyId",BIGINT},
  {"triple_cast_party_slot","position",BIGINT},
  {"triple_cast_party_slot","characterId",BIGINT},
  {"triple_cast_party_slot","posterId",BIGINT},
  {"triple_cast_party_slot","accessoryId",BIGINT},
  {"triple_cast_party_slot","bonusAbilityEnableFlags",BIGINT},
  {"triple_cast_basic","rowId",INT},
  {"triple_cast_basic","userId",BIGINT},
  {"triple_cast_basic","id",BIGINT},
  {"triple_cast_basic","starEnrollCount",BIGINT},
  {"triple_cast_basic","daiStarEnrollCount",BIGINT},
  {"triple_cast_basic","currentClassType",BIGINT},
  {"triple_cast_basic","bestClassType",BIGINT},
  {"triple_cast_basic","lastJoinedTripleCastSeasonMasterId",BIGINT},
  {"triple_cast_basic","partyOrder1",BIGINT},
  {"triple_cast_basic","partyOrder2",BIGINT},
  {"triple_cast_basic","partyOrder3",BIGINT},
  {"triple_cast_group","rowId",INT},
  {"triple_cast_group","userId",BIGINT},
  {"triple_cast_group","tripleCastMasterId",BIGINT},
  {"triple_cast_group","classType",BIGINT},
  {"triple_cast_group","classOrder",BIGINT},
  {"triple_cast_group_member","rowId",INT},
  {"triple_cast_group_member","userId",BIGINT},
  {"triple_cast_group_member","tripleCastGroupId",BIGINT},
  {"triple_cast_group_member","tripleCastMasterId",BIGINT},
  {"triple_cast_group_member","bestScore",BIGINT},
  {"triple_cast_high_score_party","rowId",INT},
  {"triple_cast_high_score_party","userId",BIGINT},
  {"triple_cast_high_score_party","id",BIGINT},
  {"triple_cast_high_score_party","tripleCastMasterId",BIGINT},
  {"triple_cast_high_score_party","order",BIGINT},
  {"triple_cast_high_score_party","highScore",BIGINT},
  {"triple_cast_high_score_party","slots",JSON},
  {"triple_cast_high_score_party","actingAbility",BIGINT},
  {"triple_cast_high_score_party","leaderPosition",BIGINT},
  {"triple_cast_high_score_party","difficulty",BIGINT},
  {"triple_cast_high_score_party_slot","rowId",INT},
  {"triple_cast_high_score_party_slot","userId",BIGINT},
  {"triple_cast_high_score_party_slot","id",BIGINT},
  {"triple_cast_high_score_party_slot","tripleCastHighScorePartyId",BIGINT},
  {"triple_cast_high_score_party_slot","position",BIGINT},
  {"triple_cast_high_score_party_slot","characterMasterId",BIGINT},
  {"triple_cast_high_score_party_slot","characterLevel",BIGINT},
  {"triple_cast_high_score_party_slot","posterMasterId",BIGINT},
  {"triple_cast_high_score_party_slot","posterLevel",BIGINT},
  {"triple_cast_high_score_party_slot","posterBreakthroughPhase",BIGINT},
  {"triple_cast_high_score_party_slot","accessoryMasterId",BIGINT},
  {"triple_cast_high_score_party_slot","accessoryLevel",BIGINT},
  {"triple_cast_high_score_party_slot","currentStatus",JSON},
  {"triple_cast_high_score_party_slot","characterTalentStage",BIGINT},
  {"triple_cast_high_score_party_slot","characterAwakeningPhase",BIGINT},
  {"triple_cast_high_score_party_slot","characterDisplayAwakeningStatus",BOOL},
  {"triple_cast_season_result","rowId",INT},
  {"triple_cast_season_result","userId",BIGINT},
  {"triple_cast_season_result","tripleCastSeasonMasterId",BIGINT},
  {"triple_cast_season_result","daiStarMaxEnrollCount",BIGINT},
  {"album_preset","rowId",INT},
  {"album_preset","userId",BIGINT},
  {"album_preset","id",BIGINT},
  {"album_preset","name",TEXT},
  {"album_preset","order",BIGINT},
  {"gacha","rowId",INT},
  {"gacha","userId",BIGINT},
  {"gacha","id",BIGINT},
  {"gacha","gachaMasterId",BIGINT},
  {"gacha","rollCount",BIGINT},
  {"triple_cast_history","rowId",INT},
  {"triple_cast_history","userId",BIGINT},
  {"triple_cast_history","tripleCastMasterId",BIGINT},
  {"triple_cast_history","classType",BIGINT},
  {"triple_cast_history","historyCount",BIGINT},
  {"triple_cast_history","isSendedReward",BOOL},
  {"triple_cast_history","isPlayed",BOOL},
  {"triple_cast_history","classChangeType",BIGINT},
  {"triple_cast_history","groupRank",BIGINT},
  {"triple_cast_history","globalRank",BIGINT},
  {"triple_cast_history","allClassGlobalRank",BIGINT},
  {"dugong_run","rowId",INT},
  {"dugong_run","userId",BIGINT},
  {"dugong_run","id",BIGINT},
  {"dugong_run","clearedCourseIds",JSON},
  {"dugong_run","noMistakeCourseIds",JSON},
  {"dugong_run","dugongRunCourseGroupId",BIGINT},
  {"music_course_ranking","rowId",INT},
  {"music_course_ranking","userId",BIGINT},
  {"music_course_ranking","id",BIGINT},
  {"music_course_ranking","musicCourseMasterId",BIGINT},
  {"music_course_ranking","currentChallengeCount",BIGINT},
  {"music_course_ranking","perfectStar",BIGINT},
  {"music_course_ranking","perfect",BIGINT},
  {"music_course_ranking","great",BIGINT},
  {"music_course_ranking","good",BIGINT},
  {"music_course_ranking","bad",BIGINT},
  {"music_course_ranking","miss",BIGINT},
  {"music_course_ranking","totalAchievementRatePercentRecord",JSON},
  {"music_course_ranking","bestRecordChallengeCount",BIGINT},
  {"music_course_ranking","bestRecordDate",BIGINT},
  {"music_course_ranking","hasReceivedReward",BOOL},
  {"friend_invitation","rowId",INT},
  {"friend_invitation","userId",BIGINT},
  {"friend_invitation","id",BIGINT},
  {"friend_invitation","invitationCode",TEXT},
  {"friend_invitation","hasInputOtherInvitationCode",BOOL},
  {"friend_invitation_mission","rowId",INT},
  {"friend_invitation_mission","userId",BIGINT},
  {"friend_invitation_mission","id",BIGINT},
  {"friend_invitation_mission","friendInvitationMissionMasterId",BIGINT},
  {"friend_invitation_mission","friendInvitationMissionStageMasterId",BIGINT},
  {"friend_invitation_mission","currentCount",BIGINT},
  {"friend_invitation_mission","isCleared",BOOL},
  {"friend_invitation_mission","isRewardReceived",BOOL},
  {"name_base_color","rowId",INT},
  {"name_base_color","userId",BIGINT},
  {"name_base_color","id",BIGINT},
  {"name_base_color","nameBaseColorMasterIds",JSON},
  {"icon_frame","rowId",INT},
  {"icon_frame","userId",BIGINT},
  {"icon_frame","id",BIGINT},
  {"icon_frame","iconFrameMasterIds",JSON},
  {"gacha_re_roll","rowId",INT},
  {"gacha_re_roll","userId",BIGINT},
  {"gacha_re_roll","gachaMasterId",BIGINT},
  {"gacha_re_roll","rollCount",BIGINT},
  {"gacha_re_roll","isDecided",BOOL},
  {"trial_party_event","rowId",INT},
  {"trial_party_event","userId",BIGINT},
  {"trial_party_event","id",BIGINT},
  {"trial_party_event","trialPartyEventMasterId",BIGINT},
  {"trial_party_event","currentStageOrder",BIGINT},
  {"trial_party_event","isCompleted",BOOL},
  {"trial_party_event_stage","rowId",INT},
  {"trial_party_event_stage","userId",BIGINT},
  {"trial_party_event_stage","id",BIGINT},
  {"trial_party_event_stage","trialPartyEventStageMasterId",BIGINT},
  {"trial_party_event_stage","isCleared",BOOL},
  {"trial_party_event_stage_party","rowId",INT},
  {"trial_party_event_stage_party","userId",BIGINT},
  {"trial_party_event_stage_party","id",BIGINT},
  {"trial_party_event_stage_party","trialPartyEventStageMasterId",BIGINT},
  {"trial_party_event_stage_party","leaderPosition",BIGINT},
  {"trial_party_event_stage_party_slot","rowId",INT},
  {"trial_party_event_stage_party_slot","userId",BIGINT},
  {"trial_party_event_stage_party_slot","id",BIGINT},
  {"trial_party_event_stage_party_slot","trialPartyEventStagePartyId",BIGINT},
  {"trial_party_event_stage_party_slot","position",BIGINT},
  {"trial_party_event_stage_party_slot","trialPartyCharacterMasterId",BIGINT},
  {"trial_party_event_stage_party_slot","trialPartyPosterMasterId",BIGINT},
  {"trial_party_event_stage_party_slot","trialPartyAccessoryMasterId",BIGINT},
  {"user_block","rowId",INT},
  {"user_block","userId",BIGINT},
  {"user_block","id",BIGINT},
  {"user_block","blockUserId",TEXT},
  {"friend","rowId",INT},
  {"friend","userId",BIGINT},
  {"friend","friendUserId",BIGINT},
  {"friend","isFavorite",BOOL},
  {"friend","createdAt",BIGINT},
  {"friend_request","rowId",INT},
  {"friend_request","fromUserId",BIGINT},
  {"friend_request","toUserId",BIGINT},
  {"friend_request","createdAt",BIGINT},
  {"home_skin","rowId",INT},
  {"home_skin","userId",BIGINT},
  {"home_skin","id",BIGINT},
  {"home_skin","homeSkinMasterIds",JSON},
  {"accessory_auto_sell","rowId",INT},
  {"accessory_auto_sell","userId",BIGINT},
  {"accessory_auto_sell","id",BIGINT},
  {"accessory_auto_sell","autoSellRarity",BIGINT},
  {"favorite_costume","rowId",INT},
  {"favorite_costume","userId",BIGINT},
  {"favorite_costume","id",BIGINT},
  {"favorite_costume","characterBaseMasterId",BIGINT},
  {"favorite_costume","favoriteCostumeMasterIds",JSON},
  {"buff_item_status","rowId",INT},
  {"buff_item_status","userId",BIGINT},
  {"buff_item_status","id",BIGINT},
  {"buff_item_status","effectType",BIGINT},
  {"buff_item_status","buffItemMasterId",BIGINT},
  {"buff_item_status","validUntil",BIGINT},
  {"multi_room_basic","rowId",INT},
  {"multi_room_basic","userId",BIGINT},
  {"multi_room_basic","id",BIGINT},
  {"multi_room_basic","ownerMultiRoomId",TEXT},
  {"event_camp","rowId",INT},
  {"event_camp","userId",BIGINT},
  {"event_camp","id",BIGINT},
  {"event_camp","eventMasterId",BIGINT},
  {"event_camp","campType",BIGINT},
  {"event_camp","totalSupportPoint",BIGINT},
  {"hash_user_id","hashUserId",TEXT},
  {"hash_user_id","userId",BIGINT},
  {"active_live","userId",BIGINT},
  {"active_live","id",BIGINT},
  {"active_live","liveMasterId",BIGINT},
  {"active_live","partyId",BIGINT},
  {"active_live","liveSettingMasterId",BIGINT},
  {"active_live","staminaSpent",BOOL},
  {"gacha_history","rowId",INT},
  {"gacha_history","userId",BIGINT},
  {"gacha_history","cardType",BIGINT},
  {"gacha_history","masterId",BIGINT},
  {"gacha_history","createdAt",BIGINT},
  {"preservation_live_context","userId",BIGINT},
  {"preservation_live_context","mode",TEXT},
  {"preservation_live_context","masterId",INT},
  {"preservation_live_context","extra",JSON},
  {"preservation_course_run","userId",BIGINT},
  {"preservation_course_run","data",JSON},
};
static const int COL_TYPES_COUNT = 1503;

} // namespace schema
