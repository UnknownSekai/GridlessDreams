#include "headers.h"

#include "config.h"

namespace headers {

std::vector<std::pair<std::string, std::string>> response_headers() {
    return {{"X-FM", config::get_str("feature_maintenance_flags")}};
}

}  // namespace headers
