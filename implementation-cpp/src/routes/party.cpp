#include "routes/party.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "headers.h"
#include "helpers/user_data.h"
#include "wire.h"

// ports routes/party.py. EditParty/EditPosition/SetLeader/ChangeName/SetMultiParty carry real
// logic (diff the stored slots, write only what moved); the triple-cast endpoints are stubs.

namespace routes {
namespace {

using wire::json;

long long jint(const json& obj, const char* key, long long def) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->get<long long>();
}

// nullable field -> the value if present, else json null (python Optional[int] = None).
json jopt(const json& obj, const char* key) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return json(nullptr);
    return *it;
}

const json& opt_array(const json& obj, const char* key) {
    static const json kEmpty = json::array();
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null() || !it->is_array()) return kEmpty;
    return *it;
}

void respond(httplib::Response& res, const char* result_name, const json& result,
             const json& present = json::array()) {
    res.set_content(wire::common_response(result_name, result, json::array(), present),
                    "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

// Write an EditParty-shaped loadout onto the caller's slots. The client submits the party's full
// slot list every time, so diff against what is stored -- only the slots that actually moved are
// written, and only those go in present.
json _apply_slot_edits(long long user_id, const json& payload) {
    std::unordered_map<long long, json> owned;
    for (const db::json& s : db::fetch(db::user::get_party_slots(user_id)))
        owned.insert_or_assign(s.at("id").get<long long>(), json(s));
    std::vector<json> changed;
    for (const json& edit : opt_array(payload, "party_slots")) {
        auto it = owned.find(jint(edit, "u_party_slot_id", 0));
        if (it == owned.end()) continue;  // not one of this user's slots -- ignore, never create
        json updated = it->second;
        updated["characterId"] = jint(edit, "u_character_id", 0);
        // NB the payload orders these accessory-then-poster, the reverse of the PartySlot entity
        // -- confirmed against a capture, don't "fix" it
        updated["posterId"] = jopt(edit, "u_poster_id");
        updated["accessoryId"] = jopt(edit, "u_accessory_id");
        updated["bonusAbilityEnableFlags"] = jint(edit, "bonus_ability_enable_flags", 0);
        if (updated != it->second) changed.push_back(std::move(updated));
    }
    if (changed.empty()) return json::array();
    db::execute(db::user::update_party_slots(user_id, changed));
    json present = json::array();
    for (const json& s : changed) present.push_back(user_data::data_object("PartySlot", s));
    return present;
}

// Move a party's leader marker; returns the present entries for what changed.
json _set_leader(long long user_id, long long party_id, long long position) {
    std::optional<json> party;
    for (const db::json& p : db::fetch(db::user::get_partys(user_id))) {
        if (p.at("id").get<long long>() == party_id) {
            party = json(p);
            break;
        }
    }
    if (!party || party->at("leaderPosition").get<long long>() == position) return json::array();
    db::execute(db::user::update_party_leader(user_id, party_id, position));
    (*party)["leaderPosition"] = position;
    json present = json::array();
    present.push_back(user_data::data_object("Party", *party));
    return present;
}

}  // namespace

void register_party(httplib::Server& svr) {
    // /api/Parties/{uPartyId}/ChangeName
    svr.Post("/api/Parties/:uPartyId/ChangeName",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long u_party_id = std::stoll(req.path_params.at("uPartyId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = wire::read_request(req.body, "ChangeNamePayload");
                 if (!user_id || payload.is_null() || !payload.contains("name") ||
                     payload.at("name").is_null()) {
                     respond(res, "BooleanResult", json::object());
                     return;
                 }

                 json present = json::array();
                 std::optional<json> party;
                 for (const db::json& p : db::fetch(db::user::get_partys(*user_id))) {
                     if (p.at("id").get<long long>() == u_party_id) {
                         party = json(p);
                         break;
                     }
                 }
                 if (!party) {
                     respond(res, "BooleanResult", json::object());  // not the caller's party
                     return;
                 }
                 if (party->at("name") != payload.at("name")) {
                     db::execute(db::user::update_party_name(*user_id, u_party_id,
                                                             payload.at("name").get<std::string>()));
                     (*party)["name"] = payload.at("name");
                     present = json::array();
                     present.push_back(user_data::data_object("Party", *party));
                 }
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Parties/ChangeTripleCastPartyName?partyOrder=&newName=
    svr.Post("/api/Parties/ChangeTripleCastPartyName",
             [](const httplib::Request&, httplib::Response& res) {
                 respond(res, "BooleanResult", json::object());
             });

    // /api/Parties/{srcOrder}/CopyTo/{destOrder}
    svr.Post("/api/Parties/:srcOrder/CopyTo/:destOrder",
             [](const httplib::Request&, httplib::Response& res) {
                 respond(res, "BooleanResult", json::object());
             });

    // /api/Parties/{uPartyId}/EditParty
    svr.Post("/api/Parties/:uPartyId/EditParty",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long u_party_id = std::stoll(req.path_params.at("uPartyId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = wire::read_request(req.body, "EditPartyPayload");
                 if (!user_id || payload.is_null()) {
                     respond(res, "BooleanResult", json::object());
                     return;
                 }

                 json present = _apply_slot_edits(*user_id, payload);
                 // leader_position rides along on the same payload (null = leave it alone) and
                 // lives on the party row, not the slots
                 if (payload.contains("leader_position") && !payload.at("leader_position").is_null()) {
                     json leader =
                         _set_leader(*user_id, u_party_id, payload.at("leader_position").get<long long>());
                     for (json& e : leader) present.push_back(std::move(e));
                 }
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Parties/{uPartyId}/SetLeader/{leaderPosition}
    svr.Post("/api/Parties/:uPartyId/SetLeader/:leaderPosition",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long u_party_id = std::stoll(req.path_params.at("uPartyId"));
                 long long leader_position = std::stoll(req.path_params.at("leaderPosition"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id) {
                     respond(res, "BooleanResult", json::object());
                     return;
                 }

                 json present = _set_leader(*user_id, u_party_id, leader_position);
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Parties/{uPartyId}/EditPosition
    svr.Post("/api/Parties/:uPartyId/EditPosition",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = wire::read_request(req.body, "EditPositionPayload");
                 if (!user_id || payload.is_null()) {
                     respond(res, "BooleanResult", json::object());
                     return;
                 }

                 json present = json::array();
                 std::unordered_map<long long, json> owned;
                 for (const db::json& s : db::fetch(db::user::get_party_slots(*user_id)))
                     owned.insert_or_assign(s.at("id").get<long long>(), json(s));
                 std::vector<json> changed;
                 std::vector<std::pair<std::int64_t, std::int64_t>> positions;
                 for (const json& edit : opt_array(payload, "slots")) {
                     auto it = owned.find(jint(edit, "u_party_slot_id", 0));
                     long long position = jint(edit, "position", 0);
                     if (it == owned.end() || it->second.at("position").get<long long>() == position)
                         continue;
                     json slot = it->second;
                     slot["position"] = position;
                     changed.push_back(std::move(slot));
                 }
                 if (!changed.empty()) {
                     // a reorder is a permutation, so the whole set moves at once -- one statement,
                     // no intermediate state where two slots share a position
                     for (const json& s : changed)
                         positions.emplace_back(s.at("id").get<long long>(),
                                                s.at("position").get<long long>());
                     db::execute(db::user::update_party_slot_positions(*user_id, positions));
                     present = json::array();
                     for (const json& s : changed)
                         present.push_back(user_data::data_object("PartySlot", s));
                 }
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Parties/{uPartyId}/EditPartyWithRecommended
    svr.Post("/api/Parties/:uPartyId/EditPartyWithRecommended",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long u_party_id = std::stoll(req.path_params.at("uPartyId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json payload = wire::read_request(req.body, "EditPartyPayload");
                 if (!user_id || payload.is_null()) {
                     respond(res, "BooleanResult", json::object());
                     return;
                 }

                 // the client picks the recommended members itself and submits them in the same
                 // shape as a manual edit, so this is the same write -- the "recommend" part is
                 // entirely client-side
                 json present = _apply_slot_edits(*user_id, payload);
                 if (payload.contains("leader_position") && !payload.at("leader_position").is_null()) {
                     json leader =
                         _set_leader(*user_id, u_party_id, payload.at("leader_position").get<long long>());
                     for (json& e : leader) present.push_back(std::move(e));
                 }
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Parties/EditTripleCastBasic?partyOrder=&tripleCastGroupOrder=
    svr.Post("/api/Parties/EditTripleCastBasic",
             [](const httplib::Request&, httplib::Response& res) {
                 respond(res, "BooleanResult", json::object());
             });

    // /api/Parties/EditTripleCastParty?partyOrder=
    svr.Post("/api/Parties/EditTripleCastParty",
             [](const httplib::Request& req, httplib::Response& res) {
                 wire::read_request(req.body, "EditPartyPayload");  // payload unused
                 respond(res, "BooleanResult", json::object());
             });

    // /api/Parties/EditTripleCastPartySlotPosition?partyOrder=
    svr.Post("/api/Parties/EditTripleCastPartySlotPosition",
             [](const httplib::Request& req, httplib::Response& res) {
                 wire::read_request(req.body, "EditPositionPayload");  // payload unused
                 respond(res, "BooleanResult", json::object());
             });

    // /api/Parties/{uPartyId}/SetMultiParty
    svr.Post("/api/Parties/:uPartyId/SetMultiParty",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long u_party_id = std::stoll(req.path_params.at("uPartyId"));
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 if (!user_id) {
                     respond(res, "BooleanResult", json::object());
                     return;
                 }

                 // "which of my parties do I bring to multi-live" is a user preference, not party state
                 json present = json::array();
                 std::vector<db::json> prefs = db::fetch(db::user::get_user_preferences(*user_id));
                 if (!prefs.empty()) {
                     json pref = json(prefs.front());
                     if (pref.at("multiPartyId").get<long long>() != u_party_id) {
                         db::execute(db::user::update_multi_party(*user_id, u_party_id));
                         pref["multiPartyId"] = u_party_id;
                         present.push_back(user_data::data_object("UserPreference", pref));
                     }
                 }
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Parties/SetTripleCastPartyLeaderPosition?partyOrder=&leaderPosition=
    svr.Post("/api/Parties/SetTripleCastPartyLeaderPosition",
             [](const httplib::Request&, httplib::Response& res) {
                 respond(res, "BooleanResult", json::object());
             });
}

}  // namespace routes
