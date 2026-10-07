#include "auth.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <ctime>
#include <random>
#include <stdexcept>
#include <vector>

#include "config.h"
#include "db.h"
#include "db/account.h"
#include "db/defaults.h"
#include "jwtcrypto.h"
#include "user_hash.h"

namespace auth {
namespace {

constexpr const char* JWT_ISS = "server-of-dreams";
constexpr const char* JWT_AUD = "github.com/UnknownSekai/server-of-dreams";
constexpr long long TOKEN_TTL = 3600;    // register token
constexpr long long SESSION_TTL = 86400; // authenticated session token

std::string secret() { return config::get_str("jwt_secret"); }

// {"typ":"JWT","alg":"HS256"} serialized with PyJWT's sort_headers=True -> sorted keys.
std::string header_segment() {
    return jwtcrypto::base64url_encode(std::string("{\"alg\":\"HS256\",\"typ\":\"JWT\"}"));
}

// jwt.encode(claims, secret, algorithm="HS256"): compact json (insertion order),
// ensure_ascii like python json.dumps, then HS256 over header.payload.
std::string encode_hs256(const wire::json& claims) {
    std::string head = header_segment();
    std::string body = jwtcrypto::base64url_encode(claims.dump(-1, ' ', true));
    std::string signing_input = head + "." + body;
    auto mac = jwtcrypto::hmac_sha256(secret(), signing_input);
    return signing_input + "." + jwtcrypto::base64url_encode(mac.data(), mac.size());
}

// JST (UTC+9); roll back to the previous day before 05:00; format fixed at 05:00:00.
std::string login_day() {
    std::time_t jst = std::time(nullptr) + 9 * 3600;
    std::tm tmv = *std::gmtime(&jst);
    if (tmv.tm_hour < 5) {
        jst -= 24 * 3600;
        tmv = *std::gmtime(&jst);
    }
    char buf[32];
    std::strftime(buf, sizeof(buf), "%m/%d/%Y 05:00:00", &tmv);
    return std::string(buf);
}

// secrets.token_urlsafe(nbytes): base64url of nbytes random bytes, no padding.
std::string token_urlsafe(size_t nbytes) {
    std::random_device rd;
    std::vector<uint8_t> bytes(nbytes);
    for (auto& b : bytes) b = static_cast<uint8_t>(rd() & 0xFF);
    return jwtcrypto::base64url_encode(bytes.data(), bytes.size());
}

// GameVersions enum member name (used verbatim in the session token pf/gv claims).
std::string game_version_name(long long v) {
    switch (v) {
        case 1: return "AppStore";
        case 2: return "GooglePlay";
        default: return "Unknown";
    }
}

// _new_user_id: fetch nextval('user_id_seq') AS value.
long long new_user_id() {
    auto row = db::fetchrow(db::account::next_user_id());
    if (!row) throw std::runtime_error("could not allocate a user id");
    return row->at("value").get<long long>();
}

long long int_claim(const wire::json& v) {
    if (v.is_string()) return std::stoll(v.get<std::string>());
    return v.get<long long>();
}

}  // namespace

std::string make_jwt(long long user_id) {
    long long now = static_cast<long long>(std::time(nullptr));
    wire::json claims;
    claims["uid"] = std::to_string(user_id);
    claims["nbf"] = now;
    claims["exp"] = now + TOKEN_TTL;
    claims["iat"] = now;
    claims["iss"] = JWT_ISS;
    claims["aud"] = JWT_AUD;
    return encode_hs256(claims);
}

std::string make_session_jwt(long long user_id, const std::string& platform) {
    long long now = static_cast<long long>(std::time(nullptr));
    wire::json claims;
    claims["uid"] = std::to_string(user_id);
    claims["lc"] = "1";
    claims["pf"] = platform;
    claims["gv"] = platform;
    claims["ld"] = login_day();
    claims["nbf"] = now;
    claims["exp"] = now + SESSION_TTL;
    claims["iat"] = now;
    claims["iss"] = JWT_ISS;
    claims["aud"] = JWT_AUD;
    return encode_hs256(claims);
}

std::optional<long long> decode_jwt(const std::string& token, bool verify_exp) {
    try {
        size_t p1 = token.find('.');
        if (p1 == std::string::npos) return std::nullopt;
        size_t p2 = token.find('.', p1 + 1);
        if (p2 == std::string::npos) return std::nullopt;
        if (token.find('.', p2 + 1) != std::string::npos) return std::nullopt;

        std::string head_b64 = token.substr(0, p1);
        std::string body_b64 = token.substr(p1 + 1, p2 - p1 - 1);
        std::string sig_b64 = token.substr(p2 + 1);
        std::string signing_input = token.substr(0, p2);

        bool ok = false;
        std::string head_json = jwtcrypto::base64url_decode(head_b64, &ok);
        if (!ok) return std::nullopt;
        wire::json header = wire::json::parse(head_json);
        if (!header.contains("alg") || !header["alg"].is_string() ||
            header["alg"].get<std::string>() != "HS256")
            return std::nullopt;

        auto mac = jwtcrypto::hmac_sha256(secret(), signing_input);
        if (jwtcrypto::base64url_encode(mac.data(), mac.size()) != sig_b64) return std::nullopt;

        std::string body_json = jwtcrypto::base64url_decode(body_b64, &ok);
        if (!ok) return std::nullopt;
        wire::json claims = wire::json::parse(body_json);

        double now = static_cast<double>(std::time(nullptr));
        if (claims.contains("iat") && static_cast<double>(int_claim(claims["iat"])) > now)
            return std::nullopt;
        if (claims.contains("nbf") && static_cast<double>(int_claim(claims["nbf"])) > now)
            return std::nullopt;
        if (verify_exp && claims.contains("exp") &&
            static_cast<double>(int_claim(claims["exp"])) <= now)
            return std::nullopt;

        // iss
        if (!claims.contains("iss")) return std::nullopt;
        if (!claims["iss"].is_string() || claims["iss"].get<std::string>() != JWT_ISS)
            return std::nullopt;

        // aud (audience=JWT_AUD, non-strict)
        if (!claims.contains("aud") || claims["aud"].is_null()) return std::nullopt;
        const wire::json& aud = claims["aud"];
        std::vector<std::string> aud_claims;
        if (aud.is_string()) {
            if (aud.get<std::string>().empty()) return std::nullopt;
            aud_claims.push_back(aud.get<std::string>());
        } else if (aud.is_array()) {
            if (aud.empty()) return std::nullopt;
            for (const auto& c : aud) {
                if (!c.is_string()) return std::nullopt;
                aud_claims.push_back(c.get<std::string>());
            }
        } else {
            return std::nullopt;
        }
        if (std::find(aud_claims.begin(), aud_claims.end(), std::string(JWT_AUD)) ==
            aud_claims.end())
            return std::nullopt;

        return int_claim(claims.at("uid"));
    } catch (...) {
        return std::nullopt;
    }
}

wire::json register_(const wire::json& payload) {
    std::string name;
    if (payload.contains("name") && payload["name"].is_string())
        name = payload["name"].get<std::string>();

    std::string token;
    {
        auto tx = db::transaction();
        long long user_id = new_user_id();
        token = make_jwt(user_id);
        long long now = static_cast<long long>(std::time(nullptr));
        // lastLoginAt=0 marks "never logged in" so the first Login can emit the full present
        db::execute(db::account::create_account(user_id, token_urlsafe(24), "Android", now, 0));
        db::execute(db::account::add_hash_user_id(user_hash::hash_id(user_id), user_id));
        db::execute(db::account::update_account_token(user_id, token));
        db::defaults::create_default_user_data(user_id, name);
        tx.commit();
    }
    wire::json result;
    result["token"] = token;
    result["error_type"] = 0;
    return result;
}

wire::json authenticate(const wire::json& payload) {
    // the login_token is a token we issued earlier; accept it even if expired.
    std::string login_token;
    if (payload.contains("login_token") && payload["login_token"].is_string())
        login_token = payload["login_token"].get<std::string>();
    std::optional<long long> user_id = decode_jwt(login_token, false);
    if (!user_id) {
        wire::json r;
        r["token"] = "";
        r["ban_level"] = 0;
        r["warned_until"] = nullptr;
        return r;
    }
    long long gv = payload.contains("game_version") && payload["game_version"].is_number()
                       ? payload["game_version"].get<long long>()
                       : 0;
    std::string platform = game_version_name(gv);
    std::string token = make_session_jwt(*user_id, platform);
    long long ban_level = 0;
    auto account = db::fetchrow(db::account::get_account_by_id(*user_id));
    if (account) ban_level = account->at("banLevel").get<long long>();
    db::execute(db::account::update_account_token(*user_id, token));

    wire::json r;
    r["token"] = token;
    r["ban_level"] = ban_level;
    r["warned_until"] = nullptr;
    return r;
}

wire::json login(const wire::json& /*payload*/) {
    wire::json r;
    r["invalided_star_passes"] = wire::json::array();
    r["login_pass_notification"] = 0;
    r["is_approaching_login_pass_invalided"] = false;
    r["invalided_item_master_ids"] = wire::json::array();
    r["approaching_item_master_ids"] = wire::json::array();
    r["story_event_point_exchange_result"] = wire::json::array();
    r["invalided_buff_item_master_ids"] = wire::json::array();
    return r;
}

}  // namespace auth
