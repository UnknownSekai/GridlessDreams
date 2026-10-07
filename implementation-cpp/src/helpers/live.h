#pragma once
#include <optional>
#include <utility>

#include "wire.h"

// Build a LiveUnit / LiveTimeEvent from a party + masterdata. Ports helpers/live.py.
// Actor acting power is a 3-stage integer-truncation pipeline (flat + component% then
// performance% then multiplication); senses/star-act lights are simulated over timing
// windows (SenseNotationMaster.details, or 8 even windows in formation 1-2-3-4-5-3-2-1).
// Results flow as wire::json keyed by the entity field names (LiveUnit / LiveTimeEvent).

namespace live {

// Sense timing schedule for a live. sense_notation_master_id=nullopt -> normal live
// (windows spread over the song duration); otherwise from SenseNotationMaster.details.
wire::json build_live_time_event(long long user_id, long long party_id,
                                 long long music_master_id,
                                 std::optional<long long> sense_notation_master_id = std::nullopt);

// (LiveUnit, active_live_id). See helpers/live.py for the placeholder parts.
std::pair<wire::json, long long> build_live_unit(long long user_id, long long party_id,
                                                 long long live_master_id);

}  // namespace live
