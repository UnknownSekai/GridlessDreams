#pragma once
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "wire.h"

// granting "things" (rewards). ports helpers/things.py. a grant adds any ThingType to its
// proper per-user table (currency, item stock, stamina, a json collection, or an owned
// possession row) and returns a ReceivedThing describing it. the per-user entity rows and
// the returned ReceivedThing all flow as wire::json keyed by the pydantic field name.

namespace things {

using json = wire::json;

// (thing_type, thing_id, quantity)
using ThingTriple = std::tuple<long long, long long, long long>;

// the per-user entity type a grant of thing_type writes (for present), or nullopt
std::optional<std::string> present_type(long long thing_type);

// add one thing to its proper per-user table; returns the ReceivedThing describing it
json grant_thing(long long user_id, long long thing_type, long long thing_id, long long quantity);

// grant many (thing_type, thing_id, quantity) tuples; returns the ReceivedThing[]
std::vector<json> grant_things(long long user_id, const std::vector<ThingTriple>& things);

// grant many with as few DB ops as possible (stackables folded into single writes);
// one ReceivedThing per distinct resource
std::vector<json> grant_things_consolidated(long long user_id, const std::vector<ThingTriple>& things);

}  // namespace things
