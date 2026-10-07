THEOS_DEVICE_IP ?= localhost
THEOS_DEVICE_PORT ?= 2222
TARGET := iphone:clang:16.5:11.0
ARCHS = arm64

include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = libdreams

COMMON_INCLUDES = -Iexternal -Iexternal/cpp-httplib -Iexternal/nlohmann -Iexternal/sqlite -Iexternal/tiny-AES-c -Iexternal/msgpack-cxx/include -Iexternal/miniz -Iexternal/lz4/lib -Iexternal/argon2/include -Iexternal/spookyhash/src -Iexternal/jwtcrypto -Iexternal/miniyaml
# AES256: crypto.cpp uses 256-bit asset ciphers; aes.c + crypto.cpp must agree on struct AES_ctx.
COMMON_DEFS = -DCBC=1 -DECB=0 -DCTR=0 -DAES256=1 -DMSGPACK_NO_BOOST=1 -DSQLITE_ENABLE_MATH_FUNCTIONS=1

# src TUs are globbed recursively to mirror CMake's GLOB_RECURSE, so db.cpp,
# routes.cpp and every new module TU are picked up as they are added.
DREAMS_SRC = $(shell find src -name '*.cpp' 2>/dev/null)
DREAMS_EXTERNAL = \
	external/sqlite/sqlite3.c \
	external/tiny-AES-c/aes.c \
	external/miniz/miniz.c \
	external/lz4/lib/lz4.c \
	external/argon2/src/argon2.c \
	external/argon2/src/core.c \
	external/argon2/src/encoding.c \
	external/argon2/src/thread.c \
	external/argon2/src/blake2/blake2b.c \
	external/argon2/src/ref.c \
	external/spookyhash/src/SpookyV2.cpp \
	external/jwtcrypto/jwtcrypto.cpp

libdreams_FILES = $(DREAMS_SRC) $(DREAMS_EXTERNAL)
libdreams_CFLAGS = -fno-rtti -Wno-unused-function -Wno-error $(COMMON_INCLUDES) $(COMMON_DEFS)
libdreams_CXXFLAGS = -std=c++17 -fno-rtti -Wno-unused-function -Wno-error $(COMMON_INCLUDES) $(COMMON_DEFS)
libdreams_CCFLAGS = -std=c++17 -fno-rtti -Wno-unused-function -Wno-error $(COMMON_INCLUDES) $(COMMON_DEFS)
libdreams_LDFLAGS =
libdreams_FRAMEWORKS =
libdreams_INSTALL_PATH = /usr/lib

include $(THEOS_MAKE_PATH)/library.mk
