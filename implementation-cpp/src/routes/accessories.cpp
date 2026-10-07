#include "routes/accessories.h"

#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "helpers/costs.h"
#include "helpers/user_data.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/accessories.py. only LevelUp has real logic; the rest are stubs.

namespace routes {
namespace {

using wire::json;

long long jint(const json& obj, const char* key, long long def) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->get<long long>();
}

const json& opt_array(const json& obj, const char* key) {
    static const json kEmpty = json::array();
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null() || !it->is_array()) return kEmpty;
    return *it;
}

// lazy master indexes keyed by id_ (built once; master_data::load() runs at startup)
const json* _accessory_master(long long master_id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& a : master_data::table("AccessoryMaster")) m[jint(a, "id_", 0)] = &a;
        return m;
    }();
    auto it = idx.find(master_id);
    return it == idx.end() ? nullptr : it->second;
}

const json* _pattern_group(long long group_id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& g : master_data::table("AccessoryLevelPatternGroupMaster"))
            m[jint(g, "id_", 0)] = &g;
        return m;
    }();
    auto it = idx.find(group_id);
    return it == idx.end() ? nullptr : it->second;
}

// coin + item bill to raise an accessory from from_level to to_level, summing every
// 1-level step in between. nullopt if a step pattern is missing.
std::optional<std::pair<long long, costs::ItemMap>> _level_cost(const json& group,
                                                                long long from_level,
                                                                long long to_level) {
    std::unordered_map<long long, const json*> by_level;
    for (const json& p : opt_array(group, "patterns")) by_level[jint(p, "level", 0)] = &p;
    long long coin = 0;
    costs::ItemMap items;
    for (long long level = from_level; level < to_level; ++level) {
        auto it = by_level.find(level);
        if (it == by_level.end()) return std::nullopt;
        const json& pattern = *it->second;
        coin += jint(pattern, "required_coin", 0);
        for (const json& entry : opt_array(pattern, "items"))
            items[jint(entry, "item_master_id", 0)] += jint(entry, "quantity", 0);
    }
    return std::make_pair(coin, items);
}

}  // namespace

void register_accessories(httplib::Server& svr) {
    // /api/Accessories/IncreaseAcquirableAccessoryLimit?toPhase=
    svr.Post("/api/Accessories/IncreaseAcquirableAccessoryLimit",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    // /api/Accessories/{uAccessoryId}/LevelUp/{levelTo}
    svr.Post("/api/Accessories/:uAccessoryId/LevelUp/:levelTo",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long u_accessory_id = std::stoll(req.path_params.at("uAccessoryId"));
                 long long level_to = std::stoll(req.path_params.at("levelTo"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id) {
                     pipeline::respond(res, "BooleanResult", json::object());
                     return;
                 }

                 long long coin = 0;
                 costs::ItemMap items;
                 {
                     auto tx = db::transaction();
                     std::optional<db::json> accessory;
                     for (const db::json& a : db::fetch(db::user::get_accessorys(*user_id))) {
                         if (a.at("id").get<long long>() == u_accessory_id) {
                             accessory = a;
                             break;
                         }
                     }
                     const json* master =
                         accessory
                             ? _accessory_master(accessory->at("accessoryMasterId").get<long long>())
                             : nullptr;
                     if (!accessory || !master) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("InvalidRequest")}));
                         return;
                     }
                     const json* group =
                         _pattern_group(jint(*master, "accessory_level_pattern_group_id", 0));
                     long long level = accessory->at("level").get<long long>();
                     if (!group || !(level < level_to && level_to <= jint(*master, "max_level", 0))) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("InvalidRequest")}));
                         return;
                     }
                     std::optional<std::pair<long long, costs::ItemMap>> cost =
                         _level_cost(*group, level, level_to);
                     if (!cost) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("InvalidRequest")}));
                         return;
                     }
                     coin = cost->first;
                     items = cost->second;
                     // check the whole bill (coin + items) before charging anything, so a partial
                     // charge can't commit when only one side is affordable
                     costs::ItemMap stock = costs::item_stock(*user_id);
                     std::vector<db::json> currency = db::fetch(db::user::get_currencys(*user_id));
                     bool short_item = false;
                     for (const auto& [i, q] : items) {
                         auto it = stock.find(i);
                         if ((it == stock.end() ? 0 : it->second) < q) {
                             short_item = true;
                             break;
                         }
                     }
                     if (currency.empty() || currency.front().at("coin").get<long long>() < coin ||
                         short_item) {
                         pipeline::respond(res, "BooleanResult", json::object(),
                                           json::array({pipeline::fault("NotEnoughThing")}));
                         return;
                     }
                     costs::pay_coin(*user_id, coin);
                     costs::pay_items(*user_id, items, stock);
                     db::execute(db::user::update_accessory_level(*user_id, u_accessory_id, level_to));
                     tx.commit();
                 }

                 std::vector<user_data::PresentSpec> updates;
                 updates.emplace_back("Accessory", std::vector<long long>{u_accessory_id});
                 std::vector<long long> item_ids;
                 for (const auto& [i, q] : items) {
                     (void)q;
                     item_ids.push_back(i);
                 }
                 updates.emplace_back("Item", item_ids);
                 if (coin) updates.push_back("Currency");
                 json present = user_data::build_present(user_id, updates);

                 json result;
                 result["is_success"] = true;
                 pipeline::respond(res, "BooleanResult", result, json::array(), present);
             });

    // /api/Accessories/Sell
    svr.Post("/api/Accessories/Sell", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "SellAccessoryPayload");
        pipeline::respond(res, "BooleanResult", json::object());
    });

    // /api/Accessories/SetAccessoryAutoSell?rarityFlag=
    svr.Post("/api/Accessories/SetAccessoryAutoSell",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    // /api/Accessories/SetFavorite
    svr.Post("/api/Accessories/SetFavorite", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "AccessoryFavoritePayload");
        pipeline::respond(res, "BooleanResult", json::object());
    });

    // /api/Accessories/{accessoryId}/SwitchLock
    svr.Post("/api/Accessories/:accessoryId/SwitchLock",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });
}

}  // namespace routes
