#pragma once
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

// In-memory master-data cache, the repacked MasterMemory blob, and the manifest.
// Mirrors helpers/master_data.py + helpers/cache.py. Rows are ordered_json keyed by the
// pydantic field name (fn), so they feed wire::to_wire directly.

namespace master_data {

using json = nlohmann::ordered_json;

// Read + normalize + extend all 229 tables. Call once at startup (after platform data is ready).
void load();

// Typed rows for a table's on-disk header name (PascalCase alias). Empty if absent.
const std::vector<json>& table(const std::string& header_name);

// MasterDataManifest as json keyed by fn {uri, sas_token, version, publish_timestamp}.
json manifest();

// Memoized MasterMemory .db bytes; nullptr until load() ran AND >=1 table is non-empty.
const std::string* db_blob();

}  // namespace master_data
