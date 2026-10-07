#include "friends.h"

#include <optional>
#include <string>

#include "../db.h"
#include "../db/friends.h"
#include "../headers.h"
#include "../helpers/friends.h"
#include "../helpers/user_data.h"
#include "../wire.h"

// Ports routes/friends.py.

namespace routes {

namespace {

using wire::json;

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

// Optional[str] query param -> std::optional<std::string>.
std::optional<std::string> param(const httplib::Request& req, const char* name) {
    if (req.has_param(name)) return req.get_param_value(name);
    return std::nullopt;
}

json boolean_result(bool is_success) {
    json r = json::object();
    r["is_success"] = is_success;
    return r;
}

}  // namespace

void register_friends(httplib::Server& svr) {
    // /api/Friends/AcceptRequest?fromUserId=
    svr.Post("/api/Friends/AcceptRequest", [](const httplib::Request& req, httplib::Response& res) {
        long long status = friends::accept_request(user_data::current_user_id(req),
                                                   param(req, "fromUserId"));
        json result = json::object();
        result["result_status"] = status;
        send(res, wire::common_response("FriendAcceptResult", result));
    });

    // /api/Friends/BlockUser?targetUserId=
    svr.Post("/api/Friends/BlockUser", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        if (user_id.has_value()) {
            std::optional<long long> target = friends::resolve_user_id(param(req, "targetUserId"));
            if (target.has_value()) {
                db::execute(db::friends::add_block(*user_id, *target));
            }
        }
        send(res, wire::common_response("BooleanResult", boolean_result(true)));
    });

    // /api/Friends/BlockUsers
    svr.Get("/api/Friends/BlockUsers", [](const httplib::Request& req, httplib::Response& res) {
        send(res, wire::common_response("BlockListResult",
                                        friends::block_list_result(user_data::current_user_id(req))));
    });

    // /api/Friends/CancelRequest?targetUserId=
    svr.Post("/api/Friends/CancelRequest", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        if (user_id.has_value()) {
            std::optional<long long> target = friends::resolve_user_id(param(req, "targetUserId"));
            if (target.has_value()) {
                db::execute(db::friends::remove_request(*user_id, *target));  // my outgoing request
            }
        }
        send(res, wire::common_response("BooleanResult", boolean_result(true)));
    });

    // /api/Friends/DenyRequest?fromUserId=
    svr.Post("/api/Friends/DenyRequest", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        if (user_id.has_value()) {
            std::optional<long long> from_id = friends::resolve_user_id(param(req, "fromUserId"));
            if (from_id.has_value()) {
                db::execute(db::friends::remove_request(*from_id, *user_id));  // their incoming request
            }
        }
        send(res, wire::common_response("BooleanResult", boolean_result(true)));
    });

    // /api/Friends
    svr.Get("/api/Friends", [](const httplib::Request& req, httplib::Response& res) {
        send(res, wire::common_response("FriendListResult",
                                        friends::list_result(user_data::current_user_id(req), "friends")));
    });

    // /api/ReceivedRequest
    svr.Post("/api/ReceivedRequest", [](const httplib::Request& req, httplib::Response& res) {
        send(res, wire::common_response("FriendListResult",
                                        friends::list_result(user_data::current_user_id(req), "received")));
    });

    // /api/SendingRequest
    svr.Get("/api/SendingRequest", [](const httplib::Request& req, httplib::Response& res) {
        send(res, wire::common_response("FriendListResult",
                                        friends::list_result(user_data::current_user_id(req), "sending")));
    });

    // /api/Friends/RemoveBlockUser?targetUserId=
    svr.Post("/api/Friends/RemoveBlockUser", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        if (user_id.has_value()) {
            std::optional<long long> target = friends::resolve_user_id(param(req, "targetUserId"));
            if (target.has_value()) {
                db::execute(db::friends::remove_block(*user_id, *target));
            }
        }
        send(res, wire::common_response("BooleanResult", boolean_result(true)));
    });

    // /api/Friends/RemoveFriend?targetUserId=
    svr.Post("/api/Friends/RemoveFriend", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        if (user_id.has_value()) {  // friendship is two rows, drop both
            std::optional<long long> target = friends::resolve_user_id(param(req, "targetUserId"));
            if (target.has_value()) {
                db::execute(db::friends::remove_friend(*user_id, *target));
                db::execute(db::friends::remove_friend(*target, *user_id));
            }
        }
        send(res, wire::common_response("BooleanResult", boolean_result(true)));
    });

    // /api/Friends/Search?targetUserId=
    svr.Post("/api/Friends/Search", [](const httplib::Request& req, httplib::Response& res) {
        send(res, wire::common_response("FriendSearchResult",
                                        friends::search(user_data::current_user_id(req),
                                                        param(req, "targetUserId"))));
    });

    // /api/Friends/SendRequest?targetUserId=
    svr.Post("/api/Friends/SendRequest", [](const httplib::Request& req, httplib::Response& res) {
        long long status = friends::send_request(user_data::current_user_id(req),
                                                 param(req, "targetUserId"));
        json result = json::object();
        result["result_status"] = status;
        send(res, wire::common_response("FriendRequestResult", result));
    });

    // /api/Friends/SetFavorite
    svr.Post("/api/Friends/SetFavorite", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        json payload = wire::read_request(req.body, "FriendFavoritePayload");
        if (user_id.has_value() && !payload.is_null()) {
            std::optional<std::string> target_user_id;
            auto it = payload.find("target_user_id");
            if (it != payload.end() && it->is_string()) target_user_id = it->get<std::string>();
            bool set_favorite = payload.value("set_favorite", false);
            std::optional<long long> target = friends::resolve_user_id(target_user_id);
            if (target.has_value()) {
                db::execute(db::friends::set_friend_favorite(*user_id, *target, set_favorite));
            }
        }
        send(res, wire::common_response("BooleanResult", boolean_result(true)));
    });
}

}  // namespace routes
