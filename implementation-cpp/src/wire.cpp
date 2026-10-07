#include "wire.h"

#include <lz4.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "generated/wire_keys_generated.h"

namespace wire {

namespace {

using mtype = msgpack::type::object_type;
using tsval = std::pair<int64_t, uint32_t>;

bool is_int_base(const char* b) {
    static const char* const ints[] = {"byte", "sbyte", "short",  "ushort", "int",
                                        "uint", "long",  "ulong", "char"};
    for (const char* n : ints)
        if (std::strcmp(b, n) == 0) return true;
    return false;
}

int64_t floor_div(int64_t a, int64_t b) {
    int64_t q = a / b, r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) --q;
    return q;
}

// Howard Hinnant's civil<->days algorithms (days relative to 1970-01-01).
int64_t days_from_civil(int y, unsigned m, unsigned d) {
    y -= m <= 2;
    const int64_t era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<int64_t>(doe) - 719468;
}

struct Ymd { int y; unsigned m; unsigned d; };
Ymd civil_from_days(int64_t z) {
    z += 719468;
    const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const int y = static_cast<int>(yoe) + static_cast<int>(era * 400);
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    const unsigned d = doy - (153 * mp + 2) / 5 + 1;
    const unsigned m = mp + (mp < 10 ? 3 : -9);
    return {y + (m <= 2), m, d};
}

void store_be32(char* p, uint32_t v) {
    p[0] = static_cast<char>(v >> 24);
    p[1] = static_cast<char>(v >> 16);
    p[2] = static_cast<char>(v >> 8);
    p[3] = static_cast<char>(v);
}
void store_be64(char* p, uint64_t v) {
    for (int i = 0; i < 8; ++i) p[i] = static_cast<char>(v >> (56 - 8 * i));
}
uint32_t load_be32(const unsigned char* p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}
uint64_t load_be64(const unsigned char* p) {
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) v = (v << 8) | p[i];
    return v;
}

int64_t obj_as_int64(const msgpack::object& o) {
    if (o.type == mtype::POSITIVE_INTEGER) return static_cast<int64_t>(o.via.u64);
    if (o.type == mtype::NEGATIVE_INTEGER) return o.via.i64;
    return 0;
}

double to_double(const json& v) {
    if (v.is_number()) return v.get<double>();
    if (v.is_boolean()) return v.get<bool>() ? 1.0 : 0.0;
    return 0.0;
}

// --- Timestamp ext(-1), byte-identical to msgpack.Timestamp.to_bytes ---
void pack_timestamp(msgpack::packer<msgpack::sbuffer>& pk, int64_t secs, uint32_t nanos) {
    if (secs >= 0 && secs < (1LL << 34)) {
        uint64_t data64 = (static_cast<uint64_t>(nanos) << 34) | static_cast<uint64_t>(secs);
        if ((data64 & 0xFFFFFFFF00000000ULL) == 0) {
            char b[4];
            store_be32(b, static_cast<uint32_t>(data64));
            pk.pack_ext(4, static_cast<int8_t>(-1));
            pk.pack_ext_body(b, 4);
        } else {
            char b[8];
            store_be64(b, data64);
            pk.pack_ext(8, static_cast<int8_t>(-1));
            pk.pack_ext_body(b, 8);
        }
    } else {
        char b[12];
        store_be32(b, nanos);
        store_be64(b + 4, static_cast<uint64_t>(secs));
        pk.pack_ext(12, static_cast<int8_t>(-1));
        pk.pack_ext_body(b, 12);
    }
}

tsval decode_timestamp(const msgpack::object& o) {
    const unsigned char* d = reinterpret_cast<const unsigned char*>(o.via.ext.data());
    uint32_t sz = o.via.ext.size;
    if (sz == 4) return {static_cast<int64_t>(load_be32(d)), 0u};
    if (sz == 8) {
        uint64_t data64 = load_be64(d);
        return {static_cast<int64_t>(data64 & 0x3FFFFFFFFULL), static_cast<uint32_t>(data64 >> 34)};
    }
    uint32_t nanos = load_be32(d);
    return {static_cast<int64_t>(load_be64(d + 4)), nanos};
}

tsval parse_iso_string(const std::string& s) {
    size_t i = 0, n = s.size();
    auto rd = [&](int len) {
        int v = 0;
        for (int k = 0; k < len && i < n && s[i] >= '0' && s[i] <= '9'; ++k, ++i) v = v * 10 + (s[i] - '0');
        return v;
    };
    int year = rd(4);
    if (i < n && s[i] == '-') ++i;
    unsigned month = static_cast<unsigned>(rd(2));
    if (i < n && s[i] == '-') ++i;
    unsigned day = static_cast<unsigned>(rd(2));
    int hour = 0, minute = 0, second = 0;
    int64_t micro = 0, tz_off = 0;
    if (i < n) {
        ++i;  // date/time separator (T or space)
        hour = rd(2);
        if (i < n && s[i] == ':') ++i;
        minute = rd(2);
        if (i < n && s[i] == ':') {
            ++i;
            second = rd(2);
        }
        if (i < n && (s[i] == '.' || s[i] == ',')) {
            ++i;
            int digits = 0;
            int64_t frac = 0;
            while (i < n && s[i] >= '0' && s[i] <= '9') {
                if (digits < 6) {
                    frac = frac * 10 + (s[i] - '0');
                    ++digits;
                }
                ++i;
            }
            while (digits < 6) {
                frac *= 10;
                ++digits;
            }
            micro = frac;
        }
        if (i < n) {
            char c = s[i];
            if (c == 'Z' || c == 'z') {
                ++i;
            } else if (c == '+' || c == '-') {
                int sign = (c == '-') ? -1 : 1;
                ++i;
                int oh = rd(2), om = 0, os = 0;
                if (i < n && s[i] == ':') {
                    ++i;
                    om = rd(2);
                    if (i < n && s[i] == ':') {
                        ++i;
                        os = rd(2);
                    }
                } else if (i + 1 < n && s[i] >= '0' && s[i] <= '9') {
                    om = rd(2);
                }
                tz_off = sign * (oh * 3600 + om * 60 + os);
            }
        }
    }
    int64_t days = days_from_civil(year, month, day);
    int64_t total_sec = days * 86400LL + hour * 3600 + minute * 60 + second - tz_off;
    int64_t total_us = total_sec * 1000000LL + micro;
    int64_t secs = floor_div(total_us, 1000000LL);
    int64_t rem = total_us - secs * 1000000LL;
    return {secs, static_cast<uint32_t>(rem * 1000)};
}

tsval parse_iso(const json& v) {
    if (v.is_string()) {
        const std::string& s = v.get_ref<const std::string&>();
        if (s.empty()) return {DATETIME_MIN_SECONDS, 0u};
        return parse_iso_string(s);
    }
    if (v.is_number_integer() || v.is_number_unsigned()) {
        int64_t us = v.get<int64_t>();
        int64_t secs = floor_div(us, 1000000LL);
        int64_t rem = us - secs * 1000000LL;
        return {secs, static_cast<uint32_t>(rem * 1000)};
    }
    return {DATETIME_MIN_SECONDS, 0u};
}

std::string ts_to_iso(int64_t secs, uint32_t nanos) {
    long long micros = static_cast<long long>(nanos / 1000);
    int64_t days = floor_div(secs, 86400LL);
    int64_t sod = secs - days * 86400LL;
    int h = static_cast<int>(sod / 3600);
    int mn = static_cast<int>((sod % 3600) / 60);
    int sc = static_cast<int>(sod % 60);
    Ymd ymd = civil_from_days(days);
    char buf[80];
    if (micros != 0)
        std::snprintf(buf, sizeof(buf), "%04d-%02u-%02uT%02d:%02d:%02d.%06lld+00:00", ymd.y, ymd.m,
                      ymd.d, h, mn, sc, micros);
    else
        std::snprintf(buf, sizeof(buf), "%04d-%02u-%02uT%02d:%02d:%02d+00:00", ymd.y, ymd.m, ymd.d, h,
                      mn, sc);
    return std::string(buf);
}

void pack_str(msgpack::packer<msgpack::sbuffer>& pk, const std::string& s) {
    pk.pack_str(static_cast<uint32_t>(s.size()));
    pk.pack_str_body(s.data(), static_cast<uint32_t>(s.size()));
}

// Non-negative -> unsigned minimal width, negative -> signed; matches Python packb exactly.
void pack_json_int(msgpack::packer<msgpack::sbuffer>& pk, const json& v) {
    if (v.is_number_unsigned()) {
        pk.pack(v.get<uint64_t>());
    } else if (v.is_number_integer()) {
        int64_t x = v.get<int64_t>();
        if (x >= 0)
            pk.pack(static_cast<uint64_t>(x));
        else
            pk.pack(x);
    } else if (v.is_number_float()) {
        double d = v.get<double>();
        if (d >= 0)
            pk.pack(static_cast<uint64_t>(static_cast<int64_t>(d)));
        else
            pk.pack(static_cast<int64_t>(d));
    } else if (v.is_boolean()) {
        pk.pack(static_cast<uint64_t>(v.get<bool>() ? 1 : 0));
    } else {
        pk.pack_nil();
    }
}

// Dictionary<>/unknown passthrough: object->map (str keys), array->array, scalar as-is.
void encode_passthrough(msgpack::packer<msgpack::sbuffer>& pk, const json& v) {
    switch (v.type()) {
        case json::value_t::null:
            pk.pack_nil();
            break;
        case json::value_t::boolean:
            pk.pack(v.get<bool>());
            break;
        case json::value_t::number_unsigned:
            pk.pack(v.get<uint64_t>());
            break;
        case json::value_t::number_integer:
            pack_json_int(pk, v);
            break;
        case json::value_t::number_float:
            pk.pack_double(v.get<double>());
            break;
        case json::value_t::string:
            pack_str(pk, v.get_ref<const std::string&>());
            break;
        case json::value_t::binary: {
            const auto& b = v.get_binary();
            pk.pack_bin(static_cast<uint32_t>(b.size()));
            pk.pack_bin_body(reinterpret_cast<const char*>(b.data()), static_cast<uint32_t>(b.size()));
            break;
        }
        case json::value_t::array:
            pk.pack_array(static_cast<uint32_t>(v.size()));
            for (const auto& x : v) encode_passthrough(pk, x);
            break;
        case json::value_t::object:
            pk.pack_map(static_cast<uint32_t>(v.size()));
            for (auto it = v.begin(); it != v.end(); ++it) {
                pack_str(pk, it.key());
                encode_passthrough(pk, it.value());
            }
            break;
        default:
            pk.pack_nil();
            break;
    }
}

// default(T) for non-nullable null/missing fields; mirrors helpers.msgpack._zero EXACTLY,
// including its case-sensitive sets ("Decimal"/"TimeSpan" fall through to nil).
void pack_zero(msgpack::packer<msgpack::sbuffer>& pk, const FieldSpec& f) {
    if (std::strcmp(f.kind, "enum") == 0) {
        pk.pack(static_cast<uint64_t>(0));
        return;
    }
    const char* b = f.base;
    if (std::strcmp(b, "bool") == 0) {
        pk.pack(false);
    } else if (std::strcmp(b, "float") == 0 || std::strcmp(b, "double") == 0) {
        pk.pack_double(0.0);
    } else if (is_int_base(b)) {
        pk.pack(static_cast<uint64_t>(0));
    } else if (std::strcmp(b, "DateTime") == 0) {
        pack_timestamp(pk, DATETIME_MIN_SECONDS, 0);
    } else {
        pk.pack_nil();
    }
}

void encode_dictionary(msgpack::packer<msgpack::sbuffer>& pk, const json& value, const char* value_model);

// Dictionary<int, Model> value-type recovery. Python's to_wire reads each value's runtime
// class; KEYS carries no value type for a generic Dictionary, so map the known fields by name.
const char* dict_value_model(const char* fn) {
    if (std::strcmp(fn, "actors") == 0) return "Actor";
    if (std::strcmp(fn, "time_events") == 0) return "TimingEvent";
    return nullptr;
}

// v guaranteed non-null; encodes one scalar/model value per FieldSpec.
void encode_one(msgpack::packer<msgpack::sbuffer>& pk, const FieldSpec& f, const json& v) {
    // a json binary value (e.g. a byte[] pre-converted by user_data) is a MessagePack bin.
    if (v.is_binary()) {
        const auto& b = v.get_binary();
        pk.pack_bin(static_cast<uint32_t>(b.size()));
        pk.pack_bin_body(reinterpret_cast<const char*>(b.data()), static_cast<uint32_t>(b.size()));
        return;
    }
    if (std::strcmp(f.kind, "model") == 0) {
        // Python to_wire maps a dict before the KEYS lookup, so a Dictionary field is a msgpack map.
        if (std::strcmp(f.base, "Dictionary") == 0 && v.is_object()) {
            encode_dictionary(pk, v, dict_value_model(f.fn));
            return;
        }
        to_wire(pk, f.base, v);
        return;
    }
    if (std::strcmp(f.kind, "enum") == 0) {
        pack_json_int(pk, v);
        return;
    }
    const char* b = f.base;
    if (std::strcmp(b, "DateTime") == 0) {
        tsval ts = parse_iso(v);
        pack_timestamp(pk, ts.first, ts.second);
    } else if (std::strcmp(b, "float") == 0 || std::strcmp(b, "double") == 0) {
        pk.pack_double(to_double(v));  // ALWAYS float64
    } else if (std::strcmp(b, "Decimal") == 0) {
        if (v.is_string())
            pack_str(pk, v.get_ref<const std::string&>());
        else if (v.is_number())
            pk.pack_double(to_double(v));
        else
            encode_passthrough(pk, v);
    } else if (std::strcmp(b, "bool") == 0) {
        if (v.is_boolean())
            pk.pack(v.get<bool>());
        else
            encode_passthrough(pk, v);
    } else if (std::strcmp(b, "string") == 0) {
        if (v.is_string())
            pack_str(pk, v.get_ref<const std::string&>());
        else
            encode_passthrough(pk, v);
    } else {
        pack_json_int(pk, v);  // int/long/byte/short/.../TimeSpan
    }
}

void encode_field(msgpack::packer<msgpack::sbuffer>& pk, const FieldSpec& f, const json& v) {
    if (f.is_array) {
        if (v.is_null()) {
            if (f.nullable)
                pk.pack_nil();
            else
                pack_zero(pk, f);  // _zero runs before the is_array branch in Python
            return;
        }
        if (v.is_array()) {
            pk.pack_array(static_cast<uint32_t>(v.size()));
            for (const auto& x : v) {
                if (x.is_null())
                    pk.pack_nil();  // array elements are always nullable in Python _encode
                else
                    encode_one(pk, f, x);
            }
            return;
        }
        encode_one(pk, f, v);
        return;
    }
    if (v.is_null()) {
        if (f.nullable)
            pk.pack_nil();
        else
            pack_zero(pk, f);
        return;
    }
    encode_one(pk, f, v);
}

int model_array_size(const ModelSpec* m) {
    int size = 0;
    for (int i = 0; i < m->count; ++i)
        if (m->fields[i].key + 1 > size) size = m->fields[i].key + 1;
    return size;
}

void encode_model(msgpack::packer<msgpack::sbuffer>& pk, const ModelSpec* m, const json& value) {
    int size = model_array_size(m);
    pk.pack_array(static_cast<uint32_t>(size));
    std::vector<const FieldSpec*> slot(static_cast<size_t>(size), nullptr);
    for (int i = 0; i < m->count; ++i) slot[m->fields[i].key] = &m->fields[i];
    const json nullj(nullptr);
    for (int i = 0; i < size; ++i) {
        if (!slot[i]) {
            pk.pack_nil();
            continue;
        }
        const FieldSpec& f = *slot[i];
        auto it = value.find(f.fn);
        encode_field(pk, f, it != value.end() ? *it : nullj);
    }
}

json obj_to_json(const msgpack::object& o);

std::string map_key_to_string(const msgpack::object& k) {
    if (k.type == mtype::STR) return std::string(k.via.str.ptr, k.via.str.size);
    if (k.type == mtype::POSITIVE_INTEGER) return std::to_string(k.via.u64);
    if (k.type == mtype::NEGATIVE_INTEGER) return std::to_string(k.via.i64);
    if (k.type == mtype::BOOLEAN) return k.via.boolean ? "true" : "false";
    return std::string();
}

json obj_to_json(const msgpack::object& o) {
    switch (o.type) {
        case mtype::NIL:
            return json(nullptr);
        case mtype::BOOLEAN:
            return json(o.via.boolean);
        case mtype::POSITIVE_INTEGER:
            return json(o.via.u64);
        case mtype::NEGATIVE_INTEGER:
            return json(o.via.i64);
        case mtype::FLOAT32:
        case mtype::FLOAT64:
            return json(o.via.f64);
        case mtype::STR:
            return json(std::string(o.via.str.ptr, o.via.str.size));
        case mtype::BIN: {
            json a = json::array();
            for (uint32_t i = 0; i < o.via.bin.size; ++i)
                a.push_back(static_cast<uint8_t>(static_cast<unsigned char>(o.via.bin.ptr[i])));
            return a;
        }
        case mtype::ARRAY: {
            json a = json::array();
            for (uint32_t i = 0; i < o.via.array.size; ++i) a.push_back(obj_to_json(o.via.array.ptr[i]));
            return a;
        }
        case mtype::MAP: {
            json m = json::object();
            for (uint32_t i = 0; i < o.via.map.size; ++i)
                m[map_key_to_string(o.via.map.ptr[i].key)] = obj_to_json(o.via.map.ptr[i].val);
            return m;
        }
        case mtype::EXT:
            if (o.via.ext.type() == static_cast<int8_t>(-1)) {
                tsval ts = decode_timestamp(o);
                return json(ts_to_iso(ts.first, ts.second));
            }
            return json(nullptr);
        default:
            return json(nullptr);
    }
}

json decode_one(const FieldSpec& f, const msgpack::object& v) {
    if (std::strcmp(f.kind, "model") == 0) return from_array(f.base, v);
    if (std::strcmp(f.base, "DateTime") == 0 && v.type == mtype::EXT &&
        v.via.ext.type() == static_cast<int8_t>(-1)) {
        tsval ts = decode_timestamp(v);
        return json(ts_to_iso(ts.first, ts.second));
    }
    return obj_to_json(v);
}

json decode_field(const FieldSpec& f, const msgpack::object& v) {
    if (f.is_array) {
        if (v.type != mtype::ARRAY) return obj_to_json(v);
        json a = json::array();
        for (uint32_t i = 0; i < v.via.array.size; ++i) {
            const msgpack::object& x = v.via.array.ptr[i];
            a.push_back(x.type == mtype::NIL ? json(nullptr) : decode_one(f, x));
        }
        return a;
    }
    return decode_one(f, v);
}

void pack_model_list(msgpack::packer<msgpack::sbuffer>& pk, const char* name, const json& list) {
    if (!list.is_array()) {
        pk.pack_array(0);
        return;
    }
    pk.pack_array(static_cast<uint32_t>(list.size()));
    for (const auto& e : list) to_wire(pk, name, e);
}

void pack_union_list(msgpack::packer<msgpack::sbuffer>& pk, const json& list) {
    if (!list.is_array()) {
        pk.pack_array(0);
        return;
    }
    pk.pack_array(static_cast<uint32_t>(list.size()));
    for (const auto& e : list) {
        if (e.is_array() && e.size() >= 3 && e[1].is_string()) {
            pk.pack_array(2);
            pack_json_int(pk, e[0]);
            to_wire(pk, e[1].get_ref<const std::string&>().c_str(), e[2]);
        } else if (e.is_array() && e.size() >= 3 && e[1].is_null()) {
            pk.pack_array(2);
            pack_json_int(pk, e[0]);
            to_wire(pk, nullptr, e[2]);
        } else {
            encode_passthrough(pk, e);
        }
    }
}

// a json object key that is a canonical signed decimal -> int. json keys are strings,
// but C# Dictionary<int,V> (actors/time_events/scores) needs integer msgpack keys.
bool parse_int_key(const std::string& s, int64_t& out) {
    size_t i = 0;
    bool neg = s.size() > 1 && s[0] == '-';
    if (neg) i = 1;
    if (i >= s.size()) return false;
    if (s[i] == '0' && s.size() - i > 1) return false;  // reject leading zeros
    uint64_t v = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
        v = v * 10 + static_cast<uint64_t>(s[i] - '0');
    }
    out = neg ? -static_cast<int64_t>(v) : static_cast<int64_t>(v);
    return true;
}

// Python to_wire hits `isinstance(obj, dict)` before the KEYS lookup, so a Dictionary
// field serializes as a msgpack map, not a model array. Each value is wired as value_model
// (Python recovers that from the value's runtime class; see dict_value_model).
void encode_dictionary(msgpack::packer<msgpack::sbuffer>& pk, const json& value, const char* value_model) {
    pk.pack_map(static_cast<uint32_t>(value.size()));
    for (auto it = value.begin(); it != value.end(); ++it) {
        int64_t ik;
        if (parse_int_key(it.key(), ik)) {
            if (ik >= 0)
                pk.pack(static_cast<uint64_t>(ik));
            else
                pk.pack(ik);
        } else {
            pack_str(pk, it.key());
        }
        to_wire(pk, value_model, it.value());
    }
}

}  // namespace

const ModelSpec* find_model(const char* name) {
    if (!name) return nullptr;
    static const std::unordered_map<std::string_view, const ModelSpec*> table = [] {
        std::unordered_map<std::string_view, const ModelSpec*> m;
        m.reserve(static_cast<size_t>(KEYS_TABLE_COUNT) * 2);
        for (int i = 0; i < KEYS_TABLE_COUNT; ++i)
            m.emplace(std::string_view(KEYS_TABLE[i].name), &KEYS_TABLE[i]);
        return m;
    }();
    auto it = table.find(std::string_view(name));
    return it == table.end() ? nullptr : it->second;
}

void to_wire(msgpack::packer<msgpack::sbuffer>& pk, const char* name, const json& value) {
    if (value.is_null()) {
        pk.pack_nil();
        return;
    }
    if (value.is_array()) {
        pk.pack_array(static_cast<uint32_t>(value.size()));
        for (const auto& x : value) to_wire(pk, name, x);
        return;
    }
    // A Dictionary reached generically (nested value) is a map, not an empty model array.
    if (name && std::strcmp(name, "Dictionary") == 0 && value.is_object()) {
        encode_dictionary(pk, value, nullptr);
        return;
    }
    const ModelSpec* m = find_model(name);
    if (m && value.is_object()) {
        encode_model(pk, m, value);
        return;
    }
    encode_passthrough(pk, value);
}

std::string pack(const char* name, const json& value) {
    msgpack::sbuffer buf;
    msgpack::packer<msgpack::sbuffer> pk(&buf);
    to_wire(pk, name, value);
    return std::string(buf.data(), buf.size());
}

json from_array(const char* name, const msgpack::object& o) {
    const ModelSpec* m = find_model(name);
    if (!m || o.type != mtype::ARRAY) return obj_to_json(o);
    json result = json::object();
    uint32_t n = o.via.array.size;
    for (int i = 0; i < m->count; ++i) {
        const FieldSpec& f = m->fields[i];
        if (static_cast<uint32_t>(f.key) >= n) continue;
        const msgpack::object& v = o.via.array.ptr[f.key];
        if (v.type == mtype::NIL) continue;  // missing/nil -> model default applies
        result[f.fn] = decode_field(f, v);
    }
    return result;
}

msgpack::object_handle unpack_raw(const std::string& body) {
    msgpack::object_handle oh = msgpack::unpack(body.data(), body.size());
    const msgpack::object& o = oh.get();

    if (o.type == mtype::EXT && o.via.ext.type() == static_cast<int8_t>(99)) {
        const char* ed = o.via.ext.data();
        uint32_t esz = o.via.ext.size;
        size_t off = 0;
        msgpack::object_handle lh = msgpack::unpack(ed, esz, off);
        int64_t ulen = obj_as_int64(lh.get());
        std::string dst(static_cast<size_t>(ulen), '\0');
        if (ulen > 0) {
            int r = LZ4_decompress_safe(ed + off, &dst[0], static_cast<int>(esz - off),
                                        static_cast<int>(ulen));
            if (r < 0) throw std::runtime_error("lz4 block decompress failed");
        }
        return msgpack::unpack(dst.data(), dst.size());
    }

    if (o.type == mtype::ARRAY && o.via.array.size >= 2) {
        const msgpack::object& head = o.via.array.ptr[0];
        bool ok = head.type == mtype::EXT && head.via.ext.type() == static_cast<int8_t>(98);
        for (uint32_t i = 1; ok && i < o.via.array.size; ++i)
            if (o.via.array.ptr[i].type != mtype::BIN) ok = false;
        if (ok) {
            const char* ld = head.via.ext.data();
            uint32_t lsz = head.via.ext.size;
            std::vector<msgpack::object_handle> hs;
            size_t off = 0;
            while (off < lsz) hs.push_back(msgpack::unpack(ld, lsz, off));
            std::vector<int64_t> lengths;
            if (hs.size() == 1 && hs[0].get().type == mtype::ARRAY) {
                const auto& a = hs[0].get().via.array;
                for (uint32_t i = 0; i < a.size; ++i) lengths.push_back(obj_as_int64(a.ptr[i]));
            } else {
                for (auto& h : hs) lengths.push_back(obj_as_int64(h.get()));
            }
            std::string out;
            size_t cnt = std::min(static_cast<size_t>(o.via.array.size - 1), lengths.size());
            for (size_t i = 0; i < cnt; ++i) {
                const msgpack::object& blk = o.via.array.ptr[i + 1];
                int64_t ulen = lengths[i];
                if (ulen <= 0) continue;
                std::string dst(static_cast<size_t>(ulen), '\0');
                int r = LZ4_decompress_safe(blk.via.bin.ptr, &dst[0],
                                            static_cast<int>(blk.via.bin.size),
                                            static_cast<int>(ulen));
                if (r < 0) throw std::runtime_error("lz4 block array decompress failed");
                out.append(dst.data(), static_cast<size_t>(ulen));
            }
            return msgpack::unpack(out.data(), out.size());
        }
    }
    return oh;
}

json read_request(const std::string& body, const char* name) {
    if (body.empty()) return json(nullptr);
    try {
        msgpack::object_handle oh = unpack_raw(body);
        if (!name) return obj_to_json(oh.get());
        return from_array(name, oh.get());
    } catch (...) {
        return json(nullptr);
    }
}

std::string common_response(const char* result_name, const json& result, const json& faults,
                            const json& present, const json& deleted, const json& notifications) {
    msgpack::sbuffer buf;
    msgpack::packer<msgpack::sbuffer> pk(&buf);
    pack_model_list(pk, "Fault", faults);
    to_wire(pk, result_name, result);
    pack_union_list(pk, present);
    pack_model_list(pk, "DeletedDataObject", deleted);
    pack_union_list(pk, notifications);
    return std::string(buf.data(), buf.size());
}

std::string common_response_union_result(const json& result, const json& faults, const json& present,
                                         const json& deleted, const json& notifications) {
    msgpack::sbuffer buf;
    msgpack::packer<msgpack::sbuffer> pk(&buf);
    pack_model_list(pk, "Fault", faults);
    pack_union_list(pk, result);
    pack_union_list(pk, present);
    pack_model_list(pk, "DeletedDataObject", deleted);
    pack_union_list(pk, notifications);
    return std::string(buf.data(), buf.size());
}

json union_entry(long long key, const char* name, const json& value) {
    json e = json::array();
    e.push_back(key);
    e.push_back(name ? json(std::string(name)) : json(nullptr));
    e.push_back(value);
    return e;
}

}  // namespace wire
