#pragma once
#include <cstdint>
#include <optional>

// Daily-usage reset at 05:00 JST. Ports helpers/daily.py. DailyLimit tracks how many
// times each capped action was used today (autoplay, lessons, free course challenges);
// when a data-fetch happens after the day's reset boundary those usage counters go back
// to zero and lastRefreshedAt advances.

namespace daily {

// epoch-micros of the most recent 05:00 JST boundary at or before now
int64_t most_recent_reset(int64_t now_micros);

// zero the caller's daily usage counters if the 05:00 JST reset has passed since the last
// refresh. no-op without a user / DailyLimit row, or if already refreshed.
void refresh_daily_limits(std::optional<int64_t> user_id);

}  // namespace daily
