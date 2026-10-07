#include "httplib_config.h"
#include "config.h"
#include "db.h"
#include "master_data.h"
#include "routes.h"
#include "platform.h"

void ssl_bypass_init();

#include <string>
#include <thread>

#ifdef __ANDROID__
#include <android/log.h>
#include <unistd.h>
#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "GridlessDreams", fmt, ##__VA_ARGS__)
#elif defined(__APPLE__)
#include <os/log.h>
#include <unistd.h>
#define LOG(fmt, ...) do { char _logbuf[1024]; snprintf(_logbuf, sizeof(_logbuf), fmt, ##__VA_ARGS__); os_log(OS_LOG_DEFAULT, "[GridlessDreams] %{public}s", _logbuf); } while(0)
#else
#include <cstdio>
#define LOG(fmt, ...) printf("[GridlessDreams] " fmt "\n", ##__VA_ARGS__)
#endif

static constexpr int SERVER_PORT = 39046;

// live/course progression bookkeeping tables (app.py lifespan _PRESERVATION_TABLES),
// adapted to SQLite: the Postgres ::jsonb casts are dropped, the jsonb column type is
// kept so the row codec round-trips them as JSON.
static const char* const PRESERVATION_TABLES[] = {
    "CREATE TABLE IF NOT EXISTS preservation_live_context ("
    "\"userId\" bigint PRIMARY KEY REFERENCES accounts(\"userId\") ON DELETE CASCADE, "
    "mode text NOT NULL, \"masterId\" integer NOT NULL, "
    "extra jsonb NOT NULL DEFAULT '{}')",
    "CREATE TABLE IF NOT EXISTS preservation_course_run ("
    "\"userId\" bigint PRIMARY KEY REFERENCES accounts(\"userId\") ON DELETE CASCADE, "
    "data jsonb NOT NULL)",
};

static void wait_for_data() {
    for (int attempt = 1; ; attempt++) {
        bool have_data = platform::file_exists("userData.json") && platform::file_exists("masterdata.json");
        if (!platform::zip_ready())
            platform::try_open_zip();
        if (have_data && platform::zip_ready()) {
            LOG("Data files + assets zip found (attempt %d)", attempt);
            return;
        }
        if (attempt % 10 == 0)
            LOG("Waiting for %s%s... (attempt %d)",
                have_data ? "" : "data files ",
                platform::zip_ready() ? "" : "assets zip",
                attempt);
#ifdef __ANDROID__
        platform::init_android();
#endif
        usleep(2000000);
    }
}

static void server_thread() {
#ifdef __ANDROID__
    platform::init_android();
#endif
    wait_for_data();

    // lifespan (app.py): config/constants -> master data -> db -> preservation tables
    LOG("Loading config + master data...");
    config::load();
    constants::load();
    master_data::load();

    LOG("Initializing database...");
    std::string db_path = platform::get_writable_dir() + "/offline.db";
    if (!db::init(db_path)) {
        LOG("DB init failed at %s", db_path.c_str());
        return;
    }
    for (const char* sql : PRESERVATION_TABLES) db::execute(db::ExecutableQuery(sql));

    std::thread(ssl_bypass_init).detach();

    LOG("Starting HTTP server...");
    httplib::Server svr;
    routes::setup(svr);

    LOG("Listening on http://127.0.0.1:%d", SERVER_PORT);
    svr.listen("127.0.0.1", SERVER_PORT);
}

__attribute__((constructor))
static void entry() {
    LOG("Library loaded");
    std::thread(server_thread).detach();
}
