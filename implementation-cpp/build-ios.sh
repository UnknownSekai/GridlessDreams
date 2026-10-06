#!/bin/bash
set -e

if [ -z "$THEOS" ]; then
    export THEOS=~/theos
fi

if [ ! -d "$THEOS" ]; then
    echo "ERROR: Theos not found at $THEOS"
    exit 1
fi

./setup-deps.sh

echo "[*] Building iOS dylib via Theos..."
make clean
make FINALPACKAGE=1

echo "[*] Copying output..."
mkdir -p output
find .theos -name "libdreams.dylib" ! -path "*debug*" ! -path "*/arm64/*" ! -path "*/arm64e/*" -exec cp {} output/libdreams.dylib \;

if [ -f output/libdreams.dylib ]; then
    file output/libdreams.dylib

    echo "[*] Cleaning build files..."
    make clean 2>/dev/null
    rm -rf .theos packages

    echo "[*] Output: output/libdreams.dylib"
    echo "[*] Done!"
else
    echo "ERROR: Build failed"
    exit 1
fi
