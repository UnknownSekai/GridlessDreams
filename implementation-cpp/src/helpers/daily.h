#pragma once
#include <cstdint>
#include <optional>

// daily-usage reset at 05:00 JST. ports helpers/daily.py. DailyLimit tracks how many
// times each capped action was used today (autoplay, lessons, free course challenges);
// when a data-fetch happens after the day's reset boundary those usage counters go back
// to zero and lastRefreshedAt advances.

namespace daily {

// epoch-micros of the most recent 05:00 JST boundary at or before now
long long most_recent_reset(long long now_micros);

// zero the caller's daily usage counters if the 05:00 JST reset has passed since the last
// refresh. no-op without a user / DailyLimit row, or if already refreshed.
void refresh_daily_limits(std::optional<long long> user_id);

}  // namespace daily
