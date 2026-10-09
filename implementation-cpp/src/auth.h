#pragma once
#include <optional>
#include <string>

#include "wire.h"

// JWT (HS256) issue/decode + the register/authenticate/login flows. ports helpers/auth.py.
// tokens carry iss='server-of-dreams' / aud='github.com/UnknownSekai/server-of-dreams'.

namespace auth {

// short-lived register token (TTL 3600s)
std::string make_jwt(long long user_id);

// authenticated session token (TTL 86400s), carrying the login day + platform
std::string make_session_jwt(long long user_id, const std::string& platform);

// verify signature + iss + aud (and exp when verify_exp), return the int uid, or
// nullopt on any failure
std::optional<long long> decode_jwt(const std::string& token, bool verify_exp = true);

// DB-backed flows. payload/result are wire::json keyed by field name; the Python
// `app` parameter is dropped (one global db). `register` is a C++ keyword -> register_.
wire::json register_(const wire::json& payload);
wire::json authenticate(const wire::json& payload);
wire::json login(const wire::json& payload = wire::json());

}  // namespace auth
