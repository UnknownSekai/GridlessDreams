#include "routes.h"

#include "routes/accessories.h"
#include "routes/account.h"
#include "routes/assets.h"
#include "routes/auditions.h"
#include "routes/character_mission.h"
#include "routes/characters.h"
#include "routes/circles.h"
#include "routes/comics.h"
#include "routes/data.h"
#include "routes/debug.h"
#include "routes/environment.h"
#include "routes/episodes.h"
#include "routes/events.h"
#include "routes/flash_sale.h"
#include "routes/friend_invitation.h"
#include "routes/friends.h"
#include "routes/gachas.h"
#include "routes/home.h"
#include "routes/inbox.h"
#include "routes/items.h"
#include "routes/kms_general_payment.h"
#include "routes/leagues.h"
#include "routes/lessons.h"
#include "routes/lives.h"
#include "routes/live_modes.h"
#include "routes/login.h"
#include "routes/mission.h"
#include "routes/multi_room.h"
#include "routes/party.h"
#include "routes/photo.h"
#include "routes/player.h"
#include "routes/possessions.h"
#include "routes/posters.h"
#include "routes/profile.h"
#include "routes/roulette.h"
#include "routes/shops.h"

// ports routes/__init__.py (routers) + app.py's include_router()/live_modes.install()
// wiring: register every base module, then prepend the live_modes overrides so they win

namespace routes {

void setup(httplib::Server& svr) {
    register_accessories(svr);
    register_account(svr);
    register_assets(svr);
    register_auditions(svr);
    register_character_mission(svr);
    register_characters(svr);
    register_circles(svr);
    register_comics(svr);
    register_data(svr);
    register_debug(svr);
    register_environment(svr);
    register_episodes(svr);
    register_events(svr);
    register_flash_sale(svr);
    register_friend_invitation(svr);
    register_friends(svr);
    register_gachas(svr);
    register_home(svr);
    register_inbox(svr);
    register_items(svr);
    register_kms_general_payment(svr);
    register_leagues(svr);
    register_lessons(svr);
    register_lives(svr);
    register_login(svr);
    register_mission(svr);
    register_multi_room(svr);
    register_party(svr);
    register_photo(svr);
    register_player(svr);
    register_possessions(svr);
    register_posters(svr);
    register_profile(svr);
    register_roulette(svr);
    register_shops(svr);

    // prepend the live/lesson/course overrides so they take precedence over the base
    // lives/lessons handlers (install() PREPENDS; cpp-httplib is first-match-wins)
    install_live_modes(svr);
}

}  // namespace routes
