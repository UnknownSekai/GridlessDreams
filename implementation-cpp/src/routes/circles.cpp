#include "circles.h"

#include "../headers.h"
#include "../wire.h"

// Ports routes/circles.py. Every endpoint is a stub returning an empty Circle*
// result (matching the Python); no payload or path param affects the response.

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

// respond([Model()]) -> a list holding one default entity.
void respond_singleton(httplib::Response& res, const char* result_name) {
    send(res, wire::common_response(result_name, json::array({json::object()})));
}

// zero status with a non-null level_limit_status (matches the official response)
json support_company_status() {
    json status = json::object();
    status["level_limit_status"] = json::object();
    return status;
}

}  // namespace

void register_circles(httplib::Server& svr) {
    svr.Post("/api/Circles/ApproveInvite/:inviteId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/ApproveRequest/:requestId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/AuthorityChange", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "CircleAuthorityChangePayload");
        respond_empty(res, "CircleAuthorityResult");
    });

    svr.Post("/api/Circles/CancelInvite/:inviteId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Get("/api/Circles/MemberInfo",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleMemberInfoResult"); });

    svr.Post("/api/Circles/Create", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "CirclePayload");
        respond_empty(res, "CreateCircleResult");
    });

    svr.Post("/api/Circles/DonateSupportCompany", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "DonateSupportLevelLimitPayload");
        respond_empty(res, "DonateSupportCompanyResult");
    });

    svr.Post("/api/Circles/Edit", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "CirclePayload");
        respond_empty(res, "CircleResult");
    });

    svr.Post("/api/Circles/EditCircleBanner", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "BannerPayload");
        respond_empty(res, "BooleanResult");
    });

    svr.Post("/api/Circles/Expulsion",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleInformationResult"); });

    svr.Get("/api/Circles/Ranking/Daily",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Get("/api/Circles/Ranking/Monthly",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Get("/api/Circles/Ranking/Weekly",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "RawRankingResult"); });

    svr.Get("/api/Circles",
            [](const httplib::Request&, httplib::Response& res) { respond_singleton(res, "CircleInformationResult"); });

    svr.Post("/api/Circles/Condition", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "CirclePayload");
        respond_singleton(res, "CircleInformationResult");
    });

    svr.Get("/api/Circles/Search",
            [](const httplib::Request&, httplib::Response& res) { respond_singleton(res, "CircleInformationResult"); });

    svr.Get("/api/Circles/Invited",
            [](const httplib::Request&, httplib::Response& res) { respond_singleton(res, "CircleInformationResult"); });

    svr.Post("/api/Circles/MyCircleInfo",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "MyCircleInformationResult"); });

    svr.Get("/api/Circles/GetSupportAndTheaterLevelInformation",
            [](const httplib::Request&, httplib::Response& res) {
                // hardcoded: all four companies at zero
                json info = json::object();
                info["sirius"] = support_company_status();
                info["eden"] = support_company_status();
                info["gingaza"] = support_company_status();
                info["denki"] = support_company_status();
                send(res, wire::common_response("SupportCompanyInformation", info));
            });

    svr.Post("/api/Circles/Join",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/ReceiveTheaterStamina",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Circles/RecommendUsers",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleSearchResult"); });

    svr.Post("/api/Circles/RejectInvite/:inviteId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/RejectRequest/:requstId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/Release",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/Resignation",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/Search/Friend",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleSearchResult"); });

    svr.Post("/api/Circles/Search/Inviting",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleSearchResult"); });

    svr.Post("/api/Circles/Search/Request",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleSearchResult"); });

    svr.Post("/api/Circles/SearchUser",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleSearchIdResult"); });

    svr.Post("/api/Circles/SendInvite",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleInviteResult"); });

    svr.Post("/api/Circles/JoinPreRequest",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/JoinRequest",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/SetIsPublishRanking/:IsPublishRanking",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/SetSupportCompany/:company",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "CircleResult"); });

    svr.Post("/api/Circles/SetVisibleActivityLog",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });
}

}  // namespace routes
