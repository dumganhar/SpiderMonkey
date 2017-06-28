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

rm -f ./dist/sdk/lib/libmozglue.a
cp ./mozglue/build/libmozglue.a ./dist/sdk/lib/
cp ./js/src/libjs_static.a ./dist/sdk/lib/

# strip
xcrun strip -S ./dist/sdk/lib/libjs_static.a
xcrun strip -S ./dist/sdk/lib/libmozglue.a

rm -rf ./dist-osx
cp -pr ./dist ./dist-osx
