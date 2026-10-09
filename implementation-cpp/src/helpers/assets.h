#pragma once
#include <optional>
#include <string>
#include <utility>

// serve the local Addressables catalogs, bundles and encrypted notations.
// ports helpers/assets.py. catalog .json.br (brotli) / .hash (SpookyHash-128,
// little-endian lowercase hex) are memoized; bundles/notations are read verbatim.
// each served body carries a Content-MD5 (base64 of md5) matching the official server.

namespace assets {

// (raw body bytes, Content-MD5 = base64(md5(body)))
using BodyMd5 = std::pair<std::string, std::string>;

std::optional<BodyMd5> catalog_br(const std::string& kind, const std::string& platform);

std::optional<BodyMd5> catalog_hash(const std::string& kind, const std::string& platform);

bool local_assets_enabled();

// a local asset bundle and its Content-MD5, or none if not downloaded. not memoized.
std::optional<BodyMd5> bundle(const std::string& kind, const std::string& platform, const std::string& rel_path);

std::string official_url(const std::string& kind, const std::string& platform, const std::string& version,
                         const std::string& rel_path);

// a local encrypted notation/music_config .enc and its Content-MD5, or none if absent
std::optional<BodyMd5> notation(const std::string& music_id, const std::string& filename);

std::string official_notation_url(const std::string& music_id, const std::string& filename);

// a local static-content file (event/gacha banner textures) and its Content-MD5, or none.
// served raw; fetched by the client from static_content_url (not the Addressables asset_url).
std::optional<BodyMd5> static_content(const std::string& rel_path);

std::string official_static_url(const std::string& rel_path);

}  // namespace assets
