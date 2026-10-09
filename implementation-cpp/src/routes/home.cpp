#include "home.h"

#include <string>

#include "../headers.h"
#include "../wire.h"
#include "generated/enums_generated.h"

namespace routes {

namespace {

using wire::json;

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

// NotificationContentResult() pydantic defaults: the two enum fields default to 1, not 0
json notification_content_result() {
    return json{
        {"notification_tab_category", enums::NotificationTabCategory::Important},
        {"notification_category", enums::NotificationCategory::Notification},
    };
}

}  // namespace

void register_home(httplib::Server& svr) {
    svr.Post("/api/Home/CheckEexternalPayment", [](const httplib::Request&, httplib::Response& res) {
        // ApiActionResult - single item not list style
        send(res, wire::common_response("EexternalPaymentResult", json::object()));
    });

    svr.Post("/api/Home/CheckReceiveLoginBonus", [](const httplib::Request&, httplib::Response& res) {
        // TODO: build LoginBonusResult[] from masterdata + grant the day's rewards via
        // things::grant_things (to_inbox=true). blocked: LoginBonusMaster /
        // LoginBonusDetailMaster / LoginBonusSpineGroupMaster are NOT in our unpacked masterdata
        // (only LoginBonusSpineCostumeMaster is). return [] (no bonus) rather than a bogus id-0 one.
        send(res, wire::common_response("LoginBonusResult", json::array()));
    });

    svr.Post("/api/Home/GetMultiLiveRestrictionNotification",
             [](const httplib::Request&, httplib::Response& res) {
                 send(res, wire::common_response("BooleanResult", json::object()));
             });

    svr.Post("/api/Home/GetNotificationsAsync/:mNotificationId",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long m_notification_id = std::stoll(req.path_params.at("mNotificationId"));
                 (void)m_notification_id;
                 send(res, wire::common_response("NotificationContentResult",
                                                 notification_content_result()));
             });

    // anonymous, title screen
    svr.Get("/api/Home/GetNotificationsInTitleAsync/:mNotificationId",
            [](const httplib::Request& req, httplib::Response& res) {
                long long m_notification_id = std::stoll(req.path_params.at("mNotificationId"));
                (void)m_notification_id;
                send(res, wire::common_response("NotificationContentResult",
                                                notification_content_result()));
            });

    svr.Post("/api/Home/GetNotificationsAsync", [](const httplib::Request&, httplib::Response& res) {
        // ApiActionResult<NotificationResult[]> -- empty for now
        send(res, wire::common_response("NotificationResult", json::array()));
    });

    // anonymous, title screen
    svr.Get("/api/Home/GetNotificationsInTitleAsync",
            [](const httplib::Request&, httplib::Response& res) {
                // ApiActionResult<NotificationResult[]> -- empty for now
                send(res, wire::common_response("NotificationResult", json::array()));
            });

    svr.Post("/api/Home/UpdateNotificationReadTime",
             [](const httplib::Request& req, httplib::Response& res) {
                 json payload = wire::read_request(req.body, "ReadNotificationPayload");
                 (void)payload;
                 send(res, wire::common_response("BooleanResult", json::object()));
             });
}

}  // namespace routes
