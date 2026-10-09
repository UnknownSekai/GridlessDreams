#pragma once
#include <string>
#include <utility>
#include <vector>

// HTTP response headers. ports helpers/headers.py.

namespace headers {

// only X-FM (feature_maintenance_flags) is sent. the client runs version /
// masterdata checks and errors if it sees X-Server-Version / X-Assets-Version /
// X-MasterData-* (even an empty one), so those are deliberately omitted.
std::vector<std::pair<std::string, std::string>> response_headers();

}  // namespace headers
