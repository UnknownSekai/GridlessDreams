#include "helpers/live_rate.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <unordered_map>

#include "generated/enums_generated.h"
#include "master_data.h"

// ports helpers/live_rate.py; see that file for the reasoning behind each curve rule.

namespace live_rate {

namespace {

using json = nlohmann::ordered_json;

constexpr int kTopN = 30;

// achievement-rate -> adjustment breakpoints, highest first; linearly interpolated between.
struct Adjustment {
    double rate;
    double adj;
};
const std::array<Adjustment, 9> kAdjustments = {{
    {101.00, 6.05},
    {100.95, 6.00},
    {100.75, 4.50},
    {100.50, 3.00},
    {100.25, 2.25},
    {100.00, 1.50},
    {99.00, 0.75},
    {98.00, 0.00},
    {97.50, -1.00},
}};

// {id_: row} over LiveMaster, built once; last duplicate id wins (mirrors the dict comp).
const std::unordered_map<long long, const json*>& by_id() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& row : master_data::table("LiveMaster"))
            r[row.at("id_").get<long long>()] = &row;
        return r;
    }();
    return m;
}

// {id_: row} over MusicMaster, built once.
const std::unordered_map<long long, const json*>& music_by_id() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& row : master_data::table("MusicMaster"))
            r[row.at("id_").get<long long>()] = &row;
        return r;
    }();
    return m;
}

// the chart if it participates in live rate -- Stella-or-lower difficulty and not a
// long-version song -- else nullptr.
const json* rated_chart(long long live_master_id) {
    const auto& ids = by_id();
    auto it = ids.find(live_master_id);
    if (it == ids.end() ||
        it->second->at("difficulty").get<long long>() > enums::MusicDifficulties::Stella)
        return nullptr;
    const auto& musics = music_by_id();
    auto mit = musics.find(it->second->at("music_master_id").get<long long>());
    if (mit != musics.end() && mit->second->at("is_long_version").get<bool>()) return nullptr;
    return it->second;
}

double adjustment(double rate) {
    // piecewise-LINEAR interpolation between the breakpoints (not a step function)
    if (rate >= kAdjustments[0].rate) return kAdjustments[0].adj;
    if (rate <= kAdjustments.back().rate) return kAdjustments.back().adj;
    for (std::size_t i = 0; i + 1 < kAdjustments.size(); ++i) {
        double hi_r = kAdjustments[i].rate, hi_a = kAdjustments[i].adj;
        double lo_r = kAdjustments[i + 1].rate, lo_a = kAdjustments[i + 1].adj;
        if (lo_r <= rate && rate <= hi_r)
            return lo_a + (rate - lo_r) / (hi_r - lo_r) * (hi_a - lo_a);
    }
    return kAdjustments.back().adj;
}

// --- Olivier star badge (SpRate) ---------------------------------------------------------

// base points, highest achievement-rate threshold first; columns are star1..star10
struct SpBase {
    double threshold;
    std::array<int, 10> points;
};
const std::array<SpBase, 25> kSpRateBase = {{
    {100.95, {60, 70, 80, 90, 100, 110, 120, 130, 140, 150}},
    {100.90, {59, 69, 79, 89, 99, 109, 118, 128, 138, 148}},
    {100.85, {58, 68, 78, 88, 98, 108, 116, 126, 136, 146}},
    {100.80, {57, 67, 77, 87, 97, 106, 114, 124, 134, 144}},
    {100.75, {56, 66, 76, 86, 96, 104, 112, 122, 132, 142}},
    {100.70, {55, 65, 75, 85, 94, 102, 110, 120, 130, 140}},
    {100.60, {54, 64, 74, 84, 92, 100, 108, 118, 128, 138}},
    {100.50, {53, 63, 73, 82, 90, 98, 106, 116, 126, 135}},
    {100.40, {52, 62, 72, 80, 88, 96, 104, 114, 124, 132}},
    {100.30, {51, 61, 70, 78, 86, 94, 102, 112, 121, 129}},
    {100.20, {50, 60, 68, 76, 84, 92, 100, 110, 118, 126}},
    {100.10, {49, 58, 66, 74, 82, 90, 98, 107, 115, 123}},
    {100.00, {48, 56, 64, 72, 80, 88, 96, 104, 112, 120}},
    {99.00, {43, 50, 57, 64, 71, 78, 85, 92, 99, 106}},
    {98.00, {41, 48, 55, 62, 68, 75, 82, 89, 96, 102}},
    {97.00, {36, 42, 48, 54, 59, 65, 71, 77, 83, 88}},
    {96.00, {34, 40, 46, 52, 56, 62, 68, 74, 80, 84}},
    {95.00, {32, 38, 44, 50, 53, 59, 65, 71, 77, 81}},
    {94.00, {27, 32, 37, 42, 44, 49, 54, 59, 64, 67}},
    {93.00, {25, 30, 35, 40, 42, 46, 51, 56, 61, 64}},
    {92.00, {23, 28, 33, 38, 40, 43, 48, 53, 58, 61}},
    {91.00, {21, 26, 31, 36, 38, 40, 45, 50, 55, 58}},
    {90.00, {19, 24, 29, 34, 36, 38, 42, 47, 52, 55}},
    {89.00, {14, 18, 22, 26, 27, 28, 31, 35, 39, 41}},
    {80.00, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}},
}};

// loss-count (notes judged below PERFECT*) thresholds, +1 for each the play is at or under
const std::array<int, 5> kSpRateLossThresholds = {10, 30, 50, 75, 100};

int sp_rate_base(int stella_lv, double rate) {
    for (const SpBase& b : kSpRateBase) {
        if (rate >= b.threshold) return b.points[stella_lv - 1];
    }
    return 0;  // below the lowest bracket earns no base points
}

int sp_rate_bonus(int clear_lamp, int loss_count) {
    int bonus = 0;
    if (clear_lamp >= enums::ClearLamps::AllPerfect)
        bonus += 3;
    else if (clear_lamp >= enums::ClearLamps::FullCombo)
        bonus += 2;
    for (int t : kSpRateLossThresholds)
        if (loss_count <= t) bonus += 1;
    return bonus;
}

}  // namespace

double live_rate(int level, double rate) {
    // TRUNCATED to 2dp, not rounded
    return std::floor((level + adjustment(rate)) * 100.0) / 100.0;
}

std::optional<double> chart_live_rate(long long live_master_id, double rate) {
    const json* lm = rated_chart(live_master_id);
    if (lm == nullptr) return std::nullopt;
    return live_rate(static_cast<int>(lm->at("level").get<long long>()), rate);
}

double total_rate(const std::vector<std::pair<long long, double>>& pairs) {
    std::vector<double> rates;
    for (const auto& [mid, rate] : pairs) {
        std::optional<double> r = chart_live_rate(mid, rate);
        if (r) rates.push_back(*r);
    }
    std::sort(rates.begin(), rates.end(), std::greater<double>());
    double sum = 0.0;
    std::size_t n = std::min<std::size_t>(rates.size(), kTopN);
    for (std::size_t i = 0; i < n; ++i) sum += rates[i];
    // python round(x, 2): nearest 0.01, ties to even
    return std::nearbyint(sum * 100.0) / 100.0;
}

std::pair<double, double> chart_live_rate_result(long long live_master_id, double this_rate,
                                                 double prev_rate) {
    const json* lm = rated_chart(live_master_id);
    if (lm == nullptr) return {0.0, 0.0};
    int level = static_cast<int>(lm->at("level").get<long long>());
    double best_ever = prev_rate > 0 ? live_rate(level, prev_rate) : 0.0;
    return {best_ever, live_rate(level, this_rate)};
}

std::optional<int> olivier_sp_rate_points(long long live_master_id, double rate, int clear_lamp,
                                          int loss_count) {
    const auto& ids = by_id();
    auto it = ids.find(live_master_id);
    if (it == ids.end() ||
        it->second->at("difficulty").get<long long>() != enums::MusicDifficulties::Olivier)
        return std::nullopt;
    int stella_lv = static_cast<int>(it->second->at("level").get<long long>()) - 100;
    if (!(1 <= stella_lv && stella_lv <= 10)) return std::nullopt;
    return sp_rate_base(stella_lv, rate) + sp_rate_bonus(clear_lamp, loss_count);
}

}  // namespace live_rate
