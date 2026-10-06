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

add_library(sqlite3 STATIC external/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC external/sqlite)

add_library(tiny_aes STATIC external/tiny-AES-c/aes.c)
target_include_directories(tiny_aes PUBLIC external/tiny-AES-c)
target_compile_definitions(tiny_aes PUBLIC CBC=1 ECB=0 CTR=0)

add_library(miniz STATIC external/miniz/miniz.c)
target_include_directories(miniz PUBLIC external/miniz)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(dreams SHARED ${SOURCES})
target_include_directories(dreams PRIVATE
    external/cpp-httplib
    external/nlohmann
    external/sqlite
    external/tiny-AES-c
    external/msgpack-cxx/include
    external/miniz
)
target_link_libraries(dreams PRIVATE sqlite3 tiny_aes miniz log z)
target_compile_definitions(dreams PRIVATE
    MSGPACK_NO_BOOST=1
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(dreams PROPERTIES LINK_FLAGS "-s")
endif()
