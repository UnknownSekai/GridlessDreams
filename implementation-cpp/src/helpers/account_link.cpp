#include "account_link.h"

#include <ctime>
#include <random>
#include <stdexcept>

#include "auth.h"
#include "db.h"
#include "db/user.h"
#include "password.h"
#include "user_hash.h"

namespace account_link {

namespace {

constexpr int _LINKAGE_CODE_DIGITS = 10;
constexpr int _CONFIRMATION_CODE_DIGITS = 6;

// a numeric code with no leading zero, so it always prints `digits` wide
std::string _code(int digits) {
    long long low = 1;
    for (int i = 0; i < digits - 1; ++i) low *= 10;
    std::random_device rd;
    std::uniform_int_distribution<long long> dist(0, 9 * low - 1);
    return std::to_string(low + dist(rd));
}

// the row id stored in connect_with_password.id: the last 6 digits of hash_id
long long _hash_column(long long user_id) {
    std::string h = user_hash::hash_id(user_id);
    std::string tail = h.size() >= static_cast<std::size_t>(_CONFIRMATION_CODE_DIGITS)
                           ? h.substr(h.size() - _CONFIRMATION_CODE_DIGITS)
                           : h;
    return std::stoll(tail);
}

// a linkage code no other account currently holds
std::string _unique_linkage_code(long long user_id) {
    for (int i = 0; i < 20; ++i) {
        std::string code = _code(_LINKAGE_CODE_DIGITS);
        std::optional<db::json> existing =
            db::fetchrow(db::user::get_connect_with_password_by_linkage_code(code));
        if (!existing || (*existing)["userId"].get<long long>() == user_id) return code;
    }
    throw std::runtime_error("could not allocate a unique linkage code");
}

}  // namespace

std::pair<wire::json, std::optional<long long>> register_take_over_password(
    std::optional<long long> user_id, const std::string& password) {
    if (!user_id || password.empty()) {
        return {wire::json::object(), std::nullopt};
    }
    // the linkage code is the player's durable account address -- reuse the existing code and
    // only allocate one on first registration, so re-registering does not invalidate it.
    std::optional<db::json> existing = db::fetchrow(db::user::get_connect_with_passwords(*user_id));
    std::string linkage_code;
    if (existing && !(*existing)["linkageCode"].is_null()) {
        linkage_code = (*existing)["linkageCode"].get<std::string>();
    }
    if (linkage_code.empty()) {
        linkage_code = _unique_linkage_code(*user_id);
    }
    long long row_id = _hash_column(*user_id);
    wire::json row;
    row["id"] = row_id;
    row["passwordHash"] = password::hash_password(password);
    row["linkageCode"] = linkage_code;
    db::execute(db::user::upsert_connect_with_password(*user_id, row));

    wire::json result;
    result["is_success"] = true;
    result["linkage_code"] = linkage_code;
    return {result, row_id};
}

wire::json get_confirmation_code(std::optional<long long> user_id) {
    if (!user_id) return wire::json::object();
    long long now = static_cast<long long>(std::time(nullptr));
    std::optional<db::json> existing = db::fetchrow(db::user::get_connect_with_passwords(*user_id));
    if (!existing) {
        // the client only reaches this after registering a password, so a missing row means
        // there is nothing to confirm yet
        return wire::json::object();
    }
    std::string code;
    if (!(*existing)["confirmationCode"].is_null()) {
        code = (*existing)["confirmationCode"].get<std::string>();
    }
    long long expires_at = (*existing)["confirmationExpiresAt"].get<long long>();
    if (code.empty() || expires_at <= now) {
        code = _code(_CONFIRMATION_CODE_DIGITS);
        expires_at = now + CONFIRMATION_TTL_SECONDS;
        db::execute(db::user::set_connect_with_password_confirmation(*user_id, code, expires_at));
    }
    wire::json result;
    result["confirmation_code"] = code;
    result["remaining_seconds"] = expires_at - now;
    return result;
}

wire::json get_take_over_account(const std::string& linkage_code, const std::string& password) {
    if (linkage_code.empty() || password.empty()) return wire::json::object();
    std::optional<db::json> row =
        db::fetchrow(db::user::get_connect_with_password_by_linkage_code(linkage_code));
    if (!row || (*row)["passwordHash"].is_null()) return wire::json::object();
    std::string password_hash = (*row)["passwordHash"].get<std::string>();
    if (password_hash.empty()) return wire::json::object();
    if (!password::verify_password(password, password_hash)) return wire::json::object();
    long long account_user_id = (*row)["userId"].get<long long>();
    std::optional<db::json> account = db::fetchrow(db::user::get_users(account_user_id));
    std::optional<db::json> profile = db::fetchrow(db::user::get_user_profiles(account_user_id));
    if (!account) return wire::json::object();

    wire::json result;
    result["is_success"] = true;
    result["user_id"] = user_hash::hash_id(account_user_id);
    if (profile && !(*profile)["name"].is_null()) {
        result["name"] = (*profile)["name"].get<std::string>();
    } else {
        result["name"] = nullptr;
    }
    result["rank"] = (*account)["playerRank"].get<long long>();
    result["login_token"] = auth::make_jwt(account_user_id);
    return result;
}

}  // namespace account_link
