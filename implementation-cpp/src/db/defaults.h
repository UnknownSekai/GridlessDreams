#pragma once
#include <string>

// seed a brand-new account's default data (ports db/defaults.py).
// create_default_user_data replays default_account.json through the db::user
// upsert_* builders for the new userId, applying the per-entity fixups.
// the Python `conn` parameter is dropped: writes go through the global db
// engine (db::execute_batch / db::execute), serialized by the caller's txn.

namespace db::defaults {

void create_default_user_data(long long user_id, const std::string& name);

}  // namespace db::defaults
