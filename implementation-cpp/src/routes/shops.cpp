#include "routes/shops.h"

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "helpers/game_state.h"
#include "helpers/music_unlock.h"
#include "helpers/shops.h"
#include "helpers/things.h"
#include "helpers/user_data.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/shops.py.

namespace routes {
namespace {

using json = wire::json;       // ordered_json: master rows, shop-helper rows, wire results
using rjson = nlohmann::json;  // db rows (camelCase columns)

template <typename J>
long long get_ll(const J& obj, const char* key, long long def) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->template get<long long>();
}

template <typename J>
bool get_bool(const J& obj, const char* key, bool def) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->template get<bool>();
}

template <typename J>
std::optional<long long> opt_ll(const J& obj, const char* key) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return std::nullopt;
    return it->template get<long long>();
}

std::mt19937_64& _engine() {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    return eng;
}

long long _random_id() {
    std::uniform_int_distribution<long long> dist(1000000LL, 9999999999LL);
    return dist(_engine());
}

long long _now() { return static_cast<long long>(std::time(nullptr)) * 1000000LL; }

// A purchase that cannot go through, raised rather than returned so it unwinds the enclosing
// db::transaction(). A purchase writes in stages (count the limit, charge, then grant) and a
// plain return out of the transaction scope commits the partial write; throwing rolls it back.
struct _Rejected {
    std::string code;
    explicit _Rejected(std::string c) : code(std::move(c)) {}
};

json result_array(const std::vector<json>& things) {
    json arr = json::array();
    for (const json& t : things) arr.push_back(t);
    return arr;
}

json present_from(std::optional<long long> user_id, const std::set<std::string>& names) {
    std::vector<user_data::PresentSpec> specs;
    for (const std::string& n : names) specs.emplace_back(n);
    return user_data::build_present(user_id, specs);
}

// Run one exchange-shop purchase: count it against the cap, charge, grant. Returns
// (received_things, refreshed_entity_names), or throws _Rejected. Ordered so nothing is granted
// unless payment succeeded and nothing is charged unless the exchange limit had room.
std::pair<std::vector<json>, std::set<std::string>> _exchange(long long user_id, const json& thing,
                                                              long long quantity, long long now) {
    std::optional<long long> limit = opt_ll(thing, "exchange_limit");
    if (limit.has_value()) {
        std::optional<long long> replace_type = opt_ll(thing, "replace_type");
        std::optional<rjson> row = db::user::consume_exchange_limit(
            user_id, get_ll(thing, "id_", 0), quantity, *limit, replace_type.value_or(0),
            shops::period_until(replace_type), now, _random_id());
        if (!row.has_value()) throw _Rejected("ExchangeLimitExceeded");
    }

    std::optional<std::set<std::string>> spent =
        shops::charge(user_id, enums::ThingTypes::Item, get_ll(thing, "required_item_master_id", 0),
                      get_ll(thing, "required_quantity", 0) * quantity);
    if (!spent.has_value()) throw _Rejected("NotEnoughThing");

    long long tq = get_ll(thing, "thing_quantity", 0);
    std::vector<things::ThingTriple> triples;
    triples.emplace_back(get_ll(thing, "thing_type", enums::ThingTypes::Item),
                         get_ll(thing, "thing_id", 0), (tq != 0 ? tq : 1) * quantity);
    std::vector<json> received = things::grant_things_consolidated(user_id, triples);

    std::set<std::string> refresh(spent->begin(), spent->end());
    for (const json& r : received) {
        std::optional<std::string> pt = things::present_type(r.at("type").get<long long>());
        if (pt.has_value()) refresh.insert(*pt);
    }
    if (limit.has_value()) refresh.insert("ExchangeLimit");
    return {received, refresh};
}

// The caller's current market, rolling a new one if the period turned over. Returns
// (MarketThing rows, refresh_times). The lineup is stored rather than regenerated per call, so a
// reconnect shows the same 15 frames -- and so has_purchased survives.
std::pair<std::vector<rjson>, long long> _market_state(long long user_id, long long now) {
    std::optional<rjson> rolled =
        db::user::roll_over_market(user_id, now, _random_id(), shops::market_reset(now));
    if (rolled.has_value()) {
        // first look since 11:00 or 23:00 JST (or ever): fresh frames, purchases cleared, and the
        // refresh count zeroed with them
        db::user::replace_market_things(user_id, shops::roll_market());
        return {db::fetch(db::user::get_market_things(user_id)), 0};
    }

    std::vector<rjson> markets = db::fetch(db::user::get_markets(user_id));
    long long refresh_times = !markets.empty() ? markets.front().at("refreshTimes").get<long long>() : 0;
    return {db::fetch(db::user::get_market_things(user_id)), refresh_times};
}

json _market_result(const std::vector<rjson>& rows, long long refresh_times) {
    json things = json::array();
    for (const rjson& r : rows) {
        json mt;
        mt["frame_number"] = r.at("frameNumber").get<long long>();
        mt["market_frame_thing_master_id"] = r.at("marketFrameThingMasterId").get<long long>();
        mt["has_purchased"] = r.at("hasPurchased").get<bool>();
        // required_jewel_for_refresh / discount_percent are nullable; leaving them unset lets the
        // wire layer emit null, matching None
        std::optional<long long> dp = opt_ll(r, "discountPercent");
        if (dp.has_value()) mt["discount_percent"] = *dp;
        things.push_back(mt);
    }
    json result;
    result["things"] = things;
    std::optional<long long> cost = shops::refresh_cost(refresh_times);
    if (cost.has_value()) result["required_jewel_for_refresh"] = *cost;
    return result;
}

// Buy one rolled market frame. (received, refreshed), or throws _Rejected.
std::pair<std::vector<json>, std::set<std::string>> _buy_market_frame(long long user_id,
                                                                      long long frame_number) {
    std::optional<rjson> claimed =
        db::fetchrow(db::user::purchase_market_thing(user_id, frame_number));
    if (!claimed.has_value()) {
        // either no such frame, or somebody already bought it -- the UPDATE distinguishes neither,
        // and both mean the same thing to the client
        throw _Rejected("AlreadyPurchased");
    }

    json frame = shops::market_frame(get_ll(*claimed, "marketFrameThingMasterId", 0));
    if (frame.is_null()) throw _Rejected("InvalidRequest");

    long long price = get_ll(frame, "required_thing_quantity", 0);
    std::optional<long long> discount = opt_ll(*claimed, "discountPercent");
    if (discount.has_value() && *discount != 0) price = price * (100 - *discount) / 100;

    std::optional<std::set<std::string>> spent = shops::charge(
        user_id, get_ll(frame, "required_thing_type", enums::ThingTypes::Item),
        get_ll(frame, "required_thing_id", 0), price);
    if (!spent.has_value()) throw _Rejected("NotEnoughThing");

    long long tq = get_ll(frame, "thing_quantity", 0);
    std::vector<things::ThingTriple> triples;
    triples.emplace_back(get_ll(frame, "thing_type", enums::ThingTypes::Item),
                         get_ll(frame, "thing_id", 0), tq != 0 ? tq : 1);
    std::vector<json> received = things::grant_things_consolidated(user_id, triples);

    std::set<std::string> refresh(spent->begin(), spent->end());
    for (const json& r : received) {
        std::optional<std::string> pt = things::present_type(r.at("type").get<long long>());
        if (pt.has_value()) refresh.insert(*pt);
    }
    return {received, refresh};
}

}  // namespace

void register_shops(httplib::Server& svr) {
    // /api/Shops/ExchangeMarketThing/{number}
    svr.Post("/api/Shops/ExchangeMarketThing/:number",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long number = std::stoll(req.path_params.at("number"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id.has_value()) {
                     pipeline::respond(res, "ReceivedThing", json::array());
                     return;
                 }

                 std::vector<json> received;
                 std::set<std::string> refresh;
                 try {
                     auto tx = db::transaction();
                     auto got = _buy_market_frame(*user_id, number);
                     received = got.first;
                     refresh = got.second;
                     tx.commit();
                 } catch (const _Rejected& rejected) {
                     pipeline::respond(res, "ReceivedThing", json::array(),
                                       json::array({pipeline::fault(rejected.code)}));
                     return;
                 }

                 json present = present_from(user_id, refresh);
                 pipeline::respond(res, "ReceivedThing", result_array(received), json::array(),
                                   present);
             });

    // /api/Shops/ExchangeMarketThings
    svr.Post("/api/Shops/ExchangeMarketThings",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = pipeline::read_request(req);
                 if (!user_id.has_value()) {
                     pipeline::respond(res, "ReceivedThing", json::array());
                     return;
                 }

                 // a bare list of frame numbers, e.g. [8, 9] -- confirmed by capture
                 std::vector<long long> frames;
                 if (payload.is_array())
                     for (const json& n : payload)
                         frames.push_back(n.is_number_float() ? static_cast<long long>(n.get<double>())
                                                              : n.get<long long>());

                 std::vector<json> received;
                 std::set<std::string> refresh;
                 try {
                     // one transaction for the whole batch: if the third frame is unaffordable the
                     // first two are rolled back too, rather than half-buying the request
                     auto tx = db::transaction();
                     for (long long number : frames) {
                         auto got = _buy_market_frame(*user_id, number);
                         received.insert(received.end(), got.first.begin(), got.first.end());
                         refresh.insert(got.second.begin(), got.second.end());
                     }
                     tx.commit();
                 } catch (const _Rejected& rejected) {
                     pipeline::respond(res, "ReceivedThing", json::array(),
                                       json::array({pipeline::fault(rejected.code)}));
                     return;
                 }

                 json present = refresh.empty() ? json::array() : present_from(user_id, refresh);
                 pipeline::respond(res, "ReceivedThing", result_array(received), json::array(),
                                   present);
             });

    // /api/Shops/ExchangeMusic/{mMusicId}
    svr.Post("/api/Shops/ExchangeMusic/:mMusicId",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long m_music_id = std::stoll(req.path_params.at("mMusicId"));
                 // only songs whose unlock condition is a store exchange (10) or a story unlock (11)
                 // can be bought; a story-gated song requires all its story episodes read first
                 try {
                     game_state::State s = game_state::transaction(req);
                     const json* m = game_state::master("music_master", m_music_id);
                     rjson* row = s.one("Music", rjson{{"musicMasterId", m_music_id}});
                     if (m == nullptr) throw game_state::Rejected();
                     long long unlock_condition_type = get_ll(*m, "unlock_condition_type", 0);
                     bool possessed = (row != nullptr) && row->at("isPossession").get<bool>();
                     if (get_bool(*m, "invisible", false) ||
                         (unlock_condition_type != 10 && unlock_condition_type != 11) || possessed)
                         throw game_state::Rejected();
                     if (unlock_condition_type == 11) {
                         std::optional<long long> story_master_id = opt_ll(*m, "story_master_id");
                         std::vector<long long> episodes;
                         if (story_master_id.has_value())
                             for (const json& e : master_data::table("EpisodeMaster"))
                                 if (get_ll(e, "story_master_id", 0) == *story_master_id)
                                     episodes.push_back(get_ll(e, "id_", 0));
                         std::set<long long> read;
                         for (const rjson& e : s.rows("Episode"))
                             read.insert(e.at("episodeMasterId").get<long long>());
                         bool any_unread = false;
                         for (long long id : episodes)
                             if (read.find(id) == read.end()) {
                                 any_unread = true;
                                 break;
                             }
                         if (episodes.empty() || any_unread) throw game_state::Rejected();
                     }
                     s.pay({{shops::MUSIC_UNLOCK_ITEM_ID, shops::MUSIC_UNLOCK_COST}});
                     if (row != nullptr) {
                         s.update("Music", row, rjson{{"isPossession", true}});
                     } else {
                         s.insert("Music", rjson{{"musicMasterId", m_music_id},
                                                 {"isPossession", true},
                                                 {"stellaReleased", false},
                                                 {"olivierReleaseStatus", 0},
                                                 {"vocalVersion", 0}});
                     }
                     s.commit();
                     json result;
                     result["type"] = enums::ThingTypes::Music;
                     result["id_"] = m_music_id;
                     result["quantity"] = 1;
                     pipeline::respond(res, "ReceivedThing", result, json::array(), s.present());
                 } catch (const game_state::Rejected&) {
                     json result;
                     result["type"] = enums::ThingTypes::Item;  // ReceivedThing() default type
                     pipeline::respond(res, "ReceivedThing", result);
                 }
             });

    // /api/Shops/ExchangeMusicScore/{mLiveId}
    svr.Post("/api/Shops/ExchangeMusicScore/:mLiveId",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long m_live_id = std::stoll(req.path_params.at("mLiveId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 // buys the Olivier chart of the song this live belongs to. Only a chart the user has
                 // already made Purchasable can be bought; the guard lives in release_music_olivier.
                 std::optional<long long> music_master_id = shops::live_music_master_id(m_live_id);
                 if (!user_id.has_value() || !music_master_id.has_value()) {
                     pipeline::respond(res, "BooleanResult", json::object());
                     return;
                 }

                 std::set<std::string> spent_names;
                 try {
                     auto tx = db::transaction();
                     std::optional<rjson> released = db::fetchrow(db::user::release_music_olivier(
                         *user_id, *music_master_id, enums::OlivierReleaseStatuses::Purchasable,
                         enums::OlivierReleaseStatuses::Released));
                     if (!released.has_value()) {
                         // not owned, not yet purchasable, or already bought
                         throw _Rejected("InvalidRequest");
                     }

                     std::optional<std::set<std::string>> spent =
                         shops::charge(*user_id, enums::ThingTypes::Item, shops::MUSIC_SCORE_ITEM_ID,
                                       shops::MUSIC_SCORE_COST);
                     if (!spent.has_value()) throw _Rejected("NotEnoughThing");
                     spent_names = *spent;

                     // the 20th Olivier bought opens every owned song's Stella; the whole Music list
                     // is rebuilt below either way
                     music_unlock::apply_unlocks(*user_id);
                     tx.commit();
                 } catch (const _Rejected& rejected) {
                     pipeline::respond(res, "BooleanResult", json::object(),
                                       json::array({pipeline::fault(rejected.code)}));
                     return;
                 }

                 std::set<std::string> refresh = spent_names;
                 refresh.insert("Music");
                 json present = present_from(user_id, refresh);
                 json result;
                 result["is_success"] = true;
                 pipeline::respond(res, "BooleanResult", result, json::array(), present);
             });

    // /api/Shops/ExchangePermanentMarketThing/{PermanentMarketThingMasterid}
    svr.Post(
        "/api/Shops/ExchangePermanentMarketThing/:PermanentMarketThingMasterid",
        [](const httplib::Request& req, httplib::Response& res) {
            long long path_master_id =
                std::stoll(req.path_params.at("PermanentMarketThingMasterid"));
            std::optional<long long> query_master_id;
            if (req.has_param("permanentMarketThingMasterId"))
                query_master_id = std::stoll(req.get_param_value("permanentMarketThingMasterId"));
            std::optional<long long> quantity;
            if (req.has_param("quantity"))
                quantity = std::stoll(req.get_param_value("quantity"));

            std::optional<long long> user_id = user_data::current_user_id(req);
            // the route carries the same master id twice, in the path and the query; they agree in
            // every capture, and the path segment is authoritative here
            std::optional<long long> master_id =
                (path_master_id != 0) ? std::optional<long long>(path_master_id) : query_master_id;
            long long base = (quantity.has_value() && *quantity != 0) ? *quantity : 1;
            long long count = std::max(1LL, base);
            json entry = master_id.has_value() ? shops::permanent_market_thing(*master_id)
                                               : json(nullptr);
            if (!user_id.has_value() || entry.is_null()) {
                pipeline::respond(res, "ReceivedThing", json::array());
                return;
            }
            if (!shops::available(entry.value("start_date", json(nullptr)),
                                  entry.value("end_date", json(nullptr)))) {
                pipeline::respond(res, "ReceivedThing", json::array(),
                                  json::array({pipeline::fault("OutOfPeriod")}));
                return;
            }

            std::vector<json> received;
            std::optional<long long> exchange_limit = opt_ll(entry, "exchange_limit");
            std::optional<std::set<std::string>> spent;
            try {
                auto tx = db::transaction();
                if (exchange_limit.has_value()) {
                    std::optional<rjson> row = db::user::consume_permanent_market_limit(
                        *user_id, get_ll(entry, "id_", 0), count, *exchange_limit);
                    if (!row.has_value()) throw _Rejected("ExchangeLimitExceeded");
                }

                spent = shops::charge(*user_id, get_ll(entry, "required_thing_type", enums::ThingTypes::Item),
                                      get_ll(entry, "required_thing_id", 0),
                                      get_ll(entry, "required_thing_quantity", 0) * count);
                if (!spent.has_value()) throw _Rejected("NotEnoughThing");

                long long tq = get_ll(entry, "thing_quantity", 0);
                std::vector<things::ThingTriple> triples;
                triples.emplace_back(get_ll(entry, "thing_type", enums::ThingTypes::Item),
                                     get_ll(entry, "thing_id", 0), (tq != 0 ? tq : 1) * count);
                received = things::grant_things_consolidated(*user_id, triples);
                tx.commit();
            } catch (const _Rejected& rejected) {
                pipeline::respond(res, "ReceivedThing", json::array(),
                                  json::array({pipeline::fault(rejected.code)}));
                return;
            }

            std::set<std::string> refresh(spent->begin(), spent->end());
            for (const json& r : received) {
                std::optional<std::string> pt = things::present_type(r.at("type").get<long long>());
                if (pt.has_value()) refresh.insert(*pt);
            }
            if (exchange_limit.has_value()) refresh.insert("PermanentMarketThing");
            json present = present_from(user_id, refresh);
            pipeline::respond(res, "ReceivedThing", result_array(received), json::array(), present);
        });

    // /api/Shops/ExchangeShopThing/{mExchangeShopThingId}/{quantity}
    svr.Post(
        "/api/Shops/ExchangeShopThing/:mExchangeShopThingId/:quantity",
        [](const httplib::Request& req, httplib::Response& res) {
            long long m_exchange_shop_thing_id =
                std::stoll(req.path_params.at("mExchangeShopThingId"));
            long long quantity = std::stoll(req.path_params.at("quantity"));
            std::optional<long long> user_id = user_data::current_user_id(req);
            std::pair<json, json> pair = shops::exchange_shop_thing(m_exchange_shop_thing_id);
            const json& shop = pair.first;
            const json& thing = pair.second;
            long long count = std::max(1LL, quantity != 0 ? quantity : 1);
            if (!user_id.has_value() || thing.is_null()) {
                pipeline::respond(res, "ReceivedThing", json::array());
                return;
            }
            if (!shops::available(thing.value("start_date", json(nullptr)),
                                  thing.value("end_date", json(nullptr))) ||
                !shops::available(shop.value("start_date", json(nullptr)),
                                  shop.value("end_date", json(nullptr)))) {
                pipeline::respond(res, "ReceivedThing", json::array(),
                                  json::array({pipeline::fault("OutOfPeriod")}));
                return;
            }

            long long now = _now();
            std::vector<json> received;
            std::set<std::string> meta;
            try {
                auto tx = db::transaction();
                auto got = _exchange(*user_id, thing, count, now);
                received = got.first;
                meta = got.second;
                tx.commit();
            } catch (const _Rejected& rejected) {
                pipeline::respond(res, "ReceivedThing", json::array(),
                                  json::array({pipeline::fault(rejected.code)}));
                return;
            }

            json present = present_from(user_id, meta);
            pipeline::respond(res, "ReceivedThing", result_array(received), json::array(), present);
        });

    // /api/Shops/ExchangeShopThings
    svr.Post("/api/Shops/ExchangeShopThings",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = pipeline::read_request(req, "ExchangeShopThingPayload");
                 if (!user_id.has_value() || payload.is_null()) {
                     pipeline::respond(res, "ReceivedThing", json::array());
                     return;
                 }

                 // TODO: the singular payload here is what read_request already decoded, so this is
                 // treated as a one-entry batch. If the real endpoint takes a list the model needs
                 // changing to match -- capture pending.
                 std::pair<json, json> pair =
                     shops::exchange_shop_thing(get_ll(payload, "m_exchange_shop_thing_id", 0));
                 const json& shop = pair.first;
                 const json& thing = pair.second;
                 if (thing.is_null()) {
                     pipeline::respond(res, "ReceivedThing", json::array());
                     return;
                 }
                 if (!shops::available(thing.value("start_date", json(nullptr)),
                                       thing.value("end_date", json(nullptr))) ||
                     !shops::available(shop.value("start_date", json(nullptr)),
                                       shop.value("end_date", json(nullptr)))) {
                     pipeline::respond(res, "ReceivedThing", json::array(),
                                       json::array({pipeline::fault("OutOfPeriod")}));
                     return;
                 }

                 long long now = _now();
                 long long q = get_ll(payload, "quantity", 0);
                 std::vector<json> received;
                 std::set<std::string> meta;
                 try {
                     auto tx = db::transaction();
                     auto got = _exchange(*user_id, thing, std::max(1LL, q != 0 ? q : 1), now);
                     received = got.first;
                     meta = got.second;
                     tx.commit();
                 } catch (const _Rejected& rejected) {
                     pipeline::respond(res, "ReceivedThing", json::array(),
                                       json::array({pipeline::fault(rejected.code)}));
                     return;
                 }

                 json present = present_from(user_id, meta);
                 pipeline::respond(res, "ReceivedThing", result_array(received), json::array(),
                                   present);
             });

    // /api/Shops/GetOrRefreshMarket
    svr.Post("/api/Shops/GetOrRefreshMarket",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id.has_value()) {
                     pipeline::respond(res, "MarketResult", json::object());
                     return;
                 }

                 long long now = _now();
                 std::vector<rjson> rows;
                 long long refresh_times = 0;
                 {
                     auto tx = db::transaction();
                     auto state = _market_state(*user_id, now);
                     rows = state.first;
                     refresh_times = state.second;
                     tx.commit();
                 }
                 pipeline::respond(res, "MarketResult", _market_result(rows, refresh_times));
             });

    // /api/Shops/Purchase
    svr.Post("/api/Shops/Purchase", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        json payload = pipeline::read_request(req, "PurchaseItemPayload");
        if (!user_id.has_value() || payload.is_null()) {
            pipeline::respond(res, "ReceivedThing", json::array());
            return;
        }

        json item = shops::jewel_shop_item(get_ll(payload, "m_jewel_shop_item_id", 0));
        if (item.is_null()) {
            pipeline::respond(res, "ReceivedThing", json::array());
            return;
        }
        if (!shops::available(item.value("start_date", json(nullptr)),
                              item.value("end_date", json(nullptr)))) {
            pipeline::respond(res, "ReceivedThing", json::array(),
                              json::array({pipeline::fault("OutOfPeriod")}));
            return;
        }

        // NOTE: this grants the pack outright. These are real-money IAPs and the actual store
        // receipt is validated by the KmsGeneralPayment routes, not here -- on a private server
        // there is no receipt to check, so the purchase is simply honoured.
        std::vector<json> received;
        {
            auto tx = db::transaction();
            std::vector<things::ThingTriple> triples;
            triples.emplace_back(enums::ThingTypes::Jewel, 0LL, get_ll(item, "give_paid_jewel", 0));
            received = things::grant_things_consolidated(*user_id, triples);
            db::user::record_jewel_shop_purchase(*user_id, get_ll(item, "id_", 0), _random_id(),
                                                 std::nullopt);
            tx.commit();
        }

        std::set<std::string> refresh;
        for (const json& r : received) {
            std::optional<std::string> pt = things::present_type(r.at("type").get<long long>());
            if (pt.has_value()) refresh.insert(*pt);
        }
        refresh.insert("JewelShop");
        json present = present_from(user_id, refresh);
        pipeline::respond(res, "ReceivedThing", result_array(received), json::array(), present);
    });

    // /api/Shops/UpdateLastViewedAt
    svr.Post("/api/Shops/UpdateLastViewedAt",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = pipeline::read_request(req, "UpdateLastViewedAtPayload");
                 if (!user_id.has_value() || payload.is_null()) {
                     pipeline::respond(res, "BooleanResult", json::object());
                     return;
                 }

                 db::user::touch_viewed_shop(
                     *user_id, get_ll(payload, "viewed_shop_category_types", 0),
                     opt_ll(payload, "exchange_shop_master_id"), _now(), _random_id());
                 json present = present_from(user_id, {"ViewedShop"});
                 json result;
                 result["is_success"] = true;
                 pipeline::respond(res, "BooleanResult", result, json::array(), present);
             });

    // /api/Shops/RefreshMarketWithJewel
    svr.Post("/api/Shops/RefreshMarketWithJewel",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id.has_value()) {
                     pipeline::respond(res, "MarketResult", json::object());
                     return;
                 }

                 long long now = _now();
                 std::vector<rjson> rows;
                 long long refresh_times = 0;
                 try {
                     auto tx = db::transaction();
                     // claim the refresh slot first: it both enforces the per-period ceiling and
                     // tells us which step of the price curve this refresh is. Charging first would
                     // mean pricing off a count a concurrent refresh could move.
                     std::optional<rjson> claimed = db::user::consume_market_refresh(
                         *user_id, now, _random_id(), shops::market_reset(now),
                         shops::MARKET_MAX_REFRESHES);
                     if (!claimed.has_value()) throw _Rejected("RefreshLimitExceeded");

                     refresh_times = claimed->at("refreshTimes").get<long long>();
                     // the claim already counted this one, so the price is the step it just took
                     std::optional<long long> price = shops::refresh_cost(refresh_times - 1);
                     std::optional<std::set<std::string>> spent =
                         shops::charge(*user_id, enums::ThingTypes::Jewel, 0, price.value_or(0));
                     if (!spent.has_value()) throw _Rejected("NotEnoughThing");

                     db::user::replace_market_things(*user_id, shops::roll_market());
                     rows = db::fetch(db::user::get_market_things(*user_id));
                     tx.commit();
                 } catch (const _Rejected& rejected) {
                     pipeline::respond(res, "MarketResult", json::object(),
                                       json::array({pipeline::fault(rejected.code)}));
                     return;
                 }

                 json present = present_from(user_id, {"Currency", "Market"});
                 pipeline::respond(res, "MarketResult", _market_result(rows, refresh_times),
                                   json::array(), present);
             });

    // /api/Shops/ViewPage
    svr.Post("/api/Shops/ViewPage", [](const httplib::Request&, httplib::Response& res) {
        // TODO: unimplemented. Returns ConvertedThingResult[], but nothing in master data says what
        // triggers a conversion -- an empty list is the safe stand-in. Blocked on a capture.
        pipeline::respond(res, "ViewShopResult", json::object());
    });
}

}  // namespace routes
