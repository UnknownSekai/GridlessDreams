#pragma once
#include "httplib_config.h"

namespace routes {

void register_data(httplib::Server& svr);

// the greedy /master-data/production/(.*) blob handler. registered LAST (after every other
// route) so specific paths like the episodes scenes/*.bin route match first, mirroring
// app.py, where this catch-all is declared after all include_router() calls
void register_master_data_blob(httplib::Server& svr);

}  // namespace routes
