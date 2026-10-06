THEOS_DEVICE_IP ?= localhost
THEOS_DEVICE_PORT ?= 2222
TARGET := iphone:clang:16.5:11.0
ARCHS = arm64

include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = libdreams

COMMON_INCLUDES = -Iexternal/cpp-httplib -Iexternal/nlohmann -Iexternal/sqlite -Iexternal/tiny-AES-c -Iexternal/msgpack-cxx/include -Iexternal/miniz
COMMON_DEFS = -DCBC=1 -DECB=0 -DCTR=0 -DMSGPACK_NO_BOOST=1

# NOTE: routes.cpp + db.cpp are the game-specific server (not copied). Add them
# back here once you implement them for this game.
libdreams_FILES = src/main.cpp src/crypto.cpp src/platform.cpp src/ssl_bypass.cpp external/sqlite/sqlite3.c external/tiny-AES-c/aes.c external/miniz/miniz.c
libdreams_CFLAGS = -fno-rtti -Wno-unused-function -Wno-error $(COMMON_INCLUDES) $(COMMON_DEFS)
libdreams_CXXFLAGS = -std=c++17 -fno-rtti -Wno-unused-function -Wno-error $(COMMON_INCLUDES) $(COMMON_DEFS)
libdreams_CCFLAGS = -std=c++17 -fno-rtti -Wno-unused-function -Wno-error $(COMMON_INCLUDES) $(COMMON_DEFS)
libdreams_LDFLAGS =
libdreams_FRAMEWORKS =
libdreams_INSTALL_PATH = /usr/lib

include $(THEOS_MAKE_PATH)/library.mk
