#include "routes/gachas.h"

#include <cstdint>
#include <ctime>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "helpers/gacha.h"
#include "helpers/things.h"
#include "helpers/user_data.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/gachas.py. Roll/{detail} and SetSelectedThings carry the real logic; the
// reroll endpoints are stubs the Python leaves as stubs.

namespace routes {
namespace {

using wire::json;

template <class J>
long long jint(const J& obj, const char* key) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return 0;
    return it->template get<long long>();
}

template <class J>
bool jbool(const J& obj, const char* key) {
    auto it = obj.find(key);
    return it != obj.end() && it->is_boolean() && it->template get<bool>();
}

long long _random_gacha_id() {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    std::uniform_int_distribution<long long> dist(1000000LL, 9999999999LL);
    return dist(eng);
}

// charge a roll. returns the present-entity names the charge touched, or nullopt -- having
// written nothing -- when the caller cannot afford it, so the roll is rejected rather than
// given away. an empty set means a free pull that charged nothing.
std::optional<std::set<std::string>> _spend_cost(long long user_id, const json& detail) {
    if (jbool(detail, "is_free")) return std::set<std::string>{};

    long long ticket_id = jint(detail, "required_ticket_m_item_id");
    long long ticket_qty = jint(detail, "required_ticket_quantity");
    if (ticket_id && ticket_qty) {
        std::optional<db::json> row;
        for (const db::json& i : db::fetch(db::user::get_items(user_id)))
            if (jint(i, "itemMasterId") == ticket_id) {
                row = i;
                break;
            }
        if (!row || jint(*row, "stock") < ticket_qty) return std::nullopt;
        db::user::increment_item_stock(user_id, ticket_id, -ticket_qty);
        return std::set<std::string>{"Item"};
    }

    // both paid and free jewels charge against freeJewel: no captured roll is priced in paid
    // jewels, leaving that path unobserved. the free-jewel path is confirmed.
    long long jewels = jint(detail, "free_jewel_amount") + jint(detail, "paid_jewel_amount");
    if (jewels) {
        std::vector<db::json> currencies = db::fetch(db::user::get_currencys(user_id));
        if (currencies.empty() || jint(currencies.front(), "freeJewel") < jewels) return std::nullopt;
        db::execute(db::user::add_currency(user_id, 0, -jewels));
        return std::set<std::string>{"Currency"};
    }
    return std::set<std::string>{};
}

// hand over one prize, returning its ReceivedThing[]. you don't own it -> you get the
// character/poster itself. you own a non-maxed poster -> it breaks through one phase and the
// result reports the phase reached. anything else -> it converts, and the conversion lands in
// received_things instead of the possession.
std::vector<json> _award_prize(long long user_id, const json& gacha, const json& thing) {
    long long thing_type = jint(thing, "thing_type");
    long long tq = jint(thing, "thing_quantity");
    long long quantity = tq ? tq : 1;
    long long thing_id = jint(thing, "thing_id");
    bool owns = false;

    if (thing_type == enums::ThingTypes::Poster) {
        std::optional<db::json> owned;
        for (const db::json& p : db::fetch(db::user::get_posters(user_id)))
            if (jint(p, "posterMasterId") == thing_id) {
                owned = p;
                break;
            }
        long long max_phase = gacha::poster_max_phase(thing_id);
        if (owned && jint(*owned, "breakthroughPhase") < max_phase) {
            db::execute(db::user::breakthrough_poster(user_id, jint(*owned, "id"), max_phase));
            json rt = json::object();
            rt["type"] = thing_type;
            rt["id_"] = thing_id;
            rt["quantity"] = quantity;
            rt["after_phase"] = jint(*owned, "breakthroughPhase") + 1;
            rt["sent_inbox"] = false;
            return {rt};
        }
        owns = owned.has_value();
    } else if (thing_type == enums::ThingTypes::Character) {
        for (const db::json& c : db::fetch(db::user::get_characters(user_id)))
            if (jint(c, "characterMasterId") == thing_id) {
                owns = true;
                break;
            }
    }

    if (owns) {
        std::vector<json> converted =
            things::grant_things_consolidated(user_id, gacha::dupe_conversion(gacha, thing));
        // every converted entry points back at what it replaced, so the client can show the
        // card that was rolled rather than a bare pile of items
        for (json& received : converted) {
            received["original_type"] = thing_type;
            received["original_id"] = thing_id;
        }
        if (!converted.empty()) return converted;
    }

    return {things::grant_thing(user_id, thing_type, thing_id, quantity)};
}

struct Selection {
    std::set<long long> allowed;
    long long min_select;
    long long max_select;
};

// (selectable thing ids, min_select, max_select) for a pickup-selection gacha, or nullopt if
// this gacha has no selection step
std::optional<Selection> _gacha_selection(long long gacha_master_id) {
    static const std::unordered_map<long long, const json*> gacha_master = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& g : master_data::table("GachaMaster")) m[g.at("id_").get<long long>()] = &g;
        return m;
    }();
    static const std::unordered_map<long long, const json*> selection_master = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& p : master_data::table("PickupSelectionGachaMaster"))
            m[p.at("gacha_master_id").get<long long>()] = &p;
        return m;
    }();

    auto sit = selection_master.find(gacha_master_id);
    auto git = gacha_master.find(gacha_master_id);
    if (sit == selection_master.end() || git == gacha_master.end()) return std::nullopt;
    const json& gacha = *git->second;
    const json& sel = *sit->second;

    Selection out;
    auto tit = gacha.find("things");
    if (tit != gacha.end() && tit->is_array())
        for (const json& t : *tit)
            if (jbool(t, "is_selectable")) out.allowed.insert(jint(t, "id_"));
    out.min_select = jint(sel, "min_select_count");
    out.max_select = jint(sel, "max_select_count");
    return out;
}

}  // namespace

void register_gachas(httplib::Server& svr) {
    // /api/Gachas/DecideReRollGacha?gachaDetailMasterId=
    svr.Post("/api/Gachas/DecideReRollGacha", [](const httplib::Request&, httplib::Response& res) {
        pipeline::respond(res, "BooleanResult", json::object());
    });

    // /api/Gachas
    svr.Get("/api/Gachas", [](const httplib::Request&, httplib::Response& res) {
        // roll_left is always the full limit -- nothing attributes spent rolls to a detail yet
        json out = json::array();
        for (long long gacha_master_id : gacha::active_gacha_ids()) {
            std::pair<long long, long long> flags = gacha::emission_flags(gacha_master_id);
            json info = json::object();
            info["id_"] = gacha_master_id;
            info["roll_limits"] = gacha::roll_limits(gacha_master_id, {});
            info["normal_emission_flags"] = flags.first;
            info["fixed_emission_flags"] = flags.second;
            out.push_back(std::move(info));
        }
        pipeline::respond(res, "GachaInfoResult", out);
    });

    // /api/Gachas/CharacterLineup/{gachaMasterId}
    svr.Get("/api/Gachas/CharacterLineup/:gachaMasterId",
            [](const httplib::Request& req, httplib::Response& res) {
                long long gacha_master_id = std::stoll(req.path_params.at("gachaMasterId"));
                std::optional<gacha::Lineup> computed = gacha::lineup(gacha_master_id);
                if (!computed) {
                    pipeline::respond(res, "CharacterLineupResult", json::object());
                    return;
                }
                json result = json::object();
                result["normal_probabilities"] = computed->normal_probabilities;
                result["fixed_probabilities"] = computed->fixed_probabilities;
                result["normal_lineup_items"] = computed->normal_items;
                result["fixed_lineup_items"] = computed->fixed_items;
                pipeline::respond(res, "CharacterLineupResult", result);
            });

    // /api/Gachas/GetGachaHistories?cardType=
    svr.Post("/api/Gachas/GetGachaHistories", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        if (!user_id) {
            pipeline::respond(res, "GachaHistoryResult", json::array());
            return;
        }
        long long card_type = enums::GachaCardTypes::Character;
        if (req.has_param("cardType")) {
            long long v = std::stoll(req.get_param_value("cardType"));
            if (v) card_type = v;
        }
        // one entry per prize pulled, newest first; every prize of a roll shares its timestamp
        json out = json::array();
        for (const db::json& r : db::fetch(db::user::get_gacha_historys(*user_id, card_type))) {
            json h = json::object();
            h["master_id"] = r.at("masterId").get<long long>();
            h["created_at"] = r.at("createdAt").get<long long>();
            out.push_back(std::move(h));
        }
        pipeline::respond(res, "GachaHistoryResult", out);
    });

    // /api/Gachas/PosterLineup/{gachaMasterId}
    svr.Get("/api/Gachas/PosterLineup/:gachaMasterId",
            [](const httplib::Request& req, httplib::Response& res) {
                long long gacha_master_id = std::stoll(req.path_params.at("gachaMasterId"));
                std::optional<gacha::Lineup> computed = gacha::lineup(gacha_master_id);
                if (!computed) {
                    pipeline::respond(res, "PosterLineupResult", json::object());
                    return;
                }
                json result = json::object();
                result["normal_probabilities"] = computed->normal_probabilities;
                result["fixed_probabilities"] = computed->fixed_probabilities;
                result["normal_lineup_items"] = computed->normal_items;
                result["fixed_lineup_items"] = computed->fixed_items;
                pipeline::respond(res, "PosterLineupResult", result);
            });

    // /api/Gachas/GetReRollGachaResults?gachaDetailMasterId=
    svr.Post("/api/Gachas/GetReRollGachaResults", [](const httplib::Request&, httplib::Response& res) {
        pipeline::respond(res, "GachaRollResult", json::object());
    });

    // /api/Gachas/{gachaMasterId}/SelectedThings
    svr.Get("/api/Gachas/:gachaMasterId/SelectedThings",
            [](const httplib::Request& req, httplib::Response& res) {
                long long gacha_master_id = std::stoll(req.path_params.at("gachaMasterId"));
                std::optional<long long> user_id = user_data::current_user_id(req);
                if (!user_id) {
                    pipeline::respond(res, "GachaSelectedThingsResult", json::object());
                    return;
                }
                std::optional<db::json> row;
                for (const db::json& r : db::fetch(db::user::get_gacha_selected_things(*user_id)))
                    if (jint(r, "gachaMasterId") == gacha_master_id) {
                        row = r;
                        break;
                    }
                json chosen = json::array();
                if (row) {
                    auto it = row->find("gachaThingIds");
                    if (it != row->end() && it->is_array())
                        for (const auto& x : *it) chosen.push_back(x.get<long long>());
                }
                json result = json::object();
                result["selected_gacha_thing_ids"] = chosen;
                pipeline::respond(res, "GachaSelectedThingsResult", result);
            });

    // /api/Gachas/ReRollGacha?gachaDetailMasterId=
    svr.Post("/api/Gachas/ReRollGacha", [](const httplib::Request&, httplib::Response& res) {
        pipeline::respond(res, "GachaRollResult", json::object());
    });

    // /api/Gachas/Roll/{gachaDetailMasterId}
    svr.Post("/api/Gachas/Roll/:gachaDetailMasterId", [](const httplib::Request& req, httplib::Response& res) {
        long long gacha_detail_master_id = std::stoll(req.path_params.at("gachaDetailMasterId"));
        std::optional<long long> user_id = user_data::current_user_id(req);
        std::pair<json, json> gd = gacha::detail_of(gacha_detail_master_id);
        const json& gacha = gd.first;
        const json& detail = gd.second;
        if (!user_id || gacha.is_null()) {
            pipeline::respond(res, "GachaRollResult", json::object());
            return;
        }

        long long now = static_cast<long long>(std::time(nullptr)) * 1000000LL;

        std::optional<std::set<std::string>> spent = _spend_cost(*user_id, detail);
        if (!spent) {
            pipeline::respond(res, "GachaRollResult", json::object(),
                              json::array({pipeline::fault("NotEnoughThing")}));
            return;
        }

        std::vector<json> prizes = gacha::roll_prizes(gacha, detail);
        long long card_type = gacha.at("card_type").get<long long>();

        std::vector<json> results;
        for (const json& thing : prizes) {
            std::vector<json> received = _award_prize(*user_id, gacha, thing);
            // a Rare4 character prize also pays out a pile of its own ActorPiece
            std::vector<json> extra =
                things::grant_things_consolidated(*user_id, gacha::piece_bonus(gacha, thing));
            json gtr = json::object();
            gtr["received_things"] = received;
            gtr["additional_received_things"] = extra;
            results.push_back(std::move(gtr));
        }

        std::vector<gacha::ThingTriple> bonus_things;
        auto bit = gacha.find("bonus_things");
        if (bit != gacha.end() && bit->is_array())
            for (const json& b : *bit)
                bonus_things.emplace_back(jint(b, "thing_type"), jint(b, "thing_id"),
                                          jint(b, "thing_quantity") * static_cast<long long>(prizes.size()));
        std::vector<json> bonus = things::grant_things_consolidated(*user_id, bonus_things);

        std::vector<gacha::ThingTriple> detail_bonus_things;
        auto dbit = detail.find("detail_bonus_things");
        if (dbit != detail.end() && dbit->is_array())
            for (const json& b : *dbit)
                detail_bonus_things.emplace_back(jint(b, "thing_type"), jint(b, "thing_id"),
                                                 jint(b, "thing_quantity"));
        std::vector<json> detail_bonus = things::grant_things_consolidated(*user_id, detail_bonus_things);

        db::user::add_gacha_rolls(*user_id, gacha.at("id_").get<long long>(),
                                  static_cast<long long>(prizes.size()), _random_gacha_id());
        if (!prizes.empty()) {
            std::vector<long long> master_ids;
            for (const json& t : prizes) master_ids.push_back(jint(t, "thing_id"));
            db::execute(db::user::add_gacha_historys(*user_id, card_type, master_ids, now));
        }

        // what the roll paid out, plus what it charged. read the types off the awarded things,
        // not the prize things: a converted dupe writes items, not the card it was rolled from.
        std::set<std::string> refresh(spent->begin(), spent->end());
        auto add_type = [&refresh](const json& r) {
            std::optional<std::string> pt = things::present_type(r.at("type").get<long long>());
            if (pt) refresh.insert(*pt);
        };
        for (const json& r : bonus) add_type(r);
        for (const json& r : detail_bonus) add_type(r);
        for (const json& gtr : results) {
            for (const json& r : gtr.at("received_things")) add_type(r);
            for (const json& r : gtr.at("additional_received_things")) add_type(r);
        }

        json present = json::array();
        if (!refresh.empty()) {
            std::vector<user_data::PresentSpec> specs;
            for (const std::string& name : refresh) specs.emplace_back(name);
            present = user_data::build_present(user_id, specs);
        }

        json result = json::object();
        // echoes the DETAIL id back, not the gacha's -- confirmed by two captures
        result["m_gacha_master_id"] = gacha_detail_master_id;
        result["received_things"] = results;
        result["received_bonus_things"] = bonus;
        result["received_detail_bonus_things"] = detail_bonus;
        // roll_bonuses (milestone rewards) not implemented; every captured banner has an empty list
        result["received_roll_bonus_things"] = json::array();
        pipeline::respond(res, "GachaRollResult", result, json::array(), present);
    });

    // /api/Gachas/{gachaMasterId}/SetSelectedThings
    svr.Post("/api/Gachas/:gachaMasterId/SetSelectedThings",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long gacha_master_id = std::stoll(req.path_params.at("gachaMasterId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = wire::read_request(req.body, "SetSelectedThingsPayload");
                 if (!user_id || payload.is_null()) {
                     pipeline::respond(res, "GachaSelectedThingsResult", json::object());
                     return;
                 }

                 std::optional<Selection> selection = _gacha_selection(gacha_master_id);
                 if (!selection) {
                     pipeline::respond(res, "GachaSelectedThingsResult", json::object());
                     return;  // gacha has no selection step
                 }

                 // keep the client's order, drop duplicates and anything not offered as selectable
                 std::vector<long long> chosen;
                 std::set<long long> seen;
                 auto it = payload.find("gacha_thing_ids");
                 if (it != payload.end() && it->is_array())
                     for (const auto& x : *it) {
                         long long i = x.get<long long>();
                         if (seen.count(i)) continue;
                         seen.insert(i);
                         if (selection->allowed.count(i)) chosen.push_back(i);
                     }
                 long long count = static_cast<long long>(chosen.size());
                 if (!(selection->min_select <= count && count <= selection->max_select)) {
                     pipeline::respond(res, "GachaSelectedThingsResult", json::object());
                     return;
                 }

                 db::user::set_gacha_selected_things(*user_id, gacha_master_id, chosen);

                 json present = user_data::build_present(user_id, {{"GachaSelectedThings"}});
                 json result = json::object();
                 result["selected_gacha_thing_ids"] = chosen;
                 pipeline::respond(res, "GachaSelectedThingsResult", result, json::array(), present);
             });
}

}  // namespace routes
