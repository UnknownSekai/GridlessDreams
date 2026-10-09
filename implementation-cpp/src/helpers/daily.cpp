#include "daily.h"

#include <ctime>

#include "db.h"
#include "db/user.h"

namespace daily {

namespace {

constexpr long long _MICRO = 1'000'000;
constexpr long long _DAY = 86400 * _MICRO;
constexpr long long _JST = 9 * 3600 * _MICRO;   // JST = UTC+9, no DST
constexpr long long _FIVE_AM = 5 * 3600 * _MICRO;

// usage counters reset to zero at 05:00 JST
constexpr const char* _USAGE_FIELDS[] = {
    "autoPlayTimes", "dailyLessonTimes", "musicCourseFreeChallengeTimes"};

}  // namespace

long long most_recent_reset(long long now_micros) {
    long long wall = now_micros + _JST;              // wall-clock micros in JST
    long long boundary = (wall / _DAY) * _DAY + _FIVE_AM;  // 05:00 JST that day
    if (wall < boundary) boundary -= _DAY;         // before 05:00 -> previous day
    return boundary - _JST;                        // back to epoch micros
}

void refresh_daily_limits(std::optional<long long> user_id) {
    if (!user_id) return;
    long long now = static_cast<long long>(std::time(nullptr)) * _MICRO;
    std::optional<db::json> row = db::fetchrow(db::user::get_daily_limits(*user_id));
    long long last_refreshed = 0;
    if (row) {
        auto it = row->find("lastRefreshedAt");
        if (it != row->end() && !it->is_null()) last_refreshed = it->get<long long>();
    }
    if (!row || last_refreshed >= most_recent_reset(now)) return;
    wire::json data = wire::json::parse(row->dump());  // row.model_dump()
    for (const char* field : _USAGE_FIELDS) data[field] = 0;
    data["lastRefreshedAt"] = now;
    db::execute(db::user::update_daily_limit(*user_id, data));
}

}  // namespace daily
