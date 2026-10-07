#include "login.h"

#include <cstdint>
#include <ctime>
#include <optional>
#include <string>

#include "../auth.h"
#include "../db.h"
#include "../db/account.h"
#include "../headers.h"
#include "../helpers/daily.h"
#include "../helpers/user_data.h"
#include "../wire.h"

namespace routes {

namespace {

using wire::json;

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

}  // namespace

void register_login(httplib::Server& svr) {
    // /api/Login
    svr.Post("/api/Login", [](const httplib::Request& req, httplib::Response& res) {
        json payload = wire::read_request(req.body, "LoginPayload");
        std::optional<long long> user_id = user_data::current_user_id(req);
        daily::refresh_daily_limits(user_id);

        std::optional<json> account =
            db::fetchrow(db::account::get_account_by_id(user_id.value_or(0)));
        bool first_login = account.has_value() && (*account).at("lastLoginAt").get<int64_t>() == 0;
        db::execute(db::account::update_last_login(user_id.value_or(0),
                                                   static_cast<int64_t>(time(nullptr))));

        // first login surfaces the whole freshly-created account; later logins only touch User
        json present = first_login
                           ? user_data::build_present(
                                 user_id, {"Inbox", "FriendInvitation", "MissionPass", "User"})
                           : user_data::build_present(user_id, {"User"});
        send(res,
             wire::common_response("LoginResult", auth::login(payload), json::array(), present));
    });
}

}  // namespace routes
