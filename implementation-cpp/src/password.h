#pragma once
#include <string>

// Argon2id password hashing for the account-link takeover password.
// Ports helpers/password.py (argon2-cffi PasswordHasher). Only hashes are
// stored, never the password. Keeps argon2-cffi's default cost parameters.

namespace password {

std::string hash_password(const std::string& password);

// Any failure -- wrong password, truncated or foreign hash -- reports false;
// the caller must not learn which part did not match.
bool verify_password(const std::string& password, const std::string& password_hash);

bool needs_rehash(const std::string& password_hash);

}  // namespace password
