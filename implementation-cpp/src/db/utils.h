#pragma once
#include <string>

// builds the postgres dsn string. mirrors db/utils.py.
namespace db::utils {

std::string create_dsn(const std::string& host, int port, const std::string& database,
                       const std::string& username, const std::string& password);

}  // namespace db::utils
