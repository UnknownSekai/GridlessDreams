#include "db/utils.h"

namespace db::utils {

std::string create_dsn(const std::string& host, int port, const std::string& database,
                       const std::string& username, const std::string& password) {
    return "postgresql://" + username + ":" + password + "@" + host + ":" +
           std::to_string(port) + "/" + database;
}

}  // namespace db::utils
