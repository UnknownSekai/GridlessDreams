#include "master_data.h"

#include <msgpack.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "config.h"
#include "generated/master_spec_generated.h"
#include "generated/wire_keys_generated.h"
#include "mastermemory.h"
#include "platform.h"
#include "wire.h"

namespace master_data {
namespace {

std::map<std::string, std::vector<json>> g_tables;

std::mutex g_blob_mutex;
std::string g_blob;
bool g_blob_built = false;

// the service ended 2026-09-29; content meant to run "until the end" carries one of these two
// boundary end dates (23:00 JST / 14:00 JST daily-reset reps). we lift them to the game's
// permanent sentinel so content stays available.
constexpr long long SERVICE_END_A = 1790658000LL;  // 2026-09-29T05:00:00Z
constexpr long long SERVICE_END_B = 1790690400LL;  // 2026-09-29T14:00:00Z

long long floor_div(long long a, long long b) {
    long long q = a / b, r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) --q;
    return q;
}

// Howard Hinnant's civil<->days algorithms (days relative to 1970-01-01)
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

struct Ymd {
    long long y;
    unsigned m;
    unsigned d;
};
Ymd civil_from_days(long long z) {
    z += 719468;
    const long long era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const long long y = static_cast<long long>(yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    const unsigned d = doy - (153 * mp + 2) / 5 + 1;
    const unsigned m = mp + (mp < 10 ? 3 : -9);
    return {y + (m <= 2), m, d};
}

// epoch seconds of an ISO-8601 string; mirrors datetime.fromisoformat (naive -> UTC, Z == +00:00).
// returns false for anything that is not a valid date(+time) so it can never match a boundary.
bool iso_epoch_seconds(const std::string& s, long long& out) {
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
    long long tz = 0;
    if (i < n) {
        ++i;  // date/time separator (T or space)
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
            while (i < n && s[i] >= '0' && s[i] <= '9') ++i;  // fractional seconds: irrelevant to the guard
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
    long long days = days_from_civil(year, static_cast<unsigned>(mon), static_cast<unsigned>(day));
    out = days * 86400LL + hh * 3600LL + mm * 60LL + ss - tz;
    return true;
}

const master_spec::ModelDefaults* find_defaults(const char* model) {
    static const std::unordered_map<std::string_view, const master_spec::ModelDefaults*> table = [] {
        std::unordered_map<std::string_view, const master_spec::ModelDefaults*> m;
        m.reserve(static_cast<size_t>(master_spec::MODEL_DEFAULTS_COUNT) * 2);
        for (int i = 0; i < master_spec::MODEL_DEFAULTS_COUNT; ++i)
            m.emplace(std::string_view(master_spec::MODEL_DEFAULTS[i].model), &master_spec::MODEL_DEFAULTS[i]);
        return m;
    }();
    auto it = table.find(std::string_view(model));
    return it == table.end() ? nullptr : it->second;
}

// make a parsed JSON row pack byte-identically to the pydantic model: remap id->id_, inject the
// non-zero enum defaults for omitted fields, and recurse into nested models
void normalize(const char* model, json& obj) {
    const wire::ModelSpec* m = wire::find_model(model);
    if (!m || !obj.is_object()) return;

    bool has_id_field = false;
    for (int i = 0; i < m->count; ++i)
        if (std::strcmp(m->fields[i].fn, "id_") == 0) {
            has_id_field = true;
            break;
        }
    if (has_id_field) {
        auto it = obj.find("id");
        if (it != obj.end()) {
            json v = std::move(*it);
            obj.erase("id");
            obj["id_"] = std::move(v);
        }
    }

    if (const master_spec::ModelDefaults* d = find_defaults(model)) {
        for (int i = 0; i < d->count; ++i)
            if (obj.find(d->defs[i].fn) == obj.end()) obj[d->defs[i].fn] = d->defs[i].ival;
    }

    for (int i = 0; i < m->count; ++i) {
        const wire::FieldSpec& f = m->fields[i];
        if (std::strcmp(f.kind, "model") != 0) continue;
        auto it = obj.find(f.fn);
        if (it == obj.end() || it->is_null()) continue;
        if (f.is_array) {
            if (it->is_array())
                for (auto& el : *it)
                    if (el.is_object()) normalize(f.base, el);
        } else if (it->is_object()) {
            normalize(f.base, *it);
        }
    }
}

void extend_walk(json& v) {
    if (v.is_array()) {
        for (auto& e : v) extend_walk(e);
    } else if (v.is_object()) {
        for (auto it = v.begin(); it != v.end(); ++it) {
            json& child = it.value();
            if (child.is_string()) {
                // the boundary-value test is the real guard; the key name just narrows it
                if (it.key().find("end") != std::string::npos) {
                    long long sec;
                    if (iso_epoch_seconds(child.get_ref<const std::string&>(), sec) &&
                        (sec == SERVICE_END_A || sec == SERVICE_END_B)) {
                        child = std::string("2100-01-01T00:00:00+00:00");
                    }
                }
            } else if (child.is_array() || child.is_object()) {
                extend_walk(child);
            }
        }
    }
}

void extend_end_dates() {
    for (auto& kv : g_tables)
        for (json& row : kv.second) extend_walk(row);
}

}  // namespace

void load() {
    g_tables.clear();
    const size_t n = sizeof(master_spec::TABLES) / sizeof(master_spec::TABLES[0]);
    for (size_t i = 0; i < n; ++i) {
        const master_spec::TableSpec& ts = master_spec::TABLES[i];
        std::vector<json>& rows = g_tables[ts.header_name];  // every table exists, even if empty
        std::string text = platform::read_file(std::string("_data/masterdata/") + ts.header_name + ".json");
        if (text.empty()) continue;
        json arr = json::parse(text, nullptr, false);
        if (!arr.is_array()) continue;
        rows.reserve(arr.size());
        for (auto& row : arr) {
            if (row.is_object()) normalize(ts.row_model, row);
            rows.push_back(std::move(row));
        }
    }
    extend_end_dates();

    std::lock_guard<std::mutex> lk(g_blob_mutex);
    g_blob_built = false;
    g_blob.clear();
}

const std::vector<json>& table(const std::string& header_name) {
    static const std::vector<json> empty;
    auto it = g_tables.find(header_name);
    return it != g_tables.end() ? it->second : empty;
}

json manifest() {
    long long ts = config::get_int("master_data_publish_timestamp");
    std::string version = std::to_string(ts) + "_" + std::to_string(ts);
    Ymd ymd = civil_from_days(floor_div(ts, 86400LL));
    char daybuf[16];
    std::snprintf(daybuf, sizeof(daybuf), "%04lld-%02u-%02u", ymd.y, ymd.m, ymd.d);
    json m = json::object();
    m["uri"] = std::string(daybuf) + "/mastermemory_" + version + ".db";
    m["sas_token"] = "";
    m["version"] = version;
    m["publish_timestamp"] = ts;
    return m;
}

const std::string* db_blob() {
    std::lock_guard<std::mutex> lk(g_blob_mutex);
    if (g_blob_built) return &g_blob;

    std::map<std::string, std::string> raw_by_name;
    const std::vector<json> empty;
    bool any = false;
    const size_t n = sizeof(master_spec::TABLES) / sizeof(master_spec::TABLES[0]);
    for (size_t i = 0; i < n; ++i) {
        const master_spec::TableSpec& ts = master_spec::TABLES[i];
        auto tit = g_tables.find(ts.header_name);
        const std::vector<json>& rows = (tit != g_tables.end()) ? tit->second : empty;
        msgpack::sbuffer buf;
        msgpack::packer<msgpack::sbuffer> pk(&buf);
        pk.pack_array(static_cast<uint32_t>(rows.size()));
        for (const json& row : rows) wire::to_wire(pk, ts.row_model, row);
        raw_by_name[ts.header_name] = std::string(buf.data(), buf.size());
        if (!rows.empty()) any = true;
    }

    if (!any) return nullptr;  // not memoized: retry until a table is populated (python `if any(...)`)
    g_blob = mastermemory::pack(raw_by_name);
    g_blob_built = true;
    return &g_blob;
}

}  // namespace master_data
