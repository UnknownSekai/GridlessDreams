#include "db/user.h"

#include <string>
#include <vector>

namespace db {
namespace user {

SelectQuery next_live_id() {
    // collision-free owned-live id; postgres nextval -> the seeded sequences table
    // (pre-increment via UPDATE ... RETURNING, first id 1). yields a {value} row.
    return SelectQuery(
        "SequenceValueModel",
        "UPDATE \"sequences\" SET \"value\" = \"value\" + 1 WHERE \"name\" = $1 RETURNING \"value\"",
        std::string("live_id_seq"));
}

ExecutableQuery update_player_rate(std::int64_t user_id, double rate) {
    return ExecutableQuery(
        "UPDATE \"user_profile\" SET \"playerRate\" = $2 WHERE \"userId\" = $1",
        user_id,
        rate);
}

ExecutableQuery update_music_releases(
    std::int64_t user_id,
    const std::vector<std::tuple<std::int64_t, bool, std::int64_t>>& changes) {
    // batch stella/olivier release updates for many music rows into one statement
    // (changes = [(music_id, stella_released, olivier_status), ...]; must be non-empty).
    std::vector<std::string> rows;
    std::vector<json> args;
    args.emplace_back(user_id);
    for (const auto& change : changes) {
        int n = static_cast<int>(args.size());
        rows.push_back("($" + std::to_string(n + 1) + ", $" + std::to_string(n + 2) +
                       ", $" + std::to_string(n + 3) + ")");
        args.emplace_back(std::get<0>(change));
        args.emplace_back(std::get<1>(change));
        args.emplace_back(std::get<2>(change));
    }
    std::string values;
    for (std::size_t i = 0; i < rows.size(); ++i) {
        if (i) values += ", ";
        values += rows[i];
    }
    // sqlite has no UPDATE ... FROM (VALUES ...) AS v(cols); use a column-list CTE instead.
    ExecutableQuery q;
    q.sql = "WITH v(id, stella, status) AS (VALUES " + values + ") "
            "UPDATE \"music\" AS m SET \"stellaReleased\" = v.stella, "
            "\"olivierReleaseStatus\" = v.status "
            "FROM v "
            "WHERE m.\"userId\" = $1 AND m.\"id\" = v.id";
    q.args = std::move(args);
    return q;
}

ExecutableQuery delete_active_lives(std::int64_t user_id) {
    return ExecutableQuery("DELETE FROM \"active_live\" WHERE \"userId\" = $1", user_id);
}

ExecutableQuery create_active_live(
    std::int64_t user_id,
    std::int64_t live_id,
    std::int64_t live_master_id,
    std::int64_t party_id,
    std::int64_t live_setting_master_id,
    bool stamina_spent) {
    return ExecutableQuery(
        "INSERT INTO \"active_live\" "
        "(\"userId\", \"id\", \"liveMasterId\", \"partyId\", \"liveSettingMasterId\", \"staminaSpent\") "
        "VALUES ($1, $2, $3, $4, $5, $6)",
        user_id,
        live_id,
        live_master_id,
        party_id,
        live_setting_master_id,
        stamina_spent);
}

SelectQuery get_active_live(std::int64_t user_id) {
    return SelectQuery(
        "ActiveLiveModel", "SELECT * FROM \"active_live\" WHERE \"userId\" = $1", user_id);
}

ExecutableQuery update_live_result(
    std::int64_t user_id,
    std::int64_t live_master_id,
    std::int64_t times_completed,
    double achievement_rate,
    double notation_rate,
    std::int64_t clear_lamp,
    std::int64_t rate_grade) {
    // best-of merge is decided by the caller; this just writes the row for (userId, liveMasterId)
    return ExecutableQuery(
        "UPDATE \"live\" SET \"timesCompleted\" = $3, \"achievementRate\" = $4, "
        "\"notationRate\" = $5, \"clearLamp\" = $6, \"rateGrade\" = $7 "
        "WHERE \"userId\" = $1 AND \"liveMasterId\" = $2",
        user_id,
        live_master_id,
        times_completed,
        achievement_rate,
        notation_rate,
        clear_lamp,
        rate_grade);
}

}  // namespace user
}  // namespace db
