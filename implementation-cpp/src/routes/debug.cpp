#include "routes/debug.h"

#include "headers.h"
#include "wire.h"

// ports routes/debug.py — DEV-ONLY tooling. every endpoint is a stub that returns a
// default result (the Python reads path/query/body but discards it), so the handlers
// only build the common_response envelope. 170 routes, source order preserved so
// httplib first-match-wins routing matches FastAPI's declaration order.

namespace routes {

namespace {

// respond(result) with the result encoded under `name`; `result` defaults to a model's
// default instance (empty object -> all fields default)
httplib::Server::Handler result_handler(const char* name,
                                         wire::json result = wire::json::object()) {
    return [name, result](const httplib::Request&, httplib::Response& res) {
        res.set_content(wire::common_response(name, result), "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    };
}

}  // namespace

void register_debug(httplib::Server& svr) {
    svr.Post("/api/Debugs/CircleEvent/AddCircleEventMissionCount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AddCircleMembers/:count", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AddExceededStarPoint/:mCharacterBaseId/:amount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AddLessonHighScore/:addScore", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AddPointForTotalPointEvent", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AddRankPoint/:rankPoint", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AddStarPoint/:mCharacterBaseId/:amount", result_handler("StarPointResult"));
    svr.Post("/api/Debugs/AlbumSimpleArranging", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/AuthenticateOrRegister", result_handler("", wire::json("")));
    svr.Post("/api/Debug/BattleGhost", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CallInboxService", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CallInboxServiceMany", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CircleUsageTimeReset", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ClearAudition", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ClearBonusLiveEvent", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ClearTrialPartyEvent", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ConvertMultiRoomId", result_handler("", wire::json("")));
    svr.Post("/api/Debugs/CreateAlbum", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CreateCircleDummy/:startUserId/:quantity", result_handler("BooleanResult"));
    svr.Post("/api/Debug/CreateConcoursRandomRankings", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CreateLeagueHistory", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CreateDummyUsers", result_handler("", wire::json::array()));
    svr.Post("/api/Debugs/CreateFriendRequestForMe/:count", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CreateTripleCastHistory", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/Circle/SupportPoint/DailyAddedSupportPoint/:point", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteAllCourseRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteCircles", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteLeagueAllData", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteLeagueData/:mLeagueId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteLeagueUserData/:userId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteLeagueUsersData", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/DeleteTripleCastData/:mTripleCastId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/EasyStartLive", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/EditLeagueBasic", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/EditLeagueHistory", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ExecuteLeagueTotalization", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/FinishLive", result_handler("FinishLiveResult"));
    svr.Post("/api/Debugs/FinishLiveSpecifiedNotes", result_handler("FinishLiveResult"));
    svr.Post("/api/Debugs/GenerateCharacterPointEventPointRanking/:mCharacterPointEventId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GenerateDummyCharacterPointEventPointRanking/:mCharacterPointEventId/:amount/:maxPoint", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GenerateDummyTotalPointEventPointRanking/:mTotalPointEventId/:amount/:maxPoint", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GenerateLeagueGroupScore", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GenerateLinkageCodeAndPassword", result_handler("DebugLinkageCodeResult"));
    svr.Post("/api/Debugs/GenerateLiveRate/:count/:achievementRate", result_handler("BooleanResult"));
    svr.Post("/api/Debug/GenerateLotteryResult", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GenerateStoryEventHighScoreRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GenerateStoryEventPointRanking", result_handler("BooleanResult"));
    svr.Get("/api/Debugs/CacheUpdatedAt", result_handler("", wire::json("")));
    svr.Post("/api/Debugs/GetToken", result_handler("", wire::json("")));
    svr.Get("/api/Debugs/GetCurrentLeague", result_handler("", wire::json(0)));
    svr.Post("/api/Debugs/GetEpisode", result_handler("EpisodeResult"));
    svr.Get("/api/Debugs/GetLeagueHistories", result_handler("", wire::json("")));
    svr.Get("/api/Debugs/LiveScoreHistory", result_handler("ScoreWithDateResult", wire::json::array({wire::json::object()})));
    svr.Post("/api/Debugs/ConvertUserId", result_handler("DebugUserIdResult"));
    svr.Post("/api/Debugs/GiveEnhanceItems", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GrowAccessoriesMax", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GiveEnhanceItemsAndGrowCharactersMax", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/GiveEnhanceItemsAndGrowPostersMax", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/Yokubari", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/InitializeLeagueBasicData", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/InitializePlayerRate", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/JoinOrCreateLeagueGroupMember", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/LeaveLeagueGroup", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/OpenAllFlashSale", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/PrepareLeagueGroupUser", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/PurchaseDaiStarPass", result_handler("BooleanResult"));
    svr.Post("/api/Debug/PurchaseExternalPaymentItem", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/PurchaseStarPass", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ReadRequiredEpisode", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/RecordActivityLog", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetLeagueDaistarEnrollCount/:enrollCount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/CircleEvent/ReestCirclePointAndMission/:mCircleEventId", result_handler("BooleanResult"));
    svr.Get("/api/Debugs/RefreshMemoryCache", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ReleaseAllEpisodes/:timesRead", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ReleaseAllMusicDifficulty", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ResetAutoPlay", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ResetCampEventSupportPointRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ResetCharacterPointEventPointRanking/:mCharacterPointEventId", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ResetEventPointRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ResetLesson", result_handler("BooleanResult"));
    svr.Post("/api/Debug/ResetPickupGachaSelectedThings", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ResetStoryEventHighScoreRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ResetStoryEventPointRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/ResetTotalPointEvent", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendAccessories", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendAccessoryWithEffect", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendAlbumThemes", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendAlbumTheme/:mAlbumThemeId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendBomb/:mBombId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendBombs", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendCharacter/:mCharacterId/:quantity", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendCharacters", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendChatworkMessage", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendCoin/:amount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendCostume/:mCostumeId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendCostumes", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendDecoration/:mDecorationId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendDecorations", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SendHomeSkin", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendHomeSkins", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SendIconFrame", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendIconFrames", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendItem/:mItemId/:quantity", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendItems", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendJewel/:amount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendLoginBonusAgain", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendMusic/:mMusicId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendMusics", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SendNameBaseColor", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNameBaseColors", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNameColor/:mNameColorId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNameColors", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNameplate/:mNamePlateId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNameplates", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNote/:mNoteId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNotes", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendNotification", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendPaidJewel/:amount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendPoster/:mPosterId/:quantity", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendPosters", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendStamps", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendStamp/:mStampId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendTrophies", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SendTrophy/:mTrophyId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetAutoPlayAvailableTimes/:times", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetCharacterEnhanceInfo", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetCharacterMissionCount/:characterBaseMasterId/:characterMissionMasterId/:count", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetCharacterPointEventPoint/:mCharacterPointEventId/:amount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/Circle/SupportPoint/:company", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/Circle/SupportCompanyLevelLimit", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetCourseRankingResultsDecreasingByPercent", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetEventPoint", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetFlashSaleCount", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetGhost", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetGingaTheaterLiveDropLimit", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetLeagueGroupScore", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetLeagueScore", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetLoginPassUntil", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetMarketFrameThing", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetMissionCount/:mMissionId/:count", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetMusicCourseRanking", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetParty", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetPhoto", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetPlayerRankWithPoint/:rank", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetPlayerRate", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetPosterEnhanceInfo", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetReleasableOlivier/:mMusicId/:releasableOlivier", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetReleasedStella/:mMusicId/:releasedStella", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetServerTime", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetSpRate/:mMusicId/:point", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetSpRatePoint", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetSpRateSegment", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetSplashLastDisplayedAt", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetStamina/:stamina", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/Circle/SupportPoint/StaminaLastReceivedAt/:lastReceivedAt", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetStarRank/:mCharacterBaseId/:rank", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetStoryEventHighScoreEnhancementPoint", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetStoryEventPoint/:mStoryEventId/:amount", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetTimeWarpByDate", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetTimeWarpBySpecifyOffset", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetTotalPointEvent", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetTournamentDetail/:tournamentDetailMasterId", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetTripleCastDaistarEnrollCount/:enrollCount", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetTripleCastGroupScore", result_handler("BooleanResult"));
    svr.Post("/api/Debug/SetTripleCastScore", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/SetUserBanLevel", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/UnlockAllFeatures", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/LeagueUpdateRankingScore/:userId/:leagueMasterId/:leagueGroupId/:score", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/UpdateUserProfileSnapshots", result_handler("BooleanResult"));
    svr.Post("/api/Debugs/UploadToutchLog", result_handler("UrlResult"));
}

}  // namespace routes
