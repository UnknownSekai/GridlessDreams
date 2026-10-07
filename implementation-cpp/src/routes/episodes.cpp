#include "routes/episodes.h"

#include <algorithm>
#include <cctype>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "helpers/character_enhance.h"
#include "helpers/episodes.h"
#include "helpers/game_state.h"
#include "master_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/episodes.py. Read/ReadAll grant an episode's read reward once (char
// episodes gate on release order + advance reading progress); GetDetails returns an
// EpisodeResult; the scenes/*.bin route serves the raw EpisodeDetailResult[] blob.

namespace routes {
namespace {

long long get_ll(const master_data::json& o, const char* k, long long def) {
    auto it = o.find(k);
    if (it == o.end() || it->is_null()) return def;
    return it->get<long long>();
}

// append (thing_type, thing_id, thing_quantity) triples as grant input entries
void add_rewards(wire::json& out, const std::vector<episodes::RewardThing>& things) {
    for (const episodes::RewardThing& t : things)
        out.push_back(wire::json::array({std::get<0>(t), std::get<1>(t), std::get<2>(t)}));
}

// python int(): the whole (stripped) string must be a signed decimal integer
std::optional<long long> py_int(const std::string& s) {
    size_t i = 0, n = s.size();
    while (i < n && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    size_t j = n;
    while (j > i && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    if (i >= j) return std::nullopt;
    size_t k = i;
    if (s[k] == '+' || s[k] == '-') ++k;
    if (k >= j) return std::nullopt;
    for (size_t p = k; p < j; ++p)
        if (!std::isdigit(static_cast<unsigned char>(s[p]))) return std::nullopt;
    try {
        return std::stoll(s.substr(i, j - i));
    } catch (...) {
        return std::nullopt;
    }
}

// grant an episode's read reward once; ReadAll additionally grants the read-all reward.
// char episodes gate on the character having the episode released, and advance the
// character's reading progress + character mission on first read.
void read_story(const httplib::Request& req, httplib::Response& res, long long eid, bool read_all) {
    try {
        game_state::State s = game_state::transaction(req);
        const master_data::json* relation = nullptr;
        for (const master_data::json& e : master_data::table("CharacterEpisodeMaster"))
            if (get_ll(e, "episode_master_id", 0) == eid) {
                relation = &e;
                break;
            }
        long long cmid = 0;
        long long episode_order = 0;
        game_state::json* chr = nullptr;
        if (relation != nullptr) {
            cmid = get_ll(*relation, "character_master_id", 0);
            episode_order = get_ll(*relation, "episode_order", 0);
            chr = s.one("Character", game_state::json{{"characterMasterId", cmid}});
            if (chr == nullptr || chr->at("releasedEpisodeOrder").get<long long>() < episode_order)
                throw game_state::Rejected();
        }
        game_state::json* existing = s.one("Episode", game_state::json{{"episodeMasterId", eid}});
        wire::json rewards = wire::json::array();
        if (existing == nullptr) {
            add_rewards(rewards, episodes::episode_read_reward_things(eid));
            if (read_all) add_rewards(rewards, episodes::episode_readall_reward_things(eid));
            s.insert("Episode", game_state::json{{"id", eid},
                                                 {"episodeMasterId", eid},
                                                 {"hasReadAll", read_all}});
            if (chr != nullptr) {
                long long order =
                    std::max(chr->at("readEpisodeOrder").get<long long>(), episode_order);
                s.update("Character", chr, game_state::json{{"readEpisodeOrder", order}});
                const character_enhance::json* cm = character_enhance::character_master(cmid);
                game_state::character_progress(
                    s, cm->at("character_base_master_id").get<long long>(), 4, 1);
            }
        } else if (read_all && !existing->at("hasReadAll").get<bool>()) {
            add_rewards(rewards, episodes::episode_readall_reward_things(eid));
            s.update("Episode", existing, game_state::json{{"hasReadAll", true}});
        }
        wire::json result = s.grant(rewards);
        wire::json present = s.present();
        s.commit();
        pipeline::respond(res, "ReceivedThing", result, wire::json::array(), present);
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "ReceivedThing", wire::json::array());
    }
}

}  // namespace

void register_episodes(httplib::Server& svr) {
    // Scene script blob: EpisodeDetailResult[] packed to msgpack, served as the .bin the client
    // downloads (from an EpisodeResult.EpisodeDetailAssetSource)
    svr.Get("/master-data/production/scenes/:filename",
            [](const httplib::Request& req, httplib::Response& res) {
                const std::string filename = req.path_params.at("filename");
                if (filename.size() < 4 ||
                    filename.compare(filename.size() - 4, 4, ".bin") != 0) {
                    res.status = httplib::StatusCode::NotFound_404;
                    return;
                }
                const std::string name = filename.substr(0, filename.size() - 4);  // "<id>_<hash>"
                std::optional<long long> episode_id = py_int(name.substr(0, name.find('_')));
                if (!episode_id) {
                    res.status = httplib::StatusCode::NotFound_404;
                    return;
                }
                // id + hash must match the manifest exactly
                if (name != episodes::scene_bin_name(*episode_id)) {
                    res.status = httplib::StatusCode::NotFound_404;
                    return;
                }
                std::optional<std::string> data = episodes::episode_scene_bin(*episode_id);
                if (!data) {
                    res.status = httplib::StatusCode::NotFound_404;
                    return;
                }
                res.set_content(*data, "application/octet-stream");
            });

    // /api/Episodes/{episodeMasterId}/Read  (read by skipping -> has_read_all = false)
    svr.Post("/api/Episodes/:episodeMasterId/Read",
             [](const httplib::Request& req, httplib::Response& res) {
                 read_story(req, res, std::stoll(req.path_params.at("episodeMasterId")), false);
             });

    // /api/Episodes/{episodeMasterId}/ReadAll  (read all the text -> has_read_all = true)
    svr.Post("/api/Episodes/:episodeMasterId/ReadAll",
             [](const httplib::Request& req, httplib::Response& res) {
                 read_story(req, res, std::stoll(req.path_params.at("episodeMasterId")), true);
             });

    // /api/Episodes/{episodeMasterId}/GetDetails
    svr.Post("/api/Episodes/:episodeMasterId/GetDetails",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long episode_master_id = std::stoll(req.path_params.at("episodeMasterId"));
                 // a single EpisodeResult (title/storyType/order + EpisodeDetailAssetSource); the
                 // client downloads that asset source to get the EpisodeDetailResult[] script.
                 // local metadata fills in the title/order/story type when available.
                 std::optional<wire::json> result = episodes::episode_result(episode_master_id);
                 std::optional<episodes::Meta> metadata = episodes::_local_meta(episode_master_id);
                 if (result && metadata) {
                     const auto& [title, order, story_type] = *metadata;
                     (*result)["episode_title"] = title ? wire::json(*title) : wire::json(nullptr);
                     (*result)["episode_order"] = order;
                     (*result)["story_type"] = story_type;
                 }
                 pipeline::respond(res, "EpisodeResult",
                                   result ? *result : wire::json::object());
             });
}

}  // namespace routes
