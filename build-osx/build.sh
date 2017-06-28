#!/bin/sh

cpus=$(sysctl hw.ncpu | awk '{print $2}')

export OLD_CONFIGURE=../js/src/old-configure

# configure
python ../configure.py \
            --enable-project=js \
            --enable-optimize=-O3 \
            --disable-shared-js \
            --disable-tests \
            --disable-debug \
            --with-thumb=no \
            --without-intl-api
            

# make
xcrun make -j$cpus

rm -rf ./dist/libs
mkdir -p ./dist/libs
cp ./mozglue/build/libmozglue.a ./dist/libs/
cp ./js/src/libjs_static.a ./dist/libs/

# strip
xcrun strip -S ./dist/libs/libjs_static.a
xcrun strip -S ./dist/libs/libmozglue.a

rm -rf ./dist-osx
cp -pr ./dist ./dist-osx
