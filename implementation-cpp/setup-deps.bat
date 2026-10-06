@echo off
REM Fetch deps + Android NDK

echo [*] Setting up dependencies...

if not exist external\cpp-httplib (
    echo [*] Cloning cpp-httplib...
    git clone --depth 1 https://github.com/yhirose/cpp-httplib.git external/cpp-httplib
)

if not exist external\nlohmann (
    echo [*] Downloading nlohmann/json...
    mkdir external\nlohmann
    curl -L -o external\nlohmann\json.hpp https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
)

if not exist external\sqlite (
    echo [*] Downloading SQLite amalgamation...
    mkdir external\sqlite
    curl -L -o external\sqlite\sqlite-amalgamation.zip https://www.sqlite.org/2024/sqlite-amalgamation-3450100.zip
    cd external\sqlite
    tar -xf sqlite-amalgamation.zip
    move sqlite-amalgamation-3450100\sqlite3.c sqlite3.c
    move sqlite-amalgamation-3450100\sqlite3.h sqlite3.h
    move sqlite-amalgamation-3450100\sqlite3ext.h sqlite3ext.h
    rmdir /s /q sqlite-amalgamation-3450100
    del sqlite-amalgamation.zip
    cd ..\..
)

if not exist external\tiny-AES-c (
    echo [*] Cloning tiny-AES-c...
    git clone --depth 1 https://github.com/kokke/tiny-AES-c.git external/tiny-AES-c
)

if not exist external\msgpack-cxx (
    echo [*] Cloning msgpack-cxx...
    git clone --depth 1 --branch cpp_master https://github.com/msgpack/msgpack-c.git external/msgpack-cxx
)

if not exist external\miniz (
    echo [*] Downloading miniz...
    mkdir external\miniz
    curl -L -o external\miniz\miniz.zip https://github.com/richgel999/miniz/releases/download/3.0.2/miniz-3.0.2.zip
    cd external\miniz
    tar -xf miniz.zip miniz.h miniz.c
    del miniz.zip
    cd ..\..
)

if not exist external\android-ndk-r26d (
    echo [*] Downloading Android NDK r26d ^(this may take a while^)...
    curl -L -o external\android-ndk-r26d-windows.zip https://dl.google.com/android/repository/android-ndk-r26d-windows.zip
    echo [*] Extracting NDK...
    powershell -Command "Expand-Archive -Path external\android-ndk-r26d-windows.zip -DestinationPath external -Force"
    del external\android-ndk-r26d-windows.zip
)

echo [*] Dependencies ready!
