#pragma once
#include <map>
#include <optional>

// Charge a caller's items/coin atomically for an upgrade or purchase.
// Ports helpers/costs.py. Each helper checks affordability first and writes
// nothing unless the whole bill can be paid, so an op can never half-charge and
// then fail; call inside the operation's own db transaction.

namespace costs {

// {itemMasterId: stock-or-quantity}
using ItemMap = std::map<long long, long long>;

// everything the caller owns, {itemMasterId: stock}
ItemMap item_stock(long long user_id);

// charge a whole item bill ({itemMasterId: quantity}) in one statement;
// false (writing nothing) if any line is unaffordable
bool pay_items(long long user_id, const ItemMap& cost,
               const std::optional<ItemMap>& stock = std::nullopt);

// charge amount coin; false (writing nothing) if the balance is short
bool pay_coin(long long user_id, long long amount);

}  // namespace costs
