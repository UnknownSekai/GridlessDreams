#include "db/user.h"

#include <cstddef>
#include <string>
#include <vector>

namespace db {
namespace user {

namespace {
// row.get(k): the column value, or null when absent (binds as NULL), as in Python
json row_get(const wire::json& row, const char* key) {
    auto it = row.find(key);
    return it != row.end() ? json(*it) : json(nullptr);
}
}  // namespace

ExecutableQuery update_user_tutorial_status(long long user_id, long long status) {
    return ExecutableQuery(
        "UPDATE \"user\" SET \"tutorialStatus\" = $2 WHERE \"userId\" = $1",
        user_id,
        status);
}

ExecutableQuery update_user_splash_last_displayed_at(long long user_id, long long ts) {
    return ExecutableQuery(
        "UPDATE \"user\" SET \"splashLastDisplayedAt\" = $2 WHERE \"userId\" = $1",
        user_id,
        ts);
}

ExecutableQuery upsert_user(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "playerRank", "currentRankPoint", "currentStamina", "maxStaminaRestoredAt", "playerRankLimit", "staminaRecoverTimesWithJewel", "circleUsageRestrictionsEndTime", "circleId", "gameStartAt", "hashUserId", "banLevel", "tutorialStatus", "monthlyPayment", "splashLastDisplayedAt", "isCapedPlayerRank", "requireCapedPlayerRankAnnounce"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"user\" (\"userId\", \"id\", \"playerRank\", \"currentRankPoint\", \"currentStamina\", \"maxStaminaRestoredAt\", \"playerRankLimit\", \"staminaRecoverTimesWithJewel\", \"circleUsageRestrictionsEndTime\", \"circleId\", \"gameStartAt\", \"hashUserId\", \"banLevel\", \"tutorialStatus\", \"monthlyPayment\", \"splashLastDisplayedAt\", \"isCapedPlayerRank\", \"requireCapedPlayerRankAnnounce\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_user_profile(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "introduction", "mainUCharacterId", "mNameplateId", "mNameColorId", "mTrophyId1", "mTrophyId2", "mTrophyId3", "playerRate", "isPublicPlayerRate", "leagueClass", "totalSpCount", "isPublicAlbumMainPage", "mNameplateDetailId", "mainCharacterMasterId", "displayAwakeningStatus", "isPublicActivityLog", "nameBaseColorMasterId", "iconFrameMasterId", "homeSkinMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"user_profile\" (\"userId\", \"id\", \"name\", \"introduction\", \"mainUCharacterId\", \"mNameplateId\", \"mNameColorId\", \"mTrophyId1\", \"mTrophyId2\", \"mTrophyId3\", \"playerRate\", \"isPublicPlayerRate\", \"leagueClass\", \"totalSpCount\", \"isPublicAlbumMainPage\", \"mNameplateDetailId\", \"mainCharacterMasterId\", \"displayAwakeningStatus\", \"isPublicActivityLog\", \"nameBaseColorMasterId\", \"iconFrameMasterId\", \"homeSkinMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_user_preference(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "multiPartyId", "birthDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"user_preference\" (\"userId\", \"id\", \"multiPartyId\", \"birthDate\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_home_display_preference(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "homeCharacterBaseMasterId", "memberCharacterBaseMasterId", "storyCharacterBaseMasterId", "shopCharacterBaseMasterId", "homeCostumeMasterId", "memberCostumeMasterId", "storyCostumeMasterId", "shopCostumeMasterId", "illustCharacterMasterId", "displayAwakeningStatus", "homeCharacterDisplayType", "loginBonusCharacterBaseMasterId", "loginBonusCostumeMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"home_display_preference\" (\"userId\", \"id\", \"homeCharacterBaseMasterId\", \"memberCharacterBaseMasterId\", \"storyCharacterBaseMasterId\", \"shopCharacterBaseMasterId\", \"homeCostumeMasterId\", \"memberCostumeMasterId\", \"storyCostumeMasterId\", \"shopCostumeMasterId\", \"illustCharacterMasterId\", \"displayAwakeningStatus\", \"homeCharacterDisplayType\", \"loginBonusCharacterBaseMasterId\", \"loginBonusCostumeMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterMasterId", "level", "currentExperience", "talentStage", "awakeningPhase", "characterBaseId", "senseLevel", "readEpisodeOrder", "releasedEpisodeOrder", "displayAwakeningStatus", "secondaryCharacterBaseId", "secondarySenseLevel", "selectionType", "isFavorite"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character\" (\"userId\", \"id\", \"characterMasterId\", \"level\", \"currentExperience\", \"talentStage\", \"awakeningPhase\", \"characterBaseId\", \"senseLevel\", \"readEpisodeOrder\", \"releasedEpisodeOrder\", \"displayAwakeningStatus\", \"secondaryCharacterBaseId\", \"secondarySenseLevel\", \"selectionType\", \"isFavorite\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_base(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterId", "starRank", "totalStarPoint", "costumeMasterId", "keyMissionLevel", "portalCharacterId", "portalDisplayAwakeningStatus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_base\" (\"userId\", \"id\", \"characterBaseMasterId\", \"starRank\", \"totalStarPoint\", \"costumeMasterId\", \"keyMissionLevel\", \"portalCharacterId\", \"portalDisplayAwakeningStatus\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_party(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "order", "name", "leaderPosition"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"party\" (\"userId\", \"id\", \"order\", \"name\", \"leaderPosition\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_party_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "partyId", "position", "characterId", "posterId", "accessoryId", "bonusAbilityEnableFlags"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"party_slot\" (\"userId\", \"id\", \"partyId\", \"position\", \"characterId\", \"posterId\", \"accessoryId\", \"bonusAbilityEnableFlags\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterId", "name", "description", "assetId", "rarity", "attribute", "minLevelStatus", "starActMasterId", "awakenStarActMasterId", "senseMasterId", "forbidGenericItemBloom", "bloomBonusGroupMasterId", "senseEnhanceItemGroupMasterId", "firstEpisodeReleaseItemGroupId", "secondEpisodeReleaseItemGroupId", "characterAwakeningItemGroupMasterId", "displayStartAt", "displayEndAt", "unlockText", "categories", "leaderSenseMasterId", "maxTalentStage", "maxTalentStageReleaseDate", "secondaryCharacterBaseMasterId", "secondarySenseMasterId", "secondaryAttribute"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_master\" (\"userId\", \"id\", \"characterBaseMasterId\", \"name\", \"description\", \"assetId\", \"rarity\", \"attribute\", \"minLevelStatus\", \"starActMasterId\", \"awakenStarActMasterId\", \"senseMasterId\", \"forbidGenericItemBloom\", \"bloomBonusGroupMasterId\", \"senseEnhanceItemGroupMasterId\", \"firstEpisodeReleaseItemGroupId\", \"secondEpisodeReleaseItemGroupId\", \"characterAwakeningItemGroupMasterId\", \"displayStartAt\", \"displayEndAt\", \"unlockText\", \"categories\", \"leaderSenseMasterId\", \"maxTalentStage\", \"maxTalentStageReleaseDate\", \"secondaryCharacterBaseMasterId\", \"secondarySenseMasterId\", \"secondaryAttribute\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_base_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "school", "grade", "birthMonth", "birthDay", "height", "hobby", "companyMasterId", "nameRomanization", "senseName", "senseEffect", "characterVoice", "profileImageAssetId", "age", "familyNameRomanization", "firstNameRomanization", "pronounceFamilyName", "pronounceFirstName", "familyName", "firstName", "evoSenseName", "evoSenseEffect", "defaultCostumeMasterId", "characterBaseType"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_base_master\" (\"userId\", \"id\", \"name\", \"description\", \"school\", \"grade\", \"birthMonth\", \"birthDay\", \"height\", \"hobby\", \"companyMasterId\", \"nameRomanization\", \"senseName\", \"senseEffect\", \"characterVoice\", \"profileImageAssetId\", \"age\", \"familyNameRomanization\", \"firstNameRomanization\", \"pronounceFamilyName\", \"pronounceFirstName\", \"familyName\", \"firstName\", \"evoSenseName\", \"evoSenseEffect\", \"defaultCostumeMasterId\", \"characterBaseType\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_level_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"level", "experienceToLevelUp", "characterStatusLevel", "startDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_level_master\" (\"userId\", \"level\", \"experienceToLevelUp\", \"characterStatusLevel\", \"startDate\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "posterMasterId", "level", "breakthroughPhase", "releasedEpisode", "itemConsumeBreakThroughCount", "isFavorite", "alternativeImagePattern"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster\" (\"userId\", \"id\", \"posterMasterId\", \"level\", \"breakthroughPhase\", \"releasedEpisode\", \"itemConsumeBreakThroughCount\", \"isFavorite\", \"alternativeImagePattern\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_accessory_level_pattern_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "patterns"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"accessory_level_pattern_group_master\" (\"userId\", \"id\", \"patterns\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_accessory_level_pattern_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "accessoryLevelPatternGroupMasterId", "level", "requiredCoin", "items"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"accessory_level_pattern_master\" (\"userId\", \"id\", \"accessoryLevelPatternGroupMasterId\", \"level\", \"requiredCoin\", \"items\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_accessory_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "rarity", "accessoryLevelPatternGroupId", "fixedAccessoryEffects", "randomEffectGroups", "pronounceName", "series", "maxLevel"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"accessory_master\" (\"userId\", \"id\", \"name\", \"description\", \"rarity\", \"accessoryLevelPatternGroupId\", \"fixedAccessoryEffects\", \"randomEffectGroups\", \"pronounceName\", \"series\", \"maxLevel\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_episode_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyMasterId", "title", "order", "episodeRewardPackageMasterId", "conditions", "preEpisodeMasterId", "displayStartDate", "displayEndDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"episode_master\" (\"userId\", \"id\", \"storyMasterId\", \"title\", \"order\", \"episodeRewardPackageMasterId\", \"conditions\", \"preEpisodeMasterId\", \"displayStartDate\", \"displayEndDate\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_episode_reward_package_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "rewards"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"episode_reward_package_master\" (\"userId\", \"id\", \"rewards\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_live_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "difficulty", "musicMasterId", "level", "noteCount", "unlockCondition", "unlockValue", "startDate", "endDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"live_master\" (\"userId\", \"id\", \"difficulty\", \"musicMasterId\", \"level\", \"noteCount\", \"unlockCondition\", \"unlockValue\", \"startDate\", \"endDate\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "rewardRuleMasterId", "pronounceName", "lyricWriter", "composer", "arranger", "unlockText", "isLongVersion", "releasedAt", "staminaConsumption", "musicTimeSecond", "invisible", "sampleStartSeconds", "sampleEndSeconds", "delaySeconds", "vocalVersions", "unlockConditionType", "unlockConditionValue", "musicVideoType", "musicCoverType", "storyEventMasterId", "storyMasterId", "eventMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music_master\" (\"userId\", \"id\", \"name\", \"description\", \"rewardRuleMasterId\", \"pronounceName\", \"lyricWriter\", \"composer\", \"arranger\", \"unlockText\", \"isLongVersion\", \"releasedAt\", \"staminaConsumption\", \"musicTimeSecond\", \"invisible\", \"sampleStartSeconds\", \"sampleEndSeconds\", \"delaySeconds\", \"vocalVersions\", \"unlockConditionType\", \"unlockConditionValue\", \"musicVideoType\", \"musicCoverType\", \"storyEventMasterId\", \"storyMasterId\", \"eventMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_sense_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "type", "preEffects", "branches", "acquirableGauge", "acquirableScorePercent", "scoreUpPerLevel", "lightCount", "coolTime", "branchCondition1", "conditionValue1", "branchCondition2", "conditionValue2", "subTypes"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"sense_master\" (\"userId\", \"id\", \"name\", \"description\", \"type\", \"preEffects\", \"branches\", \"acquirableGauge\", \"acquirableScorePercent\", \"scoreUpPerLevel\", \"lightCount\", \"coolTime\", \"branchCondition1\", \"conditionValue1\", \"branchCondition2\", \"conditionValue2\", \"subTypes\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "type", "companyMasterId", "eventMasterId", "chapterOrder", "displayStartAt", "displayEndAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_master\" (\"userId\", \"id\", \"type\", \"companyMasterId\", \"eventMasterId\", \"chapterOrder\", \"displayStartAt\", \"displayEndAt\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster_level_pattern_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "patterns"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster_level_pattern_group_master\" (\"userId\", \"id\", \"patterns\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster_level_pattern_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "levelPatternGroupId", "level", "itemMasterId", "quantity"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster_level_pattern_master\" (\"userId\", \"id\", \"levelPatternGroupId\", \"level\", \"itemMasterId\", \"quantity\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "organizeRestrictGroupId", "rarity", "levelPatternGroupMasterId", "subTitlePositionX1", "subTitlePositionY1", "subTitlePositionX2", "subTitlePositionY2", "subTitlePositionX3", "subTitlePositionY3", "releaseItemGroupId", "pronounceName", "costumes", "appearanceCharacterBaseMasterIds", "isRestrictItemBreakThrough", "displayStartAt", "displayEndAt", "unlockText", "orientation", "subTitleDisplayCondition", "subTitleDisplayConditionValue", "posterBreakthroughMaxPhase", "posterBreakthroughMaxPhaseReleaseDate", "secondarySubTitleDisplayCondition", "secondarySubTitleDisplayConditionValue", "alternateImagePositionX1", "alternateImagePositionY1", "alternateImageReleasePhase1", "alternateImagePositionX2", "alternateImagePositionY2", "alternateImageReleasePhase2", "alternateImagePositionX3", "alternateImagePositionY3", "alternateImageReleasePhase3"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster_master\" (\"userId\", \"id\", \"name\", \"organizeRestrictGroupId\", \"rarity\", \"levelPatternGroupMasterId\", \"subTitlePositionX1\", \"subTitlePositionY1\", \"subTitlePositionX2\", \"subTitlePositionY2\", \"subTitlePositionX3\", \"subTitlePositionY3\", \"releaseItemGroupId\", \"pronounceName\", \"costumes\", \"appearanceCharacterBaseMasterIds\", \"isRestrictItemBreakThrough\", \"displayStartAt\", \"displayEndAt\", \"unlockText\", \"orientation\", \"subTitleDisplayCondition\", \"subTitleDisplayConditionValue\", \"posterBreakthroughMaxPhase\", \"posterBreakthroughMaxPhaseReleaseDate\", \"secondarySubTitleDisplayCondition\", \"secondarySubTitleDisplayConditionValue\", \"alternateImagePositionX1\", \"alternateImagePositionY1\", \"alternateImageReleasePhase1\", \"alternateImagePositionX2\", \"alternateImagePositionY2\", \"alternateImageReleasePhase2\", \"alternateImagePositionX3\", \"alternateImagePositionY3\", \"alternateImageReleasePhase3\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34, $35, $36)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_live(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "liveMasterId", "timesCompleted", "achievementRate", "notationRate", "clearLamp", "status", "rateGrade"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"live\" (\"userId\", \"id\", \"liveMasterId\", \"timesCompleted\", \"achievementRate\", \"notationRate\", \"clearLamp\", \"status\", \"rateGrade\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "musicMasterId", "stellaReleased", "vocalVersion", "olivierReleaseStatus", "isPossession"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music\" (\"userId\", \"id\", \"musicMasterId\", \"stellaReleased\", \"vocalVersion\", \"olivierReleaseStatus\", \"isPossession\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_accessory(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "accessoryMasterId", "level", "locked", "accessoryEffects", "referenceCounting", "isFavorite"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"accessory\" (\"userId\", \"id\", \"accessoryMasterId\", \"level\", \"locked\", \"accessoryEffects\", \"referenceCounting\", \"isFavorite\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_item(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "itemMasterId", "stock"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"item\" (\"userId\", \"id\", \"itemMasterId\", \"stock\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

std::string increment_item_stock(long long user_id, long long item_master_id,
                                 long long delta) {
    // data-modifying CTE -> composite: add to stock; if no row changed, insert it
    ExecutableQuery update_q("UPDATE \"item\" SET \"stock\" = \"stock\" + $3 WHERE \"userId\" = $1 AND \"itemMasterId\" = $2",
                            user_id, item_master_id, delta);
    ExecutableQuery insert_q("INSERT INTO \"item\" (\"userId\", \"id\", \"itemMasterId\", \"stock\") SELECT $1, $2, $2, $3",
                            user_id, item_master_id, delta);
    return db::composite_update_or_insert(update_q, insert_q);
}

std::string increment_item_stocks(
    long long user_id, const std::vector<std::pair<long long, long long>>& items) {
    // data-modifying CTE -> composite: add to existing items, then insert the new ones
    std::string values;
    std::vector<json> args;
    args.push_back(json(user_id));
    for (const std::pair<long long, long long>& it : items) {
        std::size_t n = args.size();
        if (!values.empty()) values += ", ";
        values += "($" + std::to_string(n + 1) + ", $" + std::to_string(n + 2) + ")";
        args.push_back(json(it.first));
        args.push_back(json(it.second));
    }
    ExecutableQuery update_q;
    update_q.sql = "WITH v(item_id, delta) AS (VALUES " + values + ") UPDATE \"item\" SET \"stock\" = \"stock\" + v.delta FROM v WHERE \"item\".\"userId\" = $1 AND \"item\".\"itemMasterId\" = v.item_id";
    update_q.args = args;
    ExecutableQuery insert_q;
    insert_q.sql = "WITH v(item_id, delta) AS (VALUES " + values + ") INSERT INTO \"item\" (\"userId\", \"id\", \"itemMasterId\", \"stock\") SELECT $1, v.item_id, v.item_id, v.delta FROM v WHERE v.item_id NOT IN (SELECT \"itemMasterId\" FROM \"item\" WHERE \"userId\" = $1)";
    insert_q.args = args;
    return db::composite_exec({update_q, insert_q}).back();
}

ExecutableQuery add_currency(long long user_id, long long coin, long long free_jewel) {
    return ExecutableQuery(
        "UPDATE \"currency\" SET \"coin\" = \"coin\" + $2, \"freeJewel\" = \"freeJewel\" + $3 WHERE \"userId\" = $1",
        user_id,
        coin,
        free_jewel);
}

ExecutableQuery mark_inboxs_checked(long long user_id) {
    return ExecutableQuery(
        "UPDATE \"inbox\" SET \"checked\" = true WHERE \"userId\" = $1 AND \"checked\" = false",
        user_id);
}

ExecutableQuery create_inbox(long long user_id, long long inbox_id,
                             long long thing_type, long long thing_id,
                             long long quantity, std::optional<std::string> description,
                             long long sent_at, long long receive_limit_at) {
    ExecutableQuery q;
    q.sql = "INSERT INTO \"inbox\" (\"userId\", \"id\", \"thingType\", \"thingId\", \"thingQuantity\", \"isTimeLimited\", \"hasReceived\", \"description\", \"sentAt\", \"receiveLimitAt\") VALUES ($1, $2, $3, $4, $5, true, false, $6, $7, $8)";
    q.args.push_back(json(user_id));
    q.args.push_back(json(inbox_id));
    q.args.push_back(json(thing_type));
    q.args.push_back(json(thing_id));
    q.args.push_back(json(quantity));
    q.args.push_back(description.has_value() ? json(*description) : json(nullptr));
    q.args.push_back(json(sent_at));
    q.args.push_back(json(receive_limit_at));
    return q;
}

ExecutableQuery upsert_accessory_effect_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "effectMasterId", "name", "description", "variety"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"accessory_effect_master\" (\"userId\", \"id\", \"effectMasterId\", \"name\", \"description\", \"variety\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_company_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "companies", "description", "isOther"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"company_master\" (\"userId\", \"id\", \"name\", \"companies\", \"description\", \"isOther\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_effect_duration_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "durations"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"effect_duration_group_master\" (\"userId\", \"id\", \"durations\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_effect_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "type", "range", "calculationType", "details", "conditions", "durationSecond", "triggers", "fireTimingType"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"effect_master\" (\"userId\", \"id\", \"type\", \"range\", \"calculationType\", \"details\", \"conditions\", \"durationSecond\", \"triggers\", \"fireTimingType\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_item_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "displayOrder", "displayEndDate", "maxStock", "category", "consumable", "jumpType", "jumpTargetId", "tabCategory", "rarity"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"item_master\" (\"userId\", \"id\", \"name\", \"description\", \"displayOrder\", \"displayEndDate\", \"maxStock\", \"category\", \"consumable\", \"jumpType\", \"jumpTargetId\", \"tabCategory\", \"rarity\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_random_effect_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "accessoryEffects"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"random_effect_group_master\" (\"userId\", \"id\", \"accessoryEffects\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_reward_rule_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "achivementRateRewards"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"reward_rule_master\" (\"userId\", \"id\", \"achivementRateRewards\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_sense_effect_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"order", "effectMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"sense_effect_master\" (\"userId\", \"order\", \"effectMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trophy_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "category"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trophy_group_master\" (\"userId\", \"id\", \"category\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trophy_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "rarity", "order", "trophyGroupMasterId", "hidden", "unlockText"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trophy_master\" (\"userId\", \"id\", \"name\", \"description\", \"rarity\", \"order\", \"trophyGroupMasterId\", \"hidden\", \"unlockText\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_lesson(long long user_id, const wire::json& row) {
    static const char* cols[] = {"characterBaseMasterId", "setCharacters", "bestScore", "leaderPosition", "rewardReceivedHighScore"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_lesson\" (\"userId\", \"characterBaseMasterId\", \"setCharacters\", \"bestScore\", \"leaderPosition\", \"rewardReceivedHighScore\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_daily_lesson(long long user_id, const wire::json& row) {
    static const char* cols[] = {"timesLeft"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"daily_lesson\" (\"userId\", \"timesLeft\") VALUES ($1, $2)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_inbox(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "thingType", "thingId", "thingQuantity", "isTimeLimited", "hasReceived", "title", "description", "sentAt", "receivedAt", "receiveLimitAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"inbox\" (\"userId\", \"id\", \"thingType\", \"thingId\", \"thingQuantity\", \"isTimeLimited\", \"hasReceived\", \"title\", \"description\", \"sentAt\", \"receivedAt\", \"receiveLimitAt\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_bomb(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "bombMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"bomb\" (\"userId\", \"id\", \"bombMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_costume(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "costumeMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"costume\" (\"userId\", \"id\", \"costumeMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_name_color(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "nameColorMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"name_color\" (\"userId\", \"id\", \"nameColorMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_nameplate(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "namePlateMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"nameplate\" (\"userId\", \"id\", \"namePlateMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_note(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "noteMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"note\" (\"userId\", \"id\", \"noteMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_stamp(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "stampMasterIds", "favoriteStampMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"stamp\" (\"userId\", \"id\", \"stampMasterIds\", \"favoriteStampMasterIds\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_mission(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "isCleared", "isRewardReceived", "missionCurrentCount", "missionMasterId", "currentMissionStageMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"mission\" (\"userId\", \"id\", \"isCleared\", \"isRewardReceived\", \"missionCurrentCount\", \"missionMasterId\", \"currentMissionStageMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_audition_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "musicMasterId", "recommendedCompany", "canSkip", "senseNotationMasterId", "maxPhase", "displayStartAt", "displayEndAt", "vocalVersion", "auditionGroupNumber", "skipStartAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"audition_master\" (\"userId\", \"id\", \"musicMasterId\", \"recommendedCompany\", \"canSkip\", \"senseNotationMasterId\", \"maxPhase\", \"displayStartAt\", \"displayEndAt\", \"vocalVersion\", \"auditionGroupNumber\", \"skipStartAt\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_bomb_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "order", "hidden", "isDefault"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"bomb_master\" (\"userId\", \"id\", \"name\", \"description\", \"order\", \"hidden\", \"isDefault\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_star_rank_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"rank", "nextRankPoint", "requiredLessonScore", "statusBonus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_star_rank_master\" (\"userId\", \"rank\", \"nextRankPoint\", \"requiredLessonScore\", \"statusBonus\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_star_rank_reward_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "rewards"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_star_rank_reward_group_master\" (\"userId\", \"id\", \"rewards\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_costume_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "order", "isDefault", "costumeGroupMasterId", "description"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"costume_master\" (\"userId\", \"id\", \"name\", \"order\", \"isDefault\", \"costumeGroupMasterId\", \"description\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_home_character_voice_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterId", "text", "weight", "characterVoicePeriodMasterId", "isPlayerBirthDateVoice", "voiceFileName1", "voiceFileName2", "voiceFileName3", "voiceFileName4", "voiceInterval1", "voiceInterval2", "voiceInterval3", "mouthMotionId1", "mouthMotionId2", "mouthMotionId3", "mouthMotionId4", "bodyMotionId1", "bodyMotionId2", "bodyMotionId3", "bodyMotionId4"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"home_character_voice_master\" (\"userId\", \"id\", \"characterBaseMasterId\", \"text\", \"weight\", \"characterVoicePeriodMasterId\", \"isPlayerBirthDateVoice\", \"voiceFileName1\", \"voiceFileName2\", \"voiceFileName3\", \"voiceFileName4\", \"voiceInterval1\", \"voiceInterval2\", \"voiceInterval3\", \"mouthMotionId1\", \"mouthMotionId2\", \"mouthMotionId3\", \"mouthMotionId4\", \"bodyMotionId1\", \"bodyMotionId2\", \"bodyMotionId3\", \"bodyMotionId4\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_name_color_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "order", "hidden", "isDefault", "unlockText"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"name_color_master\" (\"userId\", \"id\", \"name\", \"description\", \"order\", \"hidden\", \"isDefault\", \"unlockText\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_nameplate_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "order", "hidden", "isDefault", "details", "unlockText", "changeType", "changeValue1", "changeValue2"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"nameplate_master\" (\"userId\", \"id\", \"name\", \"description\", \"order\", \"hidden\", \"isDefault\", \"details\", \"unlockText\", \"changeType\", \"changeValue1\", \"changeValue2\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_note_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "description", "order", "hidden", "isDefault"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"note_master\" (\"userId\", \"id\", \"name\", \"description\", \"order\", \"hidden\", \"isDefault\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_spot_conversation_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "spot", "characterId1", "characterId2", "characterId3", "characterId4", "characterId5", "episodeMasterId", "costumeId1", "costumeId2", "costumeId3", "costumeId4", "costumeId5", "title"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"spot_conversation_master\" (\"userId\", \"id\", \"spot\", \"characterId1\", \"characterId2\", \"characterId3\", \"characterId4\", \"characterId5\", \"episodeMasterId\", \"costumeId1\", \"costumeId2\", \"costumeId3\", \"costumeId4\", \"costumeId5\", \"title\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_stamp_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "order", "isDefault", "characterBaseMasterId", "type", "assetId", "voiceAssetId", "name", "characterBaseMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"stamp_master\" (\"userId\", \"id\", \"order\", \"isDefault\", \"characterBaseMasterId\", \"type\", \"assetId\", \"voiceAssetId\", \"name\", \"characterBaseMasterIds\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_lesson_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"position", "setCharacterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_lesson_slot\" (\"userId\", \"position\", \"setCharacterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trophy(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "trophyMasterId", "trophyGroupMasterId", "currentOrder"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trophy\" (\"userId\", \"id\", \"trophyMasterId\", \"trophyGroupMasterId\", \"currentOrder\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_market(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "lastRefreshedAt", "refreshTimes"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"market\" (\"userId\", \"id\", \"lastRefreshedAt\", \"refreshTimes\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_viewed_shop(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "exchangeShopMasterId", "lastViewedAt", "viewedShopCategory"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"viewed_shop\" (\"userId\", \"id\", \"exchangeShopMasterId\", \"lastViewedAt\", \"viewedShopCategory\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_game_hint(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "pageCategory", "hasAlreadyRead"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"game_hint\" (\"userId\", \"id\", \"pageCategory\", \"hasAlreadyRead\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

std::string mark_game_hint_read(long long user_id, long long page_category) {
    // data-modifying CTE -> composite: mark read; if no row changed, insert (id = category)
    ExecutableQuery update_q("UPDATE \"game_hint\" SET \"hasAlreadyRead\" = true WHERE \"userId\" = $1 AND \"pageCategory\" = $2",
                            user_id, page_category);
    ExecutableQuery insert_q("INSERT INTO \"game_hint\" (\"userId\", \"id\", \"pageCategory\", \"hasAlreadyRead\") SELECT $1, $2, $2, true",
                            user_id, page_category);
    return db::composite_update_or_insert(update_q, insert_q);
}

ExecutableQuery upsert_user_bonus(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "experienceBonus", "lessonStarRankBonus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"user_bonus\" (\"userId\", \"id\", \"experienceBonus\", \"lessonStarRankBonus\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_audition_phase_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "auditionasterId", "phase", "recommendedPlayerRank", "clearScore", "starActCount", "auditionRewardPackageMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"audition_phase_master\" (\"userId\", \"id\", \"auditionasterId\", \"phase\", \"recommendedPlayerRank\", \"clearScore\", \"starActCount\", \"auditionRewardPackageMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_audition_reward_package_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "rewards"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"audition_reward_package_master\" (\"userId\", \"id\", \"rewards\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_campaign_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "title", "description", "iconImagePath", "order", "startDate", "endDate", "comebackCampaignMasterId", "campaignEffectType", "campaignEffectValue"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"campaign_master\" (\"userId\", \"id\", \"title\", \"description\", \"iconImagePath\", \"order\", \"startDate\", \"endDate\", \"comebackCampaignMasterId\", \"campaignEffectType\", \"campaignEffectValue\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_awakening_item_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "awakeningPhase", "itemMasterId", "requiredQuantity"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_awakening_item_master\" (\"userId\", \"id\", \"awakeningPhase\", \"itemMasterId\", \"requiredQuantity\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_bloom_bonus_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "bloomBonuses", "bloomRewards"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_bloom_bonus_group_master\" (\"userId\", \"id\", \"bloomBonuses\", \"bloomRewards\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_bloom_item_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"rarity", "currentStage", "requiredPieceAmount", "talentBloomItemType", "genericBloomItemMasterId", "requiredItemMasterId", "requiredItemAmount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_bloom_item_master\" (\"userId\", \"rarity\", \"currentStage\", \"requiredPieceAmount\", \"talentBloomItemType\", \"genericBloomItemMasterId\", \"requiredItemMasterId\", \"requiredItemAmount\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_experience_item_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "itemMasterId", "acquirableExperience", "acquirableExperienceBonus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_experience_item_master\" (\"userId\", \"id\", \"itemMasterId\", \"acquirableExperience\", \"acquirableExperienceBonus\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_mission_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "title", "jumpType", "jumpValue", "stages"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_mission_master\" (\"userId\", \"id\", \"title\", \"jumpType\", \"jumpValue\", \"stages\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_mission_stage_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterMissionCategoryLevelMasterId", "characterMissionMasterId", "exclusionNoSenseCharacter", "order", "stageOrder", "goalCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_mission_stage_master\" (\"userId\", \"id\", \"characterMissionCategoryLevelMasterId\", \"characterMissionMasterId\", \"exclusionNoSenseCharacter\", \"order\", \"stageOrder\", \"goalCount\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_piece_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"itemMasterId", "characterMasterId", "dugongRequiredAmount", "talentBloomItemType"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_piece_master\" (\"userId\", \"itemMasterId\", \"characterMasterId\", \"dugongRequiredAmount\", \"talentBloomItemType\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_sense_enhance_item_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "items"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_sense_enhance_item_group_master\" (\"userId\", \"id\", \"items\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_costume_wearable_character_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"costume_wearable_character_group_master\" (\"userId\", \"id\", \"characterBaseMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_exchange_shop_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "isDisplayRequiredHavingItem", "category", "name", "displayThingType", "displayItemMasterId", "bannerPath", "startDate", "endDate", "lastRefreshedAt", "lineup", "order"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"exchange_shop_master\" (\"userId\", \"id\", \"isDisplayRequiredHavingItem\", \"category\", \"name\", \"displayThingType\", \"displayItemMasterId\", \"bannerPath\", \"startDate\", \"endDate\", \"lastRefreshedAt\", \"lineup\", \"order\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_live_setting_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "liveType", "liveDropFrameGroupMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"live_setting_master\" (\"userId\", \"id\", \"name\", \"liveType\", \"liveDropFrameGroupMasterId\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_mission_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "missionCategory", "missionViewOrder", "title", "description", "eventMasterId", "jumpType", "jumpTargetId", "startDate", "endDate", "stages", "comebackCampaignMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"mission_master\" (\"userId\", \"id\", \"missionCategory\", \"missionViewOrder\", \"title\", \"description\", \"eventMasterId\", \"jumpType\", \"jumpTargetId\", \"startDate\", \"endDate\", \"stages\", \"comebackCampaignMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music_vocal_version_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "musicMasterId", "vocalVersion", "singer", "name", "musicTimeSecond", "sampleStartSeconds", "sampleEndSeconds", "musicVideoType", "characters"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music_vocal_version_master\" (\"userId\", \"id\", \"musicMasterId\", \"vocalVersion\", \"singer\", \"name\", \"musicTimeSecond\", \"sampleStartSeconds\", \"sampleEndSeconds\", \"musicVideoType\", \"characters\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster_release_item_group_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "items", "itemConsumeApplyFlag"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster_release_item_group_master\" (\"userId\", \"id\", \"items\", \"itemConsumeApplyFlag\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster_release_item_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"currentPhase", "itemMasterId", "requiredQuantity"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster_release_item_master\" (\"userId\", \"currentPhase\", \"itemMasterId\", \"requiredQuantity\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_poster_story_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "posterMasterId", "episodeType", "characterBaseMasterId", "description", "order", "characterIconId", "characterName"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"poster_story_master\" (\"userId\", \"id\", \"posterMasterId\", \"episodeType\", \"characterBaseMasterId\", \"description\", \"order\", \"characterIconId\", \"characterName\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_star_rank_reward_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"rank", "characterBaseMasterId", "characterStarRankRewardGroupMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"star_rank_reward_master\" (\"userId\", \"rank\", \"characterBaseMasterId\", \"characterStarRankRewardGroupMasterId\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_audition_clear(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "auditionMasterId", "clearPhase", "auditionClearPartyId", "skipClearPhase"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"audition_clear\" (\"userId\", \"id\", \"auditionMasterId\", \"clearPhase\", \"auditionClearPartyId\", \"skipClearPhase\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_sp_rate(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "liveMasterId", "point"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"sp_rate\" (\"userId\", \"id\", \"liveMasterId\", \"point\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_notification(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "importantReadAt", "updateReadAt", "bugReadAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"notification\" (\"userId\", \"id\", \"importantReadAt\", \"updateReadAt\", \"bugReadAt\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_episode(long long user_id, const wire::json& row) {
    static const char* cols[] = {"episodeMasterId", "hasReadAll"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"episode\" (\"userId\", \"episodeMasterId\", \"hasReadAll\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery update_episode_read_all(long long user_id, long long episode_master_id,
                                        bool has_read_all) {
    return ExecutableQuery(
        "UPDATE \"episode\" SET \"hasReadAll\" = $3 WHERE \"userId\" = $1 AND \"episodeMasterId\" = $2",
        user_id,
        episode_master_id,
        has_read_all);
}

ExecutableQuery upsert_character_mission(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterId", "characterMissionMasterId", "currentStageMasterId", "currentCount", "clearedStageOrder", "rewardReceivedStageOrder", "completedLevel"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_mission\" (\"userId\", \"id\", \"characterBaseMasterId\", \"characterMissionMasterId\", \"currentStageMasterId\", \"currentCount\", \"clearedStageOrder\", \"rewardReceivedStageOrder\", \"completedLevel\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_mission_pass(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "missionPassMasterId", "paid", "freeRewardReceivedPhase", "spRewardReceivedPhase", "terminated", "freeRewardLoopCount", "freeRewardLoopReceivedPhase", "paidRewardLoopCount", "paidRewardLoopReceivedPhase"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"mission_pass\" (\"userId\", \"id\", \"missionPassMasterId\", \"paid\", \"freeRewardReceivedPhase\", \"spRewardReceivedPhase\", \"terminated\", \"freeRewardLoopCount\", \"freeRewardLoopReceivedPhase\", \"paidRewardLoopCount\", \"paidRewardLoopReceivedPhase\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_mission_pass_detail_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "phase", "missionPassMasterId", "clearPoint", "startDate", "endDate", "rewards"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"mission_pass_detail_master\" (\"userId\", \"id\", \"phase\", \"missionPassMasterId\", \"clearPoint\", \"startDate\", \"endDate\", \"rewards\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_mission_pass_master(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "itemMasterId", "startDate", "endDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"mission_pass_master\" (\"userId\", \"id\", \"itemMasterId\", \"startDate\", \"endDate\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_league_basic(long long user_id, const wire::json& row) {
    static const char* cols[] = {"myProperty", "starEnrollCount", "daiStarEnrollCount", "currentClassType", "bestClassType", "lastJoinedLeagueSeasonMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_basic\" (\"userId\", \"myProperty\", \"starEnrollCount\", \"daiStarEnrollCount\", \"currentClassType\", \"bestClassType\", \"lastJoinedLeagueSeasonMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventMasterId", "totalAcquiredPoint", "acquiredPointUpdatedDate", "lastRank", "readTips", "loginDays"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event\" (\"userId\", \"id\", \"storyEventMasterId\", \"totalAcquiredPoint\", \"acquiredPointUpdatedDate\", \"lastRank\", \"readTips\", \"loginDays\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_exchange_limit(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "exchangeShopThingId", "replaceType", "specifiedNumberOfDaysLimit", "exchangedCount", "until"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"exchange_limit\" (\"userId\", \"id\", \"exchangeShopThingId\", \"replaceType\", \"specifiedNumberOfDaysLimit\", \"exchangedCount\", \"until\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_league_group(long long user_id, const wire::json& row) {
    static const char* cols[] = {"leagueMasterId", "classType", "classOrder"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_group\" (\"userId\", \"leagueMasterId\", \"classType\", \"classOrder\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_league_group_member(long long user_id, const wire::json& row) {
    static const char* cols[] = {"leagueGroupId", "leagueMasterId", "bestScore"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_group_member\" (\"userId\", \"leagueGroupId\", \"leagueMasterId\", \"bestScore\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_league_history(long long user_id, const wire::json& row) {
    static const char* cols[] = {"leagueMasterId", "classType", "historyCount", "isSendedReward", "isPlayed", "classChangeType", "groupRank", "globalRank", "allClassGlobalRank"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_history\" (\"userId\", \"leagueMasterId\", \"classType\", \"historyCount\", \"isSendedReward\", \"isPlayed\", \"classChangeType\", \"groupRank\", \"globalRank\", \"allClassGlobalRank\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_jewel_shop(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "jewelShopItemMasterId", "purchaseCount", "totalPurchaseCount", "rePurchaseDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"jewel_shop\" (\"userId\", \"id\", \"jewelShopItemMasterId\", \"purchaseCount\", \"totalPurchaseCount\", \"rePurchaseDate\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_daily_limit(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "autoPlayTimes", "dailyLessonTimes", "lastRefreshedAt", "musicCourseFreeChallengeTimes"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"daily_limit\" (\"userId\", \"id\", \"autoPlayTimes\", \"dailyLessonTimes\", \"lastRefreshedAt\", \"musicCourseFreeChallengeTimes\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery update_daily_limit(long long user_id, const wire::json& row) {
    ExecutableQuery q;
    q.sql = "UPDATE \"daily_limit\" SET \"autoPlayTimes\" = $2, \"dailyLessonTimes\" = $3, \"lastRefreshedAt\" = $4, \"musicCourseFreeChallengeTimes\" = $5 WHERE \"userId\" = $1";
    q.args.push_back(json(user_id));
    q.args.push_back(row_get(row, "autoPlayTimes"));
    q.args.push_back(row_get(row, "dailyLessonTimes"));
    q.args.push_back(row_get(row, "lastRefreshedAt"));
    q.args.push_back(row_get(row, "musicCourseFreeChallengeTimes"));
    return q;
}

ExecutableQuery upsert_league_high_score_party(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "leagueMasterId", "highScore", "classType", "difficulty", "musicMasterId", "leagueGroupId", "slots", "userName", "actingAbility", "leaderPosition"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_high_score_party\" (\"userId\", \"id\", \"leagueMasterId\", \"highScore\", \"classType\", \"difficulty\", \"musicMasterId\", \"leagueGroupId\", \"slots\", \"userName\", \"actingAbility\", \"leaderPosition\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_league_high_score_party_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "leagueHighScorePartyId", "position", "characterMasterId", "characterLevel", "posterMasterId", "posterLevel", "posterBreakthroughPhase", "accessoryMasterId", "accessoryLevel", "currentStatus", "characterTalentStage", "characterAwakeningPhase", "characterDisplayAwakeningStatus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_high_score_party_slot\" (\"userId\", \"id\", \"leagueHighScorePartyId\", \"position\", \"characterMasterId\", \"characterLevel\", \"posterMasterId\", \"posterLevel\", \"posterBreakthroughPhase\", \"accessoryMasterId\", \"accessoryLevel\", \"currentStatus\", \"characterTalentStage\", \"characterAwakeningPhase\", \"characterDisplayAwakeningStatus\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_circle(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventMasterId", "currentPoint", "highScore", "circleId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_circle\" (\"userId\", \"id\", \"storyEventMasterId\", \"currentPoint\", \"highScore\", \"circleId\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_circle_mission(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventCircleMissionMasterId", "currentCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_circle_mission\" (\"userId\", \"id\", \"storyEventCircleMissionMasterId\", \"currentCount\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_circle_mission_reward(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventMasterId", "storyEventCircleMissionRewardMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_circle_mission_reward\" (\"userId\", \"id\", \"storyEventMasterId\", \"storyEventCircleMissionRewardMasterId\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_high_score_buff_setting(long long user_id, const wire::json& row) {
    static const char* cols[] = {"storyEventHighScoreBuffSettingMasterId", "currentLevel"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_high_score_buff_setting\" (\"userId\", \"storyEventHighScoreBuffSettingMasterId\", \"currentLevel\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_high_score_party(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventMasterId", "highScore", "rateGrade", "difficulty", "highScoreType", "liveSettingMasterId", "slots", "leaderPosition"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_high_score_party\" (\"userId\", \"id\", \"storyEventMasterId\", \"highScore\", \"rateGrade\", \"difficulty\", \"highScoreType\", \"liveSettingMasterId\", \"slots\", \"leaderPosition\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_high_score_party_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventHighScoreClearPartyId", "position", "characterMasterId", "characterLevel", "characterTalentStage", "characterAwakeningPhase", "posterMasterId", "posterLevel", "posterBreakthroughPhase", "accessoryMasterId", "accessoryLevel", "currentStatus", "characterDisplayAwakeningStatus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_high_score_party_slot\" (\"userId\", \"id\", \"storyEventHighScoreClearPartyId\", \"position\", \"characterMasterId\", \"characterLevel\", \"characterTalentStage\", \"characterAwakeningPhase\", \"posterMasterId\", \"posterLevel\", \"posterBreakthroughPhase\", \"accessoryMasterId\", \"accessoryLevel\", \"currentStatus\", \"characterDisplayAwakeningStatus\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_connect_with_account(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "provider"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"connect_with_account\" (\"userId\", \"id\", \"provider\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_connect_with_password(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "passwordHash", "linkageCode", "confirmationCode", "confirmationExpiresAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"connect_with_password\" (\"userId\", \"id\", \"passwordHash\", \"linkageCode\", \"confirmationCode\", \"confirmationExpiresAt\") VALUES ($1, $2, $3, $4, $5, $6) ON CONFLICT (\"userId\") DO UPDATE SET \"id\" = EXCLUDED.\"id\", \"passwordHash\" = EXCLUDED.\"passwordHash\", \"linkageCode\" = EXCLUDED.\"linkageCode\", \"confirmationCode\" = COALESCE(EXCLUDED.\"confirmationCode\", \"connect_with_password\".\"confirmationCode\"), \"confirmationExpiresAt\" = CASE WHEN EXCLUDED.\"confirmationCode\" IS NULL THEN \"connect_with_password\".\"confirmationExpiresAt\" ELSE EXCLUDED.\"confirmationExpiresAt\" END";
    q.args.push_back(json(user_id));
    for (const char* k : cols) {
        json v = row_get(row, k);
        // confirmationExpiresAt is NOT NULL: default a missing value to 0, not NULL
        if (std::string(k) == "confirmationExpiresAt" && v.is_null()) v = json(0);
        q.args.push_back(v);
    }
    return q;
}

ExecutableQuery set_connect_with_password_confirmation(long long user_id,
                                                       const std::string& confirmation_code,
                                                       long long expires_at) {
    return ExecutableQuery(
        "UPDATE \"connect_with_password\" SET \"confirmationCode\" = $2, \"confirmationExpiresAt\" = $3 WHERE \"userId\" = $1",
        user_id,
        confirmation_code,
        expires_at);
}

ExecutableQuery upsert_tournament_detail(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "tournamentDetailMasterId", "bestUniqueScore", "perfectStar", "perfect", "great", "good", "bad", "miss", "recordedAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"tournament_detail\" (\"userId\", \"id\", \"tournamentDetailMasterId\", \"bestUniqueScore\", \"perfectStar\", \"perfect\", \"great\", \"good\", \"bad\", \"miss\", \"recordedAt\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_gradual_mission_group(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "gradualMissionGroupMasterId", "startAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"gradual_mission_group\" (\"userId\", \"id\", \"gradualMissionGroupMasterId\", \"startAt\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_photo(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "fileName", "sasToken", "photoEffectMasterId", "lock", "useAlbumPage", "level", "rarity", "signMasterId", "generatedAt", "thumbnailSasToken", "appearedCharacterBaseMasterIds", "taggedCharacterBaseMasterIds", "useDecoPage"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"photo\" (\"userId\", \"id\", \"fileName\", \"sasToken\", \"photoEffectMasterId\", \"lock\", \"useAlbumPage\", \"level\", \"rarity\", \"signMasterId\", \"generatedAt\", \"thumbnailSasToken\", \"appearedCharacterBaseMasterIds\", \"taggedCharacterBaseMasterIds\", \"useDecoPage\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_album(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "level", "publishPageNumber", "currentPresetOrder"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"album\" (\"userId\", \"id\", \"level\", \"publishPageNumber\", \"currentPresetOrder\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_album_page(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "page", "editType", "publishing", "items", "albumThemeMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"album_page\" (\"userId\", \"id\", \"page\", \"editType\", \"publishing\", \"items\", \"albumThemeMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_star_pass_status(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "type", "totalPurchasedCount", "validUntil"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"star_pass_status\" (\"userId\", \"id\", \"type\", \"totalPurchasedCount\", \"validUntil\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_login_pass_status(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "validUntil"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"login_pass_status\" (\"userId\", \"id\", \"validUntil\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_currency(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "coin", "freeJewel", "paidJewel"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"currency\" (\"userId\", \"id\", \"coin\", \"freeJewel\", \"paidJewel\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_decoration(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "decorationMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"decoration\" (\"userId\", \"id\", \"decorationMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_live_achievement(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "olivierReleasedCount", "olivierClearedLevel"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"live_achievement\" (\"userId\", \"id\", \"olivierReleasedCount\", \"olivierClearedLevel\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music_video(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "musicVideoMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music_video\" (\"userId\", \"id\", \"musicVideoMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_theater_story(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "theaterStoryMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"theater_story\" (\"userId\", \"id\", \"theaterStoryMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_live_drop_celling(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "multiLiveScheduleMasterId", "count", "totalCellingCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"live_drop_celling\" (\"userId\", \"id\", \"multiLiveScheduleMasterId\", \"count\", \"totalCellingCount\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_story_event_high_score(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "storyEventMasterId", "currentEnhancementPoint", "totalAcquiredEnhancementPoint"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"story_event_high_score\" (\"userId\", \"id\", \"storyEventMasterId\", \"currentEnhancementPoint\", \"totalAcquiredEnhancementPoint\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_comic(long long user_id, const wire::json& row) {
    static const char* cols[] = {"comicEpisodeMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"comic\" (\"userId\", \"comicEpisodeMasterId\") VALUES ($1, $2)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_comeback_campaign(long long user_id, const wire::json& row) {
    static const char* cols[] = {"comebackCampaignMasterId", "activatedAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"comeback_campaign\" (\"userId\", \"comebackCampaignMasterId\", \"activatedAt\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_concert_stage(long long user_id, const wire::json& row) {
    static const char* cols[] = {"concertStageMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"concert_stage\" (\"userId\", \"concertStageMasterId\") VALUES ($1, $2)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_limit(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "additionalAcquirablePhotoLimit", "acquirablePhotoLimitIncreasedTimes", "additionalAcquirableAccessoryLimit", "acquirableAccessoryLimitIncreasedTimes"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"limit\" (\"userId\", \"id\", \"additionalAcquirablePhotoLimit\", \"acquirablePhotoLimitIncreasedTimes\", \"additionalAcquirableAccessoryLimit\", \"acquirableAccessoryLimitIncreasedTimes\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_gacha_selected_thing(long long user_id, const wire::json& row) {
    static const char* cols[] = {"gachaMasterId", "gachaThingIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"gacha_selected_thing\" (\"userId\", \"gachaMasterId\", \"gachaThingIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_total_point_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "totalPointEventMasterId", "totalAcquiredPoint", "receivedRewardOrder"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"total_point_event\" (\"userId\", \"id\", \"totalPointEventMasterId\", \"totalAcquiredPoint\", \"receivedRewardOrder\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_event_box_gacha(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "eventBoxGachaMasterId", "currentBoxCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"event_box_gacha\" (\"userId\", \"id\", \"eventBoxGachaMasterId\", \"currentBoxCount\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_event_box_gacha_box_thing(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "eventBoxGachaBoxThingMasterId", "hitCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"event_box_gacha_box_thing\" (\"userId\", \"id\", \"eventBoxGachaBoxThingMasterId\", \"hitCount\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_special_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "specialEventMasterId", "readTips"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"special_event\" (\"userId\", \"id\", \"specialEventMasterId\", \"readTips\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_character_point_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterPointEventMasterId", "characterBaseMasterId", "totalAcquiredPoint", "lastRank", "readTips"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"character_point_event\" (\"userId\", \"id\", \"characterPointEventMasterId\", \"characterBaseMasterId\", \"totalAcquiredPoint\", \"lastRank\", \"readTips\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_another_notation(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "anotherNotationMasterId", "clearLamp", "rateGrade", "achievementRatePercentRecord"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"another_notation\" (\"userId\", \"id\", \"anotherNotationMasterId\", \"clearLamp\", \"rateGrade\", \"achievementRatePercentRecord\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music_bookmark(long long user_id, const wire::json& row) {
    static const char* cols[] = {"musicMasterId", "musicBookmarkFlag"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music_bookmark\" (\"userId\", \"musicMasterId\", \"musicBookmarkFlag\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_live_drop_limit(long long user_id, const wire::json& row) {
    static const char* cols[] = {"multiLiveScheduleMasterId", "currentCount", "countLimit"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"live_drop_limit\" (\"userId\", \"multiLiveScheduleMasterId\", \"currentCount\", \"countLimit\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_restriction(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "multiLiveRestrictionFinishedAt", "readMultiLiveRestrictionDialog"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"restriction\" (\"userId\", \"id\", \"multiLiveRestrictionFinishedAt\", \"readMultiLiveRestrictionDialog\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_permanent_market_thing(long long user_id, const wire::json& row) {
    static const char* cols[] = {"permanentMarketThingMasterId", "purchaseCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"permanent_market_thing\" (\"userId\", \"permanentMarketThingMasterId\", \"purchaseCount\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_time_limited_control(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "timeLimitedControlMasterId", "expiredAt"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"time_limited_control\" (\"userId\", \"id\", \"timeLimitedControlMasterId\", \"expiredAt\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_flash_sale_stage(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "flashSaleStageMasterId", "purchaseLimitedAt", "isDefault", "isCompleted"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"flash_sale_stage\" (\"userId\", \"id\", \"flashSaleStageMasterId\", \"purchaseLimitedAt\", \"isDefault\", \"isCompleted\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_album_theme(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "albumThemeMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"album_theme\" (\"userId\", \"id\", \"albumThemeMasterId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_circle_event_mission(long long user_id, const wire::json& row) {
    static const char* cols[] = {"circleEventMissionMasterId", "currentCount", "isActive"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"circle_event_mission\" (\"userId\", \"circleEventMissionMasterId\", \"currentCount\", \"isActive\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_pickup_character_mission(long long user_id, const wire::json& row) {
    static const char* cols[] = {"pickupCharacterMissionMasterId", "receivedDetailMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"pickup_character_mission\" (\"userId\", \"pickupCharacterMissionMasterId\", \"receivedDetailMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_league_season_result(long long user_id, const wire::json& row) {
    static const char* cols[] = {"leagueSeasonMasterId", "daiStarMaxEnrollCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"league_season_result\" (\"userId\", \"leagueSeasonMasterId\", \"daiStarMaxEnrollCount\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "eventMasterId", "totalAcquiredPoint", "acquiredPointUpdatedDate", "lastRank", "readTips", "loginDays"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"event\" (\"userId\", \"id\", \"eventMasterId\", \"totalAcquiredPoint\", \"acquiredPointUpdatedDate\", \"lastRank\", \"readTips\", \"loginDays\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_bonus_live(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "bonusLiveMasterId", "clearedStageOrder", "readTips", "dailyClearTimes"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"bonus_live\" (\"userId\", \"id\", \"bonusLiveMasterId\", \"clearedStageOrder\", \"readTips\", \"dailyClearTimes\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_bonus_live_stage(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "bonusLiveMasterStageId", "clearTimes"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"bonus_live_stage\" (\"userId\", \"id\", \"bonusLiveMasterStageId\", \"clearTimes\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_roulette_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "rouletteEventMasterId", "totalAcquiredPoint"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"roulette_event\" (\"userId\", \"id\", \"rouletteEventMasterId\", \"totalAcquiredPoint\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_roulette(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "rouletteMasterId", "rollCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"roulette\" (\"userId\", \"id\", \"rouletteMasterId\", \"rollCount\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_home_b_g_m(long long user_id, const wire::json& row) {
    static const char* cols[] = {"homeBGMMasterId", "selectionType", "homeBGMDetailMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"home_b_g_m\" (\"userId\", \"homeBGMMasterId\", \"selectionType\", \"homeBGMDetailMasterId\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_link_character(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterId", "companyMasterId", "linkedCharacterBaseMasterId", "rewardReceivedMaxRank"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"link_character\" (\"userId\", \"id\", \"characterBaseMasterId\", \"companyMasterId\", \"linkedCharacterBaseMasterId\", \"rewardReceivedMaxRank\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music_course(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "musicCourseMasterId", "clearLamp", "certificationGrade", "totalAchievementRatePercentRecord"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music_course\" (\"userId\", \"id\", \"musicCourseMasterId\", \"clearLamp\", \"certificationGrade\", \"totalAchievementRatePercentRecord\") VALUES ($1, $2, $3, $4, $5, $6)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_tournament_qualifying(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "tournamentQualifyingMasterId", "musicCourseMasterId", "currentChallengeCount", "perfectStar", "perfect", "great", "good", "bad", "miss", "totalAchievementRatePercentRecord", "bestRecordChallengeCount", "bestRecordDate"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"tournament_qualifying\" (\"userId\", \"id\", \"tournamentQualifyingMasterId\", \"musicCourseMasterId\", \"currentChallengeCount\", \"perfectStar\", \"perfect\", \"great\", \"good\", \"bad\", \"miss\", \"totalAchievementRatePercentRecord\", \"bestRecordChallengeCount\", \"bestRecordDate\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_lottery(long long user_id, const wire::json& row) {
    static const char* cols[] = {"lotteryMasterId", "results"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"lottery\" (\"userId\", \"lotteryMasterId\", \"results\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_party(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "order", "name", "leaderPosition"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_party\" (\"userId\", \"id\", \"order\", \"name\", \"leaderPosition\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_party_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "partyId", "position", "characterId", "posterId", "accessoryId", "bonusAbilityEnableFlags"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_party_slot\" (\"userId\", \"id\", \"partyId\", \"position\", \"characterId\", \"posterId\", \"accessoryId\", \"bonusAbilityEnableFlags\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_basic(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "starEnrollCount", "daiStarEnrollCount", "currentClassType", "bestClassType", "lastJoinedTripleCastSeasonMasterId", "partyOrder1", "partyOrder2", "partyOrder3"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_basic\" (\"userId\", \"id\", \"starEnrollCount\", \"daiStarEnrollCount\", \"currentClassType\", \"bestClassType\", \"lastJoinedTripleCastSeasonMasterId\", \"partyOrder1\", \"partyOrder2\", \"partyOrder3\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_group(long long user_id, const wire::json& row) {
    static const char* cols[] = {"tripleCastMasterId", "classType", "classOrder"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_group\" (\"userId\", \"tripleCastMasterId\", \"classType\", \"classOrder\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_group_member(long long user_id, const wire::json& row) {
    static const char* cols[] = {"tripleCastGroupId", "tripleCastMasterId", "bestScore"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_group_member\" (\"userId\", \"tripleCastGroupId\", \"tripleCastMasterId\", \"bestScore\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_high_score_party(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "tripleCastMasterId", "order", "highScore", "slots", "actingAbility", "leaderPosition", "difficulty"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_high_score_party\" (\"userId\", \"id\", \"tripleCastMasterId\", \"order\", \"highScore\", \"slots\", \"actingAbility\", \"leaderPosition\", \"difficulty\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_high_score_party_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "tripleCastHighScorePartyId", "position", "characterMasterId", "characterLevel", "posterMasterId", "posterLevel", "posterBreakthroughPhase", "accessoryMasterId", "accessoryLevel", "currentStatus", "characterTalentStage", "characterAwakeningPhase", "characterDisplayAwakeningStatus"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_high_score_party_slot\" (\"userId\", \"id\", \"tripleCastHighScorePartyId\", \"position\", \"characterMasterId\", \"characterLevel\", \"posterMasterId\", \"posterLevel\", \"posterBreakthroughPhase\", \"accessoryMasterId\", \"accessoryLevel\", \"currentStatus\", \"characterTalentStage\", \"characterAwakeningPhase\", \"characterDisplayAwakeningStatus\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_season_result(long long user_id, const wire::json& row) {
    static const char* cols[] = {"tripleCastSeasonMasterId", "daiStarMaxEnrollCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_season_result\" (\"userId\", \"tripleCastSeasonMasterId\", \"daiStarMaxEnrollCount\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_album_preset(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "name", "order"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"album_preset\" (\"userId\", \"id\", \"name\", \"order\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_gacha(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "gachaMasterId", "rollCount"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"gacha\" (\"userId\", \"id\", \"gachaMasterId\", \"rollCount\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_triple_cast_history(long long user_id, const wire::json& row) {
    static const char* cols[] = {"tripleCastMasterId", "classType", "historyCount", "isSendedReward", "isPlayed", "classChangeType", "groupRank", "globalRank", "allClassGlobalRank"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"triple_cast_history\" (\"userId\", \"tripleCastMasterId\", \"classType\", \"historyCount\", \"isSendedReward\", \"isPlayed\", \"classChangeType\", \"groupRank\", \"globalRank\", \"allClassGlobalRank\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_dugong_run(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "clearedCourseIds", "noMistakeCourseIds", "dugongRunCourseGroupId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"dugong_run\" (\"userId\", \"id\", \"clearedCourseIds\", \"noMistakeCourseIds\", \"dugongRunCourseGroupId\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_music_course_ranking(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "musicCourseMasterId", "currentChallengeCount", "perfectStar", "perfect", "great", "good", "bad", "miss", "totalAchievementRatePercentRecord", "bestRecordChallengeCount", "bestRecordDate", "hasReceivedReward"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"music_course_ranking\" (\"userId\", \"id\", \"musicCourseMasterId\", \"currentChallengeCount\", \"perfectStar\", \"perfect\", \"great\", \"good\", \"bad\", \"miss\", \"totalAchievementRatePercentRecord\", \"bestRecordChallengeCount\", \"bestRecordDate\", \"hasReceivedReward\") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_friend_invitation(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "invitationCode", "hasInputOtherInvitationCode"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"friend_invitation\" (\"userId\", \"id\", \"invitationCode\", \"hasInputOtherInvitationCode\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_friend_invitation_mission(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "friendInvitationMissionMasterId", "friendInvitationMissionStageMasterId", "currentCount", "isCleared", "isRewardReceived"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"friend_invitation_mission\" (\"userId\", \"id\", \"friendInvitationMissionMasterId\", \"friendInvitationMissionStageMasterId\", \"currentCount\", \"isCleared\", \"isRewardReceived\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_name_base_color(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "nameBaseColorMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"name_base_color\" (\"userId\", \"id\", \"nameBaseColorMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_icon_frame(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "iconFrameMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"icon_frame\" (\"userId\", \"id\", \"iconFrameMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_gacha_re_roll(long long user_id, const wire::json& row) {
    static const char* cols[] = {"gachaMasterId", "rollCount", "isDecided"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"gacha_re_roll\" (\"userId\", \"gachaMasterId\", \"rollCount\", \"isDecided\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trial_party_event(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "trialPartyEventMasterId", "currentStageOrder", "isCompleted"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trial_party_event\" (\"userId\", \"id\", \"trialPartyEventMasterId\", \"currentStageOrder\", \"isCompleted\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trial_party_event_stage(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "trialPartyEventStageMasterId", "isCleared"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trial_party_event_stage\" (\"userId\", \"id\", \"trialPartyEventStageMasterId\", \"isCleared\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trial_party_event_stage_party(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "trialPartyEventStageMasterId", "leaderPosition"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trial_party_event_stage_party\" (\"userId\", \"id\", \"trialPartyEventStageMasterId\", \"leaderPosition\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_trial_party_event_stage_party_slot(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "trialPartyEventStagePartyId", "position", "trialPartyCharacterMasterId", "trialPartyPosterMasterId", "trialPartyAccessoryMasterId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"trial_party_event_stage_party_slot\" (\"userId\", \"id\", \"trialPartyEventStagePartyId\", \"position\", \"trialPartyCharacterMasterId\", \"trialPartyPosterMasterId\", \"trialPartyAccessoryMasterId\") VALUES ($1, $2, $3, $4, $5, $6, $7)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_user_block(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "blockUserId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"user_block\" (\"userId\", \"id\", \"blockUserId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_home_skin(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "homeSkinMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"home_skin\" (\"userId\", \"id\", \"homeSkinMasterIds\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_accessory_auto_sell(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "autoSellRarity"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"accessory_auto_sell\" (\"userId\", \"id\", \"autoSellRarity\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_favorite_costume(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "characterBaseMasterId", "favoriteCostumeMasterIds"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"favorite_costume\" (\"userId\", \"id\", \"characterBaseMasterId\", \"favoriteCostumeMasterIds\") VALUES ($1, $2, $3, $4)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_buff_item_status(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "effectType", "buffItemMasterId", "validUntil"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"buff_item_status\" (\"userId\", \"id\", \"effectType\", \"buffItemMasterId\", \"validUntil\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_multi_room_basic(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "ownerMultiRoomId"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"multi_room_basic\" (\"userId\", \"id\", \"ownerMultiRoomId\") VALUES ($1, $2, $3)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

ExecutableQuery upsert_event_camp(long long user_id, const wire::json& row) {
    static const char* cols[] = {"id", "eventMasterId", "campType", "totalSupportPoint"};
    ExecutableQuery q;
    q.sql = "INSERT INTO \"event_camp\" (\"userId\", \"id\", \"eventMasterId\", \"campType\", \"totalSupportPoint\") VALUES ($1, $2, $3, $4, $5)";
    q.args.push_back(json(user_id));
    for (const char* k : cols) q.args.push_back(row_get(row, k));
    return q;
}

}  // namespace user
}  // namespace db
