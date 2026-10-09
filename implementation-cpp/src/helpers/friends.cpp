#include "friends.h"

#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "db.h"
#include "db/account.h"
#include "db/friends.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "user_hash.h"

namespace friends {

namespace {

constexpr long long _FRIEND_LIMIT = 200;

long long _now() {
    return static_cast<long long>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
}

std::string _iso(long long seconds) {
    std::time_t t = static_cast<std::time_t>(seconds ? seconds : 0);
    std::tm* tm = std::gmtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S+00:00", tm);
    return std::string(buf);
}

// project another user's User + UserProfile (+ account.lastLoginAt) into a FriendResult
std::optional<wire::json> _friend_result(long long target_id, bool is_favorite) {
    std::optional<db::json> user = db::fetchrow(db::user::get_users(target_id));
    std::optional<db::json> profile = db::fetchrow(db::user::get_user_profiles(target_id));
    if (!user || !profile) return std::nullopt;
    std::optional<db::json> account = db::fetchrow(db::account::get_account_by_id(target_id));

    wire::json fr;
    if (!(*user)["hashUserId"].is_null() && !(*user)["hashUserId"].get<std::string>().empty()) {
        fr["user_id"] = (*user)["hashUserId"].get<std::string>();
    } else {
        fr["user_id"] = user_hash::hash_id(target_id);
    }
    fr["player_rank"] = (*user)["playerRank"].get<long long>();
    fr["trophy_master_id1"] =
        (*profile)["mTrophyId1"].is_null() ? 0LL : (*profile)["mTrophyId1"].get<long long>();
    fr["trophy_master_id2"] =
        (*profile)["mTrophyId2"].is_null() ? 0LL : (*profile)["mTrophyId2"].get<long long>();
    fr["trophy_master_id3"] =
        (*profile)["mTrophyId3"].is_null() ? 0LL : (*profile)["mTrophyId3"].get<long long>();
    if ((*profile)["introduction"].is_null()) {
        fr["introduction"] = nullptr;
    } else {
        fr["introduction"] = (*profile)["introduction"].get<std::string>();
    }
    long long last_login = account ? (*account)["lastLoginAt"].get<long long>() : 0LL;
    fr["last_logged_in_at"] = _iso(last_login);
    if ((*profile)["name"].is_null()) {
        fr["name"] = nullptr;
    } else {
        fr["name"] = (*profile)["name"].get<std::string>();
    }
    bool is_public = (*profile)["isPublicPlayerRate"].get<bool>();
    // another user's rate is hidden (null) unless they made it public
    if (is_public && !(*profile)["playerRate"].is_null()) {
        fr["player_rate"] = (*profile)["playerRate"].get<double>();
    } else {
        fr["player_rate"] = nullptr;
    }
    fr["is_public_player_rate"] = is_public;
    fr["league_class"] = (*profile)["leagueClass"].get<long long>();
    fr["character_ranks"] = wire::json::array();  // TODO: populate from the target's character star ranks
    fr["is_public_album_main_page"] = (*profile)["isPublicAlbumMainPage"].get<bool>();
    fr["main_character_master_id"] = (*profile)["mainCharacterMasterId"].get<long long>();
    fr["display_awakening_status"] = (*profile)["displayAwakeningStatus"].get<bool>();
    fr["icon_frame_master_id"] = (*profile)["iconFrameMasterId"].get<long long>();
    fr["is_favorite"] = is_favorite;
    return fr;
}

}  // namespace

std::optional<long long> resolve_user_id(const std::optional<std::string>& value) {
    if (!value) return std::nullopt;
    std::optional<db::json> row = db::fetchrow(db::account::get_user_id_by_hash(*value));
    if (!row) return std::nullopt;
    return (*row)["userId"].get<long long>();
}

wire::json list_result(std::optional<long long> user_id, const std::string& kind) {
    if (!user_id) {
        wire::json result;
        result["results"] = wire::json::array();
        result["current_friend_count"] = 0LL;
        return result;
    }
    std::vector<db::json> friends = db::fetch(db::friends::get_friends(*user_id));
    std::vector<std::pair<long long, bool>> targets;
    if (kind == "friends") {
        for (const db::json& f : friends) {
            targets.emplace_back(f["friendUserId"].get<long long>(), f["isFavorite"].get<bool>());
        }
    } else if (kind == "received") {
        for (const db::json& r : db::fetch(db::friends::get_received_requests(*user_id))) {
            targets.emplace_back(r["fromUserId"].get<long long>(), false);
        }
    } else {  // sending
        for (const db::json& r : db::fetch(db::friends::get_sending_requests(*user_id))) {
            targets.emplace_back(r["toUserId"].get<long long>(), false);
        }
    }
    wire::json results = wire::json::array();
    for (const auto& [target_id, favorite] : targets) {
        std::optional<wire::json> fr = _friend_result(target_id, favorite);
        if (fr) results.push_back(*fr);
    }
    wire::json result;
    result["results"] = results;
    result["current_friend_count"] = static_cast<long long>(friends.size());
    return result;
}

wire::json block_list_result(std::optional<long long> user_id) {
    wire::json result;
    if (!user_id) {
        result["results"] = wire::json::array();
        return result;
    }
    wire::json results = wire::json::array();
    for (const db::json& block : db::fetch(db::friends::get_blocks(*user_id))) {
        std::optional<long long> target_id;
        if (!block["blockUserId"].is_null()) {
            try {
                target_id = std::stoll(block["blockUserId"].get<std::string>());
            } catch (const std::exception&) {
                target_id = std::nullopt;
            }
        }
        if (target_id) {
            std::optional<wire::json> fr = _friend_result(*target_id, false);
            if (fr) results.push_back(*fr);
        }
    }
    result["results"] = results;
    return result;
}

long long send_request(std::optional<long long> user_id,
                       const std::optional<std::string>& target_str) {
    namespace S = enums::FriendRequestResultStatus;
    if (!user_id) return S::DataNotFound;
    std::optional<long long> target = resolve_user_id(target_str);
    if (!target) return S::DataNotFound;
    if (*target == *user_id) return S::IsMySelf;
    if (!db::fetchrow(db::user::get_users(*target))) return S::DataNotFound;
    if (db::fetchrow(db::friends::get_friend(*user_id, *target))) return S::IsFriends;
    if (db::fetchrow(db::friends::get_request(*user_id, *target))) return S::IsApplying;
    // mutual: they already requested us -> becoming friends instead of re-requesting
    if (db::fetchrow(db::friends::get_request(*target, *user_id))) {
        long long now = _now();
        db::execute(db::friends::remove_request(*target, *user_id));
        db::execute(db::friends::add_friend(*user_id, *target, now));
        db::execute(db::friends::add_friend(*target, *user_id, now));
        return S::RequestSuccess;
    }
    if (static_cast<long long>(db::fetch(db::friends::get_friends(*user_id)).size()) >= _FRIEND_LIMIT) {
        return S::IsFriendCountLimit;
    }
    db::execute(db::friends::add_request(*user_id, *target, _now()));
    return S::RequestSuccess;
}

long long accept_request(std::optional<long long> user_id,
                         const std::optional<std::string>& from_str) {
    namespace S = enums::FriendAcceptResultStatus;
    if (!user_id) return S::Canceled;
    std::optional<long long> from_id = resolve_user_id(from_str);
    if (!from_id) return S::Canceled;
    if (!db::fetchrow(db::friends::get_request(*from_id, *user_id))) {
        return S::Canceled;  // request was cancelled / never existed
    }
    if (static_cast<long long>(db::fetch(db::friends::get_friends(*user_id)).size()) >= _FRIEND_LIMIT) {
        return S::AcceptUserFriendsLimitOver;
    }
    if (static_cast<long long>(db::fetch(db::friends::get_friends(*from_id)).size()) >= _FRIEND_LIMIT) {
        return S::RequestUserFriendsLimitOver;
    }
    long long now = _now();
    db::execute(db::friends::remove_request(*from_id, *user_id));
    db::execute(db::friends::add_friend(*user_id, *from_id, now));
    db::execute(db::friends::add_friend(*from_id, *user_id, now));
    return S::AcceptSuccess;
}

wire::json search(std::optional<long long> user_id,
                  const std::optional<std::string>& target_str) {
    namespace S = enums::FriendSearchResultStatus;
    wire::json result;
    if (!user_id) {
        result["friend_result"] = nullptr;
        result["result_status"] = S::None_;
        return result;
    }
    std::optional<long long> target = resolve_user_id(target_str);
    if (!target) {
        result["friend_result"] = nullptr;
        result["result_status"] = S::None_;
        return result;
    }
    if (!db::fetchrow(db::user::get_users(*target))) {
        result["friend_result"] = nullptr;
        result["result_status"] = S::None_;
        return result;
    }
    std::optional<db::json> friend_row = db::fetchrow(db::friends::get_friend(*user_id, *target));
    long long status;
    if (friend_row) {
        status = S::Friend;
    } else if (db::fetchrow(db::friends::get_request(*user_id, *target))) {
        status = S::Request;
    } else if (db::fetchrow(db::friends::get_request(*target, *user_id))) {
        status = S::ReceivedRequest;
    } else {
        status = S::User;
    }
    bool favorite = friend_row ? (*friend_row)["isFavorite"].get<bool>() : false;
    std::optional<wire::json> fr = _friend_result(*target, favorite);
    if (fr) {
        result["friend_result"] = *fr;
    } else {
        result["friend_result"] = nullptr;
    }
    result["result_status"] = status;
    return result;
}

}  // namespace friends
