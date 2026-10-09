#include "routes/inbox.h"

#include <chrono>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "helpers/things.h"
#include "helpers/user_data.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/inbox.py

namespace routes {
namespace {

using wire::json;

// claim the given inboxes: grant each reward, mark received, and present the
// inventory entities the grants wrote plus the now-received inbox rows
void receive(httplib::Response& res, std::optional<long long> user_id,
             const std::vector<long long>& inbox_ids) {
    json result;
    result["received_things"] = json::array();
    result["has_not_receive_things"] = false;
    if (!user_id || inbox_ids.empty()) {
        pipeline::respond(res, "InboxReceiveResult", result);
        return;
    }
    long long now = static_cast<long long>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    std::vector<json> received;
    std::set<std::string> present_types;
    std::vector<long long> received_ids;

    std::unordered_map<long long, db::json> by_id;
    for (const db::json& r : db::fetch(db::user::get_inboxes_by_ids(*user_id, inbox_ids)))
        by_id[r.at("id").get<long long>()] = r;
    for (long long inbox_id : inbox_ids) {
        auto it = by_id.find(inbox_id);
        if (it == by_id.end() || it->second.at("hasReceived").get<bool>())
            continue;  // missing or already claimed
        const db::json& row = it->second;
        long long receive_limit_at = row.at("receiveLimitAt").get<long long>();
        if (receive_limit_at && receive_limit_at < now) {
            result["has_not_receive_things"] = true;  // expired -> left unclaimed
            continue;
        }
        received.push_back(things::grant_thing(*user_id, row.at("thingType").get<long long>(),
                                               row.at("thingId").get<long long>(),
                                               row.at("thingQuantity").get<long long>()));
        long long row_id = row.at("id").get<long long>();
        db::execute(db::user::receive_inbox(*user_id, row_id, now));
        received_ids.push_back(row_id);
        std::optional<std::string> pt = things::present_type(row.at("thingType").get<long long>());
        if (pt) present_types.insert(*pt);
    }
    result["received_things"] = received;
    if (present_types.count("Currency"))  // User mirrors coin/freeJewel/paidJewel
        present_types.insert("User");
    // present: the inventory entities the grants wrote + the now-received inbox rows
    std::vector<user_data::PresentSpec> updated;
    for (const std::string& t : present_types) updated.push_back(t);
    updated.emplace_back("Inbox", received_ids);
    json present = user_data::build_present(user_id, updated);
    pipeline::respond(res, "InboxReceiveResult", result, json::array(), present);
}

}  // namespace

void register_inbox(httplib::Server& svr) {
    // /api/Inboxes/BulkReceive
    svr.Post("/api/Inboxes/BulkReceive", [](const httplib::Request& req, httplib::Response& res) {
        json payload = pipeline::read_request(req, "BulkReceivePayload");
        std::vector<long long> inbox_ids;
        if (!payload.is_null() && payload.contains("inbox_ids") && !payload["inbox_ids"].is_null())
            for (const json& i : payload["inbox_ids"]) inbox_ids.push_back(i.get<long long>());
        receive(res, user_data::current_user_id(req), inbox_ids);
    });

    // /api/Inboxes/CheckPackagesAsync
    svr.Post("/api/Inboxes/CheckPackagesAsync",
             [](const httplib::Request& req, httplib::Response& res) {
                 std::optional<long long> user_id = user_data::current_user_id(req);
                 // a diff, not the whole inbox: surface only packages not seen yet, then mark them,
                 // so a follow-up call returns an empty present (matching the official)
                 json present = json::array();
                 if (user_id) {
                     std::vector<db::json> rows = db::fetch(db::user::get_unchecked_inboxs(*user_id));
                     for (const db::json& row : rows) present.push_back(user_data::data_object("Inbox", row));
                     if (!rows.empty()) db::execute(db::user::mark_inboxs_checked(*user_id));
                 }
                 json result;
                 result["is_success"] = true;
                 pipeline::respond(res, "BooleanResult", result, json::array(), present);
             });

    // /api/Inboxes/{inboxId}/Receive
    svr.Post("/api/Inboxes/:inboxId/Receive", [](const httplib::Request& req, httplib::Response& res) {
        long long inbox_id = std::stoll(req.path_params.at("inboxId"));
        receive(res, user_data::current_user_id(req), {inbox_id});
    });
}

}  // namespace routes
