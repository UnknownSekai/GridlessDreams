#pragma once
// httplib's win32 mmap path uses CreateFile2 (Win8+); target it so the header
// compiles under MinGW, whose default _WIN32_WINNT leaves CreateFile2 undeclared.
// no effect on the NDK/clang android/ios builds (that path is win32-only).
#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0A00
#endif
#include "httplib.h"
