#include "routes/comics.h"

#include "headers.h"
#include "wire.h"

// python itself stubs this endpoint (respond([]))

namespace routes {

void register_comics(httplib::Server& svr) {
    svr.Post("/api/Comics/Read/:mComicEpisodeId", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(wire::common_response("", wire::json::array()), "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    });
}

}  // namespace routes
