#include "account.h"

#include <optional>
#include <stdexcept>
#include <string>

#include "../auth.h"
#include "../db.h"
#include "../db/user.h"
#include "../headers.h"
#include "../helpers/account_link.h"
#include "../helpers/user_data.h"
#include "../unions.h"
#include "../wire.h"

// Ports routes/account.py. register/auth (JWT), takeover + password link and
// birthdate. Stubs stay stubs (connect/delete/disconnect/transition/current).

namespace routes {

namespace {

using wire::json;

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

// respond(result, present=...) -> common envelope.
void respond(httplib::Response& res, const char* result_name, const json& result,
             const json& present = json::array()) {
    send(res, wire::common_response(result_name, result, json::array(), present));
}

// respond(Model()) -> empty result entity, no present.
void respond_empty(httplib::Response& res, const char* result_name) {
    respond(res, result_name, json::object());
}

// days since the unix epoch for a civil date (proleptic gregorian).
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

long long floor_div(long long a, long long b) {
    long long q = a / b, r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) --q;
    return q;
}

// epoch microseconds of an iso-8601 string; mirrors datetime.fromisoformat after
// the Z->+00:00 replacement (naive -> utc). false for anything not a valid date(+time).
bool iso_epoch_micros(const std::string& s, long long& out) {
    size_t i = 0, n = s.size();
    auto read = [&](int len, int& v) -> bool {
        v = 0;
        int k = 0;
        for (; k < len && i < n && s[i] >= '0' && s[i] <= '9'; ++k, ++i) v = v * 10 + (s[i] - '0');
        return k == len;
    };
    int year, mon, day;
    if (!read(4, year)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, mon)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, day)) return false;

    int hh = 0, mm = 0, ss = 0;
    long long tz = 0, frac_us = 0;
    if (i < n) {
        ++i;  // date/time separator
        if (!read(2, hh)) return false;
        if (i >= n || s[i] != ':') return false;
        ++i;
        if (!read(2, mm)) return false;
        if (i < n && s[i] == ':') {
            ++i;
            if (!read(2, ss)) return false;
        }
        if (i < n && (s[i] == '.' || s[i] == ',')) {
            ++i;
            long long f = 0;
            int fd = 0;
            while (i < n && s[i] >= '0' && s[i] <= '9') {
                if (fd < 6) {
                    f = f * 10 + (s[i] - '0');
                    ++fd;
                }
                ++i;
            }
            while (fd < 6) {
                f *= 10;
                ++fd;
            }
            frac_us = f;
        }
        if (i < n) {
            char c = s[i];
            if (c == 'Z' || c == 'z') {
                ++i;
            } else if (c == '+' || c == '-') {
                int sign = (c == '-') ? -1 : 1;
                ++i;
                int oh = 0, om = 0, os_ = 0;
                if (!read(2, oh)) return false;
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, om);
                } else {
                    read(2, om);
                }
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, os_);
                }
                tz = sign * (oh * 3600LL + om * 60LL + os_);
            }
        }
    }
    long long secs = days_from_civil(year, static_cast<unsigned>(mon), static_cast<unsigned>(day)) *
                         86400LL +
                     hh * 3600LL + mm * 60LL + ss - tz;
    out = secs * 1000000LL + frac_us;
    return true;
}

// helpers.msgpack.iso_to_micros: a wire DateTime -> the epoch micros the db stores.
// default(DateTime) ("") means "unset" -> none. throws std::invalid_argument when the
// string is non-empty but unparseable (python's fromisoformat raises ValueError).
std::optional<long long> iso_to_micros(const std::string& s) {
    if (s.empty()) return std::nullopt;
    long long total_us = 0;
    if (!iso_epoch_micros(s, total_us)) throw std::invalid_argument("invalid isoformat string");
    if (floor_div(total_us, 1000000LL) == wire::DATETIME_MIN_SECONDS) return std::nullopt;
    return total_us;
}

}  // namespace

void register_account(httplib::Server& svr) {
    svr.Post("/api/Account/Authenticate", [](const httplib::Request& req, httplib::Response& res) {
        json payload = wire::read_request(req.body, "AuthenticatePayload");
        respond(res, "AuthenticateResult", auth::authenticate(payload));
    });

    svr.Post("/api/Account/ConnectAccount", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "AccountConnectPayload");
        respond_empty(res, "BooleanResult");
    });

    svr.Post("/api/Account/Delete",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "AccountDeletionResult"); });

    svr.Post("/api/Account/DisconnectAccount/:provider",
             [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "BooleanResult"); });

    svr.Post("/api/Account/GetConfirmationCode", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        respond(res, "TimedConfirmationCode", account_link::get_confirmation_code(user_id));
    });

    svr.Get("/api/Account/GetCurrentUserData",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "UserResult"); });

    svr.Get("/api/Account/GetPrecedenceTransitionToken",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "TransitionTokenResult"); });

    svr.Get("/api/Account/GetRoootTransitionToken",
            [](const httplib::Request&, httplib::Response& res) { respond_empty(res, "TransitionTokenResult"); });

    svr.Post("/api/Account/GetTakeOverAccount", [](const httplib::Request& req, httplib::Response& res) {
        json payload = wire::read_request(req.body, "TakeOverAccountPayload");
        if (payload.is_null()) {
            respond_empty(res, "TakeOverAccountResult");
            return;
        }
        // unauthenticated on purpose: this is the fresh-install entry point
        const std::string linkage_code = payload.value("linkage_code", std::string());
        const std::string password = payload.value("password", std::string());
        respond(res, "TakeOverAccountResult",
                account_link::get_take_over_account(linkage_code, password));
    });

    svr.Post("/api/Account/Register", [](const httplib::Request& req, httplib::Response& res) {
        json payload = wire::read_request(req.body, "RegisterPayload");
        respond(res, "AccountRegistResult", auth::register_(payload));
    });

    svr.Post("/api/Account/RegisterTakeOverPassword", [](const httplib::Request& req, httplib::Response& res) {
        json payload = wire::read_request(req.body, "RegisterTakeOverPasswordPayload");
        std::optional<long long> user_id = user_data::current_user_id(req);
        std::string password;
        if (!payload.is_null()) password = payload.value("password", std::string());
        std::pair<json, std::optional<long long>> result =
            account_link::register_take_over_password(user_id, password);
        // the captured response echoes the ConnectWithPassword row so the client's local
        // copy reflects the id it now holds. built from the union key directly: data_object()
        // would drop the aliased id and echo a zero.
        json present = json::array();
        if (result.second.has_value()) {
            present.push_back(json::array(
                {unions::idata_object_key("ConnectWithPassword"), json::array({*result.second})}));
        }
        respond(res, "TakeOverCodeResult", result.first, present);
    });

    svr.Post("/api/Account/TakeOverWithAccountConnect", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "AccountConnectPayload");
        respond_empty(res, "TakeOverAccountResult");
    });

    svr.Post("/api/Account/UpdateBirthDate", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        json payload = wire::read_request(req.body, "RegisterBirthDayPayload");
        if (payload.is_null() || !user_id.has_value()) {
            respond_empty(res, "BooleanResult");
            return;
        }
        std::optional<long long> birth_date;
        try {
            birth_date = iso_to_micros(payload.value("birth_date", std::string()));
        } catch (const std::invalid_argument&) {
            respond_empty(res, "BooleanResult");
            return;
        }
        db::execute(db::user::update_birth_date(*user_id, birth_date));
        std::optional<json> preference = db::fetchrow(db::user::get_user_preferences(*user_id));
        // echo the row back so the client's UserPreference reflects the stored date
        json present = json::array();
        if (preference.has_value()) {
            present.push_back(user_data::data_object("UserPreference", *preference));
        }
        json result = json::object();
        result["is_success"] = true;
        respond(res, "BooleanResult", result, present);
    });
}

}  // namespace routes
