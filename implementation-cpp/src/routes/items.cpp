#include "routes/items.h"

#include "generated/enums_generated.h"
#include "headers.h"
#include "wire.h"

// ports routes/items.py — every endpoint is a stub

namespace routes {

namespace {
using wire::json;
}  // namespace

void register_items(httplib::Server& svr) {
    // /api/Items/ExchangeCharacterPiece
    svr.Post("/api/Items/ExchangeCharacterPiece",
             [](const httplib::Request&, httplib::Response& res) {
                 // ReceivedThing() — type defaults to ThingTypes.Item, not 0
                 json result;
                 result["type"] = enums::ThingTypes::Item;
                 res.set_content(wire::common_response("ReceivedThing", result),
                                 "application/vnd.msgpack");
                 for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
             });

    // /api/Items/UseBuffItem?itemMasterId=
    svr.Post("/api/Items/UseBuffItem", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(wire::common_response("BooleanResult", json::object()),
                        "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    });

    // /api/Items/UseStaminaRecoveryItems
    svr.Post("/api/Items/UseStaminaRecoveryItems",
             [](const httplib::Request& req, httplib::Response& res) {
                 wire::read_request(req.body, "UseStaminaRecoveryItemsPayload");  // payload unused
                 res.set_content(wire::common_response("BooleanResult", json::object()),
                                 "application/vnd.msgpack");
                 for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
             });
}

}  // namespace routes
