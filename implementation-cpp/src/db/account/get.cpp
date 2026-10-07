#include "db/account.h"

namespace db::account {

// nextval('user_id_seq') -> sequences-table UPDATE ... RETURNING value (pre-increment).
SelectQuery next_user_id() {
    return SelectQuery(
        "SequenceValueModel",
        "UPDATE \"sequences\" SET \"value\" = \"value\" + 1 WHERE \"name\" = $1 RETURNING \"value\"",
        "user_id_seq");
}

SelectQuery get_user_id_by_hash(const std::string& hash_user_id) {
    return SelectQuery(
        "HashUserIdModel",
        "SELECT * FROM \"hash_user_id\" WHERE \"hashUserId\" = $1",
        hash_user_id);
}

SelectQuery get_account_by_id(int64_t user_id) {
    return SelectQuery(
        "AccountModel", "SELECT * FROM \"accounts\" WHERE \"userId\" = $1", user_id);
}

SelectQuery get_account_by_credential(const std::string& credential) {
    return SelectQuery(
        "AccountModel", "SELECT * FROM \"accounts\" WHERE \"credential\" = $1", credential);
}

SelectQuery get_account_by_token(const std::string& api_token) {
    return SelectQuery(
        "AccountModel", "SELECT * FROM \"accounts\" WHERE \"apiToken\" = $1", api_token);
}

}  // namespace db::account
