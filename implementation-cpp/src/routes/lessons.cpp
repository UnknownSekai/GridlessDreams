#include "routes/lessons.h"

#include "pipeline.h"
#include "wire.h"

// ports routes/lessons.py. all handlers are stubs returning BooleanResult —
// overridden by live_modes.py for CreateParty/SetParty/SetPartyLeader (and Start
// via /api/Lives/StartLesson).

namespace routes {

using wire::json;

void register_lessons(httplib::Server& svr) {
    svr.Post("/api/Lessons/:characterBaseMasterId/CreateParty",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    svr.Post("/api/Lessons/Finish", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "FinishLessonPayload");
        pipeline::respond(res, "BooleanResult", json::object());
    });

    svr.Post("/api/Lessons/:characterBaseMasterId/SetParty",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "SetLessonPartyPayload");
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    svr.Post("/api/Lessons/:characterBaseMasterId/SetPartyLeader/:leaderPosition",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    svr.Post("/api/Lessons/:characterBaseMasterId/Start/:liveMasterId",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "StartLessonPayload");
                 // TODO consume stamina before the lesson (see server-of-dreams)
                 pipeline::respond(res, "BooleanResult", json::object());
             });
}

}  // namespace routes
