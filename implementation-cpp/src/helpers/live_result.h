#pragma once
#include <utility>

#include "wire.h"

// live-finish result computation for Lives/FinishAndValidate. ports
// helpers/live_result.py. everything is derived from the hash-verified score
// blocks: the client sends judges=null and score/max_combo/is_cleared as
// 0/0/false, so every headline figure is the server's to recompute.
//
// data flows as wire::json: a score block is an object keyed by the entity
// field names (score, life, timing_type, combo, ...); a payload is a
// FinishLivePayload object whose *_score_blocks are null or arrays of blocks.

namespace live_result {

// whether the blocks' own running combo ever strictly decreases
bool _combo_broken(const wire::json& blocks);

// fail (not cleared) / AllPerfect / FullCombo / Clear, as a ClearLamps int
int clear_lamp(bool is_cleared, const wire::json& base_score_blocks);

// notes judged GOOD or below (GOOD / BAD / MISS / unjudged)
int good_or_worse_count(const wire::json& base_score_blocks);

// notes judged below PERFECT* (everything except the top judgement)
int non_perfect_star_count(const wire::json& base_score_blocks);

// weighted achievement rate: sum(weight[timing]) / note count
double achievement_rate(const wire::json& base_score_blocks);

// (score, is_cleared): score = sum over all four block lists; cleared = last
// base block life > 0
std::pair<long long, bool> play_totals(const wire::json& payload);

// AchievementRateGrades int for a rate (None_ above the 101 cap)
int rate_grade(double rate);

}  // namespace live_result
