#pragma once
#include "db.h"
#include <cstdint>

// queries for the friend system: friendships (one row per direction), pending requests
// (from -> to), and blocks (reusing the user_block table). mirrors db/friends.py.
namespace db::friends {

// friendships
db::SelectQuery get_friends(long long user_id);
db::SelectQuery get_friend(long long user_id, long long friend_user_id);
db::ExecutableQuery add_friend(long long user_id, long long friend_user_id, long long now);
db::ExecutableQuery remove_friend(long long user_id, long long friend_user_id);
db::ExecutableQuery set_friend_favorite(long long user_id, long long friend_user_id, bool favorite);

// requests
db::SelectQuery get_received_requests(long long user_id);
db::SelectQuery get_sending_requests(long long user_id);
db::SelectQuery get_request(long long from_user_id, long long to_user_id);
db::ExecutableQuery add_request(long long from_user_id, long long to_user_id, long long now);
db::ExecutableQuery remove_request(long long from_user_id, long long to_user_id);

// blocks (user_block table)
db::SelectQuery get_blocks(long long user_id);
db::ExecutableQuery add_block(long long user_id, long long block_user_id);
db::ExecutableQuery remove_block(long long user_id, long long block_user_id);

}  // namespace db::friends
