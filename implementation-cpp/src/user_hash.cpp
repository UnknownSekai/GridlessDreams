#include "user_hash.h"

#include <array>
#include <unordered_map>
#include <vector>

namespace user_hash {

namespace {

const std::unordered_map<char, std::string>& sboxes() {
    static const std::unordered_map<char, std::string> m = {
        {'0', "187320496"}, {'1', "072368941"}, {'2', "724396108"},
        {'3', "274619083"}, {'4', "743901826"}, {'6', "102394768"},
        {'7', "082416793"}, {'8', "842309716"}, {'9', "923864701"},
    };
    return m;
}

// {char -> index} reverse of each S-box (built from sboxes())
const std::unordered_map<char, std::unordered_map<char, char>>& sboxes_rev() {
    static const std::unordered_map<char, std::unordered_map<char, char>> r = [] {
        std::unordered_map<char, std::unordered_map<char, char>> out;
        for (const auto& kv : sboxes()) {
            auto& inner = out[kv.first];
            for (size_t i = 0; i < kv.second.size(); ++i)
                inner[kv.second[i]] = static_cast<char>('0' + static_cast<int>(i));
        }
        return out;
    }();
    return r;
}

const std::unordered_map<char, std::pair<std::string, std::string>>& paddings() {
    static const std::unordered_map<char, std::pair<std::string, std::string>> m = {
        {'0', {"19875", "520"}}, {'1', {"37805", "592"}}, {'2', {"02415", "593"}},
        {'3', {"67215", "548"}}, {'4', {"74685", "523"}}, {'6', {"80725", "549"}},
        {'7', {"82305", "541"}}, {'8', {"31685", "592"}}, {'9', {"82635", "574"}},
    };
    return m;
}

std::string base9(long long x) {
    if (x < 9) return std::string(1, static_cast<char>('0' + x));
    return base9(x / 9) + static_cast<char>('0' + (x % 9));
}

// Python s[start:] semantics (negative/out-of-range clamping included)
std::string slice_from(const std::string& s, long long start) {
    long long n = static_cast<long long>(s.size());
    if (start < 0) { start += n; if (start < 0) start = 0; }
    if (start > n) start = n;
    return s.substr(static_cast<size_t>(start));
}

// Python s[:end] semantics
std::string slice_to(const std::string& s, long long end) {
    long long n = static_cast<long long>(s.size());
    if (end < 0) { end += n; if (end < 0) end = 0; }
    if (end > n) end = n;
    return s.substr(0, static_cast<size_t>(end));
}

std::vector<std::string> split_on(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == sep) { out.push_back(cur); cur.clear(); }
        else cur += c;
    }
    out.push_back(cur);
    return out;
}

}  // namespace

std::string hash_id(long long user_id) {
    static const char* const idx_table = "691082374";
    char sbox_idx = idx_table[(user_id % 100) % 9];
    const std::string& sbox = sboxes().at(sbox_idx);
    const auto& pad = paddings().at(sbox_idx);
    const std::string& pleft = pad.first;
    const std::string& pright = pad.second;

    std::string inner(1, sbox_idx);
    for (char digit : base9(user_id)) inner += sbox[digit - '0'];

    long long plen = 10 - static_cast<long long>(inner.size());
    if (plen > 0) {
        long long right = (plen + (plen <= 2 ? 1 : 0) - 1) / 2;
        return slice_from(pleft, right - plen) + inner + slice_to(pright, right);
    }
    return inner;
}

std::optional<long long> unhash_id(const std::string& hashed_id) {
    if (hashed_id.size() < 10) return std::nullopt;
    std::vector<std::string> parts = split_on(hashed_id, '5');
    if (parts.size() > 3) return std::nullopt;
    const std::string& picked = parts[parts.size() > 1 ? 1 : 0];
    if (picked.empty()) return std::nullopt;

    auto bit = sboxes_rev().find(picked[0]);
    if (bit == sboxes_rev().end()) return std::nullopt;
    const auto& rev = bit->second;

    std::string digits;
    for (size_t i = 1; i < picked.size(); ++i) {
        auto dit = rev.find(picked[i]);
        if (dit == rev.end()) return std::nullopt;
        digits += dit->second;
    }
    if (digits.empty()) return std::nullopt;

    long long user_id = 0;
    for (char c : digits) user_id = user_id * 9 + (c - '0');

    if (hash_id(user_id) != hashed_id) return std::nullopt;
    return user_id;
}

}  // namespace user_hash
