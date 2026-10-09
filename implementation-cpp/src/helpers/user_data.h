#pragma once
#include <optional>
#include <string>
#include <vector>

#include "wire.h"

// builds the /api/data/user response (an IDataObject[]) and the per-route present
// diffs, entirely from the DB. ports helpers/user_data.py. the async app/conn
// parameters are dropped: reads go through the global db engine. each per-user row
// is a camelCase <Entity>Model json; _to_array remaps it to the entity's wire
// field names (id_, player_rank, ...) and the generic wire layer does the final
// [Key(n)] positional encoding (DateTime/enum/byte[] handling included). present /
// user_data / data_object entries are wire::union_entry arrays [key, name, value].

namespace httplib {
struct Request;
}

namespace user_data {

// one build_present target: an entity type (all of the caller's rows of that type)
// or a type plus the specific row ids to keep. mirrors the python str | (str, ids)
// varargs spec; non-explicit ctors so {"User", {"Inbox", {id...}}} braces convert.
struct PresentSpec {
    std::string name;
    std::optional<std::vector<long long>> ids;

    PresentSpec(const char* n) : name(n) {}
    PresentSpec(std::string n) : name(std::move(n)) {}
    PresentSpec(std::string n, std::vector<long long> i) : name(std::move(n)), ids(std::move(i)) {}
};

// PascalCase type name -> snake_case per-user table name (CamelCase -> camel_case)
std::string _table(const std::string& type_name);

// a camelCase DB row -> the entity's wire value keyed by field name (fn). the
// generic wire layer turns this into the [Key(n)] positional array at pack time.
wire::json _to_array(const std::string& type_name, const wire::json& value);

// resolve the caller's userId from the JWT bearer token, or none
std::optional<long long> current_user_id(const httplib::Request& request);

// one IDataObject[] present entry [unionKey, name, value] from a DB row
wire::json data_object(const std::string& type_name, const wire::json& row);

// a present entry carrying an empty value array -- a bare typed marker (e.g.
// UpdateGameHintRead returns an empty GameHint)
wire::json empty_data_object(const std::string& type_name);

// a route's present: the IDataObject[] diff of the resources the operation updated.
// each spec names what changed (a whole type, or a type limited to specific ids);
// rows are read back post-update. unknown types are skipped. empty if user is none.
wire::json build_present(std::optional<long long> user_id,
                         const std::vector<PresentSpec>& updated);

// the caller's full IDataObject[] from every per-user table, in one query
wire::json user_data(std::optional<long long> user_id);

}  // namespace user_data
