#pragma once
#include "wire.h"

// Ports helpers/environment.py. Builds the EnvironmentResult the client fetches
// on its first, anonymous /api/Environment call: version strings, endpoints/urls
// and the maintenance flag, all read from config. Returned as a wire::json keyed
// by the EnvironmentResult field names.

namespace environment {

wire::json environment();

}  // namespace environment
