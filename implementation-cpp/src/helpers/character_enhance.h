#pragma once
#include <nlohmann/json.hpp>

#include <map>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

// Awakening / sense-enhance / talent-bloom costs read off a CharacterMaster.
// Ports helpers/character_enhance.py. Master rows are ordered_json keyed by the
// pydantic field name; own pieces pay first and the generic stand-in covers the rest.

namespace character_enhance {

using json = nlohmann::ordered_json;

// {item_master_id: quantity}
using item_counts = std::map<long long, long long>;

// The CharacterMaster row a character is built from, or nullptr.
const json* character_master(long long character_master_id);

// --- awakening ---------------------------------------------------------------------------

// The phase an awakened character sits at (0 when it can't awaken).
long long max_awakening_phase(const json& master);

// The items to awaken out of `phase`, or nullopt when there is no such step.
std::optional<item_counts> awakening_cost(const json& master, long long phase);

// --- sense enhancement -------------------------------------------------------------------

// The highest sense level this character's enhance group can reach.
long long max_sense_level(const json& master);

// The items to take a sense from `level_from` to `level_to`, or nullopt if any step between
// isn't priced.
std::optional<item_counts> sense_enhance_cost(const json& master, long long level_from, long long level_to);

// --- talent bloom ------------------------------------------------------------------------

// The highest bloom stage available; `now` (epoch seconds, UTC) defaults to the current time.
long long max_talent_stage(const json& master, std::optional<long long> now = std::nullopt);

// The items a bloom from `stage_from` to `stage_to` costs, or nullopt when a step isn't priced
// or can't be paid for at all. `stock` ({item_master_id: owned}) is read, never written.
std::optional<item_counts> bloom_cost(const json& master, long long stage_from, long long stage_to,
                                      const item_counts& stock);

// The (thing_type, thing_id, quantity) handed out for the stages newly reached.
std::vector<std::tuple<long long, long long, long long>> bloom_rewards(const json& master, long long stage_from,
                                                                       long long stage_to);

// internal to the module, but imported by routes/characters.py -- declared for that port.
const json* _bloom_step(long long rarity, long long stage);
const json* _piece(long long character_master_id, long long bloom_item_type);
// The stand-in item for a step's piece (or nullopt) and how many of it one piece is worth.
std::pair<std::optional<long long>, long long> _generic_piece(const json& master, const json& step,
                                                              const json& piece);

}  // namespace character_enhance
