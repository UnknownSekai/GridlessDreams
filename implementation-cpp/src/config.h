#pragma once
#include "json.hpp"
#include <string>

// Deployment config (config.yml) + server-logic constants (constants.yml).
// Ports helpers/config.py and helpers/constants.py. config.yml's required keys
// are validated on load (hard-fail, like the Python RuntimeError). The host/port
// and database section are the Python public deployment -- they parse but do NOT
// drive the C++ server (it listens on 127.0.0.1:39046 and uses SQLite).

namespace config {

struct DatabaseSettings {
    int min_size = 3;
    int max_size = 20;
};

struct Database {
    std::string host;
    int port = 0;
    std::string database;
    std::string username;
    std::string password;
    DatabaseSettings settings;
};

struct Realtime {
    std::string host = "0.0.0.0";
    int port = 8443;
    std::string certfile;
    std::string keyfile;
    bool auto_start = false;
};

// Read+validate config.yml. Safe to call repeatedly; the first access lazily
// loads anyway. Throws std::runtime_error if a required key is missing.
void load();

long long   get_int(const std::string& key, long long def = 0);
std::string get_str(const std::string& key, const std::string& def = "");
bool        get_bool(const std::string& key, bool def = false);
double      get_double(const std::string& key, double def = 0.0);
bool        has(const std::string& key);

const nlohmann::json& raw();
const Database& database();
const Realtime& realtime();

}  // namespace config

namespace constants {

void load();

long long   get_int(const std::string& key, long long def = 0);
std::string get_str(const std::string& key, const std::string& def = "");
bool        get_bool(const std::string& key, bool def = false);
double      get_double(const std::string& key, double def = 0.0);
bool        has(const std::string& key);

const nlohmann::json& raw();

}  // namespace constants
