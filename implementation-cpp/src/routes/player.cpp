#include "routes/player.h"

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "headers.h"
#include "helpers/game_state.h"
#include "helpers/tutorial.h"
#include "helpers/user_data.h"
#include "wire.h"

// ports routes/player.py

namespace routes {
namespace {

using wire::json;              // wire-facing results + present; also master rows (ordered_json)
using rjson = nlohmann::json;  // db rows + game_state where/values (camelCase columns)

void send(httplib::Response& res, const std::string& body) {
    res.set_content(body, "application/vnd.msgpack");
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

// respond(result, present=...) -> common envelope
void respond(httplib::Response& res, const char* result_name, const json& result,
             const json& present = json::array()) {
    send(res, wire::common_response(result_name, result, json::array(), present));
}

// python truthiness for the int|bool|None values carried in game_state value dicts
bool truthy(const rjson& v) {
    if (v.is_null()) return false;
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number()) return v.get<long long>() != 0;
    return !v.empty();
}

// now as epoch micros, like the other User timestamps
long long now_micros() {
    return static_cast<long long>(std::chrono::duration_cast<std::chrono::microseconds>(
                                      std::chrono::system_clock::now().time_since_epoch())
                                      .count());
}

}  // namespace

void register_player(httplib::Server& svr) {
    // /api/Player/RecoverStaminaByJewel?times=
    svr.Post("/api/Player/RecoverStaminaByJewel", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> times =
            req.has_param("times") ? std::optional<long long>(std::stoll(req.get_param_value("times")))
                                   : std::nullopt;
        (void)times;
        // TODO spend `times` jewels, then recover stamina (overfill allowed, so no clamp):
        //   adjust_and_check_stamina(user_id, +recover_amount, player_rank)
        respond(res, "BooleanResult", json::object());
    });

    // /api/Player/ReleasePlayerRankCap
    svr.Post("/api/Player/ReleasePlayerRankCap", [](const httplib::Request&, httplib::Response& res) {
        respond(res, "BooleanResult", json::object());
    });

    // /api/Player/SetHomeBGM?mHomeBGMId=&selectionType=&mHomeBGMDetailId=
    svr.Post("/api/Player/SetHomeBGM", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> m_home_bgm_id =
            req.has_param("mHomeBGMId")
                ? std::optional<long long>(std::stoll(req.get_param_value("mHomeBGMId")))
                : std::nullopt;
        std::optional<long long> selection_type =
            req.has_param("selectionType")
                ? std::optional<long long>(std::stoll(req.get_param_value("selectionType")))
                : std::nullopt;
        std::optional<long long> m_home_bgm_detail_id =
            req.has_param("mHomeBGMDetailId")
                ? std::optional<long long>(std::stoll(req.get_param_value("mHomeBGMDetailId")))
                : std::nullopt;
        // `mHomeBGMDetailId or None`: 0 / absent -> no detail
        std::optional<long long> detail_id =
            (m_home_bgm_detail_id && *m_home_bgm_detail_id) ? m_home_bgm_detail_id : std::nullopt;
        try {
            if (!m_home_bgm_id || !selection_type) throw game_state::Rejected();
            const json* m = game_state::master("home_b_g_m_master", *m_home_bgm_id);
            const json* detail =
                detail_id ? game_state::master("home_b_g_m_detail_master", *detail_id) : nullptr;
            if (!m || !(*selection_type == 0 || *selection_type == 1 || *selection_type == 2) ||
                (detail_id &&
                 (!detail || detail->at("home_bgm_master_id").get<long long>() != *m_home_bgm_id)))
                throw game_state::Rejected();
            game_state::State s = game_state::transaction(req);
            rjson values = {{"selectionType", *selection_type},
                            {"homeBGMDetailMasterId", detail_id ? rjson(*detail_id) : rjson(nullptr)}};
            rjson* row = s.one("HomeBGM", rjson{{"homeBGMMasterId", *m_home_bgm_id}});
            if (row) {
                s.update("HomeBGM", row, values);
            } else {
                rjson ins = values;
                ins["homeBGMMasterId"] = *m_home_bgm_id;
                s.insert("HomeBGM", ins);
            }
            s.commit();
            json result;
            result["is_success"] = true;
            respond(res, "BooleanResult", result, s.present());
        } catch (const game_state::Rejected&) {
            respond(res, "BooleanResult", json::object());
        }
    });

    // /api/Player/UpdateCapedPlayerRankAnnounce
    svr.Post("/api/Player/UpdateCapedPlayerRankAnnounce",
             [](const httplib::Request&, httplib::Response& res) {
                 respond(res, "BooleanResult", json::object());
             });

    // /api/Player/UpdateGameHintRead
    svr.Post("/api/Player/UpdateGameHintRead", [](const httplib::Request& req, httplib::Response& res) {
        std::optional<long long> user_id = user_data::current_user_id(req);
        json payload = wire::read_request(req.body, "UpdateGameHintPayload");
        std::vector<long long> categories;
        if (!payload.is_null() && payload.contains("categories") && !payload["categories"].is_null())
            for (const json& c : payload["categories"]) categories.push_back(c.get<long long>());
        json present = json::array();
        if (user_id && !categories.empty()) {
            for (long long category : categories) db::user::mark_game_hint_read(*user_id, category);
            // echo the updated GameHint rows (id == pageCategory) so the client's IGameHint
            // list marks them read -- an empty marker leaves IsGameHintRead false and loops
            present = user_data::build_present(user_id, {{"GameHint", categories}});
        }
        json result;
        result["is_success"] = true;
        respond(res, "BooleanResult", result, present);
    });

    // /api/Player/UpdateHomeDisplayPreference
    svr.Post("/api/Player/UpdateHomeDisplayPreference",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     json payload = wire::read_request(req.body, "UpdateHomeDisplayPreferencePayload");
                     if (payload.is_null()) throw game_state::Rejected();
                     // {_camel(k): v for k, v in payload.model_dump().items()} with the pydantic
                     // defaults for fields the client omitted (wire read_request drops those)
                     auto opt_int = [&](const char* key) -> rjson {
                         auto it = payload.find(key);
                         if (it == payload.end() || it->is_null()) return rjson(nullptr);
                         return rjson(it->get<long long>());
                     };
                     auto int_or = [&](const char* key, long long dflt) -> long long {
                         auto it = payload.find(key);
                         if (it == payload.end() || it->is_null()) return dflt;
                         return it->get<long long>();
                     };
                     auto bool_or = [&](const char* key, bool dflt) -> bool {
                         auto it = payload.find(key);
                         if (it == payload.end() || it->is_null()) return dflt;
                         return it->get<bool>();
                     };
                     rjson values;
                     values["homeCharacterBaseMasterId"] = opt_int("home_character_base_master_id");
                     values["memberCharacterBaseMasterId"] = opt_int("member_character_base_master_id");
                     values["storyCharacterBaseMasterId"] = opt_int("story_character_base_master_id");
                     values["shopCharacterBaseMasterId"] = opt_int("shop_character_base_master_id");
                     values["homeCostumeMasterId"] = opt_int("home_costume_master_id");
                     values["memberCostumeMasterId"] = opt_int("member_costume_master_id");
                     values["storyCostumeMasterId"] = opt_int("story_costume_master_id");
                     values["shopCostumeMasterId"] = opt_int("shop_costume_master_id");
                     values["illustCharacterMasterId"] = int_or("illust_character_master_id", 0);
                     values["displayAwakeningStatus"] = bool_or("display_awakening_status", false);
                     values["homeCharacterDisplayType"] =
                         int_or("home_character_display_type", enums::HomeCharacterDisplayTypes::CharacterModel);
                     values["loginBonusCharacterBaseMasterId"] =
                         opt_int("login_bonus_character_base_master_id");
                     // the client's login-bonus field is a spine costume (a different master
                     // namespace); the row stores it as the plain loginBonusCostumeMasterId column
                     values["loginBonusCostumeMasterId"] = opt_int("login_bonus_spine_costume_master_id");

                     game_state::State s = game_state::transaction(req);
                     for (const std::string& prefix : {std::string("home"), std::string("member"),
                                                       std::string("story"), std::string("shop"),
                                                       std::string("loginBonus")}) {
                         const rjson& base = values.at(prefix + "CharacterBaseMasterId");
                         const rjson& costume = values.at(prefix + "CostumeMasterId");
                         if (truthy(base) &&
                             s.one("CharacterBase", rjson{{"characterBaseMasterId", base}}) == nullptr)
                             throw game_state::Rejected();
                         if (truthy(base) && truthy(costume) && prefix != "loginBonus")
                             game_state::costume_owned(s, base.get<long long>(), costume.get<long long>());
                     }
                     if (truthy(values.at("illustCharacterMasterId")) &&
                         s.one("Character",
                               rjson{{"characterMasterId", values.at("illustCharacterMasterId")}}) == nullptr)
                         throw game_state::Rejected();
                     rjson* row = s.one("HomeDisplayPreference");
                     if (row)
                         s.update("HomeDisplayPreference", row, values);
                     else
                         s.insert("HomeDisplayPreference", values);
                     s.commit();
                     json result;
                     result["is_success"] = true;
                     respond(res, "BooleanResult", result, s.present());
                 } catch (const game_state::Rejected&) {
                     respond(res, "BooleanResult", json::object());
                 }
             });

    // /api/Player/UpdateSplashLastDisplayTime
    svr.Post("/api/Player/UpdateSplashLastDisplayTime",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 json present = json::array();
                 if (user_id) {
                     long long now = now_micros();
                     db::execute(db::user::update_user_splash_last_displayed_at(*user_id, now));
                     std::optional<json> user = db::fetchrow(db::user::get_users(*user_id));
                     if (user) present.push_back(user_data::data_object("User", *user));
                 }
                 json result;
                 result["is_success"] = true;
                 respond(res, "BooleanResult", result, present);
             });

    // /api/Player/UpdateTutorial
    svr.Post("/api/Player/UpdateTutorial", [](const httplib::Request& req, httplib::Response& res) {
        json payload = wire::read_request(req.body, "UpdateTutorialPayload");
        long long target =
            (!payload.is_null() && payload.contains("tutorial_status") && !payload["tutorial_status"].is_null())
                ? payload["tutorial_status"].get<long long>()
                : enums::TutorialStatus::Start;
        std::optional<long long> user_id = user_data::current_user_id(req);
        std::optional<json> user;
        if (user_id) user = db::fetchrow(db::user::get_users(*user_id));
        if (!user || !tutorial::can_advance(user->at("tutorialStatus").get<long long>(), target)) {
            // unknown user, or a backtrack: leave the tutorial where it is
            json present = json::array();
            if (user) present.push_back(user_data::data_object("User", *user));
            json result;
            result["is_success"] = false;
            respond(res, "BooleanResult", result, present);
            return;
        }
        db::execute(db::user::update_user_tutorial_status(*user_id, target));
        user = db::fetchrow(db::user::get_users(*user_id));
        json result;
        result["is_success"] = true;
        json present = json::array();
        present.push_back(user_data::data_object("User", *user));
        respond(res, "BooleanResult", result, present);
    });
}

}  // namespace routes
