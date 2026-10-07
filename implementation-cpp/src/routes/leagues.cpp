#include "leagues.h"

#include "../headers.h"
#include "../wire.h"

// Ports routes/leagues.py. Every endpoint is a stub; no payload, query, or path
// param affects the response ([] / LeagueTopMenuInformationResult / LeagueReceiveResults).

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

void register_leagues(httplib::Server& svr) {
    svr.Post("/api/Leagues/GlobalRanking/:leagueMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/Leagues/GroupRanking/:leagueMasterId/:leagueGroupId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/Leagues/TopMenuInformation", [](const httplib::Request&, httplib::Response& res) {
        respond_empty(res, "LeagueTopMenuInformationResult");
    });

    svr.Post("/api/TripleCast/GlobalRanking/:tripleCastMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/TripleCast/GroupRanking/:tripleCastMasterId",
             [](const httplib::Request&, httplib::Response& res) { respond_empty_list(res); });

    svr.Post("/api/TripleCast/TopMenuInformation", [](const httplib::Request&, httplib::Response& res) {
        respond_empty(res, "LeagueTopMenuInformationResult");
    });

    svr.Get("/api/Leagues", [](const httplib::Request&, httplib::Response& res) {
        respond_empty(res, "LeagueReceiveResults");
    });

    svr.Get("/api/TripleCast", [](const httplib::Request&, httplib::Response& res) {
        respond_empty(res, "LeagueReceiveResults");
    });
}

}  // namespace routes
