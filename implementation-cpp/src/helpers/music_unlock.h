#pragma once
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "wire.h"
#include "generated/enums_generated.h"

// Stella / Olivier chart unlocks, derived from the user's live clears + live records.
// Ports helpers/music_unlock.py. Release state is recomputed from the user's live rows
// against the current live_master data, so a rerate is handled on the next recompute.
// The async conn/app parameters are dropped: reads/writes go through the global db engine.
// Rows are db rows (camelCase nlohmann::json); apply_unlocks returns the music rows that
// moved for the caller to push into its present.

namespace music_unlock {

// Budget for a Stella chart that asks for ExtraGoodCount without naming one; every such
// row in masterdata carries 10.
constexpr long long DEFAULT_EXTRA_GOOD_BUDGET = 10;

// Olivier charts released before every owned song's Stella opens.
constexpr long long OLIVIER_STELLA_THRESHOLD = 20;

// Whether finishing this chart could change any Stella/Olivier release state.
bool affects_unlocks(long long live_master_id);

// GOOD-or-worse judgements an Extra clear may carry and still release this song's Stella,
// or none when the Stella chart isn't gated on that at all.
std::optional<long long> extra_good_budget(long long music_master_id);

// The music_master_id whose Stella a just-finished play releases, or none. Answered from
// the finish itself since the judgement counts are gone by re-derivation time.
std::optional<long long> stella_unlocked_by(long long live_master_id, bool is_cleared,
                                            long long good_or_worse);

// Aggregates of a user's live records, used to resolve per-song unlock state. stella_unlock
// is the one song (if any) whose Stella the finish being processed just earned.
class MusicProgress {
public:
    std::optional<long long> stella_unlock;
    bool all_stella;
    long long max_cleared_olivier_level;
    std::set<long long> stella_s_ranked;

    MusicProgress(const std::vector<nlohmann::json>& lives,
                  const std::vector<nlohmann::json>& musics,
                  std::optional<long long> stella_unlock = std::nullopt);

    bool stella_released(long long music_master_id, bool is_possession, bool stored) const;
    long long olivier_status(long long music_master_id, long long stored,
                             bool stella_released) const;
};

// MusicProgress over the user's current lives + musics.
MusicProgress load_progress(long long user_id,
                            std::optional<long long> stella_unlock = std::nullopt);

// Re-derive every owned song's release state, write what moved, return those rows.
std::vector<nlohmann::json> apply_unlocks(
    long long user_id, std::optional<long long> stella_unlock = std::nullopt);

// (stellaReleased, olivierReleaseStatus) a freshly granted, possessed song starts at.
std::pair<bool, long long> granted_music_state(long long user_id, long long music_master_id);

// The dummy song and hidden entries are excluded; everything else of this type is owned
// from the start.
constexpr long long DEFAULT_MUSIC_UNLOCK_TYPE = enums::MusicUnlockConditionTypes::Default;
constexpr long long DUMMY_MUSIC_ID = 9999;

// Songs the client treats as owned from the start (MusicUnlockConditionTypes.Default).
const std::set<long long>& default_music_master_ids();

// Materialize a Music row for every default song the user is missing one for.
void ensure_default_music(std::optional<long long> user_id);

}  // namespace music_unlock
