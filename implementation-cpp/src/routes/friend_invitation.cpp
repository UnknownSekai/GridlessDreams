#include "routes/friend_invitation.h"

#include "headers.h"
#include "wire.h"

// ports routes/friend_invitation.py — all four endpoints are stubs returning default results

namespace routes {

namespace {

void set_result(httplib::Response& res, const char* result_name, const wire::json& result) {
    res.set_content(wire::common_response(result_name, result), "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

}  // namespace

void register_friend_invitation(httplib::Server& svr) {
    // /api/FriendInvitation/Input?invitationCode=
    svr.Post("/api/FriendInvitation/Input", [](const httplib::Request&, httplib::Response& res) {
        set_result(res, "BooleanResult", wire::json::object());
    });

    // /api/FriendInvitation/Check?invitationCode=
    svr.Get("/api/FriendInvitation/Check", [](const httplib::Request&, httplib::Response& res) {
        set_result(res, "FriendInvitationUserInfoResult", wire::json::object());
    });

    // /api/FriendInvitation/Receive
    svr.Post("/api/FriendInvitation/Receive", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "FriendInvitationMissionPayload");  // no payload use
        set_result(res, "ReceivedThing", wire::json::array({wire::json::object()}));
    });

    // /api/FriendInvitation/Update
    svr.Post("/api/FriendInvitation/Update", [](const httplib::Request&, httplib::Response& res) {
        set_result(res, "BooleanResult", wire::json::object());
    });
}

}  // namespace routes
