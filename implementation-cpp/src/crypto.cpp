#include "crypto.h"
extern "C" {
#include "aes.h"
}
#include <msgpack.hpp>
#include <cstring>

static const uint8_t AES_KEY[16] = {'g','2','f','c','C','0','Z','c','z','N','9','M','T','J','6','1'};
static const uint8_t AES_IV[16]  = {'m','s','x','3','I','V','0','i','9','X','E','5','u','Y','Z','1'};

// PKCS7
static std::vector<uint8_t> pad(const std::vector<uint8_t>& data) {
    size_t block = AES_BLOCKLEN;
    size_t pad_len = block - (data.size() % block);
    std::vector<uint8_t> out(data);
    out.resize(data.size() + pad_len, static_cast<uint8_t>(pad_len));
    return out;
}

static std::vector<uint8_t> unpad(const std::vector<uint8_t>& data) {
    if (data.empty()) return {};
    uint8_t pad_len = data.back();
    if (pad_len > AES_BLOCKLEN || pad_len == 0) return data;
    return {data.begin(), data.end() - pad_len};
}

static nlohmann::json msgpack_to_json(const msgpack::object& obj) {
    switch (obj.type) {
        case msgpack::type::NIL:
            return nullptr;
        case msgpack::type::BOOLEAN:
            return obj.via.boolean;
        case msgpack::type::POSITIVE_INTEGER:
            return obj.via.u64;
        case msgpack::type::NEGATIVE_INTEGER:
            return obj.via.i64;
        case msgpack::type::FLOAT32:
        case msgpack::type::FLOAT64:
            return obj.via.f64;
        case msgpack::type::STR:
            return std::string(obj.via.str.ptr, obj.via.str.size);
        case msgpack::type::ARRAY: {
            nlohmann::json arr = nlohmann::json::array();
            for (uint32_t i = 0; i < obj.via.array.size; i++)
                arr.push_back(msgpack_to_json(obj.via.array.ptr[i]));
            return arr;
        }
        case msgpack::type::MAP: {
            nlohmann::json map = nlohmann::json::object();
            for (uint32_t i = 0; i < obj.via.map.size; i++) {
                auto& kv = obj.via.map.ptr[i];
                std::string key(kv.key.via.str.ptr, kv.key.via.str.size);
                map[key] = msgpack_to_json(kv.val);
            }
            return map;
        }
        default:
            return nullptr;
    }
}

std::vector<uint8_t> crypto::encrypt(const nlohmann::json& data) {
    auto packed = nlohmann::json::to_msgpack(data);
    auto padded = pad(packed);

    uint8_t iv[16];
    memcpy(iv, AES_IV, 16);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, AES_KEY, iv);
    AES_CBC_encrypt_buffer(&ctx, padded.data(), padded.size());

    return padded;
}

nlohmann::json crypto::decrypt(const std::string& ciphertext) {
    std::vector<uint8_t> buf(ciphertext.begin(), ciphertext.end());

    uint8_t iv[16];
    memcpy(iv, AES_IV, 16);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, AES_KEY, iv);
    AES_CBC_decrypt_buffer(&ctx, buf.data(), buf.size());

    auto unpadded = unpad(buf);

    msgpack::object_handle oh = msgpack::unpack(
        reinterpret_cast<const char*>(unpadded.data()), unpadded.size()
    );
    return msgpack_to_json(oh.get());
}
