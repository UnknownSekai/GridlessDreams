#include "helpers/stamina.h"

#include <algorithm>
#include <chrono>
#include <cmath>

#include "config.h"
#include "db.h"
#include "db/user.h"
#include "master_data.h"

namespace stamina {

constexpr int64_t _MICRO = 1'000'000;

int64_t recovery_micros() {
    return static_cast<int64_t>(config::get_int("stamina_recovery_seconds")) * _MICRO;
}

int64_t max_stamina(int64_t player_rank) {
    for (const auto& row : master_data::table("PlayerRankMaster")) {
        if (row.at("rank").get<int64_t>() == player_rank)
            return row.at("max_stamina").get<int64_t>();
    }
    return 0;
}

int64_t effective_stamina(int64_t current, int64_t restored_at, int64_t max_st, int64_t now) {
    if (current >= max_st)
        return current;
    int64_t interval = recovery_micros();
    if (interval <= 0 || now >= restored_at)
        return max_st;
    int64_t missing = static_cast<int64_t>(
        std::ceil(static_cast<double>(restored_at - now) / static_cast<double>(interval)));
    return std::max<int64_t>(0, max_st - missing);
}

int64_t restored_at(int64_t current, int64_t max_st, int64_t now) {
    if (current >= max_st)
        return now;
    return now + (max_st - current) * recovery_micros();
}

bool adjust_and_check_stamina(int64_t user_id, int64_t delta, int64_t player_rank,
                              std::optional<int64_t> now, bool auto_max_clamp) {
    int64_t now_val = now.has_value()
                          ? *now
                          : static_cast<int64_t>(
                                std::chrono::duration_cast<std::chrono::microseconds>(
                                    std::chrono::system_clock::now().time_since_epoch())
                                    .count());
    std::optional<db::json> row = db::fetchrow(db::user::adjust_user_stamina_atomic(
        user_id, delta, max_stamina(player_rank), recovery_micros(), now_val, auto_max_clamp));
    return row.has_value();
}

}  // namespace stamina
