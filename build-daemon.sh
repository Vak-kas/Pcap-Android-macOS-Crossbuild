#!/bin/bash
NDK_BIN=~/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin
SYSROOT=$NDK_BIN/../../sysroot

$NDK_BIN/aarch64-linux-android28-clang++ \
    -fPIE -pie \
    ./root/mjDaemon.cpp \
    ./root/handler.cpp \
    ./pcapmanager.cpp \
    ./common.cpp \
    -I../libpcap/out/android/include \
    -L../libpcap/out/android/lib \
    -lpcap \
    -o mjDaemon \
    -static-libstdc++ \
    -O2

echo "[Daemon build done]"