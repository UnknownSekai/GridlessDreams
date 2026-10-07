#pragma once
#include "httplib_config.h"
#include "wire.h"

// Request/response pipeline. Ports the httplib-facing surface of helpers/msgpack.py
// (read_request / read_request_list / common_response|respond / raw_response / fault /
// deleted) plus the app.py envelope integration: it is the thin glue that ties wire
// (KEYS msgpack + LZ4 + 5-pack envelope) to httplib and the X-FM header (helpers/headers.py).
//
// The msgpack codec itself lives in wire.h; bearer->userId resolution lives in
// user_data (user_data::current_user_id). This layer only adds what routes need on
// top of those: the httplib::Request/Response adapters.

namespace pipeline {

using json = wire::json;

// read + LZ4-decompress a msgpack request body (helpers.msgpack.read_request).
// name==nullptr -> the raw decoded tree; otherwise from_array into the named model.
// empty/undecodable body -> null json.
json read_request(const httplib::Request& req, const char* name = nullptr);

// a TPayload[] body -> array of decoded entries (helpers.msgpack.read_request_list).
// a bare (non-nested) payload array is read as a one-entry batch.
json read_request_list(const httplib::Request& req, const char* name);

// one Fault entry for the envelope faults[] (helpers.msgpack.fault).
json fault(const std::string& error_code, const std::string& message = "",
           const std::string& stack_trace = "");

// one DeletedDataObject entry for the envelope deleted[] (helpers.msgpack.deleted).
json deleted(const std::string& type_name, long long id);

// fill res with the 5-pack common envelope + the X-FM header, media application/vnd.msgpack
// (helpers.msgpack.common_response, aka respond). result_name==nullptr packs a nil result.
// present/notifications elements are wire::union_entry arrays; faults/deleted are model jsons.
void respond(httplib::Response& res, const char* result_name, const json& result,
             const json& faults = json::array(), const json& present = json::array(),
             const json& deleted = json::array(), const json& notifications = json::array());

// like respond but the result slot is an IDataObject[] union list (GetUserData): its entries
// are wire::union_entry arrays packed positionally, same as present.
void respond_union_result(httplib::Response& res, const json& result,
                          const json& faults = json::array(), const json& present = json::array(),
                          const json& deleted = json::array(), const json& notifications = json::array());

// fill res with pack(result) only + the X-FM header (helpers.msgpack.raw_response,
// ParseWithoutCommonResponse). almost no route uses this.
void raw_response(httplib::Response& res, const char* result_name, const json& result);

}  // namespace pipeline
