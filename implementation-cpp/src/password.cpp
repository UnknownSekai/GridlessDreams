#include "password.h"

#include <argon2.h>

#include <cstdint>
#include <cstring>
#include <random>
#include <stdexcept>

namespace password {

namespace {

// argon2-cffi PasswordHasher() defaults
constexpr uint32_t T_COST = 3;
constexpr uint32_t M_COST = 65536;
constexpr uint32_t PARALLELISM = 4;
constexpr uint32_t HASH_LEN = 32;
constexpr uint32_t SALT_LEN = 16;
constexpr uint32_t VERSION = ARGON2_VERSION_NUMBER;  // 0x13 == 19

void fill_random(uint8_t* buf, size_t n) {
    std::random_device rd;
    size_t i = 0;
    while (i < n) {
        uint32_t r = rd();
        for (int b = 0; b < 4 && i < n; ++b) {
            buf[i++] = static_cast<uint8_t>(r & 0xFF);
            r >>= 8;
        }
    }
}

// Argon2 type encoded in the hash prefix (argon2-cffi verify auto-detects it)
argon2_type type_of(const std::string& h) {
    if (h.rfind("$argon2id$", 0) == 0) return Argon2_id;
    if (h.rfind("$argon2i$", 0) == 0) return Argon2_i;
    if (h.rfind("$argon2d$", 0) == 0) return Argon2_d;
    return Argon2_id;
}

// parse the unsigned integer in `h` directly following the first occurrence of
// `marker`. returns false if the marker or a digit is missing.
bool parse_param(const std::string& h, const char* marker, long long& out) {
    size_t pos = h.find(marker);
    if (pos == std::string::npos) return false;
    pos += std::strlen(marker);
    if (pos >= h.size() || h[pos] < '0' || h[pos] > '9') return false;
    long long v = 0;
    while (pos < h.size() && h[pos] >= '0' && h[pos] <= '9') v = v * 10 + (h[pos++] - '0');
    out = v;
    return true;
}

}  // namespace

std::string hash_password(const std::string& password) {
    uint8_t salt[SALT_LEN];
    fill_random(salt, SALT_LEN);

    size_t enclen = argon2_encodedlen(T_COST, M_COST, PARALLELISM, SALT_LEN, HASH_LEN, Argon2_id);
    std::string encoded(enclen, '\0');

    int rc = argon2id_hash_encoded(T_COST, M_COST, PARALLELISM, password.data(), password.size(),
                                   salt, SALT_LEN, HASH_LEN, encoded.data(), encoded.size());
    if (rc != ARGON2_OK) throw std::runtime_error(argon2_error_message(rc));

    encoded.resize(std::strlen(encoded.c_str()));
    return encoded;
}

bool verify_password(const std::string& password, const std::string& password_hash) {
    if (password.empty() || password_hash.empty()) return false;
    int rc = argon2_verify(password_hash.c_str(), password.data(), password.size(),
                           type_of(password_hash));
    return rc == ARGON2_OK;
}

bool needs_rehash(const std::string& password_hash) {
    if (password_hash.rfind("$argon2id$", 0) != 0) return true;
    long long v = 0, m = 0, t = 0, p = 0;
    if (!parse_param(password_hash, "$v=", v)) return true;
    if (!parse_param(password_hash, "m=", m)) return true;
    if (!parse_param(password_hash, "t=", t)) return true;
    if (!parse_param(password_hash, "p=", p)) return true;

    // check_needs_rehash compares the whole Parameters tuple, which includes the
    // salt/hash byte lengths decoded from the last two `$`-separated b64 segments
    // (no padding: bytes = len*3/4)
    size_t last = password_hash.rfind('$');
    if (last == std::string::npos || last == 0) return true;
    size_t prev = password_hash.rfind('$', last - 1);
    if (prev == std::string::npos) return true;
    long long hash_len = static_cast<long long>(password_hash.size() - last - 1) * 3 / 4;
    long long salt_len = static_cast<long long>(last - prev - 1) * 3 / 4;

    return !(v == VERSION && m == M_COST && t == T_COST && p == PARALLELISM &&
             salt_len == SALT_LEN && hash_len == HASH_LEN);
}

}  // namespace password
