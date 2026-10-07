#include "routes/assets.h"

#include <optional>
#include <string>

#include "helpers/assets.h"

namespace routes {

namespace {

bool ends_with(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// (body, content_md5) -> octet-stream response carrying Content-MD5
void octet(httplib::Response& res, const assets::BodyMd5& result) {
    res.set_content(result.first, "application/octet-stream");
    res.set_header("Content-MD5", result.second);
}

}  // namespace

void register_assets(httplib::Server& svr) {
    // Notation charts + music_config, served under /production/Notations/{music}/{file}.enc.
    // This 3-segment path is matched before the generic one so the client gets the raw
    // encrypted bytes (local if downloaded, else a redirect to the real CDN).
    svr.Get(R"(/production/Notations/([^/]+)/([^/]+))",
            [](const httplib::Request& req, httplib::Response& res) {
                const std::string music_id = req.matches[1].str();
                const std::string filename = req.matches[2].str();
                if (assets::local_assets_enabled()) {
                    std::optional<assets::BodyMd5> local = assets::notation(music_id, filename);
                    if (local.has_value()) {
                        octet(res, *local);
                        return;
                    }
                }
                res.set_redirect(assets::official_notation_url(music_id, filename), 302);
            });

    // Everything the client fetches from assets-e (redirected here). kind is
    // 2d-assets|3d-assets|cri-assets, platform Android|iOS.
    //   catalog_<ver>.json.br -> brotli of the local catalog json
    //   catalog_<ver>.hash    -> spookyhash-128 of the local catalog json
    //   <group>/<name>.bundle -> local file when local_assets is on, else a redirect to
    //                            the official CDN (the mitm turns that 3xx into a passthrough)
    svr.Get(R"(/production/([^/]+)/([^/]+)/([^/]+)/(.*))",
            [](const httplib::Request& req, httplib::Response& res) {
                const std::string kind = req.matches[1].str();
                const std::string platform = req.matches[2].str();
                const std::string version = req.matches[3].str();
                const std::string filepath = req.matches[4].str();
                std::optional<assets::BodyMd5> result;
                if (ends_with(filepath, ".json.br")) {
                    result = assets::catalog_br(kind, platform);
                } else if (ends_with(filepath, ".hash")) {
                    result = assets::catalog_hash(kind, platform);
                } else {
                    if (assets::local_assets_enabled()) {
                        std::optional<assets::BodyMd5> local =
                            assets::bundle(kind, platform, filepath);
                        if (local.has_value()) {
                            octet(res, *local);
                            return;
                        }
                    }
                    res.set_redirect(assets::official_url(kind, platform, version, filepath), 302);
                    return;
                }
                if (!result.has_value()) {
                    res.status = httplib::StatusCode::NotFound_404;
                    return;
                }
                octet(res, *result);
            });
}

}  // namespace routes
