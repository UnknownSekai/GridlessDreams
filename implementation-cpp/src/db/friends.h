#pragma once
#include "db.h"
#include <cstdint>

// Queries for the friend system: friendships (one row per direction), pending requests
// (from -> to), and blocks (reusing the user_block table). Mirrors db/friends.py.
namespace db::friends {

// friendships
db::SelectQuery get_friends(int64_t user_id);
db::SelectQuery get_friend(int64_t user_id, int64_t friend_user_id);
db::ExecutableQuery add_friend(int64_t user_id, int64_t friend_user_id, int64_t now);
db::ExecutableQuery remove_friend(int64_t user_id, int64_t friend_user_id);
db::ExecutableQuery set_friend_favorite(int64_t user_id, int64_t friend_user_id, bool favorite);

// requests
db::SelectQuery get_received_requests(int64_t user_id);
db::SelectQuery get_sending_requests(int64_t user_id);
db::SelectQuery get_request(int64_t from_user_id, int64_t to_user_id);
db::ExecutableQuery add_request(int64_t from_user_id, int64_t to_user_id, int64_t now);
db::ExecutableQuery remove_request(int64_t from_user_id, int64_t to_user_id);

// blocks (user_block table)
db::SelectQuery get_blocks(int64_t user_id);
db::ExecutableQuery add_block(int64_t user_id, int64_t block_user_id);
db::ExecutableQuery remove_block(int64_t user_id, int64_t block_user_id);

}  // namespace db::friends
