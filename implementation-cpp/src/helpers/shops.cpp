#include "helpers/shops.h"

#include <algorithm>
#include <map>
#include <unordered_map>

#include "db.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "master_data.h"

// ports helpers/shops.py. reverse indexes over the four shop masters are built lazily; see the
// Python source for the reasoning behind each rule. RNG is not byte-identical to Python's
// random module -- roll_market mirrors the logic, not the draw sequence.

namespace shops {

namespace {

constexpr long long US_PER_SEC = 1000000LL;
constexpr long long US_PER_DAY = 86400LL * US_PER_SEC;
constexpr long long JST_OFFSET_US = 9LL * 3600LL * US_PER_SEC;  // _JST = UTC+9

// ExchangeShopThing.id_ -> (ExchangeShopMaster, ExchangeShopThing)
std::unordered_map<long long, std::pair<json, json>> g_exchange_things;
std::unordered_map<long long, json> g_market_frames;
// frame number (master_id // 100) -> [MarketFrameThingMaster ids]; std::map keeps the keys
// sorted for roll_market's sorted(_MARKET_POOLS)
std::map<long long, std::vector<long long>> g_market_pools;
std::unordered_map<long long, json> g_permanent;
std::unordered_map<long long, json> g_jewel_items;
std::unordered_map<long long, long long> g_live_music;  // LiveMaster.id_ -> music_master_id

void build() {
    if (!g_exchange_things.empty()) return;
    for (const json& shop : master_data::table("ExchangeShopMaster")) {
        auto lineup = shop.find("lineup");
        if (lineup != shop.end() && lineup->is_array()) {
            for (const json& thing : *lineup)
                g_exchange_things[thing.at("id_").get<long long>()] = {shop, thing};
        }
    }
    // a frame's candidates are encoded in the master id: id // 100 is the frame, id % 100 the
    // variant. build the frame lookup and the pools in one pass -- a repeated id keeps its
    // first-appearance slot in its pool while its value is overwritten, matching the dict.
    for (const json& f : master_data::table("MarketFrameThingMaster")) {
        long long id = f.at("id_").get<long long>();
        bool existed = g_market_frames.find(id) != g_market_frames.end();
        g_market_frames[id] = f;
        if (!existed) g_market_pools[id / 100].push_back(id);
    }
    for (const json& p : master_data::table("PermanentMarketThingMaster"))
        g_permanent[p.at("id_").get<long long>()] = p;
    for (const json& j : master_data::table("JewelShopItemMaster"))
        g_jewel_items[j.at("id_").get<long long>()] = j;
    for (const json& m : master_data::table("LiveMaster"))
        g_live_music[m.at("id_").get<long long>()] = m.at("music_master_id").get<long long>();
}

long long now_utc() { return static_cast<long long>(std::time(nullptr)); }

long long floordiv(long long a, long long b) {
    long long q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

// Howard Hinnant's days<->civil (days relative to 1970-01-01)
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

void civil_from_days(long long z, long long& y, unsigned& m, unsigned& d) {
    z += 719468;
    const long long era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    y = static_cast<long long>(yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    d = doy - (153 * mp + 2) / 5 + 1;
    m = mp < 10 ? mp + 3 : mp - 9;
    y += (m <= 2);
}

// epoch seconds of an iso-8601 string; mirrors datetime.fromisoformat (naive -> utc). false for
// anything not a valid date(+time), which available() reads as "no restriction".
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

std::mt19937_64& default_engine() {
    static std::mt19937_64 eng{std::random_device{}()};
    return eng;
}

// epoch-micros of the latest of today's hours (JST) at or before now, else the last of them
// yesterday. mirrors _most_recent.
long long most_recent(long long now_micros, const std::vector<int>& hours) {
    long long jst = now_micros + JST_OFFSET_US;
    long long day_index = floordiv(jst, US_PER_DAY);
    long long midnight = day_index * US_PER_DAY - JST_OFFSET_US;  // utc micros of jst midnight
    std::vector<int> sorted_hours(hours.begin(), hours.end());
    std::sort(sorted_hours.begin(), sorted_hours.end());
    std::vector<long long> boundaries;
    boundaries.reserve(sorted_hours.size());
    for (int h : sorted_hours)
        boundaries.push_back(midnight + static_cast<long long>(h) * 3600 * US_PER_SEC);
    long long latest = 0;
    bool found = false;
    for (long long b : boundaries)
        if (b <= now_micros) {
            latest = b;
            found = true;
        }
    if (!found) latest = boundaries.back() - US_PER_DAY;
    return latest;
}

}  // namespace

bool available(const json& start_date, const json& end_date, std::optional<std::time_t> now) {
    long long at = now ? static_cast<long long>(*now) : now_utc();
    long long start, end;
    // a missing/unparseable window (TypeError/ValueError) reads as available
    if (!start_date.is_string() || !iso_epoch_seconds(start_date.get<std::string>(), start) ||
        !end_date.is_string() || !iso_epoch_seconds(end_date.get<std::string>(), end))
        return true;
    return start <= at && at <= end;
}

std::pair<json, json> exchange_shop_thing(long long m_exchange_shop_thing_id) {
    build();
    auto it = g_exchange_things.find(m_exchange_shop_thing_id);
    if (it == g_exchange_things.end()) return {json(nullptr), json(nullptr)};
    return it->second;
}

json market_frame(long long market_frame_thing_master_id) {
    build();
    auto it = g_market_frames.find(market_frame_thing_master_id);
    return it != g_market_frames.end() ? it->second : json(nullptr);
}

json permanent_market_thing(long long permanent_market_thing_master_id) {
    build();
    auto it = g_permanent.find(permanent_market_thing_master_id);
    return it != g_permanent.end() ? it->second : json(nullptr);
}

json jewel_shop_item(long long m_jewel_shop_item_id) {
    build();
    auto it = g_jewel_items.find(m_jewel_shop_item_id);
    return it != g_jewel_items.end() ? it->second : json(nullptr);
}

std::optional<long long> live_music_master_id(long long live_master_id) {
    build();
    auto it = g_live_music.find(live_master_id);
    if (it == g_live_music.end()) return std::nullopt;
    return it->second;
}

std::vector<MarketFrame> roll_market(std::mt19937_64* rng) {
    build();
    std::mt19937_64& eng = rng ? *rng : default_engine();
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    std::vector<MarketFrame> frames;
    for (const auto& entry : g_market_pools) {
        long long frame_number = entry.first;
        const std::vector<long long>& pool = entry.second;
        std::uniform_int_distribution<std::size_t> pick(0, pool.size() - 1);
        long long master_id = pool[pick(eng)];
        std::optional<long long> discount =
            chance(eng) < MARKET_DISCOUNT_CHANCE ? std::optional<long long>(MARKET_DISCOUNT_PERCENT)
                                                 : std::nullopt;
        frames.emplace_back(frame_number, master_id, discount);
    }
    return frames;
}

long long market_reset(long long now_micros) {
    return most_recent(now_micros,
                       std::vector<int>(MARKET_RESET_HOURS.begin(), MARKET_RESET_HOURS.end()));
}

std::optional<long long> refresh_cost(long long refresh_times) {
    long long index = std::max(refresh_times, 0LL);
    if (index >= MARKET_MAX_REFRESHES) return std::nullopt;
    return MARKET_REFRESH_COSTS[static_cast<std::size_t>(index)];
}

std::optional<long long> period_until(std::optional<long long> replace_type,
                                      std::optional<std::time_t> now) {
    if (!replace_type) return std::nullopt;
    long long rt = *replace_type;
    // ShopReplaceTypes(int(replace_type)) -- non-member raises ValueError -> None
    if (rt != enums::ShopReplaceTypes::None_ && rt != enums::ShopReplaceTypes::Daily &&
        rt != enums::ShopReplaceTypes::Weekly && rt != enums::ShopReplaceTypes::Monthly &&
        rt != enums::ShopReplaceTypes::DailyPassExpired)
        return std::nullopt;
    long long at = now ? static_cast<long long>(*now) : now_utc();
    long long days = floordiv(at, 86400);
    long long midnight = days * 86400;
    if (rt == enums::ShopReplaceTypes::Daily) {
        return (midnight + 86400) * US_PER_SEC;
    } else if (rt == enums::ShopReplaceTypes::Weekly) {
        long long weekday = ((days % 7) + 3) % 7;  // Mon=0, 1970-01-01 was a Thursday
        if (weekday < 0) weekday += 7;
        return (midnight + (7 - weekday) * 86400) * US_PER_SEC;
    } else if (rt == enums::ShopReplaceTypes::Monthly) {
        long long y;
        unsigned m, d;
        civil_from_days(days, y, m, d);
        long long ny = (m == 12) ? y + 1 : y;
        unsigned nm = (m == 12) ? 1 : m + 1;
        return days_from_civil(ny, nm, 1) * 86400 * US_PER_SEC;
    }
    return std::nullopt;  // None_ / DailyPassExpired never roll over on a clock
}

std::optional<std::set<std::string>> charge(long long user_id, long long thing_type,
                                            long long thing_id, long long quantity) {
    if (quantity <= 0) return std::set<std::string>{};
    // ThingTypes(int(thing_type)) -- members are contiguous 1..HomeSkin, others raise -> None
    if (thing_type < enums::ThingTypes::Item || thing_type > enums::ThingTypes::HomeSkin)
        return std::nullopt;

    if (thing_type == enums::ThingTypes::Coin || thing_type == enums::ThingTypes::Jewel) {
        std::vector<db::json> rows = db::fetch(db::user::get_currencys(user_id));
        if (rows.empty()) return std::nullopt;
        const db::json& currency = rows.front();
        if (thing_type == enums::ThingTypes::Coin) {
            if (currency.at("coin").get<long long>() < quantity) return std::nullopt;
            db::execute(db::user::add_currency(user_id, -quantity, 0));
        } else {
            // only freeJewel is spent here, the same shortcut routes/gachas.py takes
            if (currency.at("freeJewel").get<long long>() < quantity) return std::nullopt;
            db::execute(db::user::add_currency(user_id, 0, -quantity));
        }
        return std::set<std::string>{"Currency"};
    }

    if (thing_type == enums::ThingTypes::Item) {
        std::vector<db::json> items = db::fetch(db::user::get_items(user_id));
        const db::json* row = nullptr;
        for (const db::json& i : items)
            if (i.at("itemMasterId").get<long long>() == thing_id) {
                row = &i;
                break;
            }
        if (row == nullptr || row->at("stock").get<long long>() < quantity) return std::nullopt;
        // increment_item_stock is a data-modifying CTE: in the SQLite port it executes itself
        // (returning the command tag), so it is not wrapped in db::execute
        db::user::increment_item_stock(user_id, thing_id, -quantity);
        return std::set<std::string>{"Item"};
    }

    // nothing else is ever priced in a captured shop entry; refuse rather than grant free
    return std::nullopt;
}

}  // namespace shops
