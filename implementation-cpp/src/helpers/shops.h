#pragma once
#include <array>
#include <ctime>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "wire.h"

// shop master-data lookups, availability windows, and paying for things. ports
// helpers/shops.py. the four shops (exchange / market / permanent market / jewel) share the
// same (thing_* you get, required_* you pay) shape but each keeps its master in its own
// table. master rows flow as wire::json keyed by the pydantic field name (id_, start_date,
// required_thing_type, ...). reverse indexes over the four masters are built lazily in the
// .cpp.

namespace shops {

using json = wire::json;

// jewels for each successive manual market refresh within one period; ten is the ceiling --
// past that the market cannot be refreshed again until reset
inline constexpr std::array<long long, 10> MARKET_REFRESH_COSTS = {
    10, 20, 30, 30, 30, 50, 50, 50, 50, 50};
inline constexpr long long MARKET_MAX_REFRESHES =
    static_cast<long long>(MARKET_REFRESH_COSTS.size());

// the market turns over twice a day, at 11:00 and 23:00 JST (its own clock, unrelated to the
// 05:00 JST daily reset)
inline constexpr std::array<int, 2> MARKET_RESET_HOURS = {11, 23};

inline constexpr long long MARKET_DISCOUNT_PERCENT = 10;
inline constexpr double MARKET_DISCOUNT_CHANCE = 2.0 / 15.0;

// unlocking a song costs 10 tickets; buying one song's Olivier chart costs 1. both are fixed
// prices with no master row behind them.
inline constexpr long long MUSIC_UNLOCK_ITEM_ID = 130001;
inline constexpr long long MUSIC_UNLOCK_COST = 10;
inline constexpr long long MUSIC_SCORE_ITEM_ID = 130001;
inline constexpr long long MUSIC_SCORE_COST = 1;

// one rolled market frame: (frame_number, master_id, discount_percent)
using MarketFrame = std::tuple<long long, long long, std::optional<long long>>;

// whether now falls inside a master row's sale window. a row whose dates are missing or
// unparseable counts as available; now==nullopt uses the current UTC time.
bool available(const json& start_date, const json& end_date,
               std::optional<std::time_t> now = std::nullopt);

// (ExchangeShopMaster, ExchangeShopThing) for one lineup entry, or (null, null)
std::pair<json, json> exchange_shop_thing(long long m_exchange_shop_thing_id);

// MarketFrameThingMaster for an id, or null
json market_frame(long long market_frame_thing_master_id);

// PermanentMarketThingMaster for an id, or null
json permanent_market_thing(long long permanent_market_thing_master_id);

// JewelShopItemMaster for an id, or null
json jewel_shop_item(long long m_jewel_shop_item_id);

// the song a live (one chart of one song) belongs to, or nullopt
std::optional<long long> live_music_master_id(long long live_master_id);

// roll a fresh market: the full 15 frames, one drawn per frame from that frame's own pool.
// rng==nullptr uses the module default engine.
std::vector<MarketFrame> roll_market(std::mt19937_64* rng = nullptr);

// epoch-micros of the most recent market turnover (11:00 / 23:00 JST)
long long market_reset(long long now_micros);

// jewels for the next manual refresh given how many were already bought this period, or
// nullopt once the ceiling is reached
std::optional<long long> refresh_cost(long long refresh_times);

// when the current limit period ends, in epoch microseconds, or nullopt for a lifetime cap.
// now==nullopt uses the current UTC time.
std::optional<long long> period_until(std::optional<long long> replace_type,
                                      std::optional<std::time_t> now = std::nullopt);

// take payment for one purchase. the present-entity names the charge touched (so the caller
// can report the balance going down), an empty set for a no-op, or nullopt -- having written
// nothing -- when the caller can't afford it.
std::optional<std::set<std::string>> charge(long long user_id, long long thing_type,
                                            long long thing_id, long long quantity);

}  // namespace shops
