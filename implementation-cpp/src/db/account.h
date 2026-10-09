#pragma once
#include "db.h"
#include <cstdint>
#include <string>

// query builders for the accounts / hash_user_id / sequence tables.
// ports db/account/get.py + db/account/create.py 1:1.

namespace db::account {

// get.py
// next_user_id: nextval('user_id_seq') -> sequences-table UPDATE ... RETURNING value
SelectQuery next_user_id();
SelectQuery get_user_id_by_hash(const std::string& hash_user_id);
SelectQuery get_account_by_id(long long user_id);
SelectQuery get_account_by_credential(const std::string& credential);
SelectQuery get_account_by_token(const std::string& api_token);

// create.py
ExecutableQuery create_account(long long user_id, const std::string& credential,
                               const std::string& platform = "Android",
                               long long registered_at = 0, long long last_login_at = 0);
ExecutableQuery add_hash_user_id(const std::string& hash_user_id, long long user_id);
ExecutableQuery update_account_token(long long user_id, const std::string& api_token);
ExecutableQuery update_last_login(long long user_id, long long last_login_at);

}  // namespace db::account
