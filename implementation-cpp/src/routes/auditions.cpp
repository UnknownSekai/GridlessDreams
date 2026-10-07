#include "routes/auditions.h"

#include "headers.h"
#include "wire.h"

namespace routes {

namespace {

void set_result(httplib::Response& res, const char* result_name) {
    res.set_content(wire::common_response(result_name, wire::json::object()), "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

}  // namespace

void register_auditions(httplib::Server& svr) {
    // /api/Auditions/{auditionId}/CopyTo/{uPartyId}
    svr.Post("/api/Auditions/:auditionId/CopyTo/:uPartyId",
             [](const httplib::Request&, httplib::Response& res) { set_result(res, "BooleanResult"); });

    // /api/Auditions/{auditionId}/HighScoreParty
    svr.Get("/api/Auditions/:auditionId/HighScoreParty",
            [](const httplib::Request&, httplib::Response& res) { set_result(res, "AuditionClearParty"); });

    // /api/Auditions/{auditionId}
    svr.Get("/api/Auditions/:auditionId",
            [](const httplib::Request&, httplib::Response& res) {
                set_result(res, "AuditionClearedInformationResult");
            });
}

}  // namespace routes
