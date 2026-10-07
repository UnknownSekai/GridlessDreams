#pragma once
#include <optional>
#include <string>

// Reversible user-id obfuscation (base-9 + per-digit S-boxes + padding).
// Ports helpers/user_hash.py. hash_id maps a userId to its public hashUserId
// string; unhash_id reverses it (and re-verifies via hash_id).

namespace user_hash {

std::string hash_id(long long user_id);
std::optional<long long> unhash_id(const std::string& hashed_id);

}  // namespace user_hash
