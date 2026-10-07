#include "helpers/score.h"

#include <array>
#include <cstddef>

namespace score {

namespace {

const wire::json kEmpty = wire::json::array();

// python `x or []`: null / missing / non-array -> empty array
const wire::json& as_array(const wire::json& v) { return v.is_array() ? v : kEmpty; }

const wire::json& member(const wire::json& payload, const char* key) {
    if (payload.is_object()) {
        auto it = payload.find(key);
        if (it != payload.end()) return as_array(*it);
    }
    return kEmpty;
}

// value fields summed into each block type's hash (it's a sum, so order is irrelevant)
constexpr std::array<const char*, 5> _BASE = {"score", "life", "note_id", "timing_type", "combo"};
constexpr std::array<const char*, 5> _SENSE = {"score", "life", "time_event_second", "sense_id", "combo"};
constexpr std::array<const char*, 4> _STAR_ACT = {"score", "life", "time_event_second", "combo"};
constexpr std::array<const char*, 4> _MULTI = {"score", "life", "time_event_second", "combo"};

template <std::size_t N>
bool _chain_ok(const wire::json& blocks, const std::array<const char*, N>& fields) {
    long long running = 0;
    for (const wire::json& block : as_array(blocks)) {
        for (const char* f : fields) running += block.value(f, 0LL);
        if (block.value("hash", 0LL) != running) return false;
    }
    return true;
}

}  // namespace

bool verify_score_blocks(const wire::json& payload) {
    return _chain_ok(member(payload, "base_score_blocks"), _BASE) &&
           _chain_ok(member(payload, "sense_score_blocks"), _SENSE) &&
           _chain_ok(member(payload, "star_act_score_blocks"), _STAR_ACT) &&
           _chain_ok(member(payload, "multi_live_additional_score_blocks"), _MULTI);
}

}  // namespace score
