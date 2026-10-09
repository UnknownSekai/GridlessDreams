#include "routes/data.h"

#include <optional>
#include <string>

#include "helpers/daily.h"
#include "helpers/music_unlock.h"
#include "helpers/user_data.h"
#include "master_data.h"
#include "pipeline.h"

// ports routes/data.py plus the /master-data/production blob handler from app.py

namespace routes {

void register_data(httplib::Server& svr) {
    // /api/data/master
    svr.Get("/api/data/master", [](const httplib::Request&, httplib::Response& res) {
        pipeline::respond(res, "MasterDataManifest", master_data::manifest());
    });

    // /api/data/user
    svr.Get("/api/data/user", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        daily::refresh_daily_limits(user_id);
        music_unlock::ensure_default_music(user_id);
        pipeline::respond_union_result(res, user_data::user_data(user_id));
    });
}

// master-data blob the client fetches from assets-e (redirected here): repacked from the
// masterdata/*.json tables. 404s (-> redirect falls back) until they're unpacked. this (.*)
// catch-all must be registered after every specific /master-data/production/ route (e.g. the
// episodes scenes/*.bin route), since cpp-httplib is first-match-wins -- see register_master_data_blob
void register_master_data_blob(httplib::Server& svr) {
    svr.Get(R"(/master-data/production/(.*))", [](const httplib::Request&, httplib::Response& res) {
        const std::string* db = master_data::db_blob();
        if (db == nullptr) {
            res.status = httplib::StatusCode::NotFound_404;
            return;
        }
        res.set_content(*db, "application/octet-stream");
    });
}

}  // namespace routes
