#pragma once
#include <nlohmann/json.hpp>

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// character leveling: the exp curve, the level ceiling, and experience items.
// ports helpers/character_level.py. master rows flow as ordered_json keyed by the
// pydantic field name; rarities flow as the enums::CharacterRarities integer value.
// datetimes flow as epoch seconds (utc).

namespace character_level {

using json = nlohmann::ordered_json;

// exact fixed-point rarity coefficient: one decimal place stored as value*kScale,
// never a double since 0.3 is not representable
struct Decimal {
    long long scaled = 0;
    static constexpr long long kScale = 10;
};

// whether start_date (iso string) is at or before at; unparseable/missing reads as no restriction
bool started(const std::string& start_date, long long at);

long long released_max_level(std::optional<long long> now = std::nullopt);

// user is the user db row; reads playerRankLimit
long long level_cap(const nlohmann::json& user, std::optional<long long> now = std::nullopt);

std::optional<long long> experience_to_level_up(long long level);

std::optional<long long> character_rarity(long long character_master_id);

Decimal required_experience_coefficient(std::optional<long long> rarity);

long long scaled_experience(long long amount, std::optional<long long> rarity);

long long experience_to_reach(long long level, long long current_experience, long long target,
                              std::optional<long long> rarity = std::nullopt);

// returns (level, currentExperience)
std::pair<long long, long long> apply_experience(long long level, long long current_experience,
                                                 long long gained, long long cap,
                                                 std::optional<long long> rarity = std::nullopt);

// CharacterExperienceItemMaster row, or nullopt if the item isn't an exp item
std::optional<json> experience_item(long long item_master_id);

// every exp item, richest first
std::vector<json> experience_items();

// pick exp items from stock ({itemMasterId: owned}) to cover needed; returns {itemMasterId: quantity}
std::map<long long, long long> spend_from_pool(const std::map<long long, long long>& stock,
                                               long long needed);

}  // namespace character_level
