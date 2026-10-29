#include "home.h"

#include <ctime>
#include <string>

#include "../headers.h"
#include "../wire.h"
#include "generated/enums_generated.h"

namespace routes {

namespace {

using wire::json;

// current UTC time as an ISO-8601 string; wire converts a DateTime field's string to a Timestamp
std::string now_iso() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::gmtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S+00:00", tm);
    return std::string(buf);
}

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
        // the client's login-bonus flow takes an AsyncLock (SemaphoreSlim) and only releases it
        // after processing at least one structurally-complete LoginBonusResult. an empty array is a
        // valid array but leaves that lock held forever -> silent deadlock on Main scene init (white
        // screen + logo, UnityMain stuck in futex_wait, no further requests). return one complete
        // result so the flow completes. real rewards still need LoginBonusMaster/LoginBonusDetailMaster
        // (absent from our unpacked masterdata), so nothing is granted yet -- received_things/details
        // stay empty.
        json result = json{
            {"login_bonus_id", 1},
            {"current_login_count", 1},
            {"received_things", json::array()},
            {"title", "ログインボーナス"},
            {"type", enums::LoginBonusTypes::Normal},
            {"start_date", now_iso()},
            {"message_template", "ログインボーナス {0}日目"},
            {"total_days", 7},
            {"is_loop", true},
            {"order", 1},
            {"details", json::array()},
            {"layout_type", enums::LoginBonusLayoutTypes::Normal},
        };
        json arr = json::array();
        arr.push_back(result);
        send(res, wire::common_response("LoginBonusResult", arr));
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
