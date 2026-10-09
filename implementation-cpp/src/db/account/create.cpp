#include "db/account.h"

namespace db::account {

ExecutableQuery create_account(long long user_id, const std::string& credential,
                               const std::string& platform, long long registered_at,
                               long long last_login_at) {
    return ExecutableQuery(
        "INSERT INTO \"accounts\" "
        "(\"userId\", \"credential\", \"platform\", \"registeredAt\", \"lastLoginAt\") "
        "VALUES ($1, $2, $3, $4, $5) "
        "ON CONFLICT (\"userId\") DO NOTHING",
        user_id,
        credential,
        platform,
        registered_at,
        last_login_at);
}

ExecutableQuery add_hash_user_id(const std::string& hash_user_id, long long user_id) {
    return ExecutableQuery(
        "INSERT INTO \"hash_user_id\" (\"hashUserId\", \"userId\") VALUES ($1, $2) "
        "ON CONFLICT (\"hashUserId\") DO NOTHING",
        hash_user_id,
        user_id);
}

ExecutableQuery update_account_token(long long user_id, const std::string& api_token) {
    return ExecutableQuery(
        "UPDATE \"accounts\" SET \"apiToken\" = $1 WHERE \"userId\" = $2",
        api_token,
        user_id);
}

ExecutableQuery update_last_login(long long user_id, long long last_login_at) {
    return ExecutableQuery(
        "UPDATE \"accounts\" SET \"lastLoginAt\" = $1 WHERE \"userId\" = $2",
        last_login_at,
        user_id);
}

}  // namespace db::account
