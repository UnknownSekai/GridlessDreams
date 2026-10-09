#pragma once
#include <optional>
#include <string>

#include "wire.h"

// friend list / request / accept / search / block, projecting other users into
// FriendResult. ports helpers/friends.py. the async `app`/`conn` parameters are
// dropped: reads/writes go through the global db engine inside db::transaction().
// hashUserId<->userId resolution via db::account; _FRIEND_LIMIT=200; mutual
// requests auto-friend; another user's rate stays hidden unless made public.
// results flow as wire::json keyed by field name (FriendListResult /
// BlockListResult / FriendSearchResult); the *_request helpers return the
// matching Friend*ResultStatus enum value.

namespace friends {

// map a client-supplied hashUserId string to its sequential userId, or none
std::optional<long long> resolve_user_id(const std::optional<std::string>& value);

// FriendListResult for kind in {friends, received, sending}; current_friend_count
// is always the real friend count regardless of which list was requested
wire::json list_result(std::optional<long long> user_id, const std::string& kind);

// BlockListResult projecting each blocked user into a FriendResult
wire::json block_list_result(std::optional<long long> user_id);

// send a friend request; returns a FriendRequestResultStatus value
long long send_request(std::optional<long long> user_id,
                       const std::optional<std::string>& target_str);

// accept an incoming friend request; returns a FriendAcceptResultStatus value
long long accept_request(std::optional<long long> user_id,
                         const std::optional<std::string>& from_str);

// FriendSearchResult (friend_result + result_status) for a hashUserId lookup
wire::json search(std::optional<long long> user_id,
                  const std::optional<std::string>& target_str);

}  // namespace friends
