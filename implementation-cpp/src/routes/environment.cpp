#include "environment.h"

#include <optional>
#include <string>

#include "../config.h"
#include "../headers.h"
#include "../helpers/environment.h"
#include "../wire.h"

// Ports routes/environment.py. The client's first, anonymous call on boot.

namespace routes {

namespace {

using wire::json;

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

}  // namespace

void register_environment(httplib::Server& svr) {
    // /api/Environment?applicationVersion=&gameVersion=
    svr.Post("/api/Environment", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<std::string> application_version;
        if (req.has_param("applicationVersion")) application_version = req.get_param_value("applicationVersion");
        // gameVersion is accepted but unused (mirrors the Python signature)
        std::optional<long long> game_version;
        if (req.has_param("gameVersion")) game_version = std::stoll(req.get_param_value("gameVersion"));
        (void)game_version;

        if (!application_version.has_value() || *application_version != config::get_str("server_version")) {
            send(res, wire::common_response("EnvironmentResult", json::object()));
            return;
        }
        send(res, wire::common_response("EnvironmentResult", environment::environment()));
    });

    // /api/Environment/Ping
    svr.Get("/api/Environment/Ping", [](const httplib::Request&, httplib::Response& res) {
        // update/maintenance check ping -> always healthy on this server
        json result = json::object();
        result["is_success"] = true;
        send(res, wire::common_response("BooleanResult", result));
    });
}

}  // namespace routes
