
# options
RELEASE_DIR="dist-android"

set -x

host_os=`uname -s | tr "[:upper:]" "[:lower:]"`
host_arch=`uname -m`

export OLD_CONFIGURE=../js/src/old-configure

rm -rf $RELEASE_DIR

build_with_arch()
{

# NDK_ROOT=$HOME/bin/android-ndk
if [[ ! $NDK_ROOT ]]; then
    echo "You have to define NDK_ROOT"
    exit 1
fi

rm -rf dist
rm -f ./config.cache

python ../configure.py \
            --enable-project=js \
            --with-android-ndk=$NDK_ROOT \
            --with-android-sdk=$HOME/bin/android-sdk \
            --with-android-version=${ANDROID_VERSION} \
            --with-arch=${CPU_ARCH} \
            --target=${TARGET_NAME} \
            --disable-shared-js \
            --disable-tests \
            --enable-strip \
            --enable-install-strip \
            --disable-debug \
            --without-intl-api \
            --with-system-zlib \
            ${EXTRA_ARGS}

make -j8

# copy specific files from dist
rm -rf "$RELEASE_DIR/$RELEASE_ARCH_DIR/include"
rm -rf "$RELEASE_DIR/$RELEASE_ARCH_DIR/libs"
mkdir -p "$RELEASE_DIR/$RELEASE_ARCH_DIR/include"
cp -RL dist/include/* "$RELEASE_DIR/$RELEASE_ARCH_DIR/include/"
mkdir -p "$RELEASE_DIR/$RELEASE_ARCH_DIR/libs"
cp -L js/src/build/libjs_static.a "$RELEASE_DIR/$RELEASE_ARCH_DIR/libs/libjs_static.a"
cp -L mozglue/build/libmozglue.a "$RELEASE_DIR/$RELEASE_ARCH_DIR/libs/libmozglue.a"

# strip unneeded symbols
STRIP=$NDK_ROOT/toolchains/${TOOLS_ARCH}-${GCC_VERSION}/prebuilt/${host_os}-${host_arch}/bin/${TOOLNAME_PREFIX}-strip

$STRIP --strip-unneeded "$RELEASE_DIR/$RELEASE_ARCH_DIR/libs/libjs_static.a"
$STRIP --strip-unneeded "$RELEASE_DIR/$RELEASE_ARCH_DIR/libs/libmozglue.a"

}

# # Build with armv6
# TOOLS_ARCH=arm-linux-androideabi
# TARGET_NAME=arm-linux-androideabi
# CPU_ARCH=armv6
# RELEASE_ARCH_DIR=armeabi
# GCC_VERSION=4.9
# ANDROID_VERSION=9
# TOOLNAME_PREFIX=arm-linux-androideabi
# build_with_arch

# Build with armv7
TOOLS_ARCH=arm-linux-androideabi
TARGET_NAME=arm-linux-androideabi
CPU_ARCH=armv7
RELEASE_ARCH_DIR=armeabi-v7a
GCC_VERSION=4.9
ANDROID_VERSION=14
TOOLNAME_PREFIX=arm-linux-androideabi
EXTRA_ARGS=--disable-jemalloc
build_with_arch

# # Build with arm64
# TOOLS_ARCH=aarch64-linux-android
# TARGET_NAME=aarch64-linux-android
# CPU_ARCH=armv8-a
# RELEASE_ARCH_DIR=arm64-v8a
# GCC_VERSION=4.9
# ANDROID_VERSION=21
# TOOLNAME_PREFIX=aarch64-linux-android
# EXTRA_ARGS=--disable-jemalloc
# build_with_arch


# # Build with x86
# TOOLS_ARCH=x86
# TARGET_NAME=i686-linux-android
# CPU_ARCH=i686
# RELEASE_ARCH_DIR=x86
# GCC_VERSION=4.9
# ANDROID_VERSION=9
# TOOLNAME_PREFIX=i686-linux-android
# build_with_arch
