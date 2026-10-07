#include "helpers/assets.h"

#include "config.h"
#include "platform.h"
#include "SpookyV2.h"

#include <cstdint>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

// brotli one-shot encoder: real header when vendored, else forward-declare its C API.
#if defined(__has_include)
#  if __has_include(<brotli/encode.h>)
#    include <brotli/encode.h>
#    define ASSETS_HAVE_BROTLI_HEADER 1
#  endif
#endif
#ifndef ASSETS_HAVE_BROTLI_HEADER
extern "C" {
typedef enum {
    BROTLI_MODE_GENERIC = 0,
    BROTLI_MODE_TEXT = 1,
    BROTLI_MODE_FONT = 2
} BrotliEncoderMode;
size_t BrotliEncoderMaxCompressedSize(size_t input_size);
int BrotliEncoderCompress(int quality, int lgwin, BrotliEncoderMode mode,
                          size_t input_size, const uint8_t* input_buffer,
                          size_t* encoded_size, uint8_t* encoded_buffer);
}
#endif

namespace assets {

namespace {

const std::string OFFICIAL_ASSET_URL = "https://assets-e.wds-stellarium.com/production";

std::mutex g_br_mutex;
std::unordered_map<std::string, BodyMd5> g_br_cache;
std::mutex g_hash_mutex;
std::unordered_map<std::string, BodyMd5> g_hash_cache;

std::string ascii_lower(const std::string& s) {
    std::string out = s;
    for (char& c : out)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return out;
}

std::string to_hex_lower(const uint8_t* data, size_t len) {
    static const char H[] = "0123456789abcdef";
    std::string out(len * 2, '\0');
    for (size_t i = 0; i < len; ++i) {
        out[2 * i] = H[data[i] >> 4];
        out[2 * i + 1] = H[data[i] & 0xf];
    }
    return out;
}

std::string b64encode(const uint8_t* data, size_t len) {
    static const char T[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    out.reserve(((len + 2) / 3) * 4);
    size_t i = 0;
    for (; i + 3 <= len; i += 3) {
        uint32_t n = (static_cast<uint32_t>(data[i]) << 16) |
                     (static_cast<uint32_t>(data[i + 1]) << 8) | data[i + 2];
        out += T[(n >> 18) & 63];
        out += T[(n >> 12) & 63];
        out += T[(n >> 6) & 63];
        out += T[n & 63];
    }
    size_t rem = len - i;
    if (rem == 1) {
        uint32_t n = static_cast<uint32_t>(data[i]) << 16;
        out += T[(n >> 18) & 63];
        out += T[(n >> 12) & 63];
        out += '=';
        out += '=';
    } else if (rem == 2) {
        uint32_t n = (static_cast<uint32_t>(data[i]) << 16) |
                     (static_cast<uint32_t>(data[i + 1]) << 8);
        out += T[(n >> 18) & 63];
        out += T[(n >> 12) & 63];
        out += T[(n >> 6) & 63];
        out += '=';
    }
    return out;
}

inline uint32_t md5_rotl(uint32_t x, uint32_t c) {
    return (x << c) | (x >> (32 - c));
}

void md5(const uint8_t* msg, size_t len, uint8_t digest[16]) {
    static const uint32_t s[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
    static const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
        0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
        0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
        0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
        0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
        0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

    uint32_t a0 = 0x67452301, b0 = 0xefcdab89, c0 = 0x98badcfe, d0 = 0x10325476;

    size_t new_len = ((len + 8) / 64 + 1) * 64;
    std::vector<uint8_t> buf(new_len, 0);
    std::memcpy(buf.data(), msg, len);
    buf[len] = 0x80;
    uint64_t bits = static_cast<uint64_t>(len) * 8;
    for (int i = 0; i < 8; ++i)
        buf[new_len - 8 + i] = static_cast<uint8_t>((bits >> (8 * i)) & 0xff);

    for (size_t off = 0; off < new_len; off += 64) {
        uint32_t M[16];
        for (int i = 0; i < 16; ++i)
            M[i] = static_cast<uint32_t>(buf[off + i * 4]) |
                   (static_cast<uint32_t>(buf[off + i * 4 + 1]) << 8) |
                   (static_cast<uint32_t>(buf[off + i * 4 + 2]) << 16) |
                   (static_cast<uint32_t>(buf[off + i * 4 + 3]) << 24);
        uint32_t A = a0, B = b0, C = c0, D = d0;
        for (int i = 0; i < 64; ++i) {
            uint32_t F, g;
            if (i < 16) {
                F = (B & C) | (~B & D);
                g = i;
            } else if (i < 32) {
                F = (D & B) | (~D & C);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else {
                F = C ^ (B | ~D);
                g = (7 * i) % 16;
            }
            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + md5_rotl(F, s[i]);
        }
        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }
    uint32_t out[4] = {a0, b0, c0, d0};
    for (int i = 0; i < 4; ++i) {
        digest[i * 4] = static_cast<uint8_t>(out[i] & 0xff);
        digest[i * 4 + 1] = static_cast<uint8_t>((out[i] >> 8) & 0xff);
        digest[i * 4 + 2] = static_cast<uint8_t>((out[i] >> 16) & 0xff);
        digest[i * 4 + 3] = static_cast<uint8_t>((out[i] >> 24) & 0xff);
    }
}

std::string content_md5(const std::string& body) {
    uint8_t digest[16];
    md5(reinterpret_cast<const uint8_t*>(body.data()), body.size(), digest);
    return b64encode(digest, 16);
}

std::string brotli_compress(const std::string& raw, int quality) {
    size_t max_out = BrotliEncoderMaxCompressedSize(raw.size());
    if (max_out == 0) max_out = raw.size() + raw.size() / 2 + 1024;
    std::vector<uint8_t> out(max_out);
    size_t out_size = max_out;
    // generic mode, default window 22 -- matches python brotli.compress(quality=N)
    int ok = BrotliEncoderCompress(
        quality, 22, BROTLI_MODE_GENERIC, raw.size(),
        reinterpret_cast<const uint8_t*>(raw.data()), &out_size, out.data());
    if (!ok) return std::string();
    return std::string(reinterpret_cast<const char*>(out.data()), out_size);
}

// read _data/assets/<kind>/<platform>/catalog.json, or nullopt if absent
std::string catalog(const std::string& kind, const std::string& plat) {
    return platform::read_file("_data/assets/" + kind + "/" + ascii_lower(plat) +
                               "/catalog.json");
}

// resolve base + rel, collapsing . and .., require the result stays strictly inside
// base (python: root in path.parents). sets joined on success.
bool resolve_within(const std::vector<std::string>& base, const std::string& rel,
                    std::string& joined) {
    std::vector<std::string> parts = base;
    size_t start = 0;
    while (start <= rel.size()) {
        size_t slash = rel.find('/', start);
        std::string comp =
            (slash == std::string::npos) ? rel.substr(start) : rel.substr(start, slash - start);
        if (!comp.empty() && comp != ".") {
            if (comp == "..") {
                if (!parts.empty()) parts.pop_back();
            } else {
                parts.push_back(comp);
            }
        }
        if (slash == std::string::npos) break;
        start = slash + 1;
    }
    if (parts.size() <= base.size()) return false;
    for (size_t i = 0; i < base.size(); ++i)
        if (parts[i] != base[i]) return false;
    joined.clear();
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) joined += '/';
        joined += parts[i];
    }
    return true;
}

}  // namespace

std::optional<BodyMd5> catalog_br(const std::string& kind, const std::string& plat) {
    std::string key = kind + "/" + ascii_lower(plat);
    std::lock_guard<std::mutex> lock(g_br_mutex);
    auto it = g_br_cache.find(key);
    if (it == g_br_cache.end()) {
        std::string raw = catalog(kind, plat);
        if (raw.empty()) return std::nullopt;
        std::string body = brotli_compress(raw, 5);
        it = g_br_cache.emplace(key, BodyMd5{body, content_md5(body)}).first;
    }
    return it->second;
}

std::optional<BodyMd5> catalog_hash(const std::string& kind, const std::string& plat) {
    std::string key = kind + "/" + ascii_lower(plat);
    std::lock_guard<std::mutex> lock(g_hash_mutex);
    auto it = g_hash_cache.find(key);
    if (it == g_hash_cache.end()) {
        std::string raw = catalog(kind, plat);
        if (raw.empty()) return std::nullopt;
        uint64_t h1 = 0, h2 = 0;
        SpookyHash::Hash128(raw.data(), raw.size(), &h1, &h2);
        uint8_t digest[16];
        for (int i = 0; i < 8; ++i) digest[i] = static_cast<uint8_t>((h1 >> (8 * i)) & 0xff);
        for (int i = 0; i < 8; ++i) digest[8 + i] = static_cast<uint8_t>((h2 >> (8 * i)) & 0xff);
        std::string body = to_hex_lower(digest, 16);
        it = g_hash_cache.emplace(key, BodyMd5{body, content_md5(body)}).first;
    }
    return it->second;
}

bool local_assets_enabled() {
    return config::get_bool("local_assets");
}

std::optional<BodyMd5> bundle(const std::string& kind, const std::string& plat,
                              const std::string& rel_path) {
    std::vector<std::string> base = {"_data", "assets", kind, ascii_lower(plat)};
    std::string full;
    if (!resolve_within(base, rel_path, full)) return std::nullopt;
    std::string body = platform::read_file(full);
    if (body.empty()) return std::nullopt;
    return BodyMd5{body, content_md5(body)};
}

std::string official_url(const std::string& kind, const std::string& plat,
                         const std::string& version, const std::string& rel_path) {
    return OFFICIAL_ASSET_URL + "/" + kind + "/" + plat + "/" + version + "/" + rel_path;
}

std::optional<BodyMd5> notation(const std::string& music_id, const std::string& filename) {
    std::vector<std::string> base = {"_data", "assets", "Notations"};
    std::string full;
    if (!resolve_within(base, music_id + "/" + filename, full)) return std::nullopt;
    std::string body = platform::read_file(full);
    if (body.empty()) return std::nullopt;
    return BodyMd5{body, content_md5(body)};
}

std::string official_notation_url(const std::string& music_id, const std::string& filename) {
    return OFFICIAL_ASSET_URL + "/Notations/" + music_id + "/" + filename;
}

}  // namespace assets
