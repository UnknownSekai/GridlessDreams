#include "pipeline.h"

#include <msgpack.hpp>

#include "headers.h"
#include "wire.h"

// Ports the httplib-facing surface of helpers/msgpack.py. The msgpack codec, LZ4
// decompression and the 5-pack envelope all live in wire.cpp; this layer only adds
// the httplib::Request/Response adapters and the Fault/DeletedDataObject builders.
// app.py's lifespan, /master-data/production route and exception handlers are not
// part of this surface -- they live in main.cpp / routes.cpp.

namespace pipeline {

json read_request(const httplib::Request& req, const char* name) {
    return wire::read_request(req.body, name);
}

json read_request_list(const httplib::Request& req, const char* name) {
    json out = json::array();
    if (req.body.empty()) return out;
    msgpack::object_handle oh;
    try {
        oh = wire::unpack_raw(req.body);
    } catch (...) {
        return out;
    }
    const msgpack::object& raw = oh.get();
    if (raw.type != msgpack::type::ARRAY) return out;
    const auto& arr = raw.via.array;
    // a bare (non-nested) payload array is read as a one-entry batch
    bool any_nested = false;
    for (uint32_t i = 0; i < arr.size; ++i) {
        if (arr.ptr[i].type == msgpack::type::ARRAY) {
            any_nested = true;
            break;
        }
    }
    if (arr.size > 0 && !any_nested) {
        out.push_back(wire::from_array(name, raw));
        return out;
    }
    for (uint32_t i = 0; i < arr.size; ++i) out.push_back(wire::from_array(name, arr.ptr[i]));
    return out;
}

json fault(const std::string& error_code, const std::string& message,
           const std::string& stack_trace) {
    json f = json::object();
    f["error_code"] = error_code;
    f["message"] = message;
    f["stack_trace"] = stack_trace;
    return f;
}

json deleted(const std::string& type_name, long long id) {
    json d = json::object();
    d["type_name"] = type_name;
    d["id_"] = id;
    return d;
}

namespace {

void apply_headers(httplib::Response& res) {
    for (const auto& h : headers::response_headers()) res.set_header(h.first, h.second);
}

}  // namespace

void respond(httplib::Response& res, const char* result_name, const json& result,
             const json& faults, const json& present, const json& deleted,
             const json& notifications) {
    res.set_content(
        wire::common_response(result_name, result, faults, present, deleted, notifications),
        "application/vnd.msgpack");
    apply_headers(res);
}

void respond_union_result(httplib::Response& res, const json& result, const json& faults,
                          const json& present, const json& deleted, const json& notifications) {
    res.set_content(
        wire::common_response_union_result(result, faults, present, deleted, notifications),
        "application/vnd.msgpack");
    apply_headers(res);
}

void raw_response(httplib::Response& res, const char* result_name, const json& result) {
    res.set_content(wire::pack(result_name, result), "application/vnd.msgpack");
    apply_headers(res);
}

}  // namespace pipeline
