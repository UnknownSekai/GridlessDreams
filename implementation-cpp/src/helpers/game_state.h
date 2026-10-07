#pragma once
#include "db.h"

#include <cstdint>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Per-account transactional state for multi-step gameplay writes. Ports helpers/game_state.py.
// A State caches the caller's rows for the duration of one write transaction, tracks what it
// changed, and can emit the `present` diff the client expects. Costs/limits come from master
// data; mutations are atomic and serialized per account (the Postgres advisory lock becomes
// the SQLite write transaction).

namespace httplib {
struct Request;
}

namespace game_state {

using json = nlohmann::json;           // db rows (camelCase columns), where/values maps
using ojson = nlohmann::ordered_json;  // master rows and wire-facing arrays

// aborts and rolls back a gameplay action; the caller returns a failure result
struct Rejected : std::runtime_error {
    explicit Rejected(const std::string& message = "") : std::runtime_error(message) {}
};

// first cached master row of `table` whose `field` equals `ident`, or nullptr
const ojson* master(const std::string& table, int64_t ident, const std::string& field = "id_");

// round-trip through float32, matching the client's single-precision bonus fields
double f32(double value);

class State {
public:
    explicit State(int64_t uid);

    // caller's rows for an entity table, fetched once then cached and mutated in place
    std::vector<json>& rows(const std::string& name);
    // first cached row matching every column=value in `where` (empty where -> first row), or nullptr
    json* one(const std::string& name, const json& where = json::object());

    // per-user primary key column for an entity ("id" unless overridden)
    static std::string key_field(const std::string& name);

    // write `values` onto `row` (Rejected if row is null) and record it dirty
    void update(const std::string& entity, json* row, const json& values);
    // upsert a new row (defaulting a fresh "id" when that is the pk) and return it
    json insert(const std::string& entity, json values);

    // spend items ({itemMasterId: quantity}) and coin atomically; Rejected if unaffordable
    void pay(const std::map<int64_t, int64_t>& costs, int64_t coin = 0);
    // grant (thing_type, thing_id, quantity) triples consolidated; returns the ReceivedThing[]
    ojson grant(const ojson& things);

    // IDataObject[] diff [[unionKey, _to_array(name, row)], ...] in dirty insertion order
    ojson present() const;

    // persist the transaction (the success path of the python `async with` block)
    void commit();

    int64_t uid;
    std::unordered_map<std::string, std::vector<json>> tables;
    // changed rows keyed by (entity, pk), in first-touch order
    std::vector<std::pair<std::pair<std::string, int64_t>, json>> dirty;

private:
    // entities whose per-user primary key is not "id"
    static const std::unordered_map<std::string, std::string> KEY_FIELD;
    db::Transaction tx_;
};

// advisory-locked transaction scope: resolves the caller (Rejected if unauthenticated) and
// returns a State bound to an open write transaction
State transaction(const httplib::Request& request);

void character_progress(State& s, int64_t base, int64_t mission_id, int64_t delta);
void mission_progress(State& s, int64_t ident, int64_t delta = 1, bool create = false,
                      std::optional<int64_t> absolute = std::nullopt);
void costume_owned(State& s, int64_t base, int64_t ident);
ojson star_points(State& s, int64_t base, int64_t points);
void level_missions(State& s, const ojson& cm, int64_t delta, int64_t level);

}  // namespace game_state
