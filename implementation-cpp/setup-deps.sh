#!/bin/bash
set -e

echo "[*] Setting up dependencies..."

if [ ! -d "external/cpp-httplib" ]; then
    echo "[*] Cloning cpp-httplib..."
    git clone --depth 1 https://github.com/yhirose/cpp-httplib.git external/cpp-httplib
fi

if [ ! -f "external/nlohmann/json.hpp" ]; then
    echo "[*] Downloading nlohmann/json..."
    mkdir -p external/nlohmann
    curl -L -o external/nlohmann/json.hpp https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
fi

if [ ! -f "external/sqlite/sqlite3.c" ]; then
    echo "[*] Downloading SQLite amalgamation..."
    mkdir -p external/sqlite
    curl -L -o external/sqlite/sqlite-amalgamation.zip https://www.sqlite.org/2024/sqlite-amalgamation-3450100.zip
    cd external/sqlite
    unzip -o sqlite-amalgamation.zip
    mv sqlite-amalgamation-3450100/sqlite3.c sqlite3.c
    mv sqlite-amalgamation-3450100/sqlite3.h sqlite3.h
    mv sqlite-amalgamation-3450100/sqlite3ext.h sqlite3ext.h
    rm -rf sqlite-amalgamation-3450100 sqlite-amalgamation.zip
    cd ../..
fi

if [ ! -d "external/tiny-AES-c" ]; then
    echo "[*] Cloning tiny-AES-c..."
    git clone --depth 1 https://github.com/kokke/tiny-AES-c.git external/tiny-AES-c
fi

if [ ! -d "external/msgpack-cxx" ]; then
    echo "[*] Cloning msgpack-cxx..."
    git clone --depth 1 --branch cpp_master https://github.com/msgpack/msgpack-c.git external/msgpack-cxx
fi

if [ ! -f "external/miniz/miniz.h" ]; then
    echo "[*] Downloading miniz..."
    mkdir -p external/miniz
    curl -L -o external/miniz/miniz.zip https://github.com/richgel999/miniz/releases/download/3.0.2/miniz-3.0.2.zip
    cd external/miniz
    unzip -o miniz.zip miniz.h miniz.c
    rm miniz.zip
    cd ../..
fi

# NDK - detect OS
if [ ! -d "external/android-ndk-r26d" ]; then
    OS=$(uname -s)
    case "$OS" in
        Linux*)  NDK_ZIP="android-ndk-r26d-linux.zip" ;;
        Darwin*) NDK_ZIP="android-ndk-r26d-darwin.zip" ;;
        *)       echo "[WARN] Unknown OS $OS, skipping NDK download"; NDK_ZIP="" ;;
    esac
    if [ -n "$NDK_ZIP" ]; then
        echo "[*] Downloading Android NDK r26d (this may take a while)..."
        curl -L -o "external/$NDK_ZIP" "https://dl.google.com/android/repository/$NDK_ZIP"
        echo "[*] Extracting NDK..."
        cd external
        unzip -o "$NDK_ZIP"
        rm "$NDK_ZIP"
        cd ..
    fi
fi


echo "[*] Dependencies ready!"
