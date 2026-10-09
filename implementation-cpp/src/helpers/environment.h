#pragma once
#include "wire.h"

// ports helpers/environment.py. builds the EnvironmentResult the client fetches
// on its first, anonymous /api/Environment call: version strings, endpoints/urls
// and the maintenance flag, all read from config. returned as a wire::json keyed
// by the EnvironmentResult field names.

namespace environment {

wire::json environment();

}  // namespace environment
