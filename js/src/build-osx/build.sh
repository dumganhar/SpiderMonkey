#!/bin/sh

cpus=$(sysctl hw.ncpu | awk '{print $2}')

configure
../configure --disable-tests --disable-shared-js \
            --disable-debug \
            --without-intl-api \
            --disable-threadsafe
# make
xcrun make -j$cpus

# strip
xcrun strip -S js/src/libjs_static.a

# info
xcrun lipo -info js/src/libjs_static.a

