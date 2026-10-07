#include "db/friends.h"

#include <string>

namespace db::friends {

// -- friendships ---------------------------------------------------------------

db::SelectQuery get_friends(int64_t user_id) {
    return db::SelectQuery(
        "FriendModel",
        "SELECT * FROM \"friend\" WHERE \"userId\" = $1 ORDER BY \"isFavorite\" DESC, \"createdAt\"",
        user_id);
}

db::SelectQuery get_friend(int64_t user_id, int64_t friend_user_id) {
    return db::SelectQuery(
        "FriendModel",
        "SELECT * FROM \"friend\" WHERE \"userId\" = $1 AND \"friendUserId\" = $2",
        user_id,
        friend_user_id);
}

db::ExecutableQuery add_friend(int64_t user_id, int64_t friend_user_id, int64_t now) {
    return db::ExecutableQuery(
        "INSERT INTO \"friend\" (\"userId\", \"friendUserId\", \"isFavorite\", \"createdAt\") "
        "VALUES ($1, $2, false, $3) "
        "ON CONFLICT (\"userId\", \"friendUserId\") DO NOTHING",
        user_id,
        friend_user_id,
        now);
}

db::ExecutableQuery remove_friend(int64_t user_id, int64_t friend_user_id) {
    return db::ExecutableQuery(
        "DELETE FROM \"friend\" WHERE \"userId\" = $1 AND \"friendUserId\" = $2",
        user_id,
        friend_user_id);
}

db::ExecutableQuery set_friend_favorite(int64_t user_id, int64_t friend_user_id, bool favorite) {
    return db::ExecutableQuery(
        "UPDATE \"friend\" SET \"isFavorite\" = $3 WHERE \"userId\" = $1 AND \"friendUserId\" = $2",
        user_id,
        friend_user_id,
        favorite);
}

// -- requests ------------------------------------------------------------------

db::SelectQuery get_received_requests(int64_t user_id) {
    return db::SelectQuery(
        "FriendRequestModel",
        "SELECT * FROM \"friend_request\" WHERE \"toUserId\" = $1 ORDER BY \"createdAt\"",
        user_id);
}

db::SelectQuery get_sending_requests(int64_t user_id) {
    return db::SelectQuery(
        "FriendRequestModel",
        "SELECT * FROM \"friend_request\" WHERE \"fromUserId\" = $1 ORDER BY \"createdAt\"",
        user_id);
}

db::SelectQuery get_request(int64_t from_user_id, int64_t to_user_id) {
    return db::SelectQuery(
        "FriendRequestModel",
        "SELECT * FROM \"friend_request\" WHERE \"fromUserId\" = $1 AND \"toUserId\" = $2",
        from_user_id,
        to_user_id);
}

db::ExecutableQuery add_request(int64_t from_user_id, int64_t to_user_id, int64_t now) {
    return db::ExecutableQuery(
        "INSERT INTO \"friend_request\" (\"fromUserId\", \"toUserId\", \"createdAt\") "
        "VALUES ($1, $2, $3) "
        "ON CONFLICT (\"fromUserId\", \"toUserId\") DO NOTHING",
        from_user_id,
        to_user_id,
        now);
}

db::ExecutableQuery remove_request(int64_t from_user_id, int64_t to_user_id) {
    return db::ExecutableQuery(
        "DELETE FROM \"friend_request\" WHERE \"fromUserId\" = $1 AND \"toUserId\" = $2",
        from_user_id,
        to_user_id);
}

// -- blocks (user_block table) -------------------------------------------------

db::SelectQuery get_blocks(int64_t user_id) {
    return db::SelectQuery(
        "UserBlockModel", "SELECT * FROM \"user_block\" WHERE \"userId\" = $1", user_id);
}

db::ExecutableQuery add_block(int64_t user_id, int64_t block_user_id) {
    return db::ExecutableQuery(
        "INSERT INTO \"user_block\" (\"userId\", \"id\", \"blockUserId\") VALUES ($1, $2, $3)",
        user_id,
        block_user_id,
        std::to_string(block_user_id));
}

db::ExecutableQuery remove_block(int64_t user_id, int64_t block_user_id) {
    return db::ExecutableQuery(
        "DELETE FROM \"user_block\" WHERE \"userId\" = $1 AND \"blockUserId\" = $2",
        user_id,
        std::to_string(block_user_id));
}

}  // namespace db::friends
