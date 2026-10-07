#include "routes/profile.h"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "helpers/game_state.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/profile.py.

namespace routes {
namespace {

using ojson = nlohmann::ordered_json;  // wire payload + results
using rjson = nlohmann::json;          // db rows (camelCase columns), update values

// ascii str.title(): capitalize the first letter of each alpha run, lowercase the rest
std::string title(const std::string& s) {
    std::string out = s;
    bool prev_alpha = false;
    for (char& c : out) {
        bool alpha = std::isalpha(static_cast<unsigned char>(c)) != 0;
        if (alpha && !prev_alpha)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        else if (alpha)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        prev_alpha = alpha;
    }
    return out;
}

std::string camel(const std::string& name) {
    std::vector<std::string> parts;
    std::string cur;
    for (char c : name) {
        if (c == '_') {
            parts.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    parts.push_back(cur);
    std::string out = parts[0];
    for (std::size_t i = 1; i < parts.size(); ++i) out += title(parts[i]);
    return out;
}

// python len(str): count unicode code points (utf-8 non-continuation bytes)
std::size_t utf8_len(const std::string& s) {
    std::size_t n = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80) ++n;
    return n;
}

// a scalar payload field -> a db-row json value, preserving its type
rjson copy_scalar(const ojson& v) {
    if (v.is_string()) return v.get<std::string>();
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number()) return v.get<int64_t>();
    return rjson(nullptr);
}

}  // namespace

void register_profile(httplib::Server& svr) {
    // /api/Profiles/Edit
    svr.Post("/api/Profiles/Edit", [](const httplib::Request& req, httplib::Response& res) {
        try {
            ojson p = wire::read_request(req.body, "EditUserProfilePayload");
            if (p.is_null()) throw game_state::Rejected();
            // EditUserProfilePayload fields in model order with their pydantic defaults
            // (model_dump yields every field; missing/nil -> the default)
            static const std::vector<std::pair<const char*, rjson>> FIELDS = {
                {"name", rjson(nullptr)},
                {"introduction", rjson(nullptr)},
                {"main_u_character_id", rjson(static_cast<int64_t>(0))},
                {"m_nameplate_id", rjson(nullptr)},
                {"m_name_color_id", rjson(static_cast<int64_t>(0))},
                {"m_trophy_id1", rjson(nullptr)},
                {"m_trophy_id2", rjson(nullptr)},
                {"m_trophy_id3", rjson(nullptr)},
                {"is_public_player_rate", rjson(false)},
                {"display_awakening_status", rjson(false)},
                {"main_character_master_id", rjson(static_cast<int64_t>(0))},
                {"name_base_color_masterid", rjson(static_cast<int64_t>(0))},
                {"icon_frame_master_id", rjson(static_cast<int64_t>(0))},
                {"home_skin_master_id", rjson(static_cast<int64_t>(0))},
            };
            rjson values = rjson::object();
            for (const auto& f : FIELDS) {
                auto it = p.find(f.first);
                values[camel(f.first)] =
                    (it != p.end() && !it->is_null()) ? copy_scalar(*it) : f.second;
            }
            values["nameBaseColorMasterId"] = values["nameBaseColorMasterid"];
            values.erase("nameBaseColorMasterid");

            game_state::State s = game_state::transaction(req);
            rjson* row = s.one("UserProfile");
            if (row == nullptr) throw game_state::Rejected();
            int64_t main_u = values.at("mainUCharacterId").get<int64_t>();
            int64_t main_cmid = values.at("mainCharacterMasterId").get<int64_t>();
            rjson* actor = main_u ? s.one("Character", rjson{{"id", main_u}})
                                  : s.one("Character", rjson{{"characterMasterId", main_cmid}});
            if (actor == nullptr || actor->at("characterMasterId").get<int64_t>() != main_cmid)
                throw game_state::Rejected();
            if (!values.at("name").is_null() &&
                utf8_len(values.at("name").get<std::string>()) > 100)
                throw game_state::Rejected();
            if (!values.at("introduction").is_null() &&
                utf8_len(values.at("introduction").get<std::string>()) > 1000)
                throw game_state::Rejected();
            if (values.at("introduction") != row->at("introduction"))
                game_state::mission_progress(s, 28, 1, true);
            if (values.at("mTrophyId1") != row->at("mTrophyId1") ||
                values.at("mTrophyId2") != row->at("mTrophyId2") ||
                values.at("mTrophyId3") != row->at("mTrophyId3"))
                game_state::mission_progress(s, 27, 1, true);
            s.update("UserProfile", row, values);
            s.commit();
            pipeline::respond(res, "BooleanResult", ojson{{"is_success", true}}, ojson::array(),
                              s.present());
        } catch (const game_state::Rejected&) {
            pipeline::respond(res, "BooleanResult", ojson::object());
        }
    });
}

}  // namespace routes
