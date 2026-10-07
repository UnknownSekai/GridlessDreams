#include "helpers/music_unlock.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>

#include "db.h"
#include "db/user.h"
#include "master_data.h"

// Ports helpers/music_unlock.py; see that file for the reasoning behind each release rule.
// Master rows are ordered_json keyed by field name (LiveMaster: id_, difficulty,
// music_master_id, level, unlock_condition, unlock_value). DB rows are nlohmann::json keyed
// by the camelCase column name. The async conn/app params are dropped for the global db
// engine; ensure_default_music runs under db::transaction() (the pg_advisory lock is dropped,
// transaction() already serializes writers).

namespace music_unlock {

namespace {

using mjson = nlohmann::ordered_json;  // master-data row

// _BY_ID / _BY_MUSIC, built once from cache.live_master.
struct Index {
    std::map<long long, const mjson*> by_id;
    std::map<long long, std::map<long long, const mjson*>> by_music;
};

const Index& index() {
    static const Index idx = [] {
        Index e;
        for (const mjson& m : master_data::table("LiveMaster")) {
            e.by_id[m.at("id_").get<long long>()] = &m;
            e.by_music[m.at("music_master_id").get<long long>()]
                      [m.at("difficulty").get<long long>()] = &m;
        }
        return e;
    }();
    return idx;
}

const mjson* live_master(long long live_master_id) {
    const Index& idx = index();
    auto it = idx.by_id.find(live_master_id);
    return it == idx.by_id.end() ? nullptr : it->second;
}

const mjson* chart(long long music_master_id, long long difficulty) {
    const Index& idx = index();
    auto mit = idx.by_music.find(music_master_id);
    if (mit == idx.by_music.end()) return nullptr;
    auto dit = mit->second.find(difficulty);
    return dit == mit->second.end() ? nullptr : dit->second;
}

bool cleared(const nlohmann::json& row) {
    return row.at("clearLamp").get<long long>() >= enums::ClearLamps::Clear;
}

bool stella_open_by_default(long long music_master_id) {
    const mjson* stella = chart(music_master_id, enums::MusicDifficulties::Stella);
    return stella != nullptr &&
           stella->at("unlock_condition").get<long long>() == enums::LiveUnlockConditionTypes::None_;
}

}  // namespace

bool affects_unlocks(long long live_master_id) {
    const mjson* lm = live_master(live_master_id);
    if (lm == nullptr) return false;
    long long d = lm->at("difficulty").get<long long>();
    return d == enums::MusicDifficulties::Extra || d == enums::MusicDifficulties::Stella ||
           d == enums::MusicDifficulties::Olivier;
}

std::optional<long long> extra_good_budget(long long music_master_id) {
    const mjson* stella = chart(music_master_id, enums::MusicDifficulties::Stella);
    if (stella == nullptr || stella->at("unlock_condition").get<long long>() !=
                                 enums::LiveUnlockConditionTypes::ExtraGoodCount)
        return std::nullopt;
    const mjson& value = stella->at("unlock_value");
    if (value.is_null()) return DEFAULT_EXTRA_GOOD_BUDGET;
    return value.get<long long>();
}

std::optional<long long> stella_unlocked_by(long long live_master_id, bool is_cleared,
                                            long long good_or_worse) {
    const mjson* lm = live_master(live_master_id);
    if (lm == nullptr || lm->at("difficulty").get<long long>() != enums::MusicDifficulties::Extra ||
        !is_cleared)
        return std::nullopt;
    std::optional<long long> budget = extra_good_budget(lm->at("music_master_id").get<long long>());
    if (!budget || good_or_worse > *budget) return std::nullopt;
    return lm->at("music_master_id").get<long long>();
}

MusicProgress::MusicProgress(const std::vector<nlohmann::json>& lives,
                             const std::vector<nlohmann::json>& musics,
                             std::optional<long long> stella_unlock_)
    : stella_unlock(stella_unlock_), all_stella(false), max_cleared_olivier_level(0) {
    long long released = 0;
    for (const nlohmann::json& m : musics)
        if (m.at("olivierReleaseStatus").get<long long>() == enums::OlivierReleaseStatuses::Released)
            ++released;
    all_stella = released >= OLIVIER_STELLA_THRESHOLD;
    for (const nlohmann::json& r : lives) {
        const mjson* lm = live_master(r.at("liveMasterId").get<long long>());
        if (lm == nullptr) continue;
        long long d = lm->at("difficulty").get<long long>();
        if (d == enums::MusicDifficulties::Olivier && cleared(r)) {
            max_cleared_olivier_level =
                std::max(max_cleared_olivier_level, lm->at("level").get<long long>());
        } else if (d == enums::MusicDifficulties::Stella &&
                   r.at("rateGrade").get<long long>() >= enums::AchievementRateGrades::S) {
            stella_s_ranked.insert(lm->at("music_master_id").get<long long>());
        }
    }
}

bool MusicProgress::stella_released(long long music_master_id, bool is_possession,
                                    bool stored) const {
    if (stored) return true;  // sticky once released
    if (is_possession && (all_stella || stella_open_by_default(music_master_id))) return true;
    return stella_unlock.has_value() && music_master_id == *stella_unlock;
}

long long MusicProgress::olivier_status(long long music_master_id, long long stored,
                                        bool stella_released) const {
    if (stored == enums::OlivierReleaseStatuses::Released)
        return enums::OlivierReleaseStatuses::Released;  // purchased -> sticky across rerates
    if (!stella_released) return enums::OlivierReleaseStatuses::None_;  // never ahead of Stella
    const mjson* olivier = chart(music_master_id, enums::MusicDifficulties::Olivier);
    if (olivier == nullptr) return enums::OlivierReleaseStatuses::None_;
    if (olivier->at("level").get<long long>() <= max_cleared_olivier_level)
        return enums::OlivierReleaseStatuses::Purchasable;
    if (stella_s_ranked.count(music_master_id)) return enums::OlivierReleaseStatuses::Challengeable;
    return enums::OlivierReleaseStatuses::None_;
}

MusicProgress load_progress(long long user_id, std::optional<long long> stella_unlock) {
    return MusicProgress(db::fetch(db::user::get_lives(user_id)),
                         db::fetch(db::user::get_musics(user_id)), stella_unlock);
}

std::vector<nlohmann::json> apply_unlocks(long long user_id,
                                          std::optional<long long> stella_unlock) {
    MusicProgress progress = load_progress(user_id, stella_unlock);
    std::vector<nlohmann::json> changed;
    std::vector<std::tuple<std::int64_t, bool, std::int64_t>> changes;
    std::vector<nlohmann::json> musics = db::fetch(db::user::get_musics(user_id));
    for (nlohmann::json& music : musics) {
        bool new_stella = progress.stella_released(music.at("musicMasterId").get<long long>(),
                                                   music.at("isPossession").get<bool>(),
                                                   music.at("stellaReleased").get<bool>());
        long long new_status = progress.olivier_status(
            music.at("musicMasterId").get<long long>(),
            music.at("olivierReleaseStatus").get<long long>(), new_stella);
        if (new_stella != music.at("stellaReleased").get<bool>() ||
            new_status != music.at("olivierReleaseStatus").get<long long>()) {
            music["stellaReleased"] = new_stella;
            music["olivierReleaseStatus"] = new_status;
            changes.emplace_back(music.at("id").get<std::int64_t>(), new_stella, new_status);
            changed.push_back(music);
        }
    }
    if (!changes.empty()) db::execute(db::user::update_music_releases(user_id, changes));
    return changed;
}

std::pair<bool, long long> granted_music_state(long long user_id, long long music_master_id) {
    MusicProgress progress = load_progress(user_id);
    bool stella = progress.stella_released(music_master_id, true, false);
    return {stella, progress.olivier_status(music_master_id, enums::OlivierReleaseStatuses::None_,
                                            stella)};
}

const std::set<long long>& default_music_master_ids() {
    static const std::set<long long> ids = [] {
        std::set<long long> r;
        for (const mjson& m : master_data::table("MusicMaster")) {
            if (m.at("unlock_condition_type").get<long long>() == DEFAULT_MUSIC_UNLOCK_TYPE &&
                !m.at("invisible").get<bool>() && m.at("id_").get<long long>() != DUMMY_MUSIC_ID)
                r.insert(m.at("id_").get<long long>());
        }
        return r;
    }();
    return ids;
}

void ensure_default_music(std::optional<long long> user_id) {
    if (!user_id) return;
    const std::set<long long>& ids = default_music_master_ids();
    if (ids.empty()) return;
    auto tx = db::transaction();
    std::vector<nlohmann::json> musics = db::fetch(db::user::get_musics(*user_id));
    std::set<long long> owned;
    for (const nlohmann::json& m : musics) owned.insert(m.at("musicMasterId").get<long long>());
    std::set<long long> missing;
    for (long long id : ids)
        if (owned.find(id) == owned.end()) missing.insert(id);
    if (missing.empty()) {
        tx.commit();
        return;
    }
    MusicProgress progress(db::fetch(db::user::get_lives(*user_id)), musics);
    long long next_id = 0;
    for (const nlohmann::json& m : musics) next_id = std::max(next_id, m.at("id").get<long long>());
    next_id += 1;
    std::string sql;
    bool have_sql = false;
    std::vector<std::vector<db::json>> args_seq;
    long long offset = 0;
    for (long long music_master_id : missing) {  // std::set iterates sorted, as python's sorted()
        bool stella = progress.stella_released(music_master_id, true, false);
        wire::json row;
        row["id"] = next_id + offset;
        row["musicMasterId"] = music_master_id;
        row["stellaReleased"] = stella;
        row["vocalVersion"] = 0;
        row["olivierReleaseStatus"] =
            progress.olivier_status(music_master_id, enums::OlivierReleaseStatuses::None_, stella);
        row["isPossession"] = true;
        db::ExecutableQuery query = db::user::upsert_music(*user_id, row);
        sql = query.sql;  // identical for every row
        have_sql = true;
        args_seq.push_back(query.args);
        ++offset;
    }
    if (have_sql) db::execute_batch(sql, args_seq);
    tx.commit();
}

}  // namespace music_unlock
