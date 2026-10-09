#pragma once
#include <cstddef>
#include <map>
#include <string>

// MasterMemory .db container codec (MessagePack-C# layout). mirrors helpers/mastermemory.py.
// blob = MessagePack map {tableName -> [offset, length]} followed by a data region of per-table
// blobs. each blob is a plain MessagePack array of rows, or an Lz4Block (ext 99) wrapping
// msgpack-uint(raw_len) + raw LZ4 block when the raw array is >= COMPRESS_MIN bytes.

namespace mastermemory {

static constexpr int LZ4_BLOCK = 99;        // MessagePackCompression.Lz4Block ext code
static constexpr size_t COMPRESS_MIN = 64;  // only compress raw blobs >= this

// tables: header_name -> already-msgpacked bytes of that table's row array
std::string pack(const std::map<std::string, std::string>& tables);

// reverse of pack: header_name -> per-table raw msgpack bytes (decompressed)
std::map<std::string, std::string> unpack(const std::string& db);

}  // namespace mastermemory
