#include "routes/roulette.h"

#include "headers.h"
#include "wire.h"

namespace routes {

void register_roulette(httplib::Server& svr) {
    // /api/Roulettes/Roll?mRouletteId=&rollCount=
    svr.Post("/api/Roulettes/Roll", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(wire::common_response("RouletteRollResult", wire::json::object()), "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    });
}

}  // namespace routes
