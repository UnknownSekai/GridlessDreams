#include "mastermemory.h"

#include <lz4.h>
#include <msgpack.hpp>

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

namespace mastermemory {
namespace {

std::string pack_uint(uint64_t n) {
    msgpack::sbuffer b;
    msgpack::packer<msgpack::sbuffer> pk(&b);
    pk.pack(n);  // minimal-width unsigned, matches python msgpack.packb(len)
    return std::string(b.data(), b.size());
}

std::string make_blob(const std::string& raw) {
    if (raw.size() < COMPRESS_MIN) return raw;
    std::string payload = pack_uint(static_cast<uint64_t>(raw.size()));
    int bound = LZ4_compressBound(static_cast<int>(raw.size()));
    std::string comp(static_cast<size_t>(bound), '\0');
    int clen = LZ4_compress_default(raw.data(), &comp[0], static_cast<int>(raw.size()), bound);
    if (clen <= 0) throw std::runtime_error("lz4 compress failed");
    payload.append(comp.data(), static_cast<size_t>(clen));
    msgpack::sbuffer eb;
    msgpack::packer<msgpack::sbuffer> epk(&eb);
    epk.pack_ext(payload.size(), static_cast<int8_t>(LZ4_BLOCK));
    epk.pack_ext_body(payload.data(), static_cast<uint32_t>(payload.size()));
    return std::string(eb.data(), eb.size());
}

}  // namespace

std::string pack(const std::map<std::string, std::string>& tables) {
    struct Entry {
        const std::string* name;
        std::string blob;
        uint64_t offset;
    };
    std::vector<Entry> entries;
    entries.reserve(tables.size());
    uint64_t offset = 0;
    for (const auto& kv : tables) {  // std::map iterates in sorted key order == python sorted()
        std::string blob = make_blob(kv.second);
        uint64_t bsz = static_cast<uint64_t>(blob.size());
        entries.push_back({&kv.first, std::move(blob), offset});
        offset += bsz;
    }

    msgpack::sbuffer hb;
    msgpack::packer<msgpack::sbuffer> hpk(&hb);
    hpk.pack_map(static_cast<uint32_t>(entries.size()));
    for (const auto& e : entries) {
        hpk.pack_str(static_cast<uint32_t>(e.name->size()));
        hpk.pack_str_body(e.name->data(), static_cast<uint32_t>(e.name->size()));
        hpk.pack_array(2);
        hpk.pack(e.offset);
        hpk.pack(static_cast<uint64_t>(e.blob.size()));
    }

    std::string out(hb.data(), hb.size());
    for (const auto& e : entries) out += e.blob;
    return out;
}

std::map<std::string, std::string> unpack(const std::string& db) {
    std::map<std::string, std::string> out;
    size_t off = 0;
    msgpack::object_handle hh = msgpack::unpack(db.data(), db.size(), off);
    const msgpack::object& header = hh.get();
    if (header.type != msgpack::type::MAP) return out;
    size_t base = off;  // data region begins right after the header map

    for (uint32_t i = 0; i < header.via.map.size; ++i) {
        const msgpack::object& k = header.via.map.ptr[i].key;
        const msgpack::object& v = header.via.map.ptr[i].val;
        if (k.type != msgpack::type::STR || v.type != msgpack::type::ARRAY || v.via.array.size < 2)
            continue;
        std::string name(k.via.str.ptr, k.via.str.size);
        uint64_t offset = v.via.array.ptr[0].as<uint64_t>();
        uint64_t length = v.via.array.ptr[1].as<uint64_t>();
        if (base + offset + length > db.size()) continue;
        std::string blob = db.substr(base + static_cast<size_t>(offset), static_cast<size_t>(length));

        msgpack::object_handle bh = msgpack::unpack(blob.data(), blob.size());
        const msgpack::object& bo = bh.get();
        if (bo.type == msgpack::type::EXT && bo.via.ext.type() == static_cast<int8_t>(LZ4_BLOCK)) {
            const char* ed = bo.via.ext.data();
            uint32_t esz = bo.via.ext.size;
            size_t eoff = 0;
            msgpack::object_handle lh = msgpack::unpack(ed, esz, eoff);
            uint64_t ulen = lh.get().as<uint64_t>();
            std::string dst(static_cast<size_t>(ulen), '\0');
            if (ulen > 0) {
                int r = LZ4_decompress_safe(ed + eoff, &dst[0], static_cast<int>(esz - eoff),
                                            static_cast<int>(ulen));
                if (r < 0) throw std::runtime_error("lz4 block decompress failed");
            }
            out.emplace(std::move(name), std::move(dst));
        } else {
            out.emplace(std::move(name), std::move(blob));
        }
    }
    return out;
}

}  // namespace mastermemory
