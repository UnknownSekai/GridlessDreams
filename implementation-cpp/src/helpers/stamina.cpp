#include "helpers/stamina.h"

#include <algorithm>
#include <chrono>
#include <cmath>

#include "config.h"
#include "db.h"
#include "db/user.h"
#include "master_data.h"

namespace stamina {

constexpr long long _MICRO = 1'000'000;

long long recovery_micros() {
    return static_cast<long long>(config::get_int("stamina_recovery_seconds")) * _MICRO;
}

long long max_stamina(long long player_rank) {
    for (const auto& row : master_data::table("PlayerRankMaster")) {
        if (row.at("rank").get<long long>() == player_rank)
            return row.at("max_stamina").get<long long>();
    }
    return 0;
}

long long effective_stamina(long long current, long long restored_at, long long max_st, long long now) {
    if (current >= max_st)
        return current;
    long long interval = recovery_micros();
    if (interval <= 0 || now >= restored_at)
        return max_st;
    long long missing = static_cast<long long>(
        std::ceil(static_cast<double>(restored_at - now) / static_cast<double>(interval)));
    return std::max<long long>(0, max_st - missing);
}

long long restored_at(long long current, long long max_st, long long now) {
    if (current >= max_st)
        return now;
    return now + (max_st - current) * recovery_micros();
}

bool adjust_and_check_stamina(long long user_id, long long delta, long long player_rank,
                              std::optional<long long> now, bool auto_max_clamp) {
    long long now_val = now.has_value()
                          ? *now
                          : static_cast<long long>(
                                std::chrono::duration_cast<std::chrono::microseconds>(
                                    std::chrono::system_clock::now().time_since_epoch())
                                    .count());
    std::optional<db::json> row = db::fetchrow(db::user::adjust_user_stamina_atomic(
        user_id, delta, max_stamina(player_rank), recovery_micros(), now_val, auto_max_clamp));
    return row.has_value();
}

}  // namespace stamina
