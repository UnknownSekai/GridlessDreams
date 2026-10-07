#include "db/user.h"

#include <cstddef>
#include <string>
#include <vector>

namespace db {
namespace user {

namespace {
std::string join(const std::vector<std::string>& parts, const char* sep) {
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i) out += sep;
        out += parts[i];
    }
    return out;
}
}  // namespace

SelectQuery get_character_base(std::int64_t user_id, std::int64_t base_master_id) {
    return SelectQuery(
        "CharacterBaseModel",
        "SELECT * FROM \"character_base\" WHERE \"userId\" = $1 AND \"characterBaseMasterId\" = $2 LIMIT 1",
        user_id,
        base_master_id);
}

ExecutableQuery create_character_base(std::int64_t user_id, std::int64_t row_id,
                                      std::int64_t base_master_id,
                                      std::optional<std::int64_t> costume_master_id,
                                      std::int64_t portal_character_id) {
    return ExecutableQuery(
        "INSERT INTO \"character_base\" "
        "(\"userId\", \"id\", \"characterBaseMasterId\", \"costumeMasterId\", \"portalCharacterId\") "
        "VALUES ($1, $2, $3, $4, $5)",
        user_id,
        row_id,
        base_master_id,
        costume_master_id.has_value() ? json(*costume_master_id) : json(nullptr),
        portal_character_id);
}

ExecutableQuery create_character(std::int64_t user_id, std::int64_t row_id, std::int64_t master_id,
                                 std::int64_t character_base_id) {
    return ExecutableQuery(
        "INSERT INTO \"character\" "
        "(\"userId\", \"id\", \"characterMasterId\", \"characterBaseId\", \"level\", \"senseLevel\", \"selectionType\") "
        "VALUES ($1, $2, $3, $4, 1, 1, 1)",
        user_id,
        row_id,
        master_id,
        character_base_id);
}

ExecutableQuery create_accessory(std::int64_t user_id, std::int64_t row_id, std::int64_t master_id,
                                 const wire::json& effects) {
    return ExecutableQuery(
        "INSERT INTO \"accessory\" "
        "(\"userId\", \"id\", \"accessoryMasterId\", \"level\", \"accessoryEffects\") "
        "VALUES ($1, $2, $3, 1, $4)",
        user_id,
        row_id,
        master_id,
        effects);
}

ExecutableQuery add_stamina(std::int64_t user_id, std::int64_t amount) {
    return ExecutableQuery(
        "UPDATE \"user\" SET \"currentStamina\" = \"currentStamina\" + $2 WHERE \"userId\" = $1",
        user_id,
        amount);
}

ExecutableQuery grant_possession(const std::string& table, const std::string& master_col,
                                 std::int64_t user_id, std::int64_t row_id, std::int64_t master_id,
                                 const wire::json& extra) {
    // table / master_col / extra keys come from a hardcoded ThingType map, never user input
    std::vector<std::string> cols = {"\"userId\"", "\"id\"", "\"" + master_col + "\""};
    std::vector<json> vals = {json(user_id), json(row_id), json(master_id)};
    for (auto it = extra.begin(); it != extra.end(); ++it) {
        cols.push_back("\"" + it.key() + "\"");
        vals.push_back(it.value());
    }
    std::vector<std::string> placeholders;
    placeholders.reserve(vals.size());
    for (std::size_t i = 0; i < vals.size(); ++i)
        placeholders.push_back("$" + std::to_string(i + 1));

    ExecutableQuery q;
    q.sql = "INSERT INTO \"" + table + "\" (" + join(cols, ", ") + ") VALUES (" +
            join(placeholders, ", ") + ")";
    q.args = std::move(vals);
    return q;
}

std::string grant_collection(const std::string& table, const std::string& array_col,
                             std::int64_t user_id, std::int64_t master_id) {
    // append master_id to the per-user singleton's json array (creating the row if absent) without
    // duplicating; pg built the jsonb in-SQL so no python-list -> json codec is needed. SQLite has
    // no data-modifying CTEs: run the guarded append, else the first-time insert (composite + changes()).
    ExecutableQuery update_q(
        "UPDATE \"" + table + "\" SET \"" + array_col + "\" = "
        "json_insert(COALESCE(\"" + array_col + "\", '[]'), '$[#]', $2) "
        "WHERE \"userId\" = $1 AND NOT EXISTS ("
        "SELECT 1 FROM json_each(COALESCE(\"" + array_col + "\", '[]')) WHERE value = $2)",
        user_id,
        master_id);
    ExecutableQuery insert_q(
        "INSERT INTO \"" + table + "\" (\"userId\", \"id\", \"" + array_col + "\") "
        "SELECT $1, $1, json_array($2) "
        "WHERE NOT EXISTS (SELECT 1 FROM \"" + table + "\" WHERE \"userId\" = $1)",
        user_id,
        master_id);
    return db::composite_update_or_insert(update_q, insert_q);
}

SelectQuery get_inboxes_by_ids(std::int64_t user_id, const std::vector<std::int64_t>& ids) {
    return SelectQuery(
        "InboxModel",
        "SELECT * FROM \"inbox\" WHERE \"userId\" = $1 AND \"id\" IN (SELECT value FROM json_each($2))",
        user_id,
        ids);
}

ExecutableQuery receive_inbox(std::int64_t user_id, std::int64_t inbox_id, std::int64_t now) {
    return ExecutableQuery(
        "UPDATE \"inbox\" SET \"hasReceived\" = true, \"receivedAt\" = $3 "
        "WHERE \"userId\" = $1 AND \"id\" = $2 AND \"hasReceived\" = false",
        user_id,
        inbox_id,
        now);
}

}  // namespace user
}  // namespace db
