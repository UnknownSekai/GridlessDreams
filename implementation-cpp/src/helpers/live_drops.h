#pragma once
#include <vector>

#include "wire.h"

// Live-drop resolution and granting. Ports helpers/live_drops.py.
// A live's rewards chain LiveSettingMaster -> LiveDropFrameGroupMaster.drop_frames
// -> LiveDropFrameMaster.rewards; each frame is gated by a FrameLotCondition and an
// availability window. Frames flow as master-data ordered_json keyed by field name
// (order/start_date/end_date/frame_lot_condition/frame_lot_condition_value/rewards);
// the granted result is a list of LiveDropThing entities keyed by field name.

namespace live_drops {

using wire::json;

// the drop frames a live awards -- every frame in its group that is currently
// available and whose lot condition this play met, sorted by order.
std::vector<json> resolve_frames(long long live_setting_master_id,
                                 bool stamina_consumed = true,
                                 long long score = 0,
                                 long long star_act_count = 0,
                                 double achievement_rate = 0.0);

// grant the rewards of one or more drop frames (consolidated into as few writes
// as possible) and return the per-reward LiveDropThing list for the result.
std::vector<json> grant_frames(long long user_id, const std::vector<json>& frames);

}  // namespace live_drops
