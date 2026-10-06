#pragma once
#include "sqlite3.h"
#include "json.hpp"
#include <string>
#include <vector>
#include <functional>

namespace db {

static constexpr int DB_VERSION = 7;
const std::string DB_NULL = "\x01__NULL__";

bool init(const std::string& path);
sqlite3* get();

bool exec(const std::string& sql);
nlohmann::json query_one(const std::string& sql);
std::vector<nlohmann::json> query_all(const std::string& sql);

// parameterized
bool exec_params(const std::string& sql, const std::vector<std::string>& params);
nlohmann::json query_one_params(const std::string& sql, const std::vector<std::string>& params);
std::vector<nlohmann::json> query_all_params(const std::string& sql, const std::vector<std::string>& params);

std::vector<int> get_release_condition_ids(int64_t user_id);
void add_release_conditions(int64_t user_id, const std::vector<int>& new_ids);

}
