#include "events.h"

#include "../headers.h"
#include "../wire.h"

// Ports routes/events.py. Every endpoint is a stub returning an empty *Result
// (matching the Python); no payload, query, or path param affects the response.

namespace routes {

namespace {

using wire::json;

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

// respond(Model()) -> empty result entity, no present.
void respond_empty(httplib::Response& res, const char* result_name) {
    send(res, wire::common_response(result_name, json::object()));
}

// respond([]) -> a bare empty list.
void respond_empty_list(httplib::Response& res) {
    send(res, wire::common_response("", json::array()));
}

}  // namespace

void register_events(httplib::Server& svr) {
    svr.Post("/api/Events/TrialPartyEvent/EditParty", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "EditTrialPartyEventStagePartyPayload");
        respond_empty(res, "BooleanResult");
    });

    svr.Get("/api/Events/CharacterPoint/GetCharacterBorderRanking/:characterPointEventMasterId",
            [](const httplib::Request&, httplib::Response& res) {
                respond_empty(res, "CharacterPointEventRankingResult");
            });

    svr.Get("/api/Events/CharacterPoint/GetCharacterNearRanking/:characterPointEventMasterId",
            [](const httplib::Request&, httplib::Response& res) {
                respond_empty(res, "CharacterPointEventRankingResult");
            });

    svr.Get("/api/Events/CharacterPoint/GetCharacterTopRanking/:characterPointEventMasterId",
            [](const httplib::Request&, httplib::Response& res) {
                respond_empty(res, "CharacterPointEventRankingResult");
            });

    svr.Get("/api/Events/CharacterPoint/GetInformation/:characterPointEventMasterId",
            [](const httplib::Request&, httplib::Response& res) {
                respond_empty(res, "CharacterPointEventInformationResult");
            });

    svr.Get("/api/Events/CharacterPoint/GetOverallTopRanking/:characterPointEventMasterId",
            [](const httplib::Request&, httplib::Response& res) {
                respond_empty(res, "CharacterPointEventRankingResult");
            });

    svr.Get("/api/Events/CircleEvent/Raking/Border/:circleEventMasterId",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleEventRanking"); });

    svr.Get("/api/Events/CircleEvent/Raking/Circle/:circleEventMasterId",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleEventRanking"); });

    svr.Get("/api/Events/CircleEvent/Information/:circleEventMasterId",
            [](const httplib::Request&, httplib::Response& res) {
                respond_empty(res, "CircleEventInformationResult");
            });

    svr.Get("/api/Events/CircleEvent/Raking/Near/:circleEventMasterId",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleEventRanking"); });

    svr.Get("/api/Events/GetConcoursInformation",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "ConcoursInfomationResult"); });

    svr.Post("/api/Events/GetEventBorderRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetEventCampBorderRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Get("/api/Events/GetEventCampMyRanking",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "StoryEventCampInfo"); });

    svr.Post("/api/Events/GetEventCampTopRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetEventCircleRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetEventFriendRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetEventMyRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRanking"); });

    svr.Post("/api/Events/GetEventNearRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetEventTopRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetStoryEventBorderRanking/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetStoryEventCampNearRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetStoryEventCircleMissionProgresses/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "StoryEventMissionCircleProgressResult");
             });

    svr.Post("/api/Events/GetStoryEventCircleRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetStoryEventFriendRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetStoryEventHighScoreTopRanking/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawHighScoreRankingResult"); });

    svr.Post("/api/Events/GetStoryEventMyRanking/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRanking"); });

    svr.Post("/api/Events/GetStoryEventNearRanking/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetStoryEventTopRanking/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Post("/api/Events/GetTotalPointEventCircleRanking",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "TotalPointEventRankingResult");
             });

    svr.Post("/api/Events/GetTotalPointEventFriendRanking",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "TotalPointEventRankingResult");
             });

    svr.Post("/api/Events/GetTotalPointEventInformation",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "TotalPointEventInformationResult");
             });

    svr.Post("/api/Events/GetTotalPointEventNearRanking",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "TotalPointEventRankingResult");
             });

    svr.Post("/api/Events/GetTotalPointEventTopRanking",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "TotalPointEventRankingResult");
             });

    svr.Post("/api/Events/TournamentQualifying/Information/:tournamentQualifyingMasterId",
             [](const httplib::Request&, httplib::Response& res) {
                 respond_empty(res, "TournamentQualifyingInformationResult");
             });

    svr.Post("/api/Events/GrowStoryEventHighScoreBuffSetting/:mStoryEventId/:mStoryEventHighScoreBuffSettingId/:levelTo",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/NoneStoryEvents/ReadTips/:mEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/CharacterPoint/ReadTips/:characterPointEventMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/ReadSpecialEventTips",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/ReadTips/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/CircleEvent/ReceiveCirclePointReward/:circleEventMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/Events/ReceiveStoryEventCircleMissionAllRewards/:mStoryEventId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/Events/ReceiveStoryEventCircleMissionReward/:mStoryEventId/:receiveMStoryEventCircleMissionRewardId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "ReceivedThing"); });

    svr.Post("/api/Events/ReceiveTotalPointEventReward",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/Events/CircleEvent/ResetMissionCount/:circleEventMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/BoxGacha/Reset/:eventBoxGachaMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/BoxGacha/Roll/:eventBoxGachaDetailMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "EventBoxGachaRollResult"); });

    svr.Post("/api/Events/DugongRun/:dugongRunCourseMasterId/Clear/:clearType",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/Events/SelectCamp",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Events/CharacterPoint/SetCharacter/:characterPointEventMasterId/:characterBaseMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });
}

}  // namespace routes
