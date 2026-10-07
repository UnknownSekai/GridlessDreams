#include "helpers/live_drops.h"

#include <algorithm>
#include <ctime>
#include <string>
#include <unordered_map>

#include "generated/enums_generated.h"
#include "helpers/things.h"
#include "master_data.h"

namespace live_drops {

namespace {

long long int_field(const json& o, const char* k, long long dflt) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null() || !it->is_number_integer()) return dflt;
    return it->get<long long>();
}

bool bool_field(const json& o, const char* k, bool dflt) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null() || !it->is_boolean()) return dflt;
    return it->get<bool>();
}

const json* array_field(const json& o, const char* k) {
    auto it = o.find(k);
    if (it == o.end() || !it->is_array()) return nullptr;
    return &(*it);
}

long long now_utc() { return static_cast<long long>(std::time(nullptr)); }

// Howard Hinnant's days-from-civil (days relative to 1970-01-01).
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

// epoch seconds of an iso-8601 string; mirrors datetime.fromisoformat after the
// Z->+00:00 replacement (naive -> utc). false for anything not a valid date(+time).
bool iso_epoch_seconds(const std::string& s, long long& out) {
    size_t i = 0, n = s.size();
    auto read = [&](int len, int& v) -> bool {
        v = 0;
        int k = 0;
        for (; k < len && i < n && s[i] >= '0' && s[i] <= '9'; ++k, ++i) v = v * 10 + (s[i] - '0');
        return k == len;
    };
    int year, mon, day;
    if (!read(4, year)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, mon)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, day)) return false;

    int hh = 0, mm = 0, ss = 0;
    long long tz = 0;
    if (i < n) {
        ++i;  // date/time separator
        if (!read(2, hh)) return false;
        if (i >= n || s[i] != ':') return false;
        ++i;
        if (!read(2, mm)) return false;
        if (i < n && s[i] == ':') {
            ++i;
            if (!read(2, ss)) return false;
        }
        if (i < n && (s[i] == '.' || s[i] == ',')) {
            ++i;
            while (i < n && s[i] >= '0' && s[i] <= '9') ++i;
        }
        if (i < n) {
            char c = s[i];
            if (c == 'Z' || c == 'z') {
                ++i;
            } else if (c == '+' || c == '-') {
                int sign = (c == '-') ? -1 : 1;
                ++i;
                int oh = 0, om = 0, os_ = 0;
                if (!read(2, oh)) return false;
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, om);
                } else {
                    read(2, om);
                }
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, os_);
                }
                tz = sign * (oh * 3600LL + om * 60LL + os_);
            }
        }
    }
    out = days_from_civil(year, static_cast<unsigned>(mon), static_cast<unsigned>(day)) * 86400LL +
          hh * 3600LL + mm * 60LL + ss - tz;
    return true;
}

// three-state read of a frame date field, matching python's inner instant():
// None_ when the value is falsy (null/missing/empty), Value when parseable,
// Error when non-empty but unparseable (fromisoformat raises).
enum class Instant { None_, Value, Error };

Instant instant(const json& frame, const char* key, long long& out) {
    auto it = frame.find(key);
    if (it == frame.end() || it->is_null() || !it->is_string()) return Instant::None_;
    const std::string& v = it->get_ref<const std::string&>();
    if (v.empty()) return Instant::None_;
    return iso_epoch_seconds(v, out) ? Instant::Value : Instant::Error;
}

// {id_: row} over LiveSettingMaster / LiveDropFrameGroupMaster, built once (mirrors _build()).
const std::unordered_map<long long, const json*>& setting_index() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& row : master_data::table("LiveSettingMaster")) {
            auto it = row.find("id_");
            if (it != row.end() && it->is_number_integer()) r[it->get<long long>()] = &row;
        }
        return r;
    }();
    return m;
}

const std::unordered_map<long long, const json*>& frame_group_index() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& row : master_data::table("LiveDropFrameGroupMaster")) {
            auto it = row.find("id_");
            if (it != row.end() && it->is_number_integer()) r[it->get<long long>()] = &row;
        }
        return r;
    }();
    return m;
}

// start-inclusive, end-exclusive availability window.
bool frame_available(const json& frame, long long now) {
    long long start = 0, end = 0;
    Instant rs = instant(frame, "start_date", start);
    Instant re = instant(frame, "end_date", end);
    if (rs == Instant::Error || re == Instant::Error) return false;
    bool start_ok = (rs == Instant::None_) || (start <= now);
    bool end_ok = (re == Instant::None_) || (now < end);
    return start_ok && end_ok;
}

bool frame_condition_met(const json& frame, bool stamina_consumed, long long score,
                         long long star_act_count, double achievement_rate) {
    long long cond = int_field(frame, "frame_lot_condition", enums::FrameLotConditionTypes::None_);
    long long value = int_field(frame, "frame_lot_condition_value", 0);  // null -> 0
    if (cond == enums::FrameLotConditionTypes::None_) return true;
    if (cond == enums::FrameLotConditionTypes::ConsumeStamina) return stamina_consumed;
    if (cond == enums::FrameLotConditionTypes::ScoreWithConsumeStamina)
        return stamina_consumed && score >= value;
    if (cond == enums::FrameLotConditionTypes::StarActCountWithConsumeStamina)
        return stamina_consumed && star_act_count >= value;
    if (cond == enums::FrameLotConditionTypes::AchievementRateWithConsumeStamina)
        return stamina_consumed && achievement_rate >= static_cast<double>(value);
    return false;
}

}  // namespace

std::vector<json> resolve_frames(long long live_setting_master_id, bool stamina_consumed,
                                 long long score, long long star_act_count,
                                 double achievement_rate) {
    const auto& settings = setting_index();
    auto sit = settings.find(live_setting_master_id);
    if (sit == settings.end()) return {};
    const json& setting = *sit->second;

    const auto& groups = frame_group_index();
    auto git = groups.find(int_field(setting, "live_drop_frame_group_master_id", 0));
    if (git == groups.end()) return {};
    const json* frames_p = array_field(*git->second, "drop_frames");
    if (frames_p == nullptr || frames_p->empty()) return {};

    std::vector<json> frames(frames_p->begin(), frames_p->end());
    std::stable_sort(frames.begin(), frames.end(), [](const json& a, const json& b) {
        return int_field(a, "order", 0) < int_field(b, "order", 0);
    });

    long long now = now_utc();
    std::vector<json> out;
    for (const json& f : frames) {
        if (frame_available(f, now) &&
            frame_condition_met(f, stamina_consumed, score, star_act_count, achievement_rate))
            out.push_back(f);
    }
    return out;
}

std::vector<json> grant_frames(long long user_id, const std::vector<json>& frames) {
    std::vector<things::ThingTriple> things;
    std::vector<json> drops;
    long long order = 0;
    for (const json& frame : frames) {
        bool fixed = int_field(frame, "frame_lot_condition",
                               enums::FrameLotConditionTypes::None_) ==
                     enums::FrameLotConditionTypes::None_;
        const json* rewards = array_field(frame, "rewards");
        if (rewards == nullptr) continue;
        for (const json& reward : *rewards) {
            long long thing_type = int_field(reward, "thing_type", enums::ThingTypes::Item);
            long long thing_id = int_field(reward, "thing_id", 0);
            long long thing_quantity = int_field(reward, "thing_quantity", 0);
            things.emplace_back(thing_type, thing_id, thing_quantity);

            json received = json::object();
            received["type"] = thing_type;
            received["id_"] = thing_id;
            received["quantity"] = thing_quantity;
            received["sent_inbox"] = false;

            json drop = json::object();
            drop["received_thing"] = std::move(received);
            drop["order"] = order;
            drop["live_drop_type"] = bool_field(reward, "is_special_fall_thing", false)
                                         ? enums::LiveDropTypes::Special
                                         : enums::LiveDropTypes::Normal;
            drop["is_fixed_drop"] = fixed;
            drops.push_back(std::move(drop));
            order += 1;
        }
    }
    if (!things.empty()) things::grant_things_consolidated(user_id, things);
    return drops;
}

}  // namespace live_drops
