#include "helpers/character_level.h"

#include <algorithm>
#include <ctime>
#include <unordered_map>

#include "generated/enums_generated.h"
#include "master_data.h"

// ports helpers/character_level.py; see that file for the reasoning behind each curve rule.

namespace character_level {

namespace {

// How much of the curve a character actually has to pay, by rarity. 0.3 / 0.5 / 0.8 / 1.0 stored
// as value*kScale so the arithmetic stays exact (0.3 is not representable as a double).
const Decimal kUnknownRarityCoefficient{0};
const Decimal kMaxExperienceCoefficient{10};  // _MAX_EXPERIENCE_COEFFICIENT (defined, unused)
const std::map<long long, Decimal> kRequiredExperienceCoefficient = {
    {enums::CharacterRarities::Rare1, Decimal{3}},
    {enums::CharacterRarities::Rare2, Decimal{5}},
    {enums::CharacterRarities::Rare3, Decimal{8}},
    {enums::CharacterRarities::Rare4, Decimal{10}},
};

// Howard Hinnant's days-from-civil (days relative to 1970-01-01).
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

// epoch seconds of an iso-8601 string; mirrors datetime.fromisoformat (naive -> utc).
// false for anything not a valid date(+time), so started() reads it as "no restriction".
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
            while (i < n && s[i] >= '0' && s[i] <= '9') ++i;
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
    out = days_from_civil(year, static_cast<unsigned>(mon), static_cast<unsigned>(day)) * 86400LL +
          hh * 3600LL + mm * 60LL + ss - tz;
    return true;
}

long long now_utc() { return static_cast<long long>(std::time(nullptr)); }

// {level: row} over CharacterLevelMaster, built once.
const std::map<long long, const json*>& levels() {
    static const std::map<long long, const json*> m = [] {
        std::map<long long, const json*> r;
        for (const json& row : master_data::table("CharacterLevelMaster"))
            r[row.at("level").get<long long>()] = &row;
        return r;
    }();
    return m;
}

// {id_: row} over CharacterMaster, built once.
const std::map<long long, const json*>& characters() {
    static const std::map<long long, const json*> m = [] {
        std::map<long long, const json*> r;
        for (const json& row : master_data::table("CharacterMaster"))
            r[row.at("id_").get<long long>()] = &row;
        return r;
    }();
    return m;
}

// {item_master_id: row} over CharacterExperienceItemMaster, keeping dict insertion order.
struct ExpIndex {
    std::vector<long long> order;
    std::unordered_map<long long, const json*> map;
};
const ExpIndex& exp_index() {
    static const ExpIndex idx = [] {
        ExpIndex e;
        for (const json& row : master_data::table("CharacterExperienceItemMaster")) {
            long long id = row.at("item_master_id").get<long long>();
            if (e.map.find(id) == e.map.end()) e.order.push_back(id);
            e.map[id] = &row;
        }
        return e;
    }();
    return idx;
}

}  // namespace

bool started(const std::string& start_date, long long at) {
    long long start;
    if (!iso_epoch_seconds(start_date, start)) return true;
    return start <= at;
}

long long released_max_level(std::optional<long long> now) {
    long long at = now.value_or(now_utc());
    std::optional<long long> best;
    for (const json& row : master_data::table("CharacterLevelMaster")) {
        if (started(row.at("start_date").get<std::string>(), at)) {
            long long lv = row.at("level").get<long long>();
            best = best ? std::max(*best, lv) : lv;
        }
    }
    return best.value_or(1);
}

long long level_cap(const nlohmann::json& user, std::optional<long long> now) {
    return std::max<long long>(
        1, std::min<long long>(user.at("playerRankLimit").get<long long>(), released_max_level(now)));
}

std::optional<long long> experience_to_level_up(long long level) {
    const auto& m = levels();
    auto it = m.find(level);
    if (it == m.end()) return std::nullopt;
    return it->second->at("experience_to_level_up").get<long long>();
}

std::optional<long long> character_rarity(long long character_master_id) {
    const auto& chars = characters();
    auto it = chars.find(character_master_id);
    if (it == chars.end()) return std::nullopt;
    const json& master = *it->second;
    auto rit = master.find("rarity");
    if (rit == master.end() || rit->is_null()) return std::nullopt;
    return rit->get<long long>();
}

Decimal required_experience_coefficient(std::optional<long long> rarity) {
    if (!rarity) return kUnknownRarityCoefficient;
    auto it = kRequiredExperienceCoefficient.find(*rarity);
    return it != kRequiredExperienceCoefficient.end() ? it->second : kUnknownRarityCoefficient;
}

long long scaled_experience(long long amount, std::optional<long long> rarity) {
    if (amount <= 0) return 0;
    Decimal coef = required_experience_coefficient(rarity);
    // int() truncates toward zero; amount and coef are both non-negative so this is a floor
    long long scaled = (amount * coef.scaled) / Decimal::kScale;
    return std::max<long long>(0, scaled);
}

long long experience_to_reach(long long level, long long current_experience, long long target,
                              std::optional<long long> rarity) {
    long long total = 0;
    for (long long lv = std::max<long long>(1, level); lv < target; ++lv) {
        std::optional<long long> need = experience_to_level_up(lv);
        if (!need) break;  // off the end of the curve
        total += *need;
    }
    return std::max<long long>(0, scaled_experience(total, rarity) - current_experience);
}

std::pair<long long, long long> apply_experience(long long level, long long current_experience,
                                                 long long gained, long long cap,
                                                 std::optional<long long> rarity) {
    level = std::max<long long>(1, level);
    long long experience = current_experience + gained;
    while (level < cap) {
        std::optional<long long> need = experience_to_level_up(level);
        if (!need) break;
        long long scaled = scaled_experience(*need, rarity);
        if (scaled <= 0 || experience < scaled) break;
        experience -= scaled;
        level += 1;
    }
    return {level, experience};
}

std::optional<json> experience_item(long long item_master_id) {
    const ExpIndex& idx = exp_index();
    auto it = idx.map.find(item_master_id);
    if (it == idx.map.end()) return std::nullopt;
    return *it->second;
}

std::vector<json> experience_items() {
    experience_item(0);  // prime the index
    const ExpIndex& idx = exp_index();
    std::vector<json> items;
    items.reserve(idx.order.size());
    for (long long id : idx.order) items.push_back(*idx.map.at(id));
    // richest first; stable so equal values keep master order (python sorted reverse=True)
    std::stable_sort(items.begin(), items.end(), [](const json& a, const json& b) {
        return a.at("acquirable_experience").get<long long>() >
               b.at("acquirable_experience").get<long long>();
    });
    return items;
}

std::map<long long, long long> spend_from_pool(const std::map<long long, long long>& stock,
                                               long long needed) {
    if (needed <= 0) return {};
    std::vector<json> items = experience_items();
    std::map<long long, long long> left = stock;
    std::map<long long, long long> spend;
    for (const json& master : items) {
        long long value = master.at("acquirable_experience").get<long long>();
        long long id = master.at("item_master_id").get<long long>();
        auto lit = left.find(id);
        long long owned = lit != left.end() ? lit->second : 0;
        if (value <= 0 || owned <= 0 || needed < value) continue;
        long long quantity = std::min(owned, needed / value);
        if (quantity > 0) {
            spend[id] = quantity;
            left[id] = owned - quantity;
            needed -= quantity * value;
        }
    }
    if (needed > 0) {
        for (auto mit = items.rbegin(); mit != items.rend(); ++mit) {  // cheapest first
            long long id = mit->at("item_master_id").get<long long>();
            auto lit = left.find(id);
            if (lit != left.end() && lit->second > 0) {
                spend[id] = (spend.count(id) ? spend[id] : 0) + 1;
                break;
            }
        }
    }
    return spend;
}

}  // namespace character_level
