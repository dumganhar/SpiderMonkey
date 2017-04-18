#!/bin/sh

cpus=$(sysctl hw.ncpu | awk '{print $2}')

export OLD_CONFIGURE=../js/src/old-configure

# configure
python ../configure.py \
            --enable-project=js \
            --disable-shared-js --disable-tests --enable-llvm-hacks \
            --enable-optimize=-O3 --with-thumb=yes --enable-strip --enable-install-strip --without-intl-api --disable-debug

# make
xcrun make -j$cpus

rm -f ./dist/sdk/lib/libmozglue.a
cp ./mozglue/build/libmozglue.a ./dist/sdk/lib/
cp ./js/src/libjs_static.a ./dist/sdk/lib/

# strip
xcrun strip -S ./dist/sdk/lib/libjs_static.a
xcrun strip -S ./dist/sdk/lib/libmozglue.a
