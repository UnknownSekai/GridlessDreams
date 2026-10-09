#pragma once
#include "../httplib_config.h"

namespace routes {

// ports routes/live_modes.py install(): registers the master-driven live / lesson /
// music-course override handlers and then prepends them so they win over the
// routes/lives.py + routes/lessons.py base handlers (cpp-httplib dispatches
// first-match-wins, so a later registration is rotated to the front)
void install_live_modes(httplib::Server& svr);

}  // namespace routes
