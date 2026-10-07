#include "db.h"

#include "generated/schema_generated.h"

#include <cctype>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

namespace db {
namespace {

sqlite3* g_db = nullptr;
std::recursive_mutex g_mutex;
int g_tx_depth = 0;

bool contains_ci(const char* hay, const char* needle) {
    size_t nlen = std::strlen(needle);
    for (const char* p = hay; *p; ++p) {
        size_t i = 0;
        while (i < nlen && p[i] &&
               std::toupper((unsigned char)p[i]) == std::toupper((unsigned char)needle[i]))
            ++i;
        if (i == nlen) return true;
    }
    return false;
}

// $n -> ?n (string-literal aware). SQLite numbered params reuse the bound value for every
// occurrence of ?n, matching Postgres $n reuse without duplicating args.
std::string translate_sql(const std::string& sql) {
    std::string out;
    out.reserve(sql.size());
    bool in_str = false;
    for (size_t i = 0; i < sql.size(); ++i) {
        char c = sql[i];
        if (in_str) {
            out.push_back(c);
            if (c == '\'') {
                if (i + 1 < sql.size() && sql[i + 1] == '\'')
                    out.push_back(sql[++i]);
                else
                    in_str = false;
            }
            continue;
        }
        if (c == '\'') {
            in_str = true;
            out.push_back(c);
            continue;
        }
        if (c == '$' && i + 1 < sql.size() && std::isdigit((unsigned char)sql[i + 1])) {
            out.push_back('?');
            continue;
        }
        out.push_back(c);
    }
    return out;
}

std::string args_repr(const std::vector<json>& args) {
    std::string a = "[";
    for (size_t i = 0; i < args.size(); ++i) {
        if (i) a += ", ";
        a += args[i].dump();
    }
    a += "]";
    return a;
}

// Mirrors DBConnWrapper's `print(query)` before re-raising.
void log_failure(const std::string& sql, const std::vector<json>& args) {
    std::fprintf(stderr, "DB query failed: sql=%s args=%s err=%s\n", sql.c_str(),
                 args_repr(args).c_str(), g_db ? sqlite3_errmsg(g_db) : "(no db)");
}

[[noreturn]] void throw_db(const char* what, const std::string& sql,
                           const std::vector<json>& args) {
    log_failure(sql, args);
    std::string msg = std::string(what) + ": " + (g_db ? sqlite3_errmsg(g_db) : "(no db)");
    throw std::runtime_error(msg);
}

void raw_exec(const char* sql) {
    char* err = nullptr;
    if (sqlite3_exec(g_db, sql, nullptr, nullptr, &err) != SQLITE_OK) {
        std::string m = err ? err : "(unknown)";
        sqlite3_free(err);
        throw std::runtime_error(std::string("sqlite exec failed: ") + sql + " : " + m);
    }
}

void raw_exec_nothrow(const char* sql) {
    sqlite3_exec(g_db, sql, nullptr, nullptr, nullptr);
}

// --- json codec: param bind half ---
void bind_arg(sqlite3_stmt* st, int idx, const json& v) {
    if (v.is_null()) {
        sqlite3_bind_null(st, idx);
    } else if (v.is_boolean()) {
        sqlite3_bind_int(st, idx, v.get<bool>() ? 1 : 0);
    } else if (v.is_number_integer()) {
        if (v.is_number_unsigned())
            sqlite3_bind_int64(st, idx, (sqlite3_int64)v.get<std::uint64_t>());
        else
            sqlite3_bind_int64(st, idx, (sqlite3_int64)v.get<std::int64_t>());
    } else if (v.is_number_float()) {
        sqlite3_bind_double(st, idx, v.get<double>());
    } else if (v.is_string()) {
        const std::string& s = v.get_ref<const std::string&>();
        sqlite3_bind_text(st, idx, s.data(), (int)s.size(), SQLITE_TRANSIENT);
    } else {
        // array / object -> compact JSON text (jsonb columns and the json_each() array trick)
        std::string s = v.dump();
        sqlite3_bind_text(st, idx, s.data(), (int)s.size(), SQLITE_TRANSIENT);
    }
}

void bind_all(sqlite3_stmt* st, const std::vector<json>& args) {
    for (size_t i = 0; i < args.size(); ++i) bind_arg(st, (int)i + 1, args[i]);
}

// --- json codec: row read half ---
// decltype carries our semantic type for every base-table column (BOOLEAN, JSONTEXT,
// INTEGER, REAL, TEXT); computed expression columns have no decltype and fall through to the
// storage class. (SQLITE_ENABLE_COLUMN_METADATA is not compiled in, so the schema::COL_TYPES
// (table,col) map is not consulted at runtime -- decltype is the equivalent signal.)
schema::ColType resolve_type(sqlite3_stmt* st, int i, bool& known) {
    const char* decl = sqlite3_column_decltype(st, i);
    known = true;
    if (decl) {
        if (contains_ci(decl, "JSON")) return schema::JSON;
        if (contains_ci(decl, "BOOL")) return schema::BOOL;
        if (contains_ci(decl, "INT")) return schema::BIGINT;
        if (contains_ci(decl, "REAL") || contains_ci(decl, "DOUB") || contains_ci(decl, "FLOA"))
            return schema::REAL;
        if (contains_ci(decl, "TEXT") || contains_ci(decl, "CHAR") || contains_ci(decl, "CLOB"))
            return schema::TEXT;
    }
    known = false;
    return schema::TEXT;
}

json read_value(sqlite3_stmt* st, int i) {
    if (sqlite3_column_type(st, i) == SQLITE_NULL) return json(nullptr);

    bool known = false;
    schema::ColType ct = resolve_type(st, i, known);
    if (known) {
        switch (ct) {
            case schema::BOOL:
                return json(sqlite3_column_int(st, i) != 0);
            case schema::JSON: {
                const char* t = (const char*)sqlite3_column_text(st, i);
                int n = sqlite3_column_bytes(st, i);
                if (!t) return json(nullptr);
                json parsed = json::parse(std::string(t, (size_t)n), nullptr, false);
                if (parsed.is_discarded()) return json(std::string(t, (size_t)n));
                return parsed;
            }
            case schema::INT:
            case schema::BIGINT:
                return json((std::int64_t)sqlite3_column_int64(st, i));
            case schema::REAL:
                return json(sqlite3_column_double(st, i));
            case schema::TEXT: {
                const char* t = (const char*)sqlite3_column_text(st, i);
                int n = sqlite3_column_bytes(st, i);
                return json(std::string(t ? t : "", (size_t)n));
            }
        }
    }

    switch (sqlite3_column_type(st, i)) {
        case SQLITE_INTEGER:
            return json((std::int64_t)sqlite3_column_int64(st, i));
        case SQLITE_FLOAT:
            return json(sqlite3_column_double(st, i));
        case SQLITE_TEXT: {
            const char* t = (const char*)sqlite3_column_text(st, i);
            int n = sqlite3_column_bytes(st, i);
            return json(std::string(t ? t : "", (size_t)n));
        }
        case SQLITE_BLOB: {
            const char* b = (const char*)sqlite3_column_blob(st, i);
            int n = sqlite3_column_bytes(st, i);
            return json(std::string(b ? b : "", (size_t)n));
        }
        default:
            return json(nullptr);
    }
}

json read_row(sqlite3_stmt* st) {
    int ncol = sqlite3_column_count(st);
    json row = json::object();
    for (int i = 0; i < ncol; ++i) {
        const char* name = sqlite3_column_name(st, i);
        row[name ? name : ""] = read_value(st, i);
    }
    return row;
}

sqlite3_stmt* prepare_or_throw(const std::string& raw_sql, const std::vector<json>& args) {
    std::string sql = translate_sql(raw_sql);
    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(g_db, sql.c_str(), (int)sql.size() + 1, &st, nullptr) != SQLITE_OK) {
        if (st) sqlite3_finalize(st);
        throw_db("sqlite prepare failed", raw_sql, args);
    }
    return st;
}

std::string leading_verb(const std::string& sql) {
    size_t i = 0;
    while (i < sql.size() && std::isspace((unsigned char)sql[i])) ++i;
    size_t j = i;
    while (j < sql.size() && std::isalpha((unsigned char)sql[j])) ++j;
    std::string v = sql.substr(i, j - i);
    for (char& c : v) c = (char)std::toupper((unsigned char)c);
    return v;
}

std::string command_tag(const std::string& sql, int changes) {
    std::string verb = leading_verb(sql);
    if (verb == "INSERT") return "INSERT 0 " + std::to_string(changes);
    if (verb == "UPDATE") return "UPDATE " + std::to_string(changes);
    if (verb == "DELETE") return "DELETE " + std::to_string(changes);
    return verb;
}

// All *_impl helpers assume the connection mutex is held by the caller.
std::string exec_impl(const std::string& sql, const std::vector<json>& args) {
    sqlite3_stmt* st = prepare_or_throw(sql, args);
    try {
        bind_all(st, args);
        int rc;
        while ((rc = sqlite3_step(st)) == SQLITE_ROW) {
        }
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(st);
            throw_db("sqlite step failed", sql, args);
        }
    } catch (...) {
        sqlite3_finalize(st);
        throw;
    }
    int changes = sqlite3_changes(g_db);
    sqlite3_finalize(st);
    return command_tag(sql, changes);
}

std::vector<json> fetch_impl(const std::string& sql, const std::vector<json>& args) {
    sqlite3_stmt* st = prepare_or_throw(sql, args);
    std::vector<json> out;
    try {
        bind_all(st, args);
        int rc;
        while ((rc = sqlite3_step(st)) == SQLITE_ROW) out.push_back(read_row(st));
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(st);
            throw_db("sqlite step failed", sql, args);
        }
    } catch (...) {
        sqlite3_finalize(st);
        throw;
    }
    sqlite3_finalize(st);
    return out;
}

}  // namespace

bool init(const std::string& path) {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    if (g_db) return true;

    if (sqlite3_open(path.c_str(), &g_db) != SQLITE_OK) {
        std::fprintf(stderr, "DB open failed at %s: %s\n", path.c_str(),
                     g_db ? sqlite3_errmsg(g_db) : "(null)");
        if (g_db) {
            sqlite3_close(g_db);
            g_db = nullptr;
        }
        return false;
    }

    raw_exec_nothrow("PRAGMA foreign_keys = ON");
    raw_exec_nothrow("PRAGMA journal_mode = WAL");
    raw_exec_nothrow("PRAGMA busy_timeout = 5000");
    raw_exec_nothrow("PRAGMA synchronous = NORMAL");

    for (int i = 0; i < schema::DDL_COUNT; ++i) {
        char* err = nullptr;
        if (sqlite3_exec(g_db, schema::DDL[i], nullptr, nullptr, &err) != SQLITE_OK) {
            std::fprintf(stderr, "DB DDL failed: %s : %s\n", schema::DDL[i],
                         err ? err : "(unknown)");
            sqlite3_free(err);
            return false;
        }
    }

    raw_exec_nothrow("DELETE FROM \"databaseinfo\"");
    std::string stamp = "INSERT INTO \"databaseinfo\" (\"version\") VALUES (" +
                        std::to_string(schema::DB_VERSION) + ")";
    raw_exec_nothrow(stamp.c_str());
    return true;
}

sqlite3* get() { return g_db; }

void close() {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    if (g_db) {
        sqlite3_close(g_db);
        g_db = nullptr;
    }
}

int64_t next_sequence(const std::string& name) {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    std::vector<json> args{json(name)};
    std::vector<json> rows = fetch_impl(
        "UPDATE \"sequences\" SET \"value\" = \"value\" + 1 WHERE \"name\" = $1 RETURNING \"value\"",
        args);
    if (rows.empty()) throw std::runtime_error("next_sequence: unknown sequence " + name);
    return rows[0].at("value").get<std::int64_t>();
}

std::string execute(const ExecutableQuery& q) {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    return exec_impl(q.sql, q.args);
}

void execute_batch(const std::string& sql, const std::vector<std::vector<json>>& args_seq) {
    Transaction tx;
    sqlite3_stmt* st = prepare_or_throw(sql, {});
    try {
        for (const std::vector<json>& args : args_seq) {
            bind_all(st, args);
            int rc;
            while ((rc = sqlite3_step(st)) == SQLITE_ROW) {
            }
            if (rc != SQLITE_DONE) {
                sqlite3_finalize(st);
                throw_db("sqlite step failed", sql, args);
            }
            sqlite3_reset(st);
            sqlite3_clear_bindings(st);
        }
    } catch (...) {
        sqlite3_finalize(st);
        throw;
    }
    sqlite3_finalize(st);
    tx.commit();
}

std::vector<std::string> execute_all(const std::vector<ExecutableQuery>& queries) {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    std::vector<std::string> out;
    out.reserve(queries.size());
    for (const ExecutableQuery& q : queries) out.push_back(exec_impl(q.sql, q.args));
    return out;
}

std::vector<json> fetch(const SelectQuery& q) {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    return fetch_impl(q.sql, q.args);
}

std::optional<json> fetchrow(const SelectQuery& q) {
    std::lock_guard<std::recursive_mutex> g(g_mutex);
    std::vector<json> rows = fetch_impl(q.sql, q.args);
    if (rows.empty()) return std::nullopt;
    return rows.front();
}

// --- Transaction ---
Transaction::Transaction() : lock_(g_mutex) {
    depth_ = g_tx_depth;
    if (depth_ == 0)
        raw_exec("BEGIN IMMEDIATE");
    else
        raw_exec(("SAVEPOINT sp" + std::to_string(depth_)).c_str());
    g_tx_depth = depth_ + 1;
    active_ = true;
}

Transaction::Transaction(Transaction&& o) noexcept
    : lock_(std::move(o.lock_)), depth_(o.depth_), active_(o.active_) {
    o.active_ = false;
    o.depth_ = -1;
}

Transaction::~Transaction() {
    if (active_) {
        try {
            rollback();
        } catch (...) {
        }
    }
}

void Transaction::commit() {
    if (!active_) return;
    if (depth_ == 0)
        raw_exec("COMMIT");
    else
        raw_exec(("RELEASE sp" + std::to_string(depth_)).c_str());
    g_tx_depth = depth_;
    active_ = false;
}

void Transaction::rollback() {
    if (!active_) return;
    if (depth_ == 0) {
        raw_exec_nothrow("ROLLBACK");
    } else {
        raw_exec_nothrow(("ROLLBACK TO sp" + std::to_string(depth_)).c_str());
        raw_exec_nothrow(("RELEASE sp" + std::to_string(depth_)).c_str());
    }
    g_tx_depth = depth_;
    active_ = false;
}

// --- composite executors ---
std::string composite_update_or_insert(const ExecutableQuery& update_q,
                                       const ExecutableQuery& insert_q) {
    Transaction tx;
    std::string tag = exec_impl(update_q.sql, update_q.args);
    if (sqlite3_changes(g_db) == 0) tag = exec_impl(insert_q.sql, insert_q.args);
    tx.commit();
    return tag;
}

std::vector<std::string> composite_exec(const std::vector<ExecutableQuery>& queries) {
    Transaction tx;
    std::vector<std::string> out;
    out.reserve(queries.size());
    for (const ExecutableQuery& q : queries) out.push_back(exec_impl(q.sql, q.args));
    tx.commit();
    return out;
}

std::vector<json> composite_upsert_returning(const SelectQuery& update_sel,
                                             const SelectQuery& insert_sel) {
    Transaction tx;
    std::vector<json> rows = fetch_impl(update_sel.sql, update_sel.args);
    if (rows.empty()) rows = fetch_impl(insert_sel.sql, insert_sel.args);
    tx.commit();
    return rows;
}

}  // namespace db
