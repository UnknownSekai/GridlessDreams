#include "costs.h"

#include <vector>

#include "../db.h"
#include "../db/user.h"

namespace costs {

ItemMap item_stock(long long user_id) {
    ItemMap out;
    for (const db::json& i : db::fetch(db::user::get_items(user_id))) {
        out[i.at("itemMasterId").get<long long>()] = i.at("stock").get<long long>();
    }
    return out;
}

bool pay_items(long long user_id, const ItemMap& cost,
               const std::optional<ItemMap>& stock) {
    ItemMap bill;
    for (const auto& [item, quantity] : cost) {
        if (quantity) bill[item] = quantity;
    }
    if (bill.empty()) return true;

    ItemMap owned_local;
    const ItemMap* owned;
    if (stock.has_value()) {
        owned = &*stock;
    } else {
        owned_local = item_stock(user_id);
        owned = &owned_local;
    }

    for (const auto& [item, quantity] : bill) {
        auto it = owned->find(item);
        long long have = it == owned->end() ? 0 : it->second;
        if (have < quantity) return false;
    }

    std::vector<std::pair<long long, long long>> deltas;
    for (const auto& [item, quantity] : bill) {
        deltas.emplace_back(item, -quantity);
    }
    db::user::increment_item_stocks(user_id, deltas);
    return true;
}

bool pay_coin(long long user_id, long long amount) {
    if (amount <= 0) return true;
    std::vector<db::json> rows = db::fetch(db::user::get_currencys(user_id));
    if (rows.empty() || rows.front().at("coin").get<long long>() < amount) {
        return false;
    }
    db::execute(db::user::add_currency(user_id, -amount));
    return true;
}

}  // namespace costs
