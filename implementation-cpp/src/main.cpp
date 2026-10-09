#include "httplib_config.h"
#include "config.h"
#include "db.h"
#include "master_data.h"
#include "routes.h"
#include "platform.h"

void ssl_bypass_init();

#include <exception>
#include <string>
#include <thread>

#ifdef __ANDROID__
#include <android/log.h>
#include <unistd.h>
#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "[UTSK GD]", fmt, ##__VA_ARGS__)
#elif defined(__APPLE__)
#include <os/log.h>
#include <unistd.h>
#define LOG(fmt, ...) do { char _logbuf[1024]; snprintf(_logbuf, sizeof(_logbuf), fmt, ##__VA_ARGS__); os_log(OS_LOG_DEFAULT, "[UTSK GD] %{public}s", _logbuf); } while(0)
#else
#include <cstdio>
#define LOG(fmt, ...) printf("[UTSK GD] " fmt "\n", ##__VA_ARGS__)
#endif

static constexpr int SERVER_PORT = 39046;

// live/course progression bookkeeping tables (app.py lifespan _PRESERVATION_TABLES),
// adapted to SQLite: the Postgres ::jsonb casts are dropped, the jsonb column type is
// kept so the row codec round-trips them as JSON
static const char* const PRESERVATION_TABLES[] = {
    "CREATE TABLE IF NOT EXISTS preservation_live_context ("
    "\"userId\" bigint PRIMARY KEY REFERENCES accounts(\"userId\") ON DELETE CASCADE, "
    "mode text NOT NULL, \"masterId\" integer NOT NULL, "
    "extra jsonb NOT NULL DEFAULT '{}')",
    "CREATE TABLE IF NOT EXISTS preservation_course_run ("
    "\"userId\" bigint PRIMARY KEY REFERENCES accounts(\"userId\") ON DELETE CASCADE, "
    "data jsonb NOT NULL)",
    // auto-play flag on the transient active-live row (db::user::create_active_live). the fresh
    // schema already carries it; this adds it to a db from an older build. SQLite has no
    // ADD COLUMN IF NOT EXISTS, so it throws once the column exists -- bring-up tolerates that
    "ALTER TABLE active_live ADD COLUMN \"isAutoPlay\" BOOLEAN NOT NULL DEFAULT false",
};

static void mount_asset_portions() {
    // server data (config is hardcoded; masterdata + episodes are bundled and, on Android,
    // extracted in init_android above), so there is nothing to wait for -- the app always boots.
    // mount the asset portion zips if the user has already dropped them; otherwise serving 404s
    // until they do (per INSTALL.md: open once to create the folder, drop the zips, reopen).
    platform::try_open_zip();
    LOG("Asset portions %s", platform::zip_ready() ? "mounted" : "not present yet");
}

static void server_thread() {
#ifdef __ANDROID__
    platform::init_android();
#endif
    mount_asset_portions();

    // install the il2cpp hooks on their own thread: they wait for libil2cpp to be fully
    // loaded themselves and are needed for asset streaming + SSL regardless of whether the
    // db or master data come up, so don't gate them on the rest of the bring-up
    std::thread(ssl_bypass_init).detach();

    // lifespan (app.py): config/constants -> master data -> db -> preservation tables
    LOG("Loading config + master data...");
    config::load();
    constants::load();
    master_data::load();

    LOG("Initializing database...");
    std::string db_path = platform::get_writable_dir() + "/offline.db";
    if (db::init(db_path)) {
        // best-effort + idempotent: the isAutoPlay ALTER throws on a db that already has the
        // column (no SQLite IF NOT EXISTS), which is expected, not fatal
        for (const char* sql : PRESERVATION_TABLES) {
            try {
                db::execute(db::ExecutableQuery(sql));
            } catch (const std::exception& e) {
                LOG("Preservation statement skipped: %s", e.what());
            }
        }
        LOG("Database ready at %s", db_path.c_str());
    } else {
        // non-fatal: still serve assets/master data so the app boots and we can diagnose
        LOG("DB init failed at %s -- continuing without persistence", db_path.c_str());
    }

    LOG("Starting HTTP server...");
    httplib::Server svr;
    routes::setup(svr);

    // log every request on arrival and every response (GS6-style). pre_routing fires before
    // route matching (incl. 404s) but also before the body is read, so the request size is logged
    // from the post-response logger where req.body is populated.
    svr.set_pre_routing_handler([](const httplib::Request& req, httplib::Response&) {
        LOG("[req] %s %s", req.method.c_str(), req.target.c_str());
        return httplib::Server::HandlerResponse::Unhandled;
    });
    svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        LOG("[resp] %s %s (%zuB req) -> %d (%zuB resp)", req.method.c_str(), req.target.c_str(),
            req.body.size(), res.status, res.body.size());
    });
    // a throwing handler otherwise returns a bare 500 with no hint; surface what() so a missing
    // bundled file or db error is diagnosable from logcat
    svr.set_exception_handler([](const httplib::Request& req, httplib::Response& res,
                                 std::exception_ptr ep) {
        std::string msg;
        try {
            std::rethrow_exception(ep);
        } catch (const std::exception& e) {
            msg = e.what();
        } catch (...) {
            msg = "unknown exception";
        }
        LOG("[error] %s %s -> %s", req.method.c_str(), req.target.c_str(), msg.c_str());
        res.status = 500;
    });

    LOG("Listening on http://127.0.0.1:%d", SERVER_PORT);
    svr.listen("127.0.0.1", SERVER_PORT);
}

__attribute__((constructor))
static void entry() {
    LOG("Library loaded");
    std::thread(server_thread).detach();
}
