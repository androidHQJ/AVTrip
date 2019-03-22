#!/bin/sh

#你自己的NDK路径。
NDK=/Users/heqijun/Documents/audio-video/android-practice/android-ndk-r15c
#PREFIX_ROOT=$(pwd)/android
PREFIX_ROOT=/Users/heqijun/Documents/audio-video/android-practice/FFPlayerDemo/app/src/main/jniLibs

CPU1=armv5te

CPU2=armv7-a
ARCH1=arm
TOOLCHAIN_NAME1=arm-linux-androideabi-4.9
TOOLCHAIN_NAME1_1=arm-linux-androideabi
CHAIN1=arm-linux-androideabi-

CPU3=armv8-a
ARCH2=aarch64
TOOLCHAIN_NAME2=aarch64-linux-android-4.9
TOOLCHAIN_NAME2_1=aarch64-linux-android
CHAIN2=aarch64-linux-android-

CPU4=i686
ARCH3=x86
TOOLCHAIN_NAME3=x86-4.9
TOOLCHAIN_NAME3_1=i686-linux-android
CHAIN3=i686-linux-android-

CPU5=x86_64
ARCH4=x86_64
TOOLCHAIN_NAME4=x86_64-4.9
TOOLCHAIN_NAME4_1=x86_64-linux-android
CHAIN4=x86_64-linux-android-

#设置你的android平台编译器的版本 
SYSROOT=$NDK/platforms/android-21
#编译使用的toolchain
TOOLCHAIN1=$NDK/toolchains/$TOOLCHAIN_NAME1/prebuilt/darwin-x86_64/bin/$CHAIN1
TOOLCHAIN2=$NDK/toolchains/$TOOLCHAIN_NAME2/prebuilt/darwin-x86_64/bin/$CHAIN2
TOOLCHAIN3=$NDK/toolchains/$TOOLCHAIN_NAME3/prebuilt/darwin-x86_64/bin/$CHAIN3
TOOLCHAIN4=$NDK/toolchains/$TOOLCHAIN_NAME4/prebuilt/darwin-x86_64/bin/$CHAIN4

function build_android {
./configure \
--target-os=android \
--enable-cross-compile \
--enable-shared \
--disable-static \
--enable-small \
--disable-programs \
--disable-doc \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-symver \
--disable-asm \
--enable-neon \
--prefix=$PREFIX_ROOT \
--libdir=$PREFIX_ROOT/${version} \
--incdir=$PREFIX_ROOT/includes/${version} \
--pkgconfigdir=${PREFIX_ROOT}/pkgconfig/${version} \
--arch=$arch \
--cpu=$cpu \
--cross-prefix="$cross_compile" \
--sysroot=$sysroot \

#--extra-cflags="$cflags"

make clean
make -j8 && make install
#只安装so文件
#make install-libs
};

#armeabi
version=armeabi
arch=$ARCH1
cpu=$CPU1
cross_compile="$TOOLCHAIN1"
sysroot=$SYSROOT/arch-$ARCH1
#cflags="-I$NDK/sysroot/usr/include/$TOOLCHAIN_NAME1_1 -isysroot $NDK/sysroot -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm -march=$CPU1"

build_android

#armeabi-v7a
version=armeabi-v7a
arch=$ARCH1
cpu=$CPU2
cross_compile="$TOOLCHAIN1"
sysroot=$SYSROOT/arch-$ARCH1
#cflags="-I$NDK/sysroot/usr/include/$TOOLCHAIN_NAME1_1 -isysroot $NDK/sysroot -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm -march=$CPU2"

build_android

#arm64-v8a
version=arm64-v8a
arch=$ARCH2
cpu=$CPU3
cross_compile="$TOOLCHAIN2"
sysroot=$SYSROOT/arch-arm64
#cflags=-"I$NDK/sysroot/usr/include/$TOOLCHAIN_NAME2_1 -isysroot $NDK/sysroot -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -march=$CPU3"

build_android

#x86
version=x86
arch=$ARCH3
cpu=$CPU4
cross_compile="$TOOLCHAIN3"
sysroot=$SYSROOT/arch-$ARCH3
#cflags="-I$NDK/sysroot/usr/include/$TOOLCHAIN_NAME3_1 -isysroot $NDK/sysroot -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -march=$CPU4"

build_android

#x86_64
version=x86_64
arch=$ARCH4
cpu=$CPU5
cross_compile="$TOOLCHAIN4"
sysroot=$SYSROOT/arch-$ARCH4
#cflags="-I$NDK/sysroot/usr/include/$TOOLCHAIN_NAME4_1 -isysroot $NDK/sysroot -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -march=$CPU5"

build_android