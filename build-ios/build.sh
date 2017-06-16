#!/bin/sh

## this script is supposed to be run one directory below the original configure script
## usually in build-ios

MIN_IOS_VERSION=7.0
IOS_SDK=10.3

LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

cpus=$(sysctl hw.ncpu | awk '{print $2}')

export OLD_CONFIGURE=../js/src/old-configure

# remove everything but the static library and this script
ls | grep -v build.sh | xargs rm -rf

#
# create i386 version (simulator)
#
python ../configure.py \
            --enable-project=js \
            --with-toolchain-prefix="`xcode-select --print-path`/Toolchains/XcodeDefault.xctoolchain/usr/bin/" \
            --target=i386-apple-darwin \
            --with-ios-target=iPhoneSimulator --with-ios-min-version=$MIN_IOS_VERSION --with-ios-arch=i386 \
            --disable-shared-js --disable-tests --disable-ion --disable-debug \
            --enable-optimize=-O3 --enable-strip --enable-install-strip --with-thumb=no --without-intl-api
            
make -j$cpus
if (( $? )) ; then
    echo "error when compiling i386 (iOS Simulator) version of the library"
    exit
fi
mkdir -p ./dist/temp/
mv ./mozglue/build/libmozglue.a ./dist/temp/libmozglue.i386.a
mv ./js/src/libjs_static.a ./dist/temp/libjs_static.i386.a

#
# create x86_64 version (simulator)
#
python ../configure.py \
            --enable-project=js \
            --with-toolchain-prefix="`xcode-select --print-path`/Toolchains/XcodeDefault.xctoolchain/usr/bin/" \
            --target=x86_64-apple-darwin \
            --with-ios-target=iPhoneSimulator --with-ios-min-version=$MIN_IOS_VERSION --with-ios-arch=x86_64 \
            --disable-shared-js --disable-tests --disable-ion --disable-debug \
            --enable-optimize=-O3 --enable-strip --enable-install-strip --with-thumb=no --without-intl-api

make -j$cpus
if (( $? )) ; then
    echo "error when compiling x86_64 (iOS Simulator) version of the library"
    exit
fi
mkdir -p ./dist/temp/
mv ./mozglue/build/libmozglue.a ./dist/temp/libmozglue.x86_64.a
mv ./js/src/libjs_static.a ./dist/temp/libjs_static.x86_64.a

#
# create ios version (armv7)
#
python ../configure.py \
            --enable-project=js \
            --with-toolchain-prefix="`xcode-select --print-path`/Toolchains/XcodeDefault.xctoolchain/usr/bin/" \
            --target=armv7-apple-darwin \
            --with-ios-target=iPhoneOS --with-ios-min-version=$MIN_IOS_VERSION --with-ios-arch=armv7 \
            --disable-shared-js --disable-tests --disable-ion --disable-debug \
            --enable-optimize=-O3 --enable-strip --enable-install-strip --with-thumb=no --without-intl-api
      
make -j$cpus
if (( $? )) ; then
    echo "error when compiling armv7 (iOS version) of the library"
    exit
fi
mkdir -p ./dist/temp/
mv ./mozglue/build/libmozglue.a ./dist/temp/libmozglue.armv7.a
mv ./js/src/libjs_static.a ./dist/temp/libjs_static.armv7.a


#
# create ios version (arm64)
#
python ../configure.py \
            --enable-project=js \
            --with-toolchain-prefix="`xcode-select --print-path`/Toolchains/XcodeDefault.xctoolchain/usr/bin/" \
            --target=arm64-apple-darwin \
            --with-ios-target=iPhoneOS --with-ios-min-version=$MIN_IOS_VERSION --with-ios-arch=arm64 \
            --disable-shared-js --disable-tests --disable-ion --disable-debug \
            --enable-optimize=-O3 --enable-strip --enable-install-strip --with-thumb=no --without-intl-api

make -j$cpus
if (( $? )) ; then
   echo "error when compiling arm64 (iOS version) of the library"
   exit
fi
mkdir -p ./dist/temp/
mv ./mozglue/build/libmozglue.a ./dist/temp/libmozglue.arm64.a
mv ./js/src/libjs_static.a ./dist/temp/libjs_static.arm64.a

#
# lipo create
#

if [ -e dist/temp/libjs_static.i386.a ] && [ -e dist/temp/libjs_static.x86_64.a ] && [ -e dist/temp/libjs_static.armv7.a ] && [ -e dist/temp/libjs_static.arm64.a ] ; then
    echo "creating fat version of the library"
    $LIPO -create -output dist/sdk/libjs_static.a dist/temp/libjs_static.i386.a dist/temp/libjs_static.x86_64.a dist/temp/libjs_static.armv7.a dist/temp/libjs_static.arm64.a
    $LIPO -create -output dist/sdk/libmozglue.a dist/temp/libmozglue.i386.a dist/temp/libmozglue.x86_64.a dist/temp/libmozglue.armv7.a dist/temp/libmozglue.arm64.a
    # remove debugging info
    $STRIP -S dist/sdk/libjs_static.a
    $STRIP -S dist/sdk/libmozglue.a
    $LIPO -info dist/sdk/libjs_static.a
    $LIPO -info dist/sdk/libmozglue.a
fi

#
# done
#
echo "*** DONE ***"
echo "If you want to use spidermonkey, copy the 'dist' directory to some accesible place"
echo "e.g. 'cp -pr dist ~/path/to/your/project'"
echo "and then add the proper search paths for headers and libraries in your Xcode project"
