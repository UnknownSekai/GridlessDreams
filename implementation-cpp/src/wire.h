#pragma once
#include <nlohmann/json.hpp>
#include <msgpack.hpp>
#include <string>

namespace wire {

using json = nlohmann::ordered_json;

// C# default(DateTime) == DateTime.MinValue (0001-01-01), seconds before the Unix epoch.
static constexpr long long DATETIME_MIN_SECONDS = -62135596800LL;

// Defined in generated/wire_keys_generated.h (included by wire.cpp).
struct FieldSpec;
struct ModelSpec;

// Known ModelSpec* or nullptr. Backed by a string_view->ModelSpec* map built once
// from KEYS_TABLE (the table is only roughly alphabetical, so no binary search).
const ModelSpec* find_model(const char* name);

// to_wire + msgpack-encode a named model instance to raw bytes.
std::string pack(const char* name, const json& value);

// Encode a named value into an existing packer (recursion / result / union values).
void to_wire(msgpack::packer<msgpack::sbuffer>& pk, const char* name, const json& value);

// Decode one msgpack model array to ordered_json keyed by field name.
json from_array(const char* name, const msgpack::object& o);

// msgpack-decode a request body and undo MessagePack-CSharp LZ4 (ext 98/99).
// Owns its zone; callers from_array() against .get().
msgpack::object_handle unpack_raw(const std::string& body);

// unpack_raw then from_array(name,...). name==nullptr returns the raw decoded tree.
json read_request(const std::string& body, const char* name);

// 5-pack envelope (faults, result, present, deleted, notifications); any list omitted -> empty array.
std::string common_response(const char* result_name, const json& result, const json& faults = json::array(),
                            const json& present = json::array(), const json& deleted = json::array(),
                            const json& notifications = json::array());

// like common_response but the result slot is itself an IDataObject[] union list (packed like
// present): GetUserData returns the full union array [[key,[values]], ...] as its result.
std::string common_response_union_result(const json& result, const json& faults = json::array(),
                                         const json& present = json::array(), const json& deleted = json::array(),
                                         const json& notifications = json::array());

// IDataObject[] / INotificationObject[] element: [discriminatorKey, to_wire(name,value)].
json union_entry(long long key, const char* name, const json& value);

}  // namespace wire
