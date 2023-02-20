#!/bin/sh

OPENSSL="openssl-1.1.1d"

if [ -d ${OPENSSL} ]; then
  rm -rf ${OPENSSL}
fi

if [ ! -f ${OPENSSL}.tar.gz ]; then
  curl -O https://www.openssl.org/source/${OPENSSL}.tar.gz
fi

tar xf ${OPENSSL}.tar.gz
cd ${OPENSSL} || exit 1

emconfigure ./Configure \
  linux-generic64 \
  no-asm \
  no-threads \
  no-engine \
  no-hw \
  no-weak-ssl-ciphers \
  no-dtls \
  no-shared \
  no-dso \
  --prefix=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm \
  --openssldir=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm
  
  #  --prefix=$EMSCRIPTEN/system

sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile
sed -i '/^CFLAGS/ s/$/ -D__STDC_NO_ATOMICS__=1/' Makefile
sed -i '/^CXXFLAGS/ s/$/ -D__STDC_NO_ATOMICS__=1/' Makefile

emmake make -j 1 build_generated libssl.a libcrypto.a

cd ..
