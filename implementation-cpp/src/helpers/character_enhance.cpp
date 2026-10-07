#include "helpers/character_enhance.h"

#include <algorithm>
#include <ctime>
#include <string>
#include <unordered_map>

#include "generated/enums_generated.h"
#include "helpers/character_level.h"
#include "master_data.h"

namespace character_enhance {
namespace {

// snake_case cache tables in the python map to these PascalCase header names.
// id_-keyed lookups are memoized exactly like python's _INDEX.
std::unordered_map<std::string, std::unordered_map<long long, const json*>> _INDEX;

const std::unordered_map<long long, const json*>& _by_id(const std::string& header_name) {
    auto it = _INDEX.find(header_name);
    if (it != _INDEX.end()) return it->second;
    auto& idx = _INDEX[header_name];
    for (const json& r : master_data::table(header_name)) {
        auto id = r.find("id_");
        if (id != r.end() && id->is_number_integer()) idx[id->get<long long>()] = &r;
    }
    return idx;
}

const json* lookup(const std::string& header_name, long long id) {
    const auto& idx = _by_id(header_name);
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

std::optional<long long> opt_int(const json& obj, const char* key) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return std::nullopt;
    return it->get<long long>();
}

std::optional<std::string> opt_str(const json& obj, const char* key) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return std::nullopt;
    return it->get<std::string>();
}

const json* opt_array(const json& obj, const char* key) {
    auto it = obj.find(key);
    if (it == obj.end() || !it->is_array()) return nullptr;
    return &(*it);
}

long long count_of(const item_counts& m, long long key) {
    auto it = m.find(key);
    return it == m.end() ? 0 : it->second;
}

void _add(item_counts& cost, long long item_master_id, long long quantity) {
    cost[item_master_id] = count_of(cost, item_master_id) + quantity;
}

}  // namespace

const json* character_master(long long character_master_id) {
    return lookup("CharacterMaster", character_master_id);
}

// --- awakening ---------------------------------------------------------------------------

long long max_awakening_phase(const json& master) {
    std::optional<long long> gid = opt_int(master, "character_awakening_item_group_master_id");
    const json* group = gid ? lookup("CharacterAwakeningItemGroupMaster", *gid) : nullptr;
    const json* items = group ? opt_array(*group, "items") : nullptr;
    if (!group || !items || items->empty()) return 0;
    long long best = items->front().at("awakening_phase").get<long long>();
    for (const auto& i : *items) best = std::max(best, i.at("awakening_phase").get<long long>());
    return best + 1;
}

std::optional<item_counts> awakening_cost(const json& master, long long phase) {
    std::optional<long long> gid = opt_int(master, "character_awakening_item_group_master_id");
    const json* group = gid ? lookup("CharacterAwakeningItemGroupMaster", *gid) : nullptr;
    if (!group) return std::nullopt;
    item_counts cost;
    if (const json* items = opt_array(*group, "items")) {
        for (const auto& item : *items) {
            if (item.at("awakening_phase").get<long long>() == phase)
                _add(cost, item.at("item_master_id").get<long long>(),
                     item.at("required_quantity").get<long long>());
        }
    }
    if (cost.empty()) return std::nullopt;
    return cost;
}

// --- sense enhancement -------------------------------------------------------------------

long long max_sense_level(const json& master) {
    std::optional<long long> gid = opt_int(master, "sense_enhance_item_group_master_id");
    const json* group = gid ? lookup("CharacterSenseEnhanceItemGroupMaster", *gid) : nullptr;
    const json* items = group ? opt_array(*group, "items") : nullptr;
    if (!group || !items || items->empty()) return 1;
    long long best = items->front().at("current_level").get<long long>();
    for (const auto& i : *items) best = std::max(best, i.at("current_level").get<long long>());
    return best + 1;
}

std::optional<item_counts> sense_enhance_cost(const json& master, long long level_from, long long level_to) {
    std::optional<long long> gid = opt_int(master, "sense_enhance_item_group_master_id");
    const json* group = gid ? lookup("CharacterSenseEnhanceItemGroupMaster", *gid) : nullptr;
    if (!group || level_to <= level_from) return std::nullopt;
    const json* items = opt_array(*group, "items");
    item_counts cost;
    for (long long level = level_from; level < level_to; ++level) {
        std::vector<const json*> step;
        if (items)
            for (const auto& i : *items)
                if (i.at("current_level").get<long long>() == level) step.push_back(&i);
        if (step.empty()) return std::nullopt;
        for (const json* item : step)
            _add(cost, item->at("item_master_id").get<long long>(),
                 item->at("required_quantity").get<long long>());
    }
    return cost;
}

// --- talent bloom ------------------------------------------------------------------------

long long max_talent_stage(const json& master, std::optional<long long> now) {
    std::optional<std::string> release = opt_str(master, "max_talent_stage_release_date");
    long long stage = master.at("max_talent_stage").get<long long>();
    long long at = now.has_value() ? *now : static_cast<long long>(std::time(nullptr));
    if (release && !release->empty() && !character_level::started(*release, at))
        return std::max(0LL, stage - 1);
    return stage;
}

const json* _bloom_step(long long rarity, long long stage) {
    for (const json& row : master_data::table("CharacterBloomItemMaster")) {
        if (row.at("rarity").get<long long>() == rarity && row.at("current_stage").get<long long>() == stage)
            return &row;
    }
    return nullptr;
}

const json* _piece(long long character_master_id, long long bloom_item_type) {
    for (const json& row : master_data::table("CharacterPieceMaster")) {
        if (row.at("character_master_id").get<long long>() == character_master_id &&
            row.at("talent_bloom_item_type").get<long long>() == bloom_item_type)
            return &row;
    }
    return nullptr;
}

std::pair<std::optional<long long>, long long> _generic_piece(const json& master, const json& step,
                                                              const json& piece) {
    long long item_type = step.at("talent_bloom_item_type").get<long long>();
    if (item_type == enums::TalentBloomItemTypes::ActorDaiPiece) {
        // dai pieces are covered by the character base's 万能ダイピース, one for one
        long long base_id = master.at("character_base_master_id").get<long long>();
        for (const json& row : master_data::table("CharacterBaseBloomGenericItemMaster")) {
            if (row.at("character_base_master_id").get<long long>() == base_id &&
                row.at("talent_bloom_item_type").get<long long>() == item_type)
                return {row.at("item_master_id").get<long long>(), 1LL};
        }
        return {std::nullopt, 0LL};
    }
    return {opt_int(step, "generic_bloom_item_master_id"),
            piece.at("dugong_required_amount").get<long long>()};
}

std::optional<item_counts> bloom_cost(const json& master, long long stage_from, long long stage_to,
                                      const item_counts& stock) {
    item_counts remaining = stock;
    item_counts cost;
    long long rarity = master.at("rarity").get<long long>();
    long long id_ = master.at("id_").get<long long>();
    bool forbid_generic = master.value("forbid_generic_item_bloom", false);
    for (long long stage = stage_from; stage < stage_to; ++stage) {
        const json* step = _bloom_step(rarity, stage);
        const json* piece =
            step ? _piece(id_, step->at("talent_bloom_item_type").get<long long>()) : nullptr;
        if (step == nullptr || piece == nullptr) return std::nullopt;

        long long piece_item = piece->at("item_master_id").get<long long>();
        long long required_piece = step->at("required_piece_amount").get<long long>();
        long long own = std::min(required_piece, count_of(remaining, piece_item));
        if (own) {
            remaining[piece_item] -= own;
            _add(cost, piece_item, own);
        }
        long long shortfall = required_piece - own;
        if (shortfall) {
            auto gp = _generic_piece(master, *step, *piece);
            std::optional<long long> generic = gp.first;
            long long rate = gp.second;
            if (!generic || rate <= 0 || forbid_generic) return std::nullopt;
            if (count_of(remaining, *generic) < shortfall * rate) return std::nullopt;
            remaining[*generic] -= shortfall * rate;
            _add(cost, *generic, shortfall * rate);
        }

        std::optional<long long> required_item = opt_int(*step, "required_item_master_id");
        std::optional<long long> required_amount = opt_int(*step, "required_item_amount");
        if (required_item && *required_item && required_amount && *required_amount) {
            if (count_of(remaining, *required_item) < *required_amount) return std::nullopt;
            remaining[*required_item] -= *required_amount;
            _add(cost, *required_item, *required_amount);
        }
    }
    if (cost.empty()) return std::nullopt;
    return cost;
}

std::vector<std::tuple<long long, long long, long long>> bloom_rewards(const json& master, long long stage_from,
                                                                       long long stage_to) {
    std::optional<long long> gid = opt_int(master, "bloom_bonus_group_master_id");
    const json* group = gid ? lookup("CharacterBloomBonusGroupMaster", *gid) : nullptr;
    std::vector<std::tuple<long long, long long, long long>> out;
    if (!group) return out;
    if (const json* rewards = opt_array(*group, "bloom_rewards")) {
        for (const auto& r : *rewards) {
            long long phase = r.at("phase").get<long long>();
            if (stage_from < phase && phase <= stage_to)
                out.emplace_back(r.at("thing_type").get<long long>(), r.at("thing_id").get<long long>(),
                                 r.at("thing_quantity").get<long long>());
        }
    }
    return out;
}

}  // namespace character_enhance
