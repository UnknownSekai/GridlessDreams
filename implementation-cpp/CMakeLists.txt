cmake_minimum_required(VERSION 3.14)
project(Dreams LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# dep checks
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/cpp-httplib/httplib.h")
    message(FATAL_ERROR "cpp-httplib not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/nlohmann/json.hpp")
    message(FATAL_ERROR "nlohmann/json not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/sqlite/sqlite3.c")
    message(FATAL_ERROR "SQLite not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/tiny-AES-c/aes.c")
    message(FATAL_ERROR "tiny-AES-c not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/miniz/miniz.c")
    message(FATAL_ERROR "miniz not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/lz4/lib/lz4.c")
    message(FATAL_ERROR "lz4 not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/argon2/include/argon2.h")
    message(FATAL_ERROR "argon2 not found. Run setup-deps first.")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/spookyhash/src/SpookyV2.cpp")
    message(FATAL_ERROR "spookyhash not found. Run setup-deps first.")
endif()

add_library(sqlite3 STATIC external/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC external/sqlite)
# ceil()/floor() for adjust_user_stamina_atomic (db layer).
target_compile_definitions(sqlite3 PRIVATE SQLITE_ENABLE_MATH_FUNCTIONS=1)

add_library(tiny_aes STATIC external/tiny-AES-c/aes.c)
target_include_directories(tiny_aes PUBLIC external/tiny-AES-c)
# AES256: crypto.cpp (the only consumer) uses 256-bit keys for the WDS asset ciphers.
# Must match aes.c's build or struct AES_ctx (RoundKey size) mismatches across TUs.
target_compile_definitions(tiny_aes PUBLIC CBC=1 ECB=0 CTR=0 AES256=1)

add_library(miniz STATIC external/miniz/miniz.c)
target_include_directories(miniz PUBLIC external/miniz)

add_library(lz4 STATIC external/lz4/lib/lz4.c)
target_include_directories(lz4 PUBLIC external/lz4/lib)

# argon2 (reference core; ref.c is the portable fill_segment -- do NOT also add opt.c).
add_library(argon2 STATIC
    external/argon2/src/argon2.c
    external/argon2/src/core.c
    external/argon2/src/encoding.c
    external/argon2/src/thread.c
    external/argon2/src/blake2/blake2b.c
    external/argon2/src/ref.c
)
target_include_directories(argon2 PUBLIC external/argon2/include)

# Bob Jenkins SpookyHash V2 (byte-identical to the python spookyhash package).
add_library(spookyhash STATIC external/spookyhash/src/SpookyV2.cpp)
target_include_directories(spookyhash PUBLIC external/spookyhash/src)

# sha256 + hmac-sha256 + base64url for JWT HS256 (no OpenSSL).
add_library(jwtcrypto STATIC external/jwtcrypto/jwtcrypto.cpp)
target_include_directories(jwtcrypto PUBLIC external/jwtcrypto)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(dreams SHARED ${SOURCES})
target_include_directories(dreams PRIVATE
    external
    external/cpp-httplib
    external/nlohmann
    external/sqlite
    external/tiny-AES-c
    external/msgpack-cxx/include
    external/miniz
    external/lz4/lib
    external/argon2/include
    external/spookyhash/src
    external/jwtcrypto
    external/miniyaml
)
target_link_libraries(dreams PRIVATE
    sqlite3 tiny_aes miniz lz4 argon2 spookyhash jwtcrypto log z)
target_compile_definitions(dreams PRIVATE
    MSGPACK_NO_BOOST=1
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(dreams PROPERTIES LINK_FLAGS "-s")
endif()
