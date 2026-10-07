#include "helpers/live_result.h"

#include <array>
#include <unordered_map>

#include "generated/enums_generated.h"

// ports helpers/live_result.py. everything is derived from the hash-verified
// score blocks; see that file for the reasoning behind each rule.

namespace live_result {

namespace {

const std::unordered_map<long long, double> kRateWeight = {
    {enums::TimingTypes::GOOD, 50.0},
    {enums::TimingTypes::GREAT, 80.0},
    {enums::TimingTypes::PERFECT, 100.0},
    {enums::TimingTypes::PERFECT_STAR, 101.0},
};

// rate is capped at 101 in the binary; anything above returns None_.
constexpr double kRateCap = 101.0;

// GetAchievementRateGrade thresholds (%), highest first.
const std::array<std::pair<double, long long>, 9> kGradeThresholds = {{
    {100.95, enums::AchievementRateGrades::SSS},
    {100.75, enums::AchievementRateGrades::SSPlus},
    {100.5, enums::AchievementRateGrades::SS},
    {100.25, enums::AchievementRateGrades::SPlus},
    {100.0, enums::AchievementRateGrades::S},
    {98.0, enums::AchievementRateGrades::APlus},
    {95.0, enums::AchievementRateGrades::A},
    {90.0, enums::AchievementRateGrades::B},
    {80.0, enums::AchievementRateGrades::C},
}};

const wire::json kEmpty = wire::json::array();

// python `x or []`: null / missing / non-array -> empty array
const wire::json& as_array(const wire::json& v) { return v.is_array() ? v : kEmpty; }

// payload list member as an array, by reference (no temporary to dangle over)
const wire::json& member(const wire::json& payload, const char* key) {
    if (payload.is_object()) {
        auto it = payload.find(key);
        if (it != payload.end()) return as_array(*it);
    }
    return kEmpty;
}

}  // namespace

bool _combo_broken(const wire::json& blocks) {
    long long previous = 0;
    for (const wire::json& block : as_array(blocks)) {
        long long combo = block.value("combo", 0LL);
        if (combo < previous) return true;
        previous = combo;
    }
    return false;
}

int clear_lamp(bool is_cleared, const wire::json& base_score_blocks) {
    if (!is_cleared) return static_cast<int>(enums::ClearLamps::None_);
    const wire::json& blocks = as_array(base_score_blocks);
    long long broken = 0;
    long long imperfect = 0;
    for (const wire::json& b : blocks) {
        long long t = b.value("timing_type", 0LL);
        if (t < enums::TimingTypes::GOOD) broken += 1;
        if (t < enums::TimingTypes::PERFECT) imperfect += 1;
    }
    if (broken || _combo_broken(blocks)) return static_cast<int>(enums::ClearLamps::Clear);
    if (imperfect) return static_cast<int>(enums::ClearLamps::FullCombo);
    return static_cast<int>(enums::ClearLamps::AllPerfect);
}

int good_or_worse_count(const wire::json& base_score_blocks) {
    int count = 0;
    for (const wire::json& b : as_array(base_score_blocks))
        if (b.value("timing_type", 0LL) <= enums::TimingTypes::GOOD) count += 1;
    return count;
}

int non_perfect_star_count(const wire::json& base_score_blocks) {
    int count = 0;
    for (const wire::json& b : as_array(base_score_blocks))
        if (b.value("timing_type", 0LL) != enums::TimingTypes::PERFECT_STAR) count += 1;
    return count;
}

double achievement_rate(const wire::json& base_score_blocks) {
    const wire::json& blocks = as_array(base_score_blocks);
    size_t total = blocks.size();
    if (total == 0) return 0.0;
    double weighted = 0.0;
    for (const wire::json& b : blocks) {
        auto it = kRateWeight.find(b.value("timing_type", 0LL));
        if (it != kRateWeight.end()) weighted += it->second;
    }
    return weighted / static_cast<double>(total);
}

std::pair<long long, bool> play_totals(const wire::json& payload) {
    static const char* kLists[] = {"base_score_blocks", "sense_score_blocks",
                                    "star_act_score_blocks",
                                    "multi_live_additional_score_blocks"};
    long long score = 0;
    for (const char* key : kLists)
        for (const wire::json& b : member(payload, key)) score += b.value("score", 0LL);
    const wire::json& base = member(payload, "base_score_blocks");
    bool is_cleared = !base.empty() && base.back().value("life", 0LL) > 0;
    return {score, is_cleared};
}

int rate_grade(double rate) {
    if (rate > kRateCap) return static_cast<int>(enums::AchievementRateGrades::None_);
    for (const auto& entry : kGradeThresholds)
        if (rate >= entry.first) return static_cast<int>(entry.second);
    return static_cast<int>(enums::AchievementRateGrades::None_);
}

}  // namespace live_result
