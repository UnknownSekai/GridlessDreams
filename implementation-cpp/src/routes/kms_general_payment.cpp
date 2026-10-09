#include "routes/kms_general_payment.h"

#include "generated/enums_generated.h"
#include "headers.h"
#include "wire.h"

// ports routes/kms_general_payment.py — both endpoints are stubs (no receipt on a private
// server); they return a default ProcessPaymentResult (result defaults to Success)

namespace routes {

void register_kms_general_payment(httplib::Server& svr) {
    // /api/Payments/process/appstore
    svr.Post("/api/Payments/process/appstore", [](const httplib::Request&, httplib::Response& res) {
        wire::json result = {{"result", enums::ProcessPaymentTransactionResult::Success}};
        res.set_content(wire::common_response("ProcessPaymentResult", result), "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    });

    // /api/Payments/process/googleplay
    svr.Post("/api/Payments/process/googleplay", [](const httplib::Request&, httplib::Response& res) {
        wire::json result = {{"result", enums::ProcessPaymentTransactionResult::Success}};
        res.set_content(wire::common_response("ProcessPaymentResult", result), "application/vnd.msgpack");
        for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
    });
}

}  // namespace routes
