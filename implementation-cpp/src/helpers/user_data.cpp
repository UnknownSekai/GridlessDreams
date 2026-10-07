#include "httplib_config.h"  // first: sets _WIN32_WINNT before anything else can

#include "helpers/user_data.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "auth.h"
#include "db.h"
#include "db/user.h"
#include "generated/wire_keys_generated.h"
#include "unions.h"

// Ports helpers/user_data.py. The async app/conn params are dropped (one global db).
// iso_to_timestamp / _conv's enum+DateTime branches live in the wire layer now: _to_array
// only remaps a camelCase DB row onto the entity's field names (fn) and recurses into nested
// model columns; wire::to_wire does the [Key(n)] positional encoding, zero-fill, enum->int,
// DateTime micros->Timestamp at pack time.

namespace user_data {

namespace {

using GetterFn = db::SelectQuery (*)(std::int64_t);

// one resolved registry row: entity type, its union key, and the db.user getter.
struct RegEntry {
    std::string name;
    int key;
    GetterFn get;
};

// camelCase column -> its FieldSpec, per entity type.
using CamelMap = std::unordered_map<std::string, const wire::FieldSpec*>;

// getattr(db_user, "get_<table>s"): every single-arg db.user getter by name.
const std::unordered_map<std::string, GetterFn>& getter_map() {
    static const std::unordered_map<std::string, GetterFn> m = {
        {"get_accessory_auto_sells", &db::user::get_accessory_auto_sells},
        {"get_accessory_effect_masters", &db::user::get_accessory_effect_masters},
        {"get_accessory_level_pattern_group_masters", &db::user::get_accessory_level_pattern_group_masters},
        {"get_accessory_level_pattern_masters", &db::user::get_accessory_level_pattern_masters},
        {"get_accessory_masters", &db::user::get_accessory_masters},
        {"get_accessorys", &db::user::get_accessorys},
        {"get_active_live", &db::user::get_active_live},
        {"get_album_pages", &db::user::get_album_pages},
        {"get_album_presets", &db::user::get_album_presets},
        {"get_album_themes", &db::user::get_album_themes},
        {"get_albums", &db::user::get_albums},
        {"get_another_notations", &db::user::get_another_notations},
        {"get_audition_clears", &db::user::get_audition_clears},
        {"get_audition_masters", &db::user::get_audition_masters},
        {"get_audition_phase_masters", &db::user::get_audition_phase_masters},
        {"get_audition_reward_package_masters", &db::user::get_audition_reward_package_masters},
        {"get_bomb_masters", &db::user::get_bomb_masters},
        {"get_bombs", &db::user::get_bombs},
        {"get_bonus_live_stages", &db::user::get_bonus_live_stages},
        {"get_bonus_lives", &db::user::get_bonus_lives},
        {"get_buff_item_statuss", &db::user::get_buff_item_statuss},
        {"get_campaign_masters", &db::user::get_campaign_masters},
        {"get_character_awakening_item_masters", &db::user::get_character_awakening_item_masters},
        {"get_character_base_masters", &db::user::get_character_base_masters},
        {"get_character_bases", &db::user::get_character_bases},
        {"get_character_bloom_bonus_group_masters", &db::user::get_character_bloom_bonus_group_masters},
        {"get_character_bloom_item_masters", &db::user::get_character_bloom_item_masters},
        {"get_character_experience_item_masters", &db::user::get_character_experience_item_masters},
        {"get_character_lesson_slots", &db::user::get_character_lesson_slots},
        {"get_character_lessons", &db::user::get_character_lessons},
        {"get_character_level_masters", &db::user::get_character_level_masters},
        {"get_character_masters", &db::user::get_character_masters},
        {"get_character_mission_masters", &db::user::get_character_mission_masters},
        {"get_character_mission_stage_masters", &db::user::get_character_mission_stage_masters},
        {"get_character_missions", &db::user::get_character_missions},
        {"get_character_piece_masters", &db::user::get_character_piece_masters},
        {"get_character_point_events", &db::user::get_character_point_events},
        {"get_character_sense_enhance_item_group_masters", &db::user::get_character_sense_enhance_item_group_masters},
        {"get_character_star_rank_masters", &db::user::get_character_star_rank_masters},
        {"get_character_star_rank_reward_group_masters", &db::user::get_character_star_rank_reward_group_masters},
        {"get_characters", &db::user::get_characters},
        {"get_circle_event_missions", &db::user::get_circle_event_missions},
        {"get_circle_supports", &db::user::get_circle_supports},
        {"get_comeback_campaigns", &db::user::get_comeback_campaigns},
        {"get_comics", &db::user::get_comics},
        {"get_company_masters", &db::user::get_company_masters},
        {"get_concert_stages", &db::user::get_concert_stages},
        {"get_connect_with_accounts", &db::user::get_connect_with_accounts},
        {"get_connect_with_passwords", &db::user::get_connect_with_passwords},
        {"get_costume_masters", &db::user::get_costume_masters},
        {"get_costume_wearable_character_group_masters", &db::user::get_costume_wearable_character_group_masters},
        {"get_costumes", &db::user::get_costumes},
        {"get_currencys", &db::user::get_currencys},
        {"get_daily_lessons", &db::user::get_daily_lessons},
        {"get_daily_limits", &db::user::get_daily_limits},
        {"get_decorations", &db::user::get_decorations},
        {"get_dugong_runs", &db::user::get_dugong_runs},
        {"get_effect_duration_group_masters", &db::user::get_effect_duration_group_masters},
        {"get_effect_masters", &db::user::get_effect_masters},
        {"get_episode_masters", &db::user::get_episode_masters},
        {"get_episode_reward_package_masters", &db::user::get_episode_reward_package_masters},
        {"get_episodes", &db::user::get_episodes},
        {"get_event_box_gacha_box_things", &db::user::get_event_box_gacha_box_things},
        {"get_event_box_gachas", &db::user::get_event_box_gachas},
        {"get_event_camps", &db::user::get_event_camps},
        {"get_events", &db::user::get_events},
        {"get_exchange_limits", &db::user::get_exchange_limits},
        {"get_exchange_shop_masters", &db::user::get_exchange_shop_masters},
        {"get_favorite_costumes", &db::user::get_favorite_costumes},
        {"get_flash_sale_stages", &db::user::get_flash_sale_stages},
        {"get_friend_invitation_missions", &db::user::get_friend_invitation_missions},
        {"get_friend_invitations", &db::user::get_friend_invitations},
        {"get_gacha_re_rolls", &db::user::get_gacha_re_rolls},
        {"get_gacha_selected_things", &db::user::get_gacha_selected_things},
        {"get_gachas", &db::user::get_gachas},
        {"get_game_hints", &db::user::get_game_hints},
        {"get_gradual_mission_groups", &db::user::get_gradual_mission_groups},
        {"get_home_b_g_ms", &db::user::get_home_b_g_ms},
        {"get_home_character_voice_masters", &db::user::get_home_character_voice_masters},
        {"get_home_display_preferences", &db::user::get_home_display_preferences},
        {"get_home_skins", &db::user::get_home_skins},
        {"get_icon_frames", &db::user::get_icon_frames},
        {"get_inboxs", &db::user::get_inboxs},
        {"get_item_masters", &db::user::get_item_masters},
        {"get_items", &db::user::get_items},
        {"get_jewel_shops", &db::user::get_jewel_shops},
        {"get_league_basics", &db::user::get_league_basics},
        {"get_league_group_members", &db::user::get_league_group_members},
        {"get_league_groups", &db::user::get_league_groups},
        {"get_league_high_score_party_slots", &db::user::get_league_high_score_party_slots},
        {"get_league_high_score_partys", &db::user::get_league_high_score_partys},
        {"get_league_historys", &db::user::get_league_historys},
        {"get_league_season_results", &db::user::get_league_season_results},
        {"get_limits", &db::user::get_limits},
        {"get_link_characters", &db::user::get_link_characters},
        {"get_live_achievements", &db::user::get_live_achievements},
        {"get_live_drop_cellings", &db::user::get_live_drop_cellings},
        {"get_live_drop_limits", &db::user::get_live_drop_limits},
        {"get_live_masters", &db::user::get_live_masters},
        {"get_live_setting_masters", &db::user::get_live_setting_masters},
        {"get_lives", &db::user::get_lives},
        {"get_login_pass_statuss", &db::user::get_login_pass_statuss},
        {"get_lotterys", &db::user::get_lotterys},
        {"get_market_things", &db::user::get_market_things},
        {"get_markets", &db::user::get_markets},
        {"get_mission_masters", &db::user::get_mission_masters},
        {"get_mission_pass_detail_masters", &db::user::get_mission_pass_detail_masters},
        {"get_mission_pass_masters", &db::user::get_mission_pass_masters},
        {"get_mission_passs", &db::user::get_mission_passs},
        {"get_missions", &db::user::get_missions},
        {"get_multi_room_basics", &db::user::get_multi_room_basics},
        {"get_music_bookmarks", &db::user::get_music_bookmarks},
        {"get_music_course_rankings", &db::user::get_music_course_rankings},
        {"get_music_courses", &db::user::get_music_courses},
        {"get_music_masters", &db::user::get_music_masters},
        {"get_music_videos", &db::user::get_music_videos},
        {"get_music_vocal_version_masters", &db::user::get_music_vocal_version_masters},
        {"get_musics", &db::user::get_musics},
        {"get_name_base_colors", &db::user::get_name_base_colors},
        {"get_name_color_masters", &db::user::get_name_color_masters},
        {"get_name_colors", &db::user::get_name_colors},
        {"get_nameplate_masters", &db::user::get_nameplate_masters},
        {"get_nameplates", &db::user::get_nameplates},
        {"get_note_masters", &db::user::get_note_masters},
        {"get_notes", &db::user::get_notes},
        {"get_notifications", &db::user::get_notifications},
        {"get_party_slots", &db::user::get_party_slots},
        {"get_partys", &db::user::get_partys},
        {"get_permanent_market_things", &db::user::get_permanent_market_things},
        {"get_photos", &db::user::get_photos},
        {"get_pickup_character_missions", &db::user::get_pickup_character_missions},
        {"get_poster_level_pattern_group_masters", &db::user::get_poster_level_pattern_group_masters},
        {"get_poster_level_pattern_masters", &db::user::get_poster_level_pattern_masters},
        {"get_poster_masters", &db::user::get_poster_masters},
        {"get_poster_release_item_group_masters", &db::user::get_poster_release_item_group_masters},
        {"get_poster_release_item_masters", &db::user::get_poster_release_item_masters},
        {"get_poster_story_masters", &db::user::get_poster_story_masters},
        {"get_posters", &db::user::get_posters},
        {"get_random_effect_group_masters", &db::user::get_random_effect_group_masters},
        {"get_restrictions", &db::user::get_restrictions},
        {"get_reward_rule_masters", &db::user::get_reward_rule_masters},
        {"get_roulette_events", &db::user::get_roulette_events},
        {"get_roulettes", &db::user::get_roulettes},
        {"get_sense_effect_masters", &db::user::get_sense_effect_masters},
        {"get_sense_masters", &db::user::get_sense_masters},
        {"get_sp_rates", &db::user::get_sp_rates},
        {"get_special_events", &db::user::get_special_events},
        {"get_spot_conversation_masters", &db::user::get_spot_conversation_masters},
        {"get_stamp_masters", &db::user::get_stamp_masters},
        {"get_stamps", &db::user::get_stamps},
        {"get_star_pass_statuss", &db::user::get_star_pass_statuss},
        {"get_star_rank_reward_masters", &db::user::get_star_rank_reward_masters},
        {"get_story_event_circle_mission_rewards", &db::user::get_story_event_circle_mission_rewards},
        {"get_story_event_circle_missions", &db::user::get_story_event_circle_missions},
        {"get_story_event_circles", &db::user::get_story_event_circles},
        {"get_story_event_high_score_buff_settings", &db::user::get_story_event_high_score_buff_settings},
        {"get_story_event_high_score_party_slots", &db::user::get_story_event_high_score_party_slots},
        {"get_story_event_high_score_partys", &db::user::get_story_event_high_score_partys},
        {"get_story_event_high_scores", &db::user::get_story_event_high_scores},
        {"get_story_events", &db::user::get_story_events},
        {"get_story_masters", &db::user::get_story_masters},
        {"get_theater_storys", &db::user::get_theater_storys},
        {"get_time_limited_controls", &db::user::get_time_limited_controls},
        {"get_total_point_events", &db::user::get_total_point_events},
        {"get_tournament_details", &db::user::get_tournament_details},
        {"get_tournament_qualifyings", &db::user::get_tournament_qualifyings},
        {"get_trial_party_event_stage_party_slots", &db::user::get_trial_party_event_stage_party_slots},
        {"get_trial_party_event_stage_partys", &db::user::get_trial_party_event_stage_partys},
        {"get_trial_party_event_stages", &db::user::get_trial_party_event_stages},
        {"get_trial_party_events", &db::user::get_trial_party_events},
        {"get_triple_cast_basics", &db::user::get_triple_cast_basics},
        {"get_triple_cast_group_members", &db::user::get_triple_cast_group_members},
        {"get_triple_cast_groups", &db::user::get_triple_cast_groups},
        {"get_triple_cast_high_score_party_slots", &db::user::get_triple_cast_high_score_party_slots},
        {"get_triple_cast_high_score_partys", &db::user::get_triple_cast_high_score_partys},
        {"get_triple_cast_historys", &db::user::get_triple_cast_historys},
        {"get_triple_cast_party_slots", &db::user::get_triple_cast_party_slots},
        {"get_triple_cast_partys", &db::user::get_triple_cast_partys},
        {"get_triple_cast_season_results", &db::user::get_triple_cast_season_results},
        {"get_trophy_group_masters", &db::user::get_trophy_group_masters},
        {"get_trophy_masters", &db::user::get_trophy_masters},
        {"get_trophys", &db::user::get_trophys},
        {"get_unchecked_inboxs", &db::user::get_unchecked_inboxs},
        {"get_user_blocks", &db::user::get_user_blocks},
        {"get_user_bonuss", &db::user::get_user_bonuss},
        {"get_user_preferences", &db::user::get_user_preferences},
        {"get_user_profiles", &db::user::get_user_profiles},
        {"get_users", &db::user::get_users},
        {"get_viewed_shops", &db::user::get_viewed_shops},
    };
    return m;
}

// python _camel: strip trailing _, split on _, keep first part, capitalize the rest.
std::string camel(const std::string& attr) {
    std::size_t end = attr.size();
    while (end > 0 && attr[end - 1] == '_') --end;
    std::string s = attr.substr(0, end);
    std::vector<std::string> parts;
    std::string cur;
    for (char c : s) {
        if (c == '_') {
            parts.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    parts.push_back(cur);
    std::string out = parts.empty() ? "" : parts[0];
    for (std::size_t i = 1; i < parts.size(); ++i) {
        const std::string& p = parts[i];
        if (!p.empty()) {
            out.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(p[0]))));
            out.append(p, 1, std::string::npos);
        }
    }
    return out;
}

// memoized camelCase->FieldSpec map for one entity type (_CAMELMAP).
const CamelMap& camelmap(const std::string& type_name) {
    static std::mutex mtx;
    static std::unordered_map<std::string, CamelMap> cache;
    std::lock_guard<std::mutex> g(mtx);
    auto it = cache.find(type_name);
    if (it != cache.end()) return it->second;
    CamelMap mapping;
    const wire::ModelSpec* m = wire::find_model(type_name.c_str());
    if (m) {
        for (int i = 0; i < m->count; ++i) {
            const wire::FieldSpec& f = m->fields[i];
            mapping[camel(f.fn)] = &f;  // last wins, like the python dict comprehension
        }
    }
    if (type_name == "AuditionClear") mapping.erase("userId");  // drop account-id/user_id collision
    auto res = cache.emplace(type_name, std::move(mapping));
    return res.first->second;
}

// python _conv: null passthrough, byte[] -> msgpack bin (a C# byte[] is a bin, not an
// array of ints), model dicts recursed to field-name keys; enum/DateTime/prim deferred
// to the wire layer.
wire::json conv(const char* base, bool is_array, const char* kind, const wire::json& v) {
    if (v.is_null()) return v;
    if (is_array) {
        // a C# byte[] serializes as a MessagePack bin, not an array of integers; emit a
        // json binary so the wire layer packs it as bin (python returns bytes(v) here).
        if (std::strcmp(base, "byte") == 0) {
            if (!v.is_array()) return v;  // mirrors python's isinstance(v, (list, ...)) guard
            std::vector<std::uint8_t> bytes;
            bytes.reserve(v.size());
            for (const wire::json& x : v) bytes.push_back(static_cast<std::uint8_t>(x.get<long long>()));
            return wire::json::binary(std::move(bytes));
        }
        if (!v.is_array()) return v;
        wire::json arr = wire::json::array();
        for (const wire::json& x : v) arr.push_back(conv(base, false, kind, x));
        return arr;
    }
    if (std::strcmp(kind, "model") == 0) return v.is_object() ? _to_array(base, v) : v;
    return v;
}

// entity type -> (unionKey, getter), for every entity with a union key, KEYS table, and getter.
const std::vector<RegEntry>& registry() {
    static const std::vector<RegEntry> reg = [] {
        std::vector<RegEntry> r;
        const std::unordered_map<std::string, GetterFn>& gm = getter_map();
        for (const auto& kv : unions::IDATA_OBJECT) {
            const std::string& name = kv.second;
            const wire::ModelSpec* m = wire::find_model(name.c_str());
            if (!m || m->count == 0) continue;
            std::string gname = "get_" + _table(name) + "s";
            auto it = gm.find(gname);
            if (it == gm.end()) continue;
            r.push_back(RegEntry{name, kv.first, it->second});
        }
        return r;
    }();
    return reg;
}

const std::unordered_map<std::string, const RegEntry*>& registry_map() {
    static const std::unordered_map<std::string, const RegEntry*> m = [] {
        std::unordered_map<std::string, const RegEntry*> t;
        for (const RegEntry& e : registry()) t.emplace(e.name, &e);
        return t;
    }();
    return m;
}

std::string strip(const std::string& s) {
    const char* ws = " \t\n\r\f\v";
    std::size_t b = s.find_first_not_of(ws);
    if (b == std::string::npos) return "";
    std::size_t e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

// python _bearer: the raw token from the Authorization header, or none.
std::optional<std::string> bearer(const httplib::Request& request) {
    if (!request.has_header("Authorization")) return std::nullopt;
    std::string auth = request.get_header_value("Authorization");
    if (auth.empty()) return std::nullopt;
    std::string lower = auth;
    for (char& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (lower.rfind("bearer ", 0) == 0) return strip(auth.substr(7));
    return strip(auth);
}

}  // namespace

std::string _table(const std::string& type_name) {
    std::string out;
    for (std::size_t i = 0; i < type_name.size(); ++i) {
        char c = type_name[i];
        if (i > 0 && c >= 'A' && c <= 'Z') out.push_back('_');
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return out;
}

wire::json _to_array(const std::string& type_name, const wire::json& value) {
    wire::json result = wire::json::object();
    if (!value.is_object()) return result;
    const CamelMap& cm = camelmap(type_name);
    for (auto it = value.begin(); it != value.end(); ++it) {
        auto fit = cm.find(it.key());
        if (fit == cm.end()) continue;
        const wire::FieldSpec* f = fit->second;
        result[f->fn] = conv(f->base, f->is_array, f->kind, it.value());
    }
    return result;
}

std::optional<long long> current_user_id(const httplib::Request& request) {
    std::optional<std::string> token = bearer(request);
    if (!token || token->empty()) return std::nullopt;
    return auth::decode_jwt(*token);
}

wire::json data_object(const std::string& type_name, const wire::json& row) {
    return wire::union_entry(unions::IDATA_OBJECT_KEY.at(type_name), type_name.c_str(),
                             _to_array(type_name, row));
}

wire::json empty_data_object(const std::string& type_name) {
    return wire::union_entry(unions::IDATA_OBJECT_KEY.at(type_name), type_name.c_str(),
                             wire::json::array());
}

wire::json build_present(std::optional<long long> user_id,
                         const std::vector<PresentSpec>& updated) {
    wire::json out = wire::json::array();
    if (!user_id.has_value()) return out;
    const std::unordered_map<std::string, const RegEntry*>& rmap = registry_map();
    for (const PresentSpec& spec : updated) {
        auto it = rmap.find(spec.name);
        if (it == rmap.end()) continue;
        const RegEntry* e = it->second;
        std::vector<db::json> rows = db::fetch(e->get(*user_id));
        for (const db::json& row : rows) {
            if (spec.ids.has_value()) {
                auto idit = row.find("id");
                if (idit == row.end() || idit->is_null()) continue;  // data.get("id") -> None
                long long rid = idit->get<long long>();
                const std::vector<long long>& ids = *spec.ids;
                if (std::find(ids.begin(), ids.end(), rid) == ids.end()) continue;
            }
            out.push_back(wire::union_entry(e->key, spec.name.c_str(), _to_array(spec.name, row)));
        }
    }
    return out;
}

wire::json user_data(std::optional<long long> user_id) {
    wire::json out = wire::json::array();
    if (!user_id.has_value()) return out;
    for (const RegEntry& e : registry()) {
        std::vector<db::json> rows = db::fetch(e.get(*user_id));
        for (const db::json& row : rows)
            out.push_back(wire::union_entry(e.key, e.name.c_str(), _to_array(e.name, row)));
    }
    return out;
}

}  // namespace user_data
