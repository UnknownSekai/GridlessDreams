@echo off
REM ============================================================================
REM build_all.bat - Build both native libraries, then patch both store packages.
REM
REM Flow:
REM   1. cd implementation-cpp   (both builds use relative paths / setup-deps)
REM   2. build-android.bat       -> implementation-cpp\output\libdreams.so
REM   3. wsl bash build-ios.sh   -> implementation-cpp\output\libdreams.dylib
REM   4. cd back to the original working directory
REM   5. patch original.apk -> GridlessDreams.apk   (inject the .so)
REM   6. patch original.ipa -> GridlessDreams.ipa   (inject the .dylib)
REM
REM The Apple build runs inside WSL ("bash build-ios.sh"). Both native builds must
REM run with the working directory set to implementation-cpp, so we cd there for
REM steps 2-3 and restore the caller's directory before the patch steps.
REM ============================================================================
setlocal enableextensions

REM --- project layout (resolved from this script's own location) ---
set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"
set "CPP=%ROOT%\implementation-cpp"

REM --- hardcoded inputs / outputs (live at the project root) ---
set "ANDROID_IN=%ROOT%\original.apk"
set "ANDROID_OUT=%ROOT%\GridlessDreams.apk"
set "IOS_IN=%ROOT%\original.ipa"
set "IOS_OUT=%ROOT%\GridlessDreams.ipa"

REM --- native library artifacts the builds produce ---
set "SO=%CPP%\output\libdreams.so"
set "DYLIB=%CPP%\output\libdreams.dylib"

REM --- remember where we started so we can restore it before patching ---
set "ORIGDIR=%CD%"

echo ============================================================
echo [build_all] 1/4  Android build  (build-android.bat)
echo ============================================================
cd /d "%CPP%"
call build-android.bat
if errorlevel 1 goto fail_build

echo ============================================================
echo [build_all] 2/4  iOS build  (wsl bash build-ios.sh)
echo ============================================================
wsl bash build-ios.sh
if errorlevel 1 goto fail_build

REM --- restore the caller's working directory before patching ---
cd /d "%ORIGDIR%"

echo ============================================================
echo [build_all] 3/4  Patch Android  original.apk -^> GridlessDreams.apk
echo ============================================================
python "%ROOT%\scripts\patch.py" "%ANDROID_IN%" -o "%ANDROID_OUT%" --inject "%SO%"
if errorlevel 1 goto fail_patch

echo ============================================================
echo [build_all] 4/4  Patch iOS  original.ipa -^> GridlessDreams.ipa
echo ============================================================
python "%ROOT%\scripts\patch.py" "%IOS_IN%" -o "%IOS_OUT%" --inject "%DYLIB%"
if errorlevel 1 goto fail_patch

echo ============================================================
echo [build_all] DONE
echo   Android: %ANDROID_OUT%
echo   iOS:     %IOS_OUT%
echo ============================================================
cd /d "%ORIGDIR%"
endlocal
exit /b 0

:fail_build
echo [build_all] ERROR: a native build step failed - aborting before patching.
cd /d "%ORIGDIR%"
endlocal
exit /b 1

:fail_patch
echo [build_all] ERROR: a patch step failed.
cd /d "%ORIGDIR%"
endlocal
exit /b 1
