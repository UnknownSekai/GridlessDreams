#pragma once
#include <cstdint>
#include <optional>

// Stamina: currentStamina + maxStaminaRestoredAt (the "full at" time). Ports
// helpers/stamina.py. Effective stamina is recomputed from that timestamp, so the value
// passively recovers 1 per stamina_recovery_seconds up to PlayerRankMaster.max_stamina for
// the player's rank. All mutation goes through db::user::adjust_user_stamina_atomic so
// concurrent requests can't corrupt the count.

namespace stamina {

// stamina_recovery_seconds as epoch-micros
int64_t recovery_micros();

// max stamina for a player rank, from PlayerRankMaster (0 if the rank is unknown)
int64_t max_stamina(int64_t player_rank);

// stamina right now, accounting for passive recovery; overfilled stamina (above max,
// from items) never regenerates and is returned as-is
int64_t effective_stamina(int64_t current, int64_t restored_at, int64_t max_st, int64_t now);

// the epoch-micros maxStaminaRestoredAt for a given current stamina (now if already at/above max)
int64_t restored_at(int64_t current, int64_t max_st, int64_t now);

// recover-then-apply delta stamina atomically. true on success, false if it would go below 0
// (or exceed max in strict mode). delta<0 consumes, >0 recovers.
bool adjust_and_check_stamina(int64_t user_id, int64_t delta, int64_t player_rank,
                              std::optional<int64_t> now = std::nullopt,
                              bool auto_max_clamp = false);

}  // namespace stamina
