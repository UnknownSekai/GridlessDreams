@echo off
REM build Android arm64-v8a shared library
REM NDK r26d's android.toolchain.cmake does not work with CMake 4.x (it fails to
REM pass --target, so the standard headers aren't found). We drive the NDK clang
REM directly as a generic aarch64 cross-compile instead, which is version-proof.
REM
REM -femulated-tls: a native-bridge translator (houdini/LDPlayer, ARM->x86) DEADLOCKS
REM (or faults in its own JIT memory) on first-use init of a thread_local with a
REM constructor on a worker thread when native ELF TLS is used. cpp-httplib uses
REM thread_local for request-path statics (builtin_methods, regexes, RNG), so a native-TLS
REM build hangs/crashes handling every request -- the client gets no response and login
REM fails. Emulated TLS routes through __emutls_get_address (pulled from compiler-rt
REM builtins; -Wl,--no-undefined guarantees it resolves at link) and works under houdini.
REM This matches the proven GS6 build; do NOT switch to -fno-emulated-tls.
REM -static-libstdc++: the game does not bundle libc++_shared.so (Unity static-links its
REM own STL), so a dynamic libc++ leaves libdreams.so with an unresolvable DT_NEEDED and
REM dlopen fails to link it. Static libc++ makes the lib self-contained.
REM -Wl,--no-undefined turns any future unresolved symbol into a link error instead
REM of a silently broken shared library.
setlocal

if not defined ANDROID_NDK_HOME (
    if exist "%CD%\external\android-ndk-r26d" (
        set "ANDROID_NDK_HOME=%CD%\external\android-ndk-r26d"
    ) else (
        echo ERROR: ANDROID_NDK_HOME not set and external\android-ndk-r26d not found
        exit /b 1
    )
)

REM resolve to an absolute path (CMake rejects a relative compiler path)
for %%A in ("%ANDROID_NDK_HOME%") do set "ANDROID_NDK_HOME=%%~fA"

call setup-deps.bat

set "NDK_BIN=%ANDROID_NDK_HOME%\toolchains\llvm\prebuilt\windows-x86_64\bin"
if not exist "%NDK_BIN%\clang.exe" (
    echo ERROR: NDK clang not found at "%NDK_BIN%"
    exit /b 1
)

set "NINJA="
for /f "delims=" %%i in ('where ninja 2^>nul') do set "NINJA=%%i"
if not defined NINJA (
    echo ERROR: ninja not found in PATH. Install it, e.g. pip install ninja
    exit /b 1
)

echo [*] Building for Android arm64-v8a...
cmake -B build-android -G Ninja ^
    -DCMAKE_MAKE_PROGRAM="%NINJA%" ^
    -DCMAKE_SYSTEM_NAME=Linux ^
    -DCMAKE_SYSTEM_PROCESSOR=aarch64 ^
    -DCMAKE_C_COMPILER="%NDK_BIN%\clang.exe" ^
    -DCMAKE_CXX_COMPILER="%NDK_BIN%\clang++.exe" ^
    -DCMAKE_C_FLAGS="--target=aarch64-none-linux-android26 -femulated-tls" ^
    -DCMAKE_CXX_FLAGS="--target=aarch64-none-linux-android26 -femulated-tls" ^
    -DCMAKE_SHARED_LINKER_FLAGS="-Wl,--no-undefined -static-libstdc++" ^
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON ^
    -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 exit /b 1

if exist build-android\compile_commands.json copy /Y build-android\compile_commands.json compile_commands.json >nul

cmake --build build-android --config Release
if %ERRORLEVEL% neq 0 exit /b 1

if not exist output mkdir output
copy /Y build-android\libdreams.so output\libdreams.so >nul

echo [*] Cleaning build files...
rmdir /s /q build-android >nul 2>&1

echo [*] Output: output\libdreams.so
echo [*] Done!
endlocal
