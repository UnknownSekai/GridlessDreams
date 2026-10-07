#pragma once
#include <optional>
#include <utility>
#include <vector>

// Per-chart live rating (level + piecewise-linear achievement-rate adjustment) plus the
// Olivier star-badge (SpRate) table. Ports helpers/live_rate.py. Charts with no live rate
// (Olivier / long-version song / unknown) rate to none; the player's total rate is the sum
// of the best live rate of their top 30 charts. EXACT NUMERIC PARITY: live_rate truncates to
// 2dp via floor (not round); adjustment is linearly interpolated between the 9 breakpoints.

namespace live_rate {

// a chart's live rate: level truncated-to-2dp with the achievement-rate adjustment
double live_rate(int level, double rate);

// a chart's live rate, or none if it has none (Olivier / long-version / unknown chart)
std::optional<double> chart_live_rate(long long live_master_id, double rate);

// sum of the top 30 chart live rates; pairs = (live_master_id, achievement_rate)
double total_rate(const std::vector<std::pair<long long, double>>& pairs);

// (best_ever, this_time) live rate for the finished chart; (0.0, 0.0) if it has no live rate.
// best_ever is the PAST best only (excludes this play) -- 0 when never played before.
std::pair<double, double> chart_live_rate_result(long long live_master_id, double this_rate,
                                                 double prev_rate);

// star-badge points for an Olivier (difficulty 5) clear, or none if the chart isn't an
// Olivier chart. loss_count is the number of notes judged below PERFECT*.
std::optional<int> olivier_sp_rate_points(long long live_master_id, double rate, int clear_lamp,
                                          int loss_count);

}  // namespace live_rate
