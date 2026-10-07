#pragma once
// ports helpers/crypto.py: AES-256-CBC crypto for wds downloadable assets
// (notation charts, music configs). iv is prepended as the first 16 bytes.
#include <cstdint>
#include <optional>
#include <vector>

namespace crypto {

using Bytes = std::vector<uint8_t>;

// keys baked into the client for downloadable assets
extern const Bytes NOTATION_KEY;
extern const Bytes MUSIC_CONFIG_KEY;

Bytes decrypt_aes(const Bytes& content, const Bytes& key);
Bytes encrypt_aes(const Bytes& plaintext, const Bytes& key,
                  std::optional<Bytes> iv = std::nullopt);

Bytes decrypt_notation(const Bytes& content, const Bytes& key = NOTATION_KEY);
Bytes encrypt_notation(const Bytes& data, const Bytes& key = NOTATION_KEY,
                       std::optional<Bytes> iv = std::nullopt);

Bytes decrypt_music_config(const Bytes& content,
                           const Bytes& key = MUSIC_CONFIG_KEY);
Bytes encrypt_music_config(const Bytes& data,
                           const Bytes& key = MUSIC_CONFIG_KEY,
                           std::optional<Bytes> iv = std::nullopt);

}
