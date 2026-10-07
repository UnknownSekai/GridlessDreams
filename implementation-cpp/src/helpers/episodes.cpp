#include "helpers/episodes.h"

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "config.h"
#include "generated/enums_generated.h"
#include "master_data.h"
#include "platform.h"

namespace episodes {
namespace {

using json = nlohmann::ordered_json;

const std::string _DIR = "_data/episodes";

long long get_ll(const json& o, const char* k, long long def) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null()) return def;
    if (it->is_boolean()) return it->get<bool>() ? 1 : 0;
    return it->get<long long>();
}

bool get_bool(const json& o, const char* k, bool def) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null()) return def;
    return it->get<bool>();
}

std::optional<std::string> get_str(const json& o, const char* k) {
    auto it = o.find(k);
    if (it == o.end() || !it->is_string()) return std::nullopt;
    return it->get<std::string>();
}

// bin_hashes.json: episode id (string) -> "<id>_<hash>" scene-blob name. Loaded once.
const json& bin_hashes() {
    static const json data = [] {
        std::string path = _DIR + "/bin_hashes.json";
        if (platform::file_exists(path)) {
            json j = json::parse(platform::read_file(path), nullptr, false);
            if (j.is_object()) return j;
        }
        return json::object();
    }();
    return data;
}

// scene bins are served under /master-data/production/scenes/ (see routes/episodes.py),
// so build the client-facing URL off the same master-data base.
[[maybe_unused]] std::string _asset_base() {
    std::string s = config::get_str("master_data_url");
    size_t end = s.find_last_not_of('/');
    return end == std::string::npos ? std::string() : s.substr(0, end + 1);
}

// episode-master tables that carry title/order and link to a StoryMaster (its .type is the
// story type: Main/Event/Special). CharacterEpisodeMaster has no story link and no title --
// its episodes are Side stories.
const char* const _STORY_EPISODE_TABLES[] = {
    "EpisodeMaster",
    "StoryEventEpisodeMaster",
    "SpecialEpisodeMaster",
};

std::map<long long, Meta> _EPISODE_META;  // episode_master_id -> (title, order, StoryTypes)

void _build_meta() {
    if (!_EPISODE_META.empty()) return;
    std::unordered_map<long long, long long> story_type;
    for (const json& sm : master_data::table("StoryMaster"))
        story_type[get_ll(sm, "id_", 0)] = get_ll(sm, "type", enums::StoryTypes::None_);
    for (const char* hdr : _STORY_EPISODE_TABLES) {
        for (const json& e : master_data::table(hdr)) {
            long long smid = get_ll(e, "story_master_id", 0);
            auto sit = story_type.find(smid);
            long long st = sit != story_type.end() ? sit->second : enums::StoryTypes::None_;
            _EPISODE_META[get_ll(e, "id_", 0)] = Meta{get_str(e, "title"), get_ll(e, "order", 0), st};
        }
    }
    for (const json& e : master_data::table("CharacterEpisodeMaster"))
        _EPISODE_META[get_ll(e, "id_", 0)] =
            Meta{std::nullopt, get_ll(e, "episode_order", 0), enums::StoryTypes::Side};
}

std::unordered_map<long long, std::optional<Meta>> _LOCAL_META_CACHE;

// The 歌劇目録 (opera catalogue) items -- their item category. It's the extra reward for reading
// ALL of an episode's text, but only for MAIN-story episodes (EpisodeMaster). Event packages
// also carry the item as a normal read reward, but their full-read grants no extra copy.
constexpr long long _CATALOGUE_ITEM_CATEGORY = 13;

std::map<long long, std::vector<RewardThing>> _READ_REWARDS;  // episode id -> read reward
std::map<long long, std::vector<RewardThing>> _READALL_REWARDS;  // main episode id -> catalogue item[s]

void _build_rewards() {
    if (!_READ_REWARDS.empty()) return;
    std::unordered_set<long long> catalogue;
    for (const json& it : master_data::table("ItemMaster"))
        if (get_ll(it, "category", 0) == _CATALOGUE_ITEM_CATEGORY) catalogue.insert(get_ll(it, "id_", 0));
    std::unordered_set<long long> main_episodes;
    for (const json& e : master_data::table("EpisodeMaster")) main_episodes.insert(get_ll(e, "id_", 0));
    for (const json& pkg : master_data::table("EpisodeRewardPackageMaster")) {
        long long pid = get_ll(pkg, "id_", 0);
        std::vector<RewardThing> read;
        auto rit = pkg.find("rewards");
        if (rit != pkg.end() && rit->is_array())
            for (const json& r : *rit) {
                if (get_bool(r, "is_chapter_all_read_reward", false)) continue;
                read.emplace_back(get_ll(r, "thing_type", 0), get_ll(r, "thing_id", 0),
                                  get_ll(r, "thing_quantity", 0));
            }
        _READ_REWARDS[pid] = read;
        if (main_episodes.count(pid)) {
            std::vector<RewardThing> all;
            for (const RewardThing& t : read)
                if (std::get<0>(t) == 1 && catalogue.count(std::get<1>(t))) all.push_back(t);
            _READALL_REWARDS[pid] = all;
        }
    }
}

std::unordered_map<long long, std::optional<std::string>> _SCENE_BIN_CACHE;

// vendored scene dicts are keyed by the entity aliases (PascalCase, plus one camelCase
// "slotNumber"); convert to the field-name keys the wire codec packs by. omitted non-zero
// pydantic defaults (FontSizes.Middle / SpineSizes.Middle) are injected so wire doesn't _zero
// them; every other omitted field defaults correctly through wire itself.
struct AliasField {
    const char* alias;
    const char* fn;
};

const AliasField _SCENE_FIELDS[] = {
    {"Id", "id_"},
    {"EpisodeMasterId", "episode_master_id"},
    {"Order", "order"},
    {"GroupOrder", "group_order"},
    {"Effect", "effect"},
    {"SpeakerName", "speaker_name"},
    {"Phrase", "phrase"},
    {"Title", "title"},
    {"BackgroundImageFileName", "background_image_file_name"},
    {"BackgroundCharacterImageFileName", "background_character_image_file_name"},
    {"BackgroundImageFileFadeType", "background_image_file_fade_type"},
    {"BgmFileName", "bgm_file_name"},
    {"SeFileName", "se_file_name"},
    {"StillPhotoFileName", "still_photo_file_name"},
    {"MovieFileName", "movie_file_name"},
    {"WindowEffect", "window_effect"},
    {"SceneCameraMasterId", "scene_camera_master_id"},
    {"VoiceFileName", "voice_file_name"},
    {"SpeakerIconId", "speaker_icon_id"},
    {"FadeValue1", "fade_value1"},
    {"FadeValue2", "fade_value2"},
    {"FadeValue3", "fade_value3"},
};

const AliasField _MOTION_FIELDS[] = {
    {"slotNumber", "slot_number"},
    {"FacialExpressionMasterId", "facial_expression_master_id"},
    {"HeadMotionMasterId", "head_motion_master_id"},
    {"HeadDirectionMasterId", "head_direction_master_id"},
    {"BodyMotionMasterId", "body_motion_master_id"},
    {"LipSyncMasterId", "lip_sync_master_id"},
    {"SpineId", "spine_id"},
    {"CharacterAppearanceType", "character_appearance_type"},
    {"CharacterPosition", "character_position"},
    {"CharacterLayerType", "character_layer_type"},
};

json _convert_motion(const json& m) {
    json out = json::object();
    for (const AliasField& af : _MOTION_FIELDS) {
        auto it = m.find(af.alias);
        if (it != m.end()) out[af.fn] = *it;
    }
    auto ss = m.find("SpineSize");
    out["spine_size"] = (ss != m.end() && !ss->is_null()) ? *ss : json(enums::SpineSizes::Middle);
    return out;
}

json _convert_scene(const json& s) {
    json out = json::object();
    for (const AliasField& af : _SCENE_FIELDS) {
        auto it = s.find(af.alias);
        if (it != s.end()) out[af.fn] = *it;
    }
    auto fs = s.find("FontSize");
    out["font_size"] = (fs != s.end() && !fs->is_null()) ? *fs : json(enums::FontSizes::Middle);
    auto cm = s.find("CharacterMotions");
    if (cm != s.end() && cm->is_array()) {
        json arr = json::array();
        for (const json& m : *cm) arr.push_back(m.is_object() ? _convert_motion(m) : m);
        out["character_motions"] = std::move(arr);
    }
    return out;
}

}  // namespace

std::optional<std::string> scene_bin_name(long long episode_id) {
    const json& h = bin_hashes();
    auto it = h.find(std::to_string(episode_id));
    if (it == h.end() || !it->is_string()) return std::nullopt;
    return it->get<std::string>();
}

std::optional<Meta> _local_meta(long long episode_id) {
    auto cit = _LOCAL_META_CACHE.find(episode_id);
    if (cit != _LOCAL_META_CACHE.end()) return cit->second;
    std::string path = _DIR + "/" + std::to_string(episode_id) + ".json";
    std::optional<Meta> result;
    if (platform::file_exists(path)) {
        json raw = json::parse(platform::read_file(path), nullptr, false);
        if (raw.is_object())
            result = Meta{get_str(raw, "Title"), get_ll(raw, "Order", 0),
                          get_ll(raw, "StoryType", enums::StoryTypes::None_)};
    }
    _LOCAL_META_CACHE[episode_id] = result;
    return result;
}

std::optional<wire::json> episode_result(long long episode_id) {
    std::optional<std::string> name = scene_bin_name(episode_id);
    if (!name) return std::nullopt;
    _build_meta();
    std::optional<Meta> meta;
    auto it = _EPISODE_META.find(episode_id);
    if (it != _EPISODE_META.end()) {
        meta = it->second;
    } else {  // absent from master data == a Character (StoryType 4) episode -> local
        meta = _local_meta(episode_id);
        if (!meta) return std::nullopt;
    }
    const auto& [title, order, story_type] = *meta;
    wire::json r = wire::json::object();
    r["episode_title"] = title ? wire::json(*title) : wire::json(nullptr);
    r["story_type"] = story_type;
    r["episode_order"] = order;
    r["episode_detail_asset_source"] = "scenes/" + *name + ".bin";
    return r;
}

std::vector<RewardThing> episode_read_reward_things(long long episode_id) {
    _build_rewards();
    auto it = _READ_REWARDS.find(episode_id);
    return it != _READ_REWARDS.end() ? it->second : std::vector<RewardThing>{};
}

std::vector<RewardThing> episode_readall_reward_things(long long episode_id) {
    _build_rewards();
    auto it = _READALL_REWARDS.find(episode_id);
    return it != _READALL_REWARDS.end() ? it->second : std::vector<RewardThing>{};
}

std::optional<std::string> episode_scene_bin(long long episode_id) {
    auto cit = _SCENE_BIN_CACHE.find(episode_id);
    if (cit != _SCENE_BIN_CACHE.end()) return cit->second;
    std::string path = _DIR + "/" + std::to_string(episode_id) + ".json";
    std::optional<std::string> result;
    if (platform::file_exists(path)) {
        json raw = json::parse(platform::read_file(path), nullptr, false);
        json scenes = json::array();
        if (raw.is_object()) {
            auto det = raw.find("EpisodeDetail");
            if (det != raw.end() && det->is_array())
                for (const json& s : *det)
                    if (s.is_object()) scenes.push_back(_convert_scene(s));
        }
        result = wire::pack("EpisodeDetailResult", scenes);
    }
    _SCENE_BIN_CACHE[episode_id] = result;
    return result;
}

}  // namespace episodes
