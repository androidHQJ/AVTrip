#!/bin/sh

NDK_HOME=/Users/heqijun/Documents/audio-video/android-practice/android-ndk-r18b
PREFIX=android-build-all
HOST_PLATFORM=darwin-x86_64
PLATFORM=android-21

TOOLCHAIN_NAME1=arm-linux-androideabi
TOOLCHAIN_NAME2=aarch64-linux-android
TOOLCHAIN_NAME3=i686-linux-android
TOOLCHAIN_NAME4=x86_64-linux-android

#-isysroot $NDK_HOME/sysroot -mfloat-abi=softfp -march=$CPU

COMMON_OPTIONS="\
    --target-os=android \
    --disable-static \
    --enable-shared \
    --enable-small \
    --enable-cross-compile \
    --enable-neon \
    --disable-programs \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-doc \
    --disable-symver \
    --disable-asm \
    --enable-decoder=h264 \
    --enable-decoder=mpeg4 \
    --enable-decoder=mjpeg \
    --enable-decoder=png \
    --enable-decoder=vorbis \
    --enable-decoder=opus \
    --enable-decoder=flac 
    "

build_all(){
    for version in armeabi armeabi-v7a arm64-v8a x86 x86_64; do
        echo "======== > Start build $version"
        case ${version} in
        armeabi )
            ARCH="arm"
            CPU="armv5te"
            CROSS_PREFIX="$NDK_HOME/toolchains/arm-linux-androideabi-4.9/prebuilt/$HOST_PLATFORM/bin/arm-linux-androideabi-"
            SYSROOT="$NDK_HOME/platforms/$PLATFORM/arch-arm/"
            EXTRA_CFLAGS="-I${NDK_HOME}/sysroot/usr/include/${TOOLCHAIN_NAME1} -isysroot ${NDK_HOME}/sysroot"
        ;;
        armeabi-v7a )
            ARCH="arm"
            CPU="armv7-a"
            CROSS_PREFIX="$NDK_HOME/toolchains/arm-linux-androideabi-4.9/prebuilt/$HOST_PLATFORM/bin/arm-linux-androideabi-"
            SYSROOT="$NDK_HOME/platforms/$PLATFORM/arch-arm/"
            EXTRA_CFLAGS="-I${NDK_HOME}/sysroot/usr/include/${TOOLCHAIN_NAME1} -isysroot ${NDK_HOME}/sysroot"
        ;;
        arm64-v8a )
            ARCH="aarch64"
            CPU="armv8-a"
            CROSS_PREFIX="$NDK_HOME/toolchains/aarch64-linux-android-4.9/prebuilt/$HOST_PLATFORM/bin/aarch64-linux-android-"
            SYSROOT="$NDK_HOME/platforms/$PLATFORM/arch-arm64/"
            EXTRA_CFLAGS="-I${NDK_HOME}/sysroot/usr/include/${TOOLCHAIN_NAME2} -isysroot ${NDK_HOME}/sysroot"
        ;;
        x86 )
            ARCH="x86"
            CPU="i686"
            CROSS_PREFIX="$NDK_HOME/toolchains/x86-4.9/prebuilt/$HOST_PLATFORM/bin/i686-linux-android-"
            SYSROOT="$NDK_HOME/platforms/$PLATFORM/arch-x86/"
            EXTRA_CFLAGS="-I${NDK_HOME}/sysroot/usr/include/${TOOLCHAIN_NAME3} -isysroot ${NDK_HOME}/sysroot"
        ;;
        x86_64 )
            ARCH="x86_64"
            CPU="x86_64"
            CROSS_PREFIX="$NDK_HOME/toolchains/x86_64-4.9/prebuilt/$HOST_PLATFORM/bin/x86_64-linux-android-"
            SYSROOT="$NDK_HOME/platforms/$PLATFORM/arch-x86_64/"
            EXTRA_CFLAGS="-I${NDK_HOME}/sysroot/usr/include/${TOOLCHAIN_NAME4} -isysroot ${NDK_HOME}/sysroot"
        ;;
        esac

        echo "-------- > Start clean workspace"
        make clean

        echo "-------- > Start config makefile"
        configuration="\
            --prefix=${PREFIX} \
            --libdir=${PREFIX}/libs/${version} \
            --incdir=${PREFIX}/includes/${version} \
            --pkgconfigdir=${PREFIX}/pkgconfig/${version} \
            --arch=${ARCH} \
            --cpu=${CPU} \
            --cross-prefix=${CROSS_PREFIX} \
            --sysroot=${SYSROOT} \
            --extra-cflags="${EXTRA_CFLAGS}" \
            --extra-ldexeflags=-pie \
            ${COMMON_OPTIONS}
            "

        echo "-------- > Start config makefile with ${configuration}"
        ./configure ${configuration}

        echo "-------- > Start make ${version} with -j8"
        make j8

        echo "-------- > Start install ${version}"
        make install
        echo "++++++++ > make and install ${version} complete."

    done
}

echo "-------- Start --------"
build_all
echo "-------- End --------"