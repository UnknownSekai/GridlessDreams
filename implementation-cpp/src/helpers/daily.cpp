#include "daily.h"

#include <ctime>

#include "db.h"
#include "db/user.h"

namespace daily {

namespace {

constexpr int64_t _MICRO = 1'000'000;
constexpr int64_t _DAY = 86400 * _MICRO;
constexpr int64_t _JST = 9 * 3600 * _MICRO;   // JST = UTC+9, no DST
constexpr int64_t _FIVE_AM = 5 * 3600 * _MICRO;

// usage counters reset to zero at 05:00 JST
constexpr const char* _USAGE_FIELDS[] = {
    "autoPlayTimes", "dailyLessonTimes", "musicCourseFreeChallengeTimes"};

}  // namespace

int64_t most_recent_reset(int64_t now_micros) {
    int64_t wall = now_micros + _JST;              // wall-clock micros in JST
    int64_t boundary = (wall / _DAY) * _DAY + _FIVE_AM;  // 05:00 JST that day
    if (wall < boundary) boundary -= _DAY;         // before 05:00 -> previous day
    return boundary - _JST;                        // back to epoch micros
}

void refresh_daily_limits(std::optional<int64_t> user_id) {
    if (!user_id) return;
    int64_t now = static_cast<int64_t>(std::time(nullptr)) * _MICRO;
    std::optional<db::json> row = db::fetchrow(db::user::get_daily_limits(*user_id));
    int64_t last_refreshed = 0;
    if (row) {
        auto it = row->find("lastRefreshedAt");
        if (it != row->end() && !it->is_null()) last_refreshed = it->get<int64_t>();
    }
    if (!row || last_refreshed >= most_recent_reset(now)) return;
    wire::json data = wire::json::parse(row->dump());  // row.model_dump()
    for (const char* field : _USAGE_FIELDS) data[field] = 0;
    data["lastRefreshedAt"] = now;
    db::execute(db::user::update_daily_limit(*user_id, data));
}

}  // namespace daily
