#pragma once
#include "httplib_config.h"

// Addressables catalog / bundle / notation routes. Ports routes/assets.py.

namespace routes {

void register_assets(httplib::Server& svr);

}  // namespace routes
