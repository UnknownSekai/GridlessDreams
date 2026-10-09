#pragma once
#include <cstdint>
#include <optional>

// stamina: currentStamina + maxStaminaRestoredAt (the "full at" time). ports
// helpers/stamina.py. effective stamina is recomputed from that timestamp, so the value
// passively recovers 1 per stamina_recovery_seconds up to PlayerRankMaster.max_stamina for
// the player's rank. all mutation goes through db::user::adjust_user_stamina_atomic so
// concurrent requests can't corrupt the count.

namespace stamina {

// stamina_recovery_seconds as epoch-micros
long long recovery_micros();

// max stamina for a player rank, from PlayerRankMaster (0 if the rank is unknown)
long long max_stamina(long long player_rank);

// stamina right now, accounting for passive recovery; overfilled stamina (above max,
// from items) never regenerates and is returned as-is
long long effective_stamina(long long current, long long restored_at, long long max_st, long long now);

// the epoch-micros maxStaminaRestoredAt for a given current stamina (now if already at/above max)
long long restored_at(long long current, long long max_st, long long now);

// recover-then-apply delta stamina atomically. true on success, false if it would go below 0
// (or exceed max in strict mode). delta<0 consumes, >0 recovers.
bool adjust_and_check_stamina(long long user_id, long long delta, long long player_rank,
                              std::optional<long long> now = std::nullopt,
                              bool auto_max_clamp = false);

}  // namespace stamina
