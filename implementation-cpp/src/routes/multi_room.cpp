#include "multi_room.h"

#include "../headers.h"
#include "../wire.h"

// The Detail/Release/Resignation decorators carry a literal "?hashedMultiRoomId="
// in the path, so (as in the Python) those routes never match a real request.

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

// respond([]) -> a bare empty list.
void respond_empty_list(httplib::Response& res) {
    send(res, wire::common_response("", json::array()));
}

}  // namespace

void register_multi_room(httplib::Server& svr) {
    svr.Post("/api/MultiRooms/Create", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "CreateMultiRoomPayload");
        respond_empty(res, "MultiRoomCreateResult");
    });

    svr.Post("/api/MultiRooms/Invited",
             [](const httplib::Request&, httplib::Response& res) { respond_singleton(res, "MultiRoomInvitedResult"); });

    svr.Post("/api/MultiRooms/Joined",
             [](const httplib::Request&, httplib::Response& res) { respond_singleton(res, "MultiRoomJoinnedResult"); });

    svr.Post("/api/MultiRooms/Detail?hashedMultiRoomId=",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "MultiRoomDetailResult"); });

    svr.Get("/api/MultiRooms", [](const httplib::Request&, httplib::Response& res) {
        respond_singleton(res, "MultiRoomInformationResult");
    });

    svr.Get("/api/MultiRoom/GetSearchMultiRooms",
            [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/MultiRooms/Join", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "JoinMultiRoomPayload");
        respond_empty(res, "BooleanResult");
    });

    svr.Post("/api/MultiRooms/Release?hashedMultiRoomId=",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/MultiRooms/Resignation?hashedMultiRoomId=",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/MultiRooms/Send/Invite", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "InviteMultiRoomPayload");
        respond_empty(res, "BooleanResult");
    });
}

}  // namespace routes
