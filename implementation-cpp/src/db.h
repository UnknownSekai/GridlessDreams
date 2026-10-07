#pragma once
#include "sqlite3.h"
#include "json.hpp"
#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// SQLite engine replacing the Python asyncpg layer. Query objects mirror db/query.py;
// the free functions mirror DBConnWrapper. Args and rows are nlohmann::json: binding a
// param and reading a column are the two halves of core.py's json/jsonb codec.
namespace db {

using json = nlohmann::json;

static constexpr int DB_VERSION = 3;

// Mirrors db.query.ExecutableQuery. Postgres $n placeholders are kept verbatim; the engine
// rewrites them to SQLite ?n at prepare time.
struct ExecutableQuery {
    std::string sql;
    std::vector<json> args;

    ExecutableQuery() = default;

    template <typename... Args>
    explicit ExecutableQuery(std::string s, Args&&... a)
        : sql(std::move(s)), args{json(std::forward<Args>(a))...} {}
};

// Mirrors db.query.SelectQuery. `model` is the pydantic model name, kept for logging/typing
// only: fetch returns raw json since the wire layer keys off the field name, not the model.
struct SelectQuery {
    std::string sql;
    std::vector<json> args;
    const char* model = nullptr;

    SelectQuery() = default;

    template <typename... Args>
    SelectQuery(const char* m, std::string s, Args&&... a)
        : sql(std::move(s)), args{json(std::forward<Args>(a))...}, model(m) {}
};

// Open `path` (the offline.db file), set PRAGMAs (foreign_keys ON, WAL, busy_timeout,
// synchronous NORMAL), run the generated schema DDL idempotently, and stamp DatabaseInfo
// at DB_VERSION. Returns false on failure. Idempotent across calls.
bool init(const std::string& path);
sqlite3* get();
void close();

// nextval() replacement, backed by the seeded "sequences" table (pre-increment -> first id 1).
int64_t next_sequence(const std::string& name);

// DBConnWrapper surface. execute()/execute_all() return the Postgres-style command tag.
std::string execute(const ExecutableQuery& q);
void execute_batch(const std::string& sql, const std::vector<std::vector<json>>& args_seq);
std::vector<std::string> execute_all(const std::vector<ExecutableQuery>& queries);
std::vector<json> fetch(const SelectQuery& q);
std::optional<json> fetchrow(const SelectQuery& q);

// RAII transaction: outermost is BEGIN IMMEDIATE/COMMIT/ROLLBACK, nested uses SAVEPOINT.
// Holds the connection mutex for its lifetime; same-thread nested ops re-enter freely.
class Transaction {
public:
    Transaction();
    Transaction(Transaction&& o) noexcept;
    ~Transaction();
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    Transaction& operator=(Transaction&&) = delete;
    void commit();
    void rollback();

private:
    std::unique_lock<std::recursive_mutex> lock_;
    int depth_ = -1;
    bool active_ = false;
};

inline Transaction transaction() { return Transaction(); }

// Composite executors for the ~20 Postgres data-modifying-CTE functions, which SQLite cannot
// express as a single statement. Each runs its sub-statements inside one transaction.

// WITH upd AS (UPDATE ... RETURNING ...) INSERT ... WHERE NOT EXISTS (SELECT 1 FROM upd):
// run the UPDATE; if it changed no rows, run the INSERT. Returns the executed tag.
std::string composite_update_or_insert(const ExecutableQuery& update_q,
                                       const ExecutableQuery& insert_q);

// WITH d AS (DELETE ...) INSERT ... (and any ordered multi-statement rewrite): run all
// unconditionally, in order, atomically.
std::vector<std::string> composite_exec(const std::vector<ExecutableQuery>& queries);

// WITH upd AS (UPDATE ... RETURNING *), ins AS (INSERT ... WHERE NOT EXISTS ... RETURNING *)
// SELECT * FROM upd UNION ALL SELECT * FROM ins: run the UPDATE ... RETURNING *; if it
// returned rows, that is the result, else run the INSERT ... RETURNING *. Returns the rows.
std::vector<json> composite_upsert_returning(const SelectQuery& update_sel,
                                             const SelectQuery& insert_sel);

}  // namespace db
