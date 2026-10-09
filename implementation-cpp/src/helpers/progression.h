#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "wire.h"

// lesson/course lifecycles + player-rank XP + daily-usage accounting. ports
// helpers/progression.py, wiring together the gameplay-result helpers. lesson
// star points and rank XP per stamina are explicit policy constants
// (constants.yml), not a reverse engineering of every modifier.
//
// operates on a game_state::State (the active per-account transaction); the
// finished-play payload p (FinishLivePayload) and the result entity being built
// (FinishLiveResult, mutated in place) both flow as wire::json keyed by field
// name. master rows (course/detail) flow as wire::json too.

namespace game_state {
struct State;
}

namespace progression {

using json = wire::json;

// (position, setCharacterId) lesson slot; setCharacterId may be absent
using Slot = std::pair<long long, std::optional<long long>>;

// the constants dict (policy knobs); e.g. rules()["unlimited_attempts"],
// rules()["lesson_star_points"]
const nlohmann::json& rules();

// ensure the caller's DailyLimit row exists and is zeroed past the 05:00 reset;
// returns the row. now defaults to the current epoch-micros.
json daily(game_state::State& s, std::optional<long long> now = std::nullopt);

// bump a daily usage counter (unless unlimited_attempts)
void use_daily(game_state::State& s, const std::string& field);

// persist the active live context (mode / masterId / extra) for this user
void context(game_state::State& s, const std::string& mode, long long ident,
             const json& extra);

// (position, setCharacterId) pairs from a CharacterLesson row's setCharacters
std::vector<Slot> lesson_slots(const json& row);

// slot pairs -> stored [{position, setCharacterId}, ...], sorted
json stored_slots(const std::vector<Slot>& slots);

// the caller's CharacterLesson party for a base, creating a default one from the
// owned actors if absent. raises Rejected if the base isn't owned / has no actors.
json lesson_party(game_state::State& s, long long base);

// apply rank XP: the level-up loop + stamina restore + rank missions.
// returns a PlayerRankPointResult.
json rank_xp(game_state::State& s, long long amount);

// settle a finished lesson: high-score rewards, star points, character/mission
// progress; writes result.lesson_result
void finish_lesson(game_state::State& s, long long base, const json& p,
                   json& result);

// (course, sorted details, index) for a course detail id. raises Rejected if
// the detail id is unknown.
std::tuple<json, std::vector<json>, long long> find_course(long long detail_id);

// advance / finish a music-course run: per-stage rate/lamp aggregation, grade &
// lamp records, master rewards granted once per newly attained grade
void finish_course(game_state::State& s, long long ident, const json& p,
                   json& result);

// dispatch a finished play by its stored context mode (course/lesson), then the
// common post-play accounting (auto/rank-xp daily usage, shared missions)
void finish_context(game_state::State& s, const json& context_row, const json& p,
                    json& result);

}  // namespace progression
