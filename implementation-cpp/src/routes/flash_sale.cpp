#include "routes/flash_sale.h"

#include "headers.h"
#include "wire.h"

namespace routes {

void register_flash_sale(httplib::Server& svr) {
    // /api/FlashSale/ReadFlashSaleStage
    svr.Post("/api/FlashSale/ReadFlashSaleStage", [](const httplib::Request& req, httplib::Response& res) {
        wire::read_request(req.body, "FlashSaleReadStagePayload");
        res.set_content(wire::common_response("BooleanResult", wire::json::object()), "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    });
}

}  // namespace routes
