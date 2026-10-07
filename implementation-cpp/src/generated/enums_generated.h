#pragma once
// Generated from server-of-dreams models/enums.py. Do not edit by hand.
namespace enums {
namespace AccountDeletionErrorTypes {
  constexpr long long None_ = 0LL;
  constexpr long long EnrolledCircle = 1LL;
}
namespace AccountRegisterErrorTypes {
  constexpr long long None_ = 0LL;
  constexpr long long NgWord = 1LL;
  constexpr long long TooManyRegistrations = 2LL;
}
namespace AchievementRateGrades {
  constexpr long long None_ = 0LL;
  constexpr long long C = 1LL;
  constexpr long long B = 2LL;
  constexpr long long A = 3LL;
  constexpr long long APlus = 4LL;
  constexpr long long S = 5LL;
  constexpr long long SPlus = 6LL;
  constexpr long long SS = 7LL;
  constexpr long long SSPlus = 8LL;
  constexpr long long SSS = 9LL;
}
namespace ActivityLogTypes {
  constexpr long long None_ = 0LL;
  constexpr long long JoinCircle = 1LL;
  constexpr long long LeaveCircle = 2LL;
  constexpr long long ChangedPosition = 3LL;
  constexpr long long GachaAcquiredCharacter = 4LL;
  constexpr long long GachaAcquiredPoster = 5LL;
  constexpr long long PlayerRate = 6LL;
  constexpr long long FirstClearedAllPerfect = 7LL;
  constexpr long long FirstClearedFullCombo = 8LL;
  constexpr long long DonateSupportLevelLimit = 9LL;
  constexpr long long ReleaseSupportLevelLimit = 10LL;
}
namespace Attributes {
  constexpr long long Cute = 1LL;
  constexpr long long Cool = 2LL;
  constexpr long long Colorful = 3LL;
  constexpr long long Cheerful = 4LL;
}
namespace AuthenticationProviders {
  constexpr long long Google = 1LL;
  constexpr long long Apple = 2LL;
}
namespace BanLevels {
  constexpr long long Normal = 0LL;
  constexpr long long Warning = 1LL;
  constexpr long long Suspend = 2LL;
  constexpr long long Delete = 3LL;
}
namespace BloomBonusTypes {
  constexpr long long None_ = 0LL;
  constexpr long long EffectDuringLive = 1LL;
  constexpr long long StatusBonus = 2LL;
  constexpr long long GrantItem = 3LL;
}
namespace BonusAbilityEnableFlags {
  constexpr long long None_ = 0LL;
  constexpr long long First = 1LL;
  constexpr long long Second = 2LL;
  constexpr long long Third = 4LL;
  constexpr long long Fourth = 8LL;
  constexpr long long Fifth = 16LL;
  constexpr long long All = 31LL;
}
namespace BranchConditionType {
  constexpr long long None_ = 0LL;
  constexpr long long StarActFireCount = 1LL;
  constexpr long long PartyPosition = 2LL;
  constexpr long long CompanyMemberCount = 3LL;
  constexpr long long CompanyCount = 4LL;
  constexpr long long AttributeMemberCount = 5LL;
  constexpr long long AttributeCount = 6LL;
  constexpr long long PosterAppearanceMemberCount = 7LL;
  constexpr long long LifeGuardCount = 8LL;
  constexpr long long PosterAppearanceCompanyCount = 9LL;
  constexpr long long StorageSenseLightCount = 10LL;
  constexpr long long CharacterBaseGroup = 11LL;
  constexpr long long SenseTriggeredCount = 12LL;
}
namespace BranchConditionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long StarActFireCount = 1LL;
  constexpr long long PartyPosition = 2LL;
  constexpr long long CompanyMemberCount = 3LL;
  constexpr long long CompanyCount = 4LL;
  constexpr long long AttributeMemberCount = 5LL;
  constexpr long long AttributeCount = 6LL;
  constexpr long long PosterAppearanceMemberCount = 7LL;
  constexpr long long LifeGuardCount = 8LL;
  constexpr long long PosterAppearanceCompanyCount = 9LL;
  constexpr long long StorageSenseLightCount = 10LL;
  constexpr long long CharacterBaseGroup = 11LL;
  constexpr long long SenseTriggeredCount = 12LL;
}
namespace BranchJudgeTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Equal = 1LL;
  constexpr long long MoreThan = 2LL;
  constexpr long long LessThan = 3LL;
}
namespace CalculationTypes {
  constexpr long long PercentageAddition = 1LL;
  constexpr long long Multiplication = 2LL;
  constexpr long long FixedAddition = 3LL;
}
namespace CampTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Camp1 = 1LL;
  constexpr long long Camp2 = 2LL;
}
namespace CampaignEffectTypes {
  constexpr long long LiveReward = 1LL;
  constexpr long long LessonReward = 2LL;
  constexpr long long StarPoint = 3LL;
  constexpr long long RankPoint = 4LL;
  constexpr long long AccessoryDropRate = 5LL;
  constexpr long long LessonCount = 6LL;
  constexpr long long ForDisplay = 10LL;
}
namespace CharacterBaseTypes {
  constexpr long long Initial = 1LL;
  constexpr long long Collaboration = 900LL;
}
namespace CharacterEpisodeOrder {
  constexpr long long None_ = 0LL;
  constexpr long long First = 1LL;
  constexpr long long Second = 2LL;
}
namespace CharacterRarities {
  constexpr long long Rare1 = 1LL;
  constexpr long long Rare2 = 2LL;
  constexpr long long Rare3 = 3LL;
  constexpr long long Rare4 = 4LL;
}
namespace CharacterSelectionTypes {
  constexpr long long Primary = 1LL;
  constexpr long long Secondary = 2LL;
  constexpr long long Random = 99LL;
}
namespace CircleAuthorities {
  constexpr long long None_ = 0LL;
  constexpr long long Member = 1LL;
  constexpr long long DeputyLeader = 2LL;
  constexpr long long Leader = 3LL;
}
namespace CircleAuthorityResultStatus {
  constexpr long long PermissionDenied = 1LL;
  constexpr long long DeputyLeaderFullJoin = 2LL;
  constexpr long long ChangeSuccess = 3LL;
  constexpr long long LeaderChangeSuccess = 4LL;
  constexpr long long DataNotFound = 5LL;
}
namespace CircleAuthorityUpdateTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Promotion = 1LL;
  constexpr long long Demotion = 2LL;
}
namespace CircleDonateSupportCompanyResult {
  constexpr long long Success = 0LL;
  constexpr long long ExceededQuantityReturned = 1LL;
  constexpr long long Error = 99LL;
}
namespace CircleResultStatus {
  constexpr long long RequestIllegal = 1LL;
  constexpr long long LessThanNecessaryRank = 2LL;
  constexpr long long CanJoinCircle = 3LL;
  constexpr long long AlreadyJoinAnyCircle = 4LL;
  constexpr long long AlreadyInvite = 5LL;
  constexpr long long FreeEntry = 6LL;
  constexpr long long AlreadyRequest = 7LL;
  constexpr long long InviteSuccess = 8LL;
  constexpr long long PreRequestSuccess = 9LL;
  constexpr long long RequestSuccess = 10LL;
  constexpr long long ApproveSuccess = 11LL;
  constexpr long long JoinSuccess = 12LL;
  constexpr long long EditSuccess = 13LL;
  constexpr long long CreateSuccess = 14LL;
  constexpr long long ExpulsionSuccess = 15LL;
  constexpr long long ResignationSuccess = 16LL;
  constexpr long long ReleaseSuccess = 17LL;
  constexpr long long Error = 18LL;
  constexpr long long DataNotFound = 19LL;
  constexpr long long SearchSuccess = 20LL;
  constexpr long long CancelSuccess = 21LL;
  constexpr long long RejectSuccess = 22LL;
  constexpr long long NgWord = 23LL;
  constexpr long long MemberAmountUpperLimit = 24LL;
  constexpr long long InvalidParameter = 25LL;
  constexpr long long ReleaseFailureByEvent = 26LL;
}
namespace ClearLamps {
  constexpr long long None_ = 0LL;
  constexpr long long Clear = 1LL;
  constexpr long long FullCombo = 2LL;
  constexpr long long FullComboMulti1 = 3LL;
  constexpr long long FullComboMulti2 = 4LL;
  constexpr long long FullComboMulti3 = 5LL;
  constexpr long long AllPerfect = 6LL;
  constexpr long long AllPerfectMulti1 = 7LL;
  constexpr long long AllPerfectMulti2 = 8LL;
  constexpr long long AllPerfectMulti3 = 9LL;
}
namespace Companies {
  constexpr long long None_ = 0LL;
  constexpr long long Sirius = 1LL;
  constexpr long long Eden = 2LL;
  constexpr long long Gingaza = 3LL;
  constexpr long long Denki = 4LL;
  constexpr long long LoveLiveSunshine = 900LL;
}
namespace DugongRunClearTypes {
  constexpr long long Failed = 0LL;
  constexpr long long Clear = 1LL;
  constexpr long long NoMisstake = 2LL;
}
namespace EditTypes {
  constexpr long long SimpleEdit = 1LL;
  constexpr long long DetailEdit = 2LL;
}
namespace EffectConditions {
  constexpr long long CharacterBase = 1LL;
  constexpr long long Company = 2LL;
  constexpr long long Attribute = 3LL;
  constexpr long long SenseType = 4LL;
  constexpr long long Character = 5LL;
  constexpr long long EquippedPoster = 6LL;
  constexpr long long NeighborPosition = 7LL;
  constexpr long long CharacterBaseGroup = 8LL;
}
namespace EffectSourceTypes {
  constexpr long long Album = 1LL;
  constexpr long long Poster = 2LL;
  constexpr long long Accessory = 3LL;
  constexpr long long BloomBonus = 4LL;
  constexpr long long Other = 5LL;
  constexpr long long LeaderSense = 6LL;
}
namespace EffectTargetRanges {
  constexpr long long None_ = 0LL;
  constexpr long long Self = 1LL;
  constexpr long long All = 2LL;
}
namespace EffectTypes {
  constexpr long long BaseVocalUp = 1LL;
  constexpr long long BaseExpressionUp = 2LL;
  constexpr long long BaseConcentrationUp = 3LL;
  constexpr long long BaseCorrection = 4LL;
  constexpr long long VocalUp = 5LL;
  constexpr long long ExpressionUp = 6LL;
  constexpr long long ConcentrationUp = 7LL;
  constexpr long long VocalLimitUp = 8LL;
  constexpr long long ExpressionLimitUp = 9LL;
  constexpr long long ConcentrationLimitUp = 10LL;
  constexpr long long PerformanceUp = 11LL;
  constexpr long long FinalPerformanceUpCancelSense = 12LL;
  constexpr long long AddSenseLightSelf = 13LL;
  constexpr long long AddSenseLightVariable = 14LL;
  constexpr long long AddSenseLightSupport = 15LL;
  constexpr long long AddSenseLightControl = 16LL;
  constexpr long long AddSenseLightAmplification = 17LL;
  constexpr long long AddSenseLightSpecial = 18LL;
  constexpr long long ChangeWrongLightToSpLight = 19LL;
  constexpr long long SenseRecastDown = 20LL;
  constexpr long long SenseCoolTimeRecastDown = 21LL;
  constexpr long long SenseAlternative = 22LL;
  constexpr long long ScoreUpByHighLife = 23LL;
  constexpr long long ScoreUpByLowLife = 24LL;
  constexpr long long ScoreUpByBuff = 25LL;
  constexpr long long BuffTimeExtend = 26LL;
  constexpr long long LifeHealing = 27LL;
  constexpr long long LifeFixedValue = 28LL;
  constexpr long long LifeGuard = 29LL;
  constexpr long long SenseScoreUp = 30LL;
  constexpr long long StarActScoreUp = 31LL;
  constexpr long long BaseScoreUp = 32LL;
  constexpr long long ScoreGainOnScore = 33LL;
  constexpr long long ScoreGainOnVocal = 34LL;
  constexpr long long ScoreGainOnExpression = 35LL;
  constexpr long long ScoreGainOnConcentration = 36LL;
  constexpr long long PrincipalGaugeGain = 37LL;
  constexpr long long RewardUp = 38LL;
  constexpr long long LightGuard = 39LL;
  constexpr long long PrincipalGaugeUp = 40LL;
  constexpr long long PrincipalGaugeLimitUp = 41LL;
  constexpr long long DecreaseRequireSupportLight = 42LL;
  constexpr long long DecreaseRequireControlLight = 43LL;
  constexpr long long DecreaseRequireAmplificationLight = 44LL;
  constexpr long long DecreaseRequireSpecialLight = 45LL;
  constexpr long long PerformanceLimitUp = 46LL;
  constexpr long long ScoreGainOnPerformance = 47LL;
  constexpr long long PrincipalGaugeBonus = 48LL;
  constexpr long long PerformanceDuplicateUp = 49LL;
  constexpr long long CombinationSense = 50LL;
  constexpr long long PlayerRankPointUp = 51LL;
  constexpr long long PrincipalGaugeGainPercentageOfLimit = 52LL;
  constexpr long long StarActProcrastinate = 53LL;
}
namespace EntryTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Free = 1LL;
  constexpr long long Approval = 2LL;
}
namespace EpisodeReleaseConditionTypes {
  constexpr long long TotalRankInCompany = 1LL;
  constexpr long long CharacterRank = 2LL;
}
namespace FireTimingTypes {
  constexpr long long StarAct = 1LL;
  constexpr long long Sense = 2LL;
  constexpr long long StartLive = 3LL;
  constexpr long long Passive = 4LL;
}
namespace FriendAcceptResultStatus {
  constexpr long long AcceptSuccess = 1LL;
  constexpr long long RequestUserFriendsLimitOver = 2LL;
  constexpr long long AcceptUserFriendsLimitOver = 3LL;
  constexpr long long Canceled = 4LL;
}
namespace FriendRequestResultStatus {
  constexpr long long RequestSuccess = 1LL;
  constexpr long long IsFriends = 2LL;
  constexpr long long IsApplying = 3LL;
  constexpr long long IsMySelf = 4LL;
  constexpr long long DataNotFound = 5LL;
  constexpr long long IsFriendCountLimit = 6LL;
}
namespace FriendSearchResultStatus {
  constexpr long long Friend = 1LL;
  constexpr long long Request = 2LL;
  constexpr long long None_ = 3LL;
  constexpr long long User = 4LL;
  constexpr long long ReceivedRequest = 5LL;
}
namespace GachaCardTypes {
  constexpr long long Character = 1LL;
  constexpr long long Poster = 2LL;
}
namespace GachaEmissionFlags {
  constexpr long long Rare2 = 1LL;
  constexpr long long Rare3 = 2LL;
  constexpr long long Rare4 = 4LL;
}
namespace GameVersions {
  constexpr long long Unknown = 0LL;
  constexpr long long AppStore = 1LL;
  constexpr long long GooglePlay = 2LL;
}
namespace HighScoreTypes {
  constexpr long long Normal = 0LL;
  constexpr long long Multi = 1LL;
  constexpr long long TheaterLeague = 2LL;
  constexpr long long Audition = 3LL;
}
namespace HomeBGMSelectionTypes {
  constexpr long long UserSelect = 1LL;
  constexpr long long Random = 2LL;
}
namespace HomeCharacterDisplayTypes {
  constexpr long long CharacterModel = 0LL;
  constexpr long long Illust = 1LL;
}
namespace InvitationCodeResultStatuses {
  constexpr long long Success = 0LL;
  constexpr long long InvalidInvitationCode = 1LL;
  constexpr long long OwnInvitationCode = 2LL;
  constexpr long long InviteUserNotFound = 3LL;
}
namespace ItemCategories {
  constexpr long long StaminaRecovery = 11LL;
  constexpr long long CharacterLevel = 12LL;
  constexpr long long ConsumeItem = 13LL;
  constexpr long long TalentBloom = 14LL;
  constexpr long long CharacterMission = 16LL;
  constexpr long long GachaTicket = 21LL;
  constexpr long long ExchangeTicket = 22LL;
  constexpr long long RouletteBall = 31LL;
  constexpr long long ScratchCoin = 32LL;
  constexpr long long GachaPoint = 41LL;
  constexpr long long EventPoint = 42LL;
  constexpr long long MissionPassPoint = 43LL;
  constexpr long long Film = 51LL;
  constexpr long long AlbumSkin = 52LL;
  constexpr long long PlayerRankAwakening = 61LL;
  constexpr long long LessonPartyQuantityLimitRelease = 62LL;
  constexpr long long BuffItem = 71LL;
}
namespace JumpTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Information = 1LL;
  constexpr long long Shop = 2LL;
  constexpr long long JewelShop = 3LL;
  constexpr long long Gacha = 4LL;
  constexpr long long Event = 5LL;
  constexpr long long Live = 6LL;
  constexpr long long Member = 7LL;
  constexpr long long Photo = 8LL;
  constexpr long long Story = 9LL;
  constexpr long long WebLink = 10LL;
  constexpr long long BeginnerMission = 11LL;
  constexpr long long MusicShop = 12LL;
  constexpr long long Spot = 13LL;
  constexpr long long Mission = 14LL;
  constexpr long long Concert = 15LL;
  constexpr long long StarRank = 16LL;
  constexpr long long Episode = 17LL;
  constexpr long long SpecialEvent = 18LL;
  constexpr long long TotalPointEventRanking = 19LL;
  constexpr long long BoxGacha = 20LL;
  constexpr long long TournamentEventTop = 21LL;
  constexpr long long Lesson = 22LL;
  constexpr long long League = 23LL;
}
namespace LeagueClassChangeTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Up = 1LL;
  constexpr long long Keep = 2LL;
  constexpr long long Down = 3LL;
}
namespace LeagueClassTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Rookie = 1LL;
  constexpr long long Hope = 2LL;
  constexpr long long Cast = 3LL;
  constexpr long long Elite = 4LL;
  constexpr long long Veteran = 5LL;
  constexpr long long Star = 6LL;
  constexpr long long DaiStar = 7LL;
}
namespace LiveDropTypes {
  constexpr long long Normal = 0LL;
  constexpr long long Special = 1LL;
}
namespace LiveReleaseStatus {
  constexpr long long None_ = 0LL;
  constexpr long long AvailableInShop = 1LL;
  constexpr long long Playable = 2LL;
}
namespace LiveTypes {
  constexpr long long Normal = 1LL;
  constexpr long long Multi = 2LL;
  constexpr long long Lesson = 3LL;
  constexpr long long Audition = 4LL;
  constexpr long long League = 5LL;
  constexpr long long MultiCollectionEvent = 6LL;
  constexpr long long Concert = 7LL;
  constexpr long long BonusLive = 8LL;
  constexpr long long CourseMode = 9LL;
  constexpr long long TripleCast = 10LL;
  constexpr long long GhostLive = 11LL;
  constexpr long long Trial = 12LL;
  constexpr long long MultiRoom = 13LL;
}
namespace LiveUnlockConditionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long ExtraGoodCount = 13LL;
  constexpr long long NotationShop = 14LL;
}
namespace LoginBonusLayoutTypes {
  constexpr long long Normal = 1LL;
  constexpr long long Special = 2LL;
}
namespace LoginBonusSpineSelectType {
  constexpr long long Random = 1LL;
  constexpr long long HomeCharacterBaseId = 2LL;
  constexpr long long SelectedSpineCostume = 3LL;
}
namespace LoginBonusTypes {
  constexpr long long Normal = 1LL;
  constexpr long long Special = 2LL;
  constexpr long long Comeback = 3LL;
}
namespace LoginPassNotificationTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Purchasable = 1LL;
  constexpr long long Invalided = 2LL;
}
namespace MatchingResult {
  constexpr long long Win = 0LL;
  constexpr long long Lose = 1LL;
  constexpr long long Draw = 2LL;
}
namespace MissionCategories {
  constexpr long long Beginner = 1LL;
  constexpr long long Normal = 2LL;
  constexpr long long Limit = 3LL;
  constexpr long long Daily = 4LL;
  constexpr long long Weekly = 5LL;
  constexpr long long SiriusDay = 6LL;
  constexpr long long TheaterSirius = 11LL;
  constexpr long long TheaterEden = 12LL;
  constexpr long long TheaterGingaza = 13LL;
  constexpr long long TheaterDenki = 14LL;
}
namespace MissionPassRewardStatus {
  constexpr long long NotReceived = 1LL;
  constexpr long long Received = 2LL;
  constexpr long long NotPaidSpItem = 3LL;
  constexpr long long NotReachedPoint = 4LL;
  constexpr long long ReceiveSuccess = 5LL;
}
namespace MultiRoomPlayModes {
  constexpr long long AchievementRate = 0LL;
  constexpr long long Score = 1LL;
}
namespace MusicBookmarkFlags {
  constexpr long long None_ = 0LL;
  constexpr long long Bookmark1 = 1LL;
  constexpr long long Bookmark2 = 2LL;
  constexpr long long Bookmark3 = 4LL;
}
namespace MusicCourseCertificationGrade {
  constexpr long long None_ = 0LL;
  constexpr long long Failed = 1LL;
  constexpr long long NormalGaugeCertificate = 2LL;
  constexpr long long HotGaugeCertificate = 3LL;
}
namespace MusicCourseGaugeType {
  constexpr long long Normal = 0LL;
  constexpr long long Hot = 1LL;
}
namespace MusicCoverTypes {
  constexpr long long Original = 1LL;
  constexpr long long Cover = 2LL;
}
namespace MusicDifficulties {
  constexpr long long None_ = 0LL;
  constexpr long long Normal = 1LL;
  constexpr long long Hard = 2LL;
  constexpr long long Extra = 3LL;
  constexpr long long Stella = 4LL;
  constexpr long long Olivier = 5LL;
}
namespace MusicUnlockConditionTypes {
  constexpr long long Default = 1LL;
  constexpr long long Distribute = 2LL;
  constexpr long long Buy = 10LL;
  constexpr long long ReadEpisodeAndBuy = 11LL;
  constexpr long long ClearAudition = 12LL;
}
namespace MusicVideoTypes {
  constexpr long long None_ = 0LL;
  constexpr long long RealTimeRendering = 1LL;
  constexpr long long Movie = 2LL;
}
namespace NamePlateChangeTypes {
  constexpr long long None_ = 0LL;
  constexpr long long DaiStarEnrollCount = 1LL;
  constexpr long long TripleCastDaiStarEnrollCount = 2LL;
}
namespace NotificationCategory {
  constexpr long long Notification = 1LL;
  constexpr long long Update = 2LL;
  constexpr long long Campaign = 3LL;
  constexpr long long Event = 4LL;
  constexpr long long Gacha = 5LL;
  constexpr long long Bug = 6LL;
}
namespace NotificationTabCategory {
  constexpr long long Important = 1LL;
  constexpr long long UpdateInformation = 2LL;
  constexpr long long BugInformation = 3LL;
}
namespace OlivierReleaseStatuses {
  constexpr long long None_ = 0LL;
  constexpr long long Challengeable = 1LL;
  constexpr long long Purchasable = 2LL;
  constexpr long long Released = 3LL;
}
namespace PageCategories {
  constexpr long long TutorialIngame = 1LL;
  constexpr long long Spot = 2LL;
  constexpr long long Photo = 3LL;
  constexpr long long Circle = 4LL;
  constexpr long long Lesson = 5LL;
  constexpr long long AuditionTop = 6LL;
  constexpr long long AuditionConfirm = 7LL;
  constexpr long long League = 8LL;
  constexpr long long SpecialLive = 9LL;
  constexpr long long MultiLive = 10LL;
  constexpr long long LiveResult = 11LL;
  constexpr long long OlivierLiveResult = 12LL;
  constexpr long long Party = 13LL;
  constexpr long long Enhancement = 14LL;
  constexpr long long Costume = 15LL;
  constexpr long long StarRank = 16LL;
  constexpr long long SideStory = 17LL;
  constexpr long long TrophyAndProfile = 18LL;
  constexpr long long Shop = 19LL;
  constexpr long long MedalExchangeShop = 20LL;
  constexpr long long BeginnerMission = 21LL;
  constexpr long long StaminaConsumption = 22LL;
  constexpr long long CompanyIntroMovieSirius = 24LL;
  constexpr long long CompanyIntroMovieEden = 25LL;
  constexpr long long CompanyIntroMovieGingaza = 26LL;
  constexpr long long CompanyIntroMovieDenki = 27LL;
  constexpr long long SpotStoryArchive = 28LL;
  constexpr long long PhotoTop = 29LL;
  constexpr long long TeamChallenge = 30LL;
  constexpr long long ActorPortal = 31LL;
  constexpr long long MusicSelection = 32LL;
  constexpr long long ConcertStageSelect = 33LL;
  constexpr long long AlbumTop = 34LL;
  constexpr long long CircleEvent = 35LL;
  constexpr long long TournamentEventTop = 36LL;
  constexpr long long CourseModeSelection = 37LL;
  constexpr long long TripleCastLeague = 38LL;
  constexpr long long TournamentCourseModeSelection = 39LL;
  constexpr long long GhostLive = 40LL;
  constexpr long long TrialPartyEvent = 41LL;
  constexpr long long StellaConcours = 42LL;
  constexpr long long MultiRoom = 43LL;
}
namespace PhotoRarities {
  constexpr long long Rare1 = 1LL;
  constexpr long long Rare2 = 2LL;
  constexpr long long Rare3 = 3LL;
  constexpr long long Rare4 = 4LL;
  constexpr long long Rare5 = 5LL;
}
namespace PlayTimeTypes {
  constexpr long long None_ = -1LL;
  constexpr long long Zero = 0LL;
  constexpr long long One = 1LL;
  constexpr long long Two = 2LL;
  constexpr long long Three = 3LL;
  constexpr long long Four = 4LL;
  constexpr long long Five = 5LL;
  constexpr long long Six = 6LL;
  constexpr long long Seven = 7LL;
  constexpr long long Eight = 8LL;
  constexpr long long Nine = 9LL;
  constexpr long long Ten = 10LL;
  constexpr long long Eleven = 11LL;
  constexpr long long Twelve = 12LL;
  constexpr long long Thirteen = 13LL;
  constexpr long long Fourteen = 14LL;
  constexpr long long Fifteen = 15LL;
  constexpr long long Sixteen = 16LL;
  constexpr long long Seventeen = 17LL;
  constexpr long long Eighteen = 18LL;
  constexpr long long Nineteen = 19LL;
  constexpr long long Twenty = 20LL;
  constexpr long long TwentyOne = 21LL;
  constexpr long long TwentyTwo = 22LL;
  constexpr long long TwentyThree = 23LL;
  constexpr long long TwentyFour = 24LL;
  constexpr long long TwentyFive = 25LL;
  constexpr long long TwentySix = 26LL;
  constexpr long long TwentySeven = 27LL;
  constexpr long long TwentyEight = 28LL;
  constexpr long long TwentyNine = 29LL;
}
namespace PossessionRarities {
  constexpr long long R = 1LL;
  constexpr long long SR = 2LL;
  constexpr long long SSR = 3LL;
}
namespace PossessionRarityFlag {
  constexpr long long None_ = 0LL;
  constexpr long long R = 2LL;
  constexpr long long SR = 4LL;
  constexpr long long SSR = 8LL;
}
namespace PosterEpisodeTypes {
  constexpr long long Information = 0LL;
  constexpr long long Chapter1 = 1LL;
  constexpr long long Chapter2 = 2LL;
  constexpr long long Chapter3 = 3LL;
  constexpr long long Chapter4 = 4LL;
  constexpr long long AfterTalk = 5LL;
  constexpr long long Sirius = 6LL;
  constexpr long long Eden = 7LL;
  constexpr long long Gingaza = 8LL;
  constexpr long long Denki = 9LL;
}
namespace PosterOrientation {
  constexpr long long Portrait = 1LL;
  constexpr long long Landscape = 2LL;
}
namespace PosterSubTitleDisplayConditions {
  constexpr long long None_ = 0LL;
  constexpr long long BreakThroughPhase = 1LL;
}
namespace ProcessPaymentTransactionResult {
  constexpr long long Success = 1LL;
  constexpr long long TemporaryIssuesTryAgain = 2LL;
  constexpr long long Failed = 3LL;
  constexpr long long CouldNotConfirm = 4LL;
  constexpr long long CouldNotAcknowledge = 5LL;
  constexpr long long Pending = 6LL;
}
namespace SearchCircleMemberConditionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long OneToFive = 1LL;
  constexpr long long SixToNine = 2LL;
}
namespace SenseFirePriority {
  constexpr long long Primary = 1LL;
  constexpr long long Secondary = 2LL;
}
namespace SenseLightTypes {
  constexpr long long Variable = 0LL;
  constexpr long long Support = 1LL;
  constexpr long long Control = 2LL;
  constexpr long long Amplification = 3LL;
  constexpr long long Special = 4LL;
}
namespace SenseTypes {
  constexpr long long Support = 1LL;
  constexpr long long Control = 2LL;
  constexpr long long Amplification = 3LL;
  constexpr long long Special = 4LL;
  constexpr long long None_ = 9LL;
  constexpr long long Alternative = 10LL;
}
namespace ShopCategories {
  constexpr long long None_ = 0LL;
  constexpr long long Normal = 1LL;
  constexpr long long Medal = 2LL;
  constexpr long long Event = 3LL;
  constexpr long long Gacha = 4LL;
  constexpr long long Special = 5LL;
  constexpr long long Music = 6LL;
}
namespace ShopReplaceTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Daily = 1LL;
  constexpr long long Weekly = 2LL;
  constexpr long long Monthly = 3LL;
  constexpr long long DailyPassExpired = 10LL;
}
namespace ShopUnlockTypes {
  constexpr long long ReadStory = 2LL;
  constexpr long long RateReached = 8LL;
  constexpr long long ClassReached = 9LL;
  constexpr long long BuyItem = 10LL;
  constexpr long long ReadFirstMainStory = 11LL;
  constexpr long long AuditionReached = 12LL;
  constexpr long long SelectedPickup = 17LL;
  constexpr long long NotInPossession = 20LL;
}
namespace SpotTypes {
  constexpr long long UtagawaHighSchool = 1LL;
  constexpr long long HigashiUenoHighSchool = 2LL;
  constexpr long long Park = 3LL;
  constexpr long long Cafe = 4LL;
  constexpr long long ElectricTown = 5LL;
  constexpr long long ThemePark = 6LL;
}
namespace StampType {
  constexpr long long Default = 1LL;
  constexpr long long Animation = 2LL;
}
namespace StarPassTypes {
  constexpr long long StarPass = 1LL;
  constexpr long long DaiStarPass = 2LL;
}
namespace StoryTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Main = 1LL;
  constexpr long long Event = 2LL;
  constexpr long long Side = 3LL;
  constexpr long long Character = 4LL;
  constexpr long long Special = 5LL;
}
namespace TabCategories {
  constexpr long long Hidden = 0LL;
  constexpr long long Normal = 1LL;
  constexpr long long Raise = 2LL;
  constexpr long long Ticket = 3LL;
  constexpr long long Piece = 4LL;
}
namespace TalentBloomItemTypes {
  constexpr long long ActorPiece = 1LL;
  constexpr long long ActorDaiPiece = 2LL;
}
namespace ThingTypes {
  constexpr long long Item = 1LL;
  constexpr long long Character = 2LL;
  constexpr long long Poster = 3LL;
  constexpr long long Accessory = 4LL;
  constexpr long long Costume = 5LL;
  constexpr long long Trophy = 6LL;
  constexpr long long Stamp = 7LL;
  constexpr long long Nameplate = 8LL;
  constexpr long long NameColor = 9LL;
  constexpr long long Bomb = 10LL;
  constexpr long long Note = 11LL;
  constexpr long long Coin = 12LL;
  constexpr long long Jewel = 13LL;
  constexpr long long Music = 14LL;
  constexpr long long Stamina = 15LL;
  constexpr long long Decoration = 16LL;
  constexpr long long AlbumTheme = 17LL;
  constexpr long long NameBaseColor = 18LL;
  constexpr long long IconFrame = 19LL;
  constexpr long long HomeSkin = 20LL;
}
namespace TimingTypes {
  constexpr long long None_ = 0LL;
  constexpr long long MISS = 1LL;
  constexpr long long BAD = 2LL;
  constexpr long long GOOD = 3LL;
  constexpr long long GREAT = 4LL;
  constexpr long long PERFECT = 5LL;
  constexpr long long PERFECT_STAR = 6LL;
}
namespace TriggerType {
  constexpr long long OverLife = 1LL;
  constexpr long long BelowLife = 2LL;
  constexpr long long CharacterBase = 3LL;
  constexpr long long Company = 4LL;
  constexpr long long Attribute = 5LL;
  constexpr long long SenseType = 6LL;
  constexpr long long CompanyCount = 7LL;
  constexpr long long AttributeCount = 8LL;
  constexpr long long CharacterBaseGroup = 9LL;
  constexpr long long AllMemberBelongingCompany = 10LL;
  constexpr long long MaxMemberBelongingCompanyCount = 11LL;
  constexpr long long MaxMemberBelongingAttributeCount = 12LL;
}
namespace TriggerTypes {
  constexpr long long OverLife = 1LL;
  constexpr long long BelowLife = 2LL;
  constexpr long long CharacterBase = 3LL;
  constexpr long long Company = 4LL;
  constexpr long long Attribute = 5LL;
  constexpr long long SenseType = 6LL;
  constexpr long long CompanyCount = 7LL;
  constexpr long long AttributeCount = 8LL;
  constexpr long long CharacterBaseGroup = 9LL;
  constexpr long long AllMemberBelongingCompany = 10LL;
  constexpr long long MaxMemberBelongingCompanyCount = 11LL;
  constexpr long long MaxMemberBelongingAttributeCount = 12LL;
}
namespace TripleCastGroupOrder {
  constexpr long long First = 0LL;
  constexpr long long Second = 1LL;
  constexpr long long Third = 2LL;
}
namespace TrophyCategories {
  constexpr long long Character = 1LL;
  constexpr long long Achievement = 2LL;
  constexpr long long Event = 3LL;
  constexpr long long Other = 4LL;
}
namespace TutorialStatus {
  constexpr long long Start = 0LL;
  constexpr long long TutorialDownLoad = 1LL;
  constexpr long long MainScenario = 2LL;
  constexpr long long TheaterMovie = 3LL;
  constexpr long long Home = 4LL;
  constexpr long long InGame = 5LL;
  constexpr long long MiniTalk = 6LL;
  constexpr long long Finish = 99LL;
}
namespace UseDecoPageFlag {
  constexpr long long None_ = 0LL;
  constexpr long long Page1 = 1LL;
  constexpr long long Page2 = 2LL;
  constexpr long long Page3 = 4LL;
  constexpr long long Page4 = 8LL;
  constexpr long long Page5 = 16LL;
  constexpr long long Page6 = 32LL;
  constexpr long long Page7 = 64LL;
  constexpr long long Page8 = 128LL;
  constexpr long long Page9 = 256LL;
  constexpr long long Page10 = 512LL;
}
namespace ViewedShopCategoryTypes {
  constexpr long long ExchangeShop = 0LL;
  constexpr long long Music = 1LL;
  constexpr long long Live = 2LL;
  constexpr long long Gacha = 3LL;
  constexpr long long Market = 4LL;
}
namespace AnotherNotationTypes {
  constexpr long long Shelved = 1LL;
  constexpr long long Defect = 2LL;
  constexpr long long Change = 3LL;
  constexpr long long Mysterious = 4LL;
  constexpr long long Madness = 5LL;
  constexpr long long Reproduction = 6LL;
  constexpr long long Another = 7LL;
  constexpr long long Easy = 8LL;
}
namespace AsideLiveButtonTypes {
  constexpr long long None_ = 0LL;
  constexpr long long TinyDreamFest = 1LL;
  constexpr long long ConcertStageSelect = 2LL;
  constexpr long long AnotherNotation = 3LL;
  constexpr long long Trial = 4LL;
}
namespace BannerDeleteConditionTypes {
  constexpr long long BuyItem = 1LL;
  constexpr long long MissionClear = 2LL;
  constexpr long long TimeLimitedControl = 3LL;
}
namespace BonusLiveUnlockConditionTypes {
  constexpr long long RouletteEventPoint = 1LL;
}
namespace CharacterMissionCategoryTypes {
  constexpr long long Live = 1LL;
  constexpr long long Enhancement = 2LL;
  constexpr long long TalentBloom = 3LL;
  constexpr long long Poster = 4LL;
  constexpr long long Other = 5LL;
}
namespace CharacterProfileRestrictionItem {
  constexpr long long Birthday = 1LL;
  constexpr long long Height = 2LL;
  constexpr long long School = 3LL;
  constexpr long long Grade = 4LL;
  constexpr long long SenseName = 5LL;
  constexpr long long SenseEffect = 6LL;
  constexpr long long EvoSenseName = 7LL;
  constexpr long long EvoSenseEffect = 8LL;
  constexpr long long Hobby = 9LL;
  constexpr long long Background = 10LL;
  constexpr long long CharacterVoice = 11LL;
  constexpr long long Description = 12LL;
}
namespace CircleEventMissionTypes {
  constexpr long long Individual = 1LL;
  constexpr long long IndividualAndCircle = 2LL;
  constexpr long long Daily = 3LL;
}
namespace ConditionUsableTableFilter {
  constexpr long long DisplayRestriction = 1LL;
  constexpr long long Costume = 2LL;
  constexpr long long JewelShop = 4LL;
  constexpr long long ExchangeItem = 8LL;
  constexpr long long MusicShop = 16LL;
  constexpr long long Music = 32LL;
}
namespace CustomLayoutActionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long JumpType = 1LL;
  constexpr long long Hint = 2LL;
  constexpr long long Toast = 3LL;
}
namespace CustomLayoutUITypes {
  constexpr long long None_ = 0LL;
  constexpr long long Image = 1LL;
  constexpr long long ImageButton = 2LL;
  constexpr long long GrayOutImageButton = 3LL;
  constexpr long long Button = 10LL;
  constexpr long long RedButton = 11LL;
  constexpr long long GrayOutButton = 12LL;
  constexpr long long GrayOutRedButton = 13LL;
  constexpr long long SpecialEventLogoWithDate = 20LL;
  constexpr long long ItemCountPanel = 21LL;
  constexpr long long Bgm = 90LL;
  constexpr long long TotalPointEvent_ScorePanel = 101LL;
  constexpr long long TotalPointEvent_RankingButton = 102LL;
}
namespace DecorationCategories {
  constexpr long long Special = 3LL;
  constexpr long long Seal = 20LL;
  constexpr long long Sticker = 30LL;
  constexpr long long SpeachBubble = 40LL;
  constexpr long long Effect = 50LL;
  constexpr long long Frame = 60LL;
  constexpr long long Comedy = 70LL;
  constexpr long long Text = 90LL;
  constexpr long long Other = 900LL;
}
namespace DugongRunDifficultyTypes {
  constexpr long long Easy = 1LL;
  constexpr long long Normal = 2LL;
  constexpr long long Hard = 3LL;
}
namespace EventStoryListFlags {
  constexpr long long Sirius = 1LL;
  constexpr long long Eden = 2LL;
  constexpr long long Gingaza = 4LL;
  constexpr long long Denki = 8LL;
  constexpr long long LoveLiveSunshine = 512LL;
  constexpr long long YuruCamp = 1024LL;
  constexpr long long GirlsUndPanzer = 2048LL;
}
namespace EventTypes {
  constexpr long long StoryEvent = 20LL;
  constexpr long long SpecialEvent = 21LL;
  constexpr long long CharacterBasePoint = 22LL;
  constexpr long long AnotherNotation = 23LL;
  constexpr long long CircleEvent = 24LL;
  constexpr long long RouletteEvent = 25LL;
  constexpr long long CampEvent = 26LL;
  constexpr long long Concert = 30LL;
  constexpr long long CourseMode = 31LL;
  constexpr long long DugongRun = 32LL;
  constexpr long long Trial = 33LL;
  constexpr long long Concours = 34LL;
}
namespace FrameLotConditionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long ConsumeStamina = 1LL;
  constexpr long long ScoreWithConsumeStamina = 2LL;
  constexpr long long StarActCountWithConsumeStamina = 3LL;
  constexpr long long AchievementRateWithConsumeStamina = 4LL;
}
namespace GachaButtonTypes {
  constexpr long long PaidJewelTenSeries = 1LL;
  constexpr long long FreeJewelTenSeries = 2LL;
  constexpr long long PaidJewelSingle = 3LL;
  constexpr long long FreeJewelSingle = 4LL;
  constexpr long long SingleTicket = 5LL;
  constexpr long long SingleTicketTenSeries = 6LL;
  constexpr long long TenSeriesTicket = 7LL;
}
namespace GachaDisplayFooterConditions {
  constexpr long long None_ = 0LL;
  constexpr long long RemainRollLeft = 1LL;
}
namespace GachaGroupTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Pickup = 1LL;
  constexpr long long Actor = 2LL;
  constexpr long long Poster = 3LL;
}
namespace GachaTypes {
  constexpr long long Pickup = 1LL;
  constexpr long long SpecialTicket = 2LL;
  constexpr long long Rare4Fixed = 3LL;
  constexpr long long NotPossessedRare4Fixed = 4LL;
  constexpr long long Selection = 5LL;
  constexpr long long Stepup = 6LL;
  constexpr long long PickupSelection = 7LL;
  constexpr long long Stationary = 8LL;
  constexpr long long ReRoll = 9LL;
}
namespace GachaUnlockTypes {
  constexpr long long None_ = 0LL;
  constexpr long long TimeLimitedControl = 1LL;
  constexpr long long GachaComplete = 2LL;
}
namespace HomeBGMPriorityTypes {
  constexpr long long Low = -1LL;
  constexpr long long Normal = 0LL;
  constexpr long long High = 1LL;
}
namespace JewelShopUnlockTypes {
  constexpr long long ReadStory = 2LL;
  constexpr long long RateReached = 8LL;
  constexpr long long ClassReached = 9LL;
  constexpr long long BuyJewelShopItem = 15LL;
  constexpr long long PlayerRank = 16LL;
  constexpr long long TimeLimitedControl = 18LL;
  constexpr long long FlashSale = 19LL;
}
namespace LiveScheduleEndDateDisplayType {
  constexpr long long None_ = 0LL;
  constexpr long long EndDate = 1LL;
  constexpr long long AdditionalEndDate = 2LL;
}
namespace MarketDisplayTypes {
  constexpr long long Default = 0LL;
  constexpr long long Recommend = 1LL;
  constexpr long long Rare = 2LL;
}
namespace MusicCourseType {
  constexpr long long DanClass = 1LL;
  constexpr long long AdvancedDanClass = 2LL;
  constexpr long long TournamentQualifying = 3LL;
  constexpr long long Special = 4LL;
  constexpr long long WeeklyCourseRanking = 5LL;
}
namespace MusicCourseUnlockConditionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long ClearedMMusicCourseId = 1LL;
  constexpr long long PlayerRank = 2LL;
}
namespace PickupCharacterMissionCheckCondition {
  constexpr long long Level = 1LL;
  constexpr long long AwakeningPhase = 2LL;
  constexpr long long TalentStage = 3LL;
  constexpr long long SenseLevel = 4LL;
  constexpr long long EpisodeOrder = 5LL;
}
namespace PosterEffectTypes {
  constexpr long long Leader = 1LL;
  constexpr long long Normal = 2LL;
}
namespace ProbabilityChangeTypes {
  constexpr long long None_ = 0LL;
  constexpr long long AccessoryFormula = 1LL;
}
namespace ResultVoiceConditionTypes {
  constexpr long long AllPerfect = 1LL;
  constexpr long long FullCombo = 2LL;
  constexpr long long ClearedAndBestAchievementRate = 3LL;
  constexpr long long Cleared = 4LL;
  constexpr long long LifeZeroAndBestAchievementRate = 5LL;
  constexpr long long LifeZero = 6LL;
}
namespace RouletteTypes {
  constexpr long long Point = 1LL;
  constexpr long long Item = 2LL;
}
namespace SaleTypes {
  constexpr long long AccountRegister = 1LL;
}
namespace SenseNotationBuffTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Attribute = 1LL;
  constexpr long long Company = 2LL;
  constexpr long long CharacterBase = 3LL;
  constexpr long long Character = 4LL;
}
namespace SenseNotationStatusTypes {
  constexpr long long Performance = 0LL;
  constexpr long long Vocal = 1LL;
  constexpr long long Expression = 2LL;
  constexpr long long Concentration = 3LL;
}
namespace ShopItemTypes {
  constexpr long long None_ = 0LL;
  constexpr long long LoginPass = 1LL;
  constexpr long long StarPass = 2LL;
  constexpr long long DaiStarPass = 3LL;
  constexpr long long MultiLiveDropBoost = 4LL;
  constexpr long long MultiLiveDropLimitUp = 5LL;
}
namespace ShopPurchaseTypes {
  constexpr long long Paid = 1LL;
  constexpr long long Money = 2LL;
  constexpr long long Free = 3LL;
  constexpr long long FreeJewel = 4LL;
}
namespace SpecialEventCategoryTypes {
  constexpr long long TotalPoint = 1LL;
}
namespace SpineBodySizeTypes {
  constexpr long long Small = 1LL;
  constexpr long long Medium = 2LL;
}
namespace SplashAdditionalValueTypes {
  constexpr long long None_ = 0LL;
  constexpr long long HomeCharacterBaseId = 1LL;
  constexpr long long LotteryHighestPrizeId = 2LL;
}
namespace SplashTypes {
  constexpr long long Video = 1LL;
  constexpr long long Picture = 2LL;
  constexpr long long Episode = 3LL;
  constexpr long long LotteryResult = 4LL;
}
namespace SplashUnlockConditionTypes {
  constexpr long long None_ = 0LL;
  constexpr long long LotteryUserCache = 1LL;
}
namespace StoryEventBonusTypes {
  constexpr long long Company = 1LL;
  constexpr long long CharacterBase = 2LL;
  constexpr long long Poster = 3LL;
}
namespace StoryEventCategoryTypes {
  constexpr long long Normal = 1LL;
  constexpr long long Voltage = 2LL;
  constexpr long long HighScore = 3LL;
  constexpr long long Circle = 4LL;
  constexpr long long MainStory = 5LL;
}
namespace TeamChallengeDifficultyTypes {
  constexpr long long Easy = 1LL;
  constexpr long long Normal = 2LL;
  constexpr long long Hard = 3LL;
}
namespace TeamChallengeGoalTypes {
  constexpr long long TotalScore = 1LL;
  constexpr long long TotalCombo = 2LL;
  constexpr long long TotalStarAct = 3LL;
}
namespace TitleBackgroundPathTypes {
  constexpr long long Path = 1LL;
  constexpr long long CharacterMasterId = 2LL;
  constexpr long long PosterMasterId = 3LL;
}
namespace TitleBackgroundPriorityTypes {
  constexpr long long Low = -1LL;
  constexpr long long Normal = 0LL;
  constexpr long long High = 1LL;
}
namespace TitleCallVoicePriorityTypes {
  constexpr long long Low = -1LL;
  constexpr long long Normal = 0LL;
  constexpr long long High = 1LL;
}
namespace TitleDecorationTypes {
  constexpr long long FirstAnniversary = 1LL;
  constexpr long long SecondAnniversary = 2LL;
  constexpr long long SecondHalfAnniversary = 3LL;
  constexpr long long ThirdAnniversary = 4LL;
}
namespace TrialPartyCharacterLockTypes {
  constexpr long long None_ = 0LL;
  constexpr long long CharacterBase = 1LL;
  constexpr long long Character = 2LL;
}
namespace TrialPartyEquipmentLockTypes {
  constexpr long long None_ = 0LL;
  constexpr long long CharacterBase = 1LL;
  constexpr long long Position = 2LL;
}
namespace UnlockConditionTypes {
  constexpr long long CharacterStarRank = 1LL;
  constexpr long long Story = 2LL;
  constexpr long long GetPoster = 3LL;
  constexpr long long ReleasePoster1 = 4LL;
  constexpr long long ReleasePoster2 = 5LL;
  constexpr long long ReleasePoster3 = 6LL;
  constexpr long long ReleasePoster4 = 7LL;
  constexpr long long PlayerRate = 8LL;
  constexpr long long LeagueClass = 9LL;
  constexpr long long BuyItem = 10LL;
  constexpr long long AnyMainStoryChapter = 11LL;
  constexpr long long AuditionClear = 12LL;
  constexpr long long ExtraGoodCount = 13LL;
  constexpr long long NotationShop = 14LL;
  constexpr long long BuyJewelShopItem = 15LL;
  constexpr long long PlayerRank = 16LL;
  constexpr long long SelectedPickup = 17LL;
  constexpr long long TimeLimitedControl = 18LL;
  constexpr long long FlashSale = 19LL;
  constexpr long long NotInPossession = 20LL;
}
namespace WebLinkTypes {
  constexpr long long Normal = 0LL;
  constexpr long long WebShop = 1LL;
}
namespace FontSizes {
  constexpr long long Small = 1LL;
  constexpr long long Middle = 2LL;
  constexpr long long Large = 3LL;
}
namespace FadeTypes {
  constexpr long long BlackFadeOutFadeIn = 1LL;
  constexpr long long WhiteFadeOutFadeIn = 2LL;
  constexpr long long TimeElapsed = 3LL;
  constexpr long long CrossFade = 4LL;
}
namespace WindowEffects {
  constexpr long long Sepia = 1LL;
  constexpr long long WhiteBlur = 2LL;
}
namespace CharacterAppearanceTypes {
  constexpr long long FadeIn = 0LL;
  constexpr long long SlideInFromRight = 1LL;
  constexpr long long SlideInFromLeft = 2LL;
  constexpr long long SlideInFromBottom = 3LL;
}
namespace CharacterPositions {
  constexpr long long None_ = 0LL;
  constexpr long long OuterLeft = 1LL;
  constexpr long long InnerLeft = 2LL;
  constexpr long long Center = 3LL;
  constexpr long long InnerRight = 4LL;
  constexpr long long OuterRight = 5LL;
}
namespace CharacterLayerTypes {
  constexpr long long None_ = 0LL;
  constexpr long long Layer1 = 1LL;
  constexpr long long Layer2 = 2LL;
  constexpr long long Layer3 = 3LL;
}
namespace SpineSizes {
  constexpr long long Small = 1LL;
  constexpr long long Middle = 2LL;
  constexpr long long Large = 3LL;
}
}  // namespace enums
