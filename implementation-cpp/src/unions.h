#pragma once
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

// Union discriminator maps from models/unions.py: entity/notification type name ->
// int union key (the reversed IDATA_OBJECT / INOTIFICATION_OBJECT dicts). Only the
// name->key maps are ported; the union classes are handled by the generic wire layer.
// user_data/game_state use the lookup for present/notification entries and iterate the
// ordered items to build their type registry.

namespace unions {

// IDATA_OBJECT_KEY as ordered (name, key) pairs, in the python dict's insertion order.
inline const std::vector<std::pair<std::string, int>>& idata_object_key_items() {
    static const std::vector<std::pair<std::string, int>> items = {
        {"User", 0},
        {"UserProfile", 1},
        {"UserPreference", 2},
        {"HomeDisplayPreference", 3},
        {"Character", 4},
        {"CharacterBase", 5},
        {"Party", 6},
        {"PartySlot", 7},
        {"CharacterMaster", 8},
        {"CharacterBaseMaster", 9},
        {"CharacterLevelMaster", 10},
        {"Poster", 11},
        {"AccessoryLevelPatternGroupMaster", 12},
        {"AccessoryLevelPatternMaster", 13},
        {"AccessoryMaster", 14},
        {"EpisodeMaster", 15},
        {"EpisodeRewardPackageMaster", 16},
        {"LiveMaster", 17},
        {"MusicMaster", 18},
        {"SenseMaster", 19},
        {"StoryMaster", 20},
        {"PosterLevelPatternGroupMaster", 21},
        {"PosterLevelPatternMaster", 22},
        {"PosterMaster", 23},
        {"Live", 24},
        {"Music", 25},
        {"Accessory", 26},
        {"Item", 27},
        {"AccessoryEffectMaster", 28},
        {"CompanyMaster", 29},
        {"EffectDurationGroupMaster", 30},
        {"EffectMaster", 31},
        {"ItemMaster", 32},
        {"RandomEffectGroupMaster", 34},
        {"RewardRuleMaster", 35},
        {"SenseEffectMaster", 36},
        {"TrophyGroupMaster", 37},
        {"TrophyMaster", 38},
        {"CharacterLesson", 39},
        {"DailyLesson", 40},
        {"Inbox", 41},
        {"Bomb", 42},
        {"Costume", 43},
        {"NameColor", 44},
        {"Nameplate", 45},
        {"Note", 46},
        {"Stamp", 47},
        {"Mission", 48},
        {"AuditionMaster", 51},
        {"BombMaster", 52},
        {"CharacterStarRankMaster", 53},
        {"CharacterStarRankRewardGroupMaster", 54},
        {"CostumeMaster", 55},
        {"HomeCharacterVoiceMaster", 56},
        {"NameColorMaster", 57},
        {"NameplateMaster", 58},
        {"NoteMaster", 59},
        {"SpotConversationMaster", 60},
        {"StampMaster", 61},
        {"CharacterLessonSlot", 62},
        {"Trophy", 63},
        {"Market", 64},
        {"ViewedShop", 65},
        {"GameHint", 66},
        {"UserBonus", 67},
        {"AuditionPhaseMaster", 68},
        {"AuditionRewardPackageMaster", 69},
        {"CampaignMaster", 71},
        {"CharacterAwakeningItemMaster", 72},
        {"CharacterBloomBonusGroupMaster", 73},
        {"CharacterBloomItemMaster", 74},
        {"CharacterExperienceItemMaster", 75},
        {"CharacterMissionMaster", 76},
        {"CharacterMissionStageMaster", 77},
        {"CharacterPieceMaster", 78},
        {"CharacterSenseEnhanceItemGroupMaster", 79},
        {"CostumeWearableCharacterGroupMaster", 80},
        {"ExchangeShopMaster", 82},
        {"LiveSettingMaster", 83},
        {"MissionMaster", 84},
        {"MusicVocalVersionMaster", 85},
        {"PosterReleaseItemGroupMaster", 86},
        {"PosterReleaseItemMaster", 87},
        {"PosterStoryMaster", 88},
        {"StarRankRewardMaster", 89},
        {"AuditionClear", 90},
        {"SpRate", 93},
        {"Notification", 94},
        {"Episode", 95},
        {"CharacterMission", 96},
        {"MissionPass", 97},
        {"MissionPassDetailMaster", 98},
        {"MissionPassMaster", 99},
        {"LeagueBasic", 100},
        {"StoryEvent", 101},
        {"ExchangeLimit", 102},
        {"LeagueGroup", 105},
        {"LeagueGroupMember", 106},
        {"LeagueHistory", 107},
        {"JewelShop", 108},
        {"DailyLimit", 109},
        {"LeagueHighScoreParty", 111},
        {"LeagueHighScorePartySlot", 112},
        {"StoryEventCircle", 113},
        {"StoryEventCircleMission", 114},
        {"StoryEventCircleMissionReward", 115},
        {"StoryEventHighScoreBuffSetting", 116},
        {"StoryEventHighScoreParty", 117},
        {"StoryEventHighScorePartySlot", 118},
        {"ConnectWithAccount", 119},
        {"ConnectWithPassword", 120},
        {"TournamentDetail", 121},
        {"GradualMissionGroup", 122},
        {"Photo", 123},
        {"Album", 124},
        {"AlbumPage", 125},
        {"StarPassStatus", 126},
        {"LoginPassStatus", 127},
        {"Currency", 128},
        {"Decoration", 129},
        {"LiveAchievement", 130},
        {"MusicVideo", 131},
        {"TheaterStory", 132},
        {"LiveDropCelling", 133},
        {"StoryEventHighScore", 134},
        {"Comic", 135},
        {"ComebackCampaign", 136},
        {"ConcertStage", 137},
        {"Limit", 138},
        {"GachaSelectedThing", 139},
        {"TotalPointEvent", 140},
        {"EventBoxGacha", 141},
        {"EventBoxGachaBoxThing", 142},
        {"SpecialEvent", 143},
        {"CharacterPointEvent", 144},
        {"AnotherNotation", 145},
        {"MusicBookmark", 146},
        {"LiveDropLimit", 147},
        {"Restriction", 148},
        {"PermanentMarketThing", 149},
        {"TimeLimitedControl", 150},
        {"FlashSaleStage", 151},
        {"AlbumTheme", 152},
        {"CircleEventMission", 153},
        {"PickupCharacterMission", 154},
        {"LeagueSeasonResult", 155},
        {"Event", 156},
        {"BonusLive", 157},
        {"BonusLiveStage", 158},
        {"RouletteEvent", 159},
        {"Roulette", 160},
        {"HomeBGM", 161},
        {"LinkCharacter", 162},
        {"MusicCourse", 163},
        {"TournamentQualifying", 164},
        {"Lottery", 165},
        {"TripleCastParty", 166},
        {"TripleCastPartySlot", 167},
        {"TripleCastBasic", 168},
        {"TripleCastGroup", 169},
        {"TripleCastGroupMember", 170},
        {"TripleCastHighScoreParty", 171},
        {"TripleCastHighScorePartySlot", 172},
        {"TripleCastSeasonResult", 173},
        {"AlbumPreset", 174},
        {"Gacha", 175},
        {"TripleCastHistory", 176},
        {"DugongRun", 177},
        {"MusicCourseRanking", 178},
        {"FriendInvitation", 179},
        {"FriendInvitationMission", 180},
        {"NameBaseColor", 181},
        {"IconFrame", 182},
        {"GachaReRoll", 183},
        {"TrialPartyEvent", 184},
        {"TrialPartyEventStage", 185},
        {"TrialPartyEventStageParty", 186},
        {"TrialPartyEventStagePartySlot", 187},
        {"UserBlock", 188},
        {"HomeSkin", 189},
        {"AccessoryAutoSell", 190},
        {"FavoriteCostume", 191},
        {"BuffItemStatus", 192},
        {"MultiRoomBasic", 193},
        {"EventCamp", 194},
    };
    return items;
}

// INOTIFICATION_OBJECT_KEY as ordered (name, key) pairs.
inline const std::vector<std::pair<std::string, int>>& inotification_object_key_items() {
    static const std::vector<std::pair<std::string, int>> items = {
        {"MissionCleared", 0},
        {"FriendRequest", 1},
        {"AcceptFriendRequest", 2},
        {"MultiLiveRestriction", 3},
    };
    return items;
}

inline const std::unordered_map<std::string, int>& idata_object_key_map() {
    static const std::unordered_map<std::string, int> m = [] {
        std::unordered_map<std::string, int> t;
        for (const auto& kv : idata_object_key_items()) t.emplace(kv.first, kv.second);
        return t;
    }();
    return m;
}

inline const std::unordered_map<std::string, int>& inotification_object_key_map() {
    static const std::unordered_map<std::string, int> m = [] {
        std::unordered_map<std::string, int> t;
        for (const auto& kv : inotification_object_key_items()) t.emplace(kv.first, kv.second);
        return t;
    }();
    return m;
}

// IDATA_OBJECT_KEY[name]: type name -> union key (throws if absent, like a python KeyError).
inline int idata_object_key(const std::string& name) {
    return idata_object_key_map().at(name);
}

// INOTIFICATION_OBJECT_KEY[name]: notification type name -> union key.
inline int inotification_object_key(const std::string& name) {
    return inotification_object_key_map().at(name);
}

}  // namespace unions
