#include "config.h"
#include "platform.h"
#include "miniyaml.h"

#include <algorithm>
#include <mutex>
#include <stdexcept>
#include <vector>

#if defined(__ANDROID__)
#include <android/log.h>
#elif defined(__APPLE__)
#include <os/log.h>
#else
#include <cstdio>
#endif

namespace {

void log_error(const std::string& msg) {
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_ERROR, "[UTSK GD]", "%s", msg.c_str());
#elif defined(__APPLE__)
    os_log_error(OS_LOG_DEFAULT, "[UTSK GD] %{public}s", msg.c_str());
#else
    std::fprintf(stderr, "[UTSK GD] %s\n", msg.c_str());
#endif
}

[[noreturn]] void fatal(const std::string& msg) {
    log_error(msg);
    throw std::runtime_error(msg);
}

// scalar coercions mirroring Python int()/str()/bool()/float() on config values
long long as_int(const nlohmann::json& v, long long def) {
    if (v.is_number_integer() || v.is_number_unsigned()) return v.get<long long>();
    if (v.is_number_float()) return static_cast<long long>(v.get<double>());
    if (v.is_boolean()) return v.get<bool>() ? 1 : 0;
    if (v.is_string()) { try { return std::stoll(v.get<std::string>()); } catch (...) { return def; } }
    return def;
}

double as_double(const nlohmann::json& v, double def) {
    if (v.is_number()) return v.get<double>();
    if (v.is_boolean()) return v.get<bool>() ? 1.0 : 0.0;
    if (v.is_string()) { try { return std::stod(v.get<std::string>()); } catch (...) { return def; } }
    return def;
}

bool as_bool(const nlohmann::json& v, bool def) {
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number()) return v.get<double>() != 0.0;
    if (v.is_string()) return !v.get<std::string>().empty();
    return def;
}

std::string as_str(const nlohmann::json& v, const std::string& def) {
    if (v.is_string()) return v.get<std::string>();
    if (v.is_number_integer() || v.is_number_unsigned()) return std::to_string(v.get<long long>());
    if (v.is_number_float()) return std::to_string(v.get<double>());
    if (v.is_boolean()) return v.get<bool>() ? "True" : "False";
    return def;
}

nlohmann::json load_yaml(const std::string& path) {
    nlohmann::json parsed = miniyaml::parse(platform::read_file(path));
    if (!parsed.is_object()) parsed = nlohmann::json::object();
    return parsed;
}

}  // namespace

namespace config {

namespace {

std::mutex g_mu;
bool g_loaded = false;
nlohmann::json g_config;
Database g_database;
Realtime g_realtime;

const char* const REQUIRED[] = {
    "host", "port", "api_endpoint", "server_version", "asset_version",
    "master_data_publish_timestamp", "feature_maintenance_flags", "maintenance",
    "maintenance_message", "local_assets", "stamina_recovery_seconds",
    "master_data_url", "static_content_url", "asset_url", "photo_content_url",
    "multi_real_time_server_url", "external_payment_url", "database", "jwt_secret",
};

void parse_database() {
    auto it = g_config.find("database");
    if (it == g_config.end() || !it->is_object()) return;
    const auto& d = *it;
    g_database.host = d.value("host", std::string());
    g_database.port = d.value("port", 0);
    g_database.database = d.value("database", std::string());
    g_database.username = d.value("username", std::string());
    g_database.password = d.value("password", std::string());
    auto sit = d.find("settings");
    if (sit != d.end() && sit->is_object()) {
        g_database.settings.min_size = sit->value("min_size", g_database.settings.min_size);
        g_database.settings.max_size = sit->value("max_size", g_database.settings.max_size);
    }
}

void parse_realtime() {
    auto it = g_config.find("realtime");
    if (it == g_config.end() || !it->is_object()) return;
    const auto& r = *it;
    g_realtime.host = r.value("host", g_realtime.host);
    g_realtime.port = r.value("port", g_realtime.port);
    g_realtime.certfile = r.value("certfile", g_realtime.certfile);
    g_realtime.keyfile = r.value("keyfile", g_realtime.keyfile);
    g_realtime.auto_start = r.value("auto_start", g_realtime.auto_start);
}

void ensure_loaded() {
    std::lock_guard<std::mutex> lk(g_mu);
    if (g_loaded) return;

    // offline build: every value is hardcoded, so there is no config.yml to ship or break.
    // the DB is a local SQLite file (db.cpp), so the Python psql `database` section is omitted
    // -- it was never read here. all client-facing URLs point at the in-process server, and
    // grant_music_tickets is on so songs unlock freely.
    g_config = {
        {"host", "127.0.0.1"},
        {"port", 39046},
        {"server_version", "2.31.3"},
        {"asset_version", "1.96.0"},
        {"master_data_publish_timestamp", 1784297595},
        {"feature_maintenance_flags", "0"},
        {"maintenance", false},
        {"maintenance_message", ""},
        {"stamina_recovery_seconds", 80},
        {"local_assets", true},
        {"grant_music_tickets", true},
        {"api_endpoint", "http://127.0.0.1:39046"},
        {"master_data_url", "http://127.0.0.1:39046/master-data/production"},
        {"static_content_url", "http://127.0.0.1:39046/production/static-assets"},
        {"asset_url", "http://127.0.0.1:39046/production"},
        {"photo_content_url", "http://127.0.0.1:39046"},
        {"multi_real_time_server_url", "http://127.0.0.1:39046"},
        {"external_payment_url", "http://127.0.0.1:39046/game/web-shop"},
        {"jwt_secret", "gridlessdreams-offline-local"},
    };
    g_loaded = true;
}

const nlohmann::json* find(const std::string& key) {
    ensure_loaded();
    auto it = g_config.find(key);
    return it == g_config.end() ? nullptr : &*it;
}

}  // namespace

void load() { ensure_loaded(); }

long long get_int(const std::string& key, long long def) {
    const auto* v = find(key);
    return v ? as_int(*v, def) : def;
}

std::string get_str(const std::string& key, const std::string& def) {
    const auto* v = find(key);
    return v ? as_str(*v, def) : def;
}

bool get_bool(const std::string& key, bool def) {
    const auto* v = find(key);
    return v ? as_bool(*v, def) : def;
}

double get_double(const std::string& key, double def) {
    const auto* v = find(key);
    return v ? as_double(*v, def) : def;
}

bool has(const std::string& key) {
    ensure_loaded();
    return g_config.contains(key);
}

const nlohmann::json& raw() { ensure_loaded(); return g_config; }
const Database& database() { ensure_loaded(); return g_database; }
const Realtime& realtime() { ensure_loaded(); return g_realtime; }

}  // namespace config

namespace constants {

namespace {

std::mutex g_mu;
bool g_loaded = false;
nlohmann::json g_constants;

void ensure_loaded() {
    std::lock_guard<std::mutex> lk(g_mu);
    if (g_loaded) return;
    g_constants = {
        {"rank_xp_per_stamina", 4.5},
        {"lesson_star_points", 250},
        {"lesson_rank_xp", 1500},
        {"course_free_attempts", 2},
        {"waive_missing_course_tickets", true},
        {"unlimited_attempts", false},
    };
    g_loaded = true;
}

const nlohmann::json* find(const std::string& key) {
    ensure_loaded();
    auto it = g_constants.find(key);
    return it == g_constants.end() ? nullptr : &*it;
}

}  // namespace

void load() { ensure_loaded(); }

long long get_int(const std::string& key, long long def) {
    const auto* v = find(key);
    return v ? as_int(*v, def) : def;
}

std::string get_str(const std::string& key, const std::string& def) {
    const auto* v = find(key);
    return v ? as_str(*v, def) : def;
}

bool get_bool(const std::string& key, bool def) {
    const auto* v = find(key);
    return v ? as_bool(*v, def) : def;
}

double get_double(const std::string& key, double def) {
    const auto* v = find(key);
    return v ? as_double(*v, def) : def;
}

bool has(const std::string& key) {
    ensure_loaded();
    return g_constants.contains(key);
}

const nlohmann::json& raw() { ensure_loaded(); return g_constants; }

}  // namespace constants
