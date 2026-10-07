#pragma once
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "wire.h"

// Serve episode scene scripts + episode metadata/read-rewards. Ports
// helpers/episodes.py. Each vendored _data/episodes/<id>.json holds an episode's
// metadata plus its EpisodeDetail (the full script); scene_bin packs that to the
// msgpack EpisodeDetailResult[] .bin the client downloads. bin_hashes.json maps an
// episode id to its "<id>_<hash>" blob name. Results flow as wire::json
// (EpisodeResult) keyed by field name.

namespace episodes {

// (title, order, story_type) from master data or a vendored file; story_type is a
// StoryTypes enum value, title is absent for side/character episodes.
using Meta = std::tuple<std::optional<std::string>, long long, long long>;

// (thing_type, thing_id, thing_quantity)
using RewardThing = std::tuple<long long, long long, long long>;

// the episode's "<id>_<hash>" scene-blob name, or none if we don't have it
std::optional<std::string> scene_bin_name(long long episode_id);

// (title, order, StoryTypes) from the vendored <id>.json top-level, or none
std::optional<Meta> _local_meta(long long episode_id);

// EpisodeResult (title/type/order + scene-bin download URL) for GetEpisodeDetail, or
// none if we can't describe the episode / have no scene blob
std::optional<wire::json> episode_result(long long episode_id);

// reward for reading an episode -- its full reward package minus the chapter-all-read
// bonus (package id == episode id). granted once, on the first Read or ReadAll.
std::vector<RewardThing> episode_read_reward_things(long long episode_id);

// extra reward for reading ALL an episode's text -- the catalogue item(s) from its
// package. granted once by ReadAll, on top of the read reward.
std::vector<RewardThing> episode_readall_reward_things(long long episode_id);

// msgpack of the episode's EpisodeDetailResult[] (its whole script), or none if the
// episode isn't vendored
std::optional<std::string> episode_scene_bin(long long episode_id);

}  // namespace episodes
