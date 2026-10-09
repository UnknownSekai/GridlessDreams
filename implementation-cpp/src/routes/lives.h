#pragma once
#include "../httplib_config.h"

namespace routes {

void register_lives(httplib::Server& svr);

// upstream handlers, exposed so routes/live_modes.cpp can run them inside its own
// transaction (the override wraps these, mirroring live_modes.py `upstream.*`)
void lives_start(const httplib::Request& req, httplib::Response& res);
void lives_finish_and_validate(const httplib::Request& req, httplib::Response& res);
void lives_retire(const httplib::Request& req, httplib::Response& res);

}  // namespace routes
