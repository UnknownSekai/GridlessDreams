#include "crypto.h"

// tiny-AES-c is vendored as AES-128; the wds keys are 32 bytes (AES-256), so
// select AES256 before including it. the project build MUST compile aes.c with
// -DAES256=1 too, or struct AES_ctx sizes mismatch between translation units.
#define AES256 1
extern "C" {
#include "aes.h"
}

#include <algorithm>
#include <cstring>
#include <random>
#include <stdexcept>

// brotli: real header when vendored, else forward-declare its C API.
#if defined(__has_include)
#  if __has_include(<brotli/decode.h>)
#    include <brotli/decode.h>
#    define CRYPTO_HAVE_BROTLI_DECODE 1
#  endif
#  if __has_include(<brotli/encode.h>)
#    include <brotli/encode.h>
#    define CRYPTO_HAVE_BROTLI_ENCODE 1
#  endif
#endif
#ifndef CRYPTO_HAVE_BROTLI_DECODE
extern "C" {
typedef enum {
    BROTLI_DECODER_RESULT_ERROR = 0,
    BROTLI_DECODER_RESULT_SUCCESS = 1,
    BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT = 2,
    BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT = 3
} BrotliDecoderResult;
typedef struct BrotliDecoderStateStruct BrotliDecoderState;
BrotliDecoderState* BrotliDecoderCreateInstance(void* alloc_func, void* free_func,
                                                void* opaque);
void BrotliDecoderDestroyInstance(BrotliDecoderState* state);
BrotliDecoderResult BrotliDecoderDecompressStream(
    BrotliDecoderState* state, size_t* available_in, const uint8_t** next_in,
    size_t* available_out, uint8_t** next_out, size_t* total_out);
}
#endif
#ifndef CRYPTO_HAVE_BROTLI_ENCODE
extern "C" {
typedef enum {
    BROTLI_MODE_GENERIC = 0,
    BROTLI_MODE_TEXT = 1,
    BROTLI_MODE_FONT = 2
} BrotliEncoderMode;
size_t BrotliEncoderMaxCompressedSize(size_t input_size);
int BrotliEncoderCompress(int quality, int lgwin, BrotliEncoderMode mode,
                          size_t input_size, const uint8_t* input_buffer,
                          size_t* encoded_size, uint8_t* encoded_buffer);
}
#endif

using crypto::Bytes;

namespace {

const char NOTATION_KEY_S[] = "k8teTB%QH.v-hY+e)7wees8bxYSLQdAg";
const char MUSIC_CONFIG_KEY_S[] = "X)|9Vs+&AB5qKBrzqWq)quqEjFug8LaK";

constexpr size_t _BLOCK = 16;

Bytes pkcs7_pad(const Bytes& data) {
    size_t pad = _BLOCK - (data.size() % _BLOCK);
    Bytes out(data);
    out.insert(out.end(), pad, static_cast<uint8_t>(pad));
    return out;
}

Bytes pkcs7_unpad(const Bytes& data) {
    if (data.empty())
        throw std::runtime_error("invalid PKCS7 padding");
    uint8_t pad = data.back();
    bool valid = pad >= 1 && pad <= _BLOCK && data.size() >= pad;
    if (valid) {
        for (size_t i = data.size() - pad; i < data.size(); ++i)
            if (data[i] != pad) {
                valid = false;
                break;
            }
    }
    if (!valid)
        throw std::runtime_error("invalid PKCS7 padding");
    return Bytes(data.begin(), data.end() - pad);
}

Bytes brotli_decompress(const Bytes& input) {
    BrotliDecoderState* s = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);
    if (!s)
        throw std::runtime_error("brotli: cannot create decoder");
    Bytes out;
    size_t avail_in = input.size();
    const uint8_t* next_in = input.data();
    Bytes chunk(1 << 16);
    BrotliDecoderResult result = BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT;
    while (result == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT ||
           result == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT) {
        size_t avail_out = chunk.size();
        uint8_t* next_out = chunk.data();
        result = BrotliDecoderDecompressStream(s, &avail_in, &next_in, &avail_out,
                                               &next_out, nullptr);
        out.insert(out.end(), chunk.data(), chunk.data() + (chunk.size() - avail_out));
        if (result == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT && avail_in == 0)
            break;
    }
    BrotliDecoderDestroyInstance(s);
    if (result != BROTLI_DECODER_RESULT_SUCCESS)
        throw std::runtime_error("brotli: decompression failed");
    return out;
}

Bytes brotli_compress(const Bytes& raw) {
    size_t max_out = BrotliEncoderMaxCompressedSize(raw.size());
    if (max_out == 0)
        max_out = raw.size() + raw.size() / 2 + 1024;
    Bytes out(max_out);
    size_t out_size = max_out;
    // python brotli.compress defaults: quality 11, window 22, generic mode
    int ok = BrotliEncoderCompress(11, 22, BROTLI_MODE_GENERIC, raw.size(),
                                   raw.data(), &out_size, out.data());
    if (!ok)
        throw std::runtime_error("brotli: compression failed");
    out.resize(out_size);
    return out;
}

}  // namespace

const Bytes crypto::NOTATION_KEY(NOTATION_KEY_S,
                                 NOTATION_KEY_S + sizeof(NOTATION_KEY_S) - 1);
const Bytes crypto::MUSIC_CONFIG_KEY(MUSIC_CONFIG_KEY_S,
                                     MUSIC_CONFIG_KEY_S + sizeof(MUSIC_CONFIG_KEY_S) - 1);

Bytes crypto::decrypt_aes(const Bytes& content, const Bytes& key) {
    size_t n = std::min<size_t>(_BLOCK, content.size());
    uint8_t iv[_BLOCK] = {0};
    std::memcpy(iv, content.data(), n);
    Bytes buf(content.begin() + n, content.end());
    if (buf.size() % _BLOCK != 0)
        throw std::runtime_error("invalid ciphertext length");

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key.data(), iv);
    AES_CBC_decrypt_buffer(&ctx, buf.data(), buf.size());
    return pkcs7_unpad(buf);
}

Bytes crypto::encrypt_aes(const Bytes& plaintext, const Bytes& key,
                          std::optional<Bytes> iv) {
    Bytes ivb;
    if (!iv.has_value()) {
        ivb.resize(_BLOCK);
        std::random_device rd;
        for (auto& b : ivb)
            b = static_cast<uint8_t>(rd());
    } else {
        ivb = *iv;
    }
    uint8_t ivbuf[_BLOCK] = {0};
    std::memcpy(ivbuf, ivb.data(), std::min<size_t>(_BLOCK, ivb.size()));

    Bytes buf = pkcs7_pad(plaintext);
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key.data(), ivbuf);
    AES_CBC_encrypt_buffer(&ctx, buf.data(), buf.size());

    Bytes out(ivb);
    out.insert(out.end(), buf.begin(), buf.end());
    return out;
}

Bytes crypto::decrypt_notation(const Bytes& content, const Bytes& key) {
    return brotli_decompress(decrypt_aes(content, key));
}

Bytes crypto::encrypt_notation(const Bytes& data, const Bytes& key,
                               std::optional<Bytes> iv) {
    return encrypt_aes(brotli_compress(data), key, iv);
}

Bytes crypto::decrypt_music_config(const Bytes& content, const Bytes& key) {
    return decrypt_aes(content, key);
}

Bytes crypto::encrypt_music_config(const Bytes& data, const Bytes& key,
                                   std::optional<Bytes> iv) {
    return encrypt_aes(data, key, iv);
}
