#pragma once
#include <ctime>
#include <map>
#include <optional>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

#include "wire.h"

// gacha lineup / roll-limit computation. ports helpers/gacha.py. per-rarity rates are
// server-side config (_RATES, only gacha_type=Pickup verified); everything else derives
// from GachaMaster + the Character/Poster master a thing points at. displayed probabilities
// are CEILED to 7 decimals, not rounded. master rows (gacha / detail / thing) and the
// returned entities (GachaLineupItemProbability, Character/PosterRarityProbability,
// GachaRollLimit) all flow as wire::json keyed by the pydantic field name.

namespace gacha {

using json = wire::json;

// (thing_type, thing_id, quantity)
using ThingTriple = std::tuple<long long, long long, long long>;

// the four lists lineup() yields, positionally: rarity probabilities + per-item
// probabilities for the normal and fixed pools
struct Lineup {
  std::vector<json> normal_probabilities;
  std::vector<json> fixed_probabilities;
  std::vector<json> normal_items;
  std::vector<json> fixed_items;
};

// probabilities for a banner, or nullopt if the gacha is unknown / has no rate entry
std::optional<Lineup> lineup(long long gacha_master_id);

// (normal, fixed) GachaEmissionFlags bitmasks; (0, 0) for an unknown / unrated banner
std::pair<long long, long long> emission_flags(long long gacha_master_id);

// (gacha, detail) master rows for a detail id, or (null, null). RollGacha is addressed by
// DETAIL id.
std::pair<json, json> detail_of(long long gacha_detail_master_id);

// draw prize_count things (the last fixed_prize_count from the fixed pool); GachaMaster
// thing rows. rng==nullptr uses the module default engine.
std::vector<json> roll_prizes(const json& gacha, const json& detail,
                              std::mt19937_64* rng = nullptr);

// (thing_type, thing_id, quantity) a duplicate pays out instead of the possession; empty
// when the prize is not a convertible type
std::vector<ThingTriple> dupe_conversion(const json& gacha, const json& thing);

// the TalentBloom ActorDaiPiece item a Rare4 character's duplicate pays out
std::optional<long long> dai_piece_item(long long character_master_id);

// how far a poster can be broken through
long long poster_max_phase(long long poster_master_id);

// the TalentBloom ActorPiece item a character converts to, per CharacterPieceMaster
std::optional<long long> piece_item(long long character_master_id);

// (thing_type, thing_id, quantity) piece bonus attached to a prize; empty for anything but
// a Rare4 character
std::vector<ThingTriple> piece_bonus(const json& gacha, const json& thing);

// banner ids currently inside their start/end window, in master order. now==nullopt uses
// the current UTC time.
std::vector<long long> active_gacha_ids(std::optional<std::time_t> now = std::nullopt);

// remaining rolls for each of a banner's limited details (unlimited details omitted);
// used maps detail id -> rolls spent. elements are GachaRollLimit entities.
std::vector<json> roll_limits(long long gacha_master_id,
                              const std::map<long long, long long>& used);

}  // namespace gacha
