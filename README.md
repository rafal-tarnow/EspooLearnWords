TEST

kompilacja mqqt, ściągnąć repozytorium, przełączyć się na branch, otworzyć Cmake w qtreator, wybrać zestaw narzędzi, potem projekty→WebAssembly 6.4.1 (Budowanie) → Kroki build → Szegóły →Zaznaczyć produkty docelowe all i install, przejść do projektu, Zbuduj







//------------- kompilacja QT

git clean -d -f -x && git reset --hard HEAD //czyci wszystkie pliki ktróe nie sa śledzone

sudo apt-get install libfontconfig1-dev libfreetype6-dev libx11-dev libx11-xcb-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev libxcb-util-dev libxcb-xinerama0-dev libxcb-xkb-dev libxkbcommon-dev libxkbcommon-x11-dev libatspi2.0-dev


./configure -help
configure -list-features
configure -no-feature-accessibility

./configure -release -opensource -confirm-license -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_build/
 ./configure -release -opensource -no-icu -confirm-license -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_build/
 
 cmake --build . --parallel
 
 cmake --install .
 
 
 //------------ emscripten
 1. set env variables:
 source /path/to/emsdk/emsdk_env.sh
 source /home/rkruszyna/Dokumenty/OPEN_SOURCES/emsdk/emsdk_env.sh
 
 
 //cmake path
 export PATH="/home/rkruszyna/Qt_6_4_2/Tools/CMake/bin/:$PATH"
 
 emmake ./config -no-asm -no-threads? -no-shared?
 emmake ./config -no-asm -no-threads -no-shared --prefix=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm --openssldir=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm
 otworzyć plik Makefile oraz configdata.pm, i zmienić ścieżki do narzędzi tak żeby było: (usunąć ścieżkę /home/rkruszyna/Dokumenty/OPEN_SOURCES/emsdk)
 CROSS_COMPILE=em
CC=emcc
CXX=em++
 
 
 
emmake make
 
 
 
./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build/

./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -openssl  -openssl-linked   -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build/

./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -openssl-linked -I'~/Qt_6_4_2/Tools/OpenSSL/src/include' -L'~/Qt_6_4_2/Tools/OpenSSL/src/'  -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build/ -- -DOPENSSL_ROOT_DIR='~/Qt_6_4_2/Tools/OpenSSL/src/' -D QT_FEATURE_openssl_linked="ON"

./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -openssl-linked -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build/ -- -D OPENSSL_ROOT_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src -D OPENSSL_CRYPTO_LIBRARY=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src -D OPENSSL_INCLUDE_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src/include


 ./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -openssl  -openssl-linked -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build/ -- -D OPENSSL_ROOT_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src -D OPENSSL_CRYPTO_LIBRARY=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src/libcrypto.a -D OPENSSL_INCLUDE_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src/include

./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -static -openssl -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build/ -- -D OPENSSL_ROOT_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src -D OPENSSL_CRYPTO_LIBRARY=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src/libcrypto.a -D OPENSSL_INCLUDE_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/src/include OPENSSL_USE_STATIC_LIBS=TRUE

1 ./configure -qt-host-path /home/rkruszyna/Qt_6_4_2/6.4.2/gcc_64/ -openssl-linked -platform wasm-emscripten -prefix /home/rkruszyna/Qt_6_4_2/6.4.2/my_wasm_build_1/ -- -D OPENSSL_ROOT_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm -D OPENSSL_SSL_LIBRARY=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm/lib/libssl.a -D OPENSSL_CRYPTO_LIBRARY=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm/lib/libcrypto.a -D OPENSSL_INCLUDE_DIR=/home/rkruszyna/Qt_6_4_2/Tools/OpenSSL/build_lib_wasm/include -D OPENSSL_USE_STATIC_LIBS=TRUE



 
 "configure.bat -v -static -opensource -confirm-license -debug-and-release -platform win32-g++ -prefix $QtDir -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -opengl desktop -openssl -I "C:\Qt\Tools\OpenSSL\src\include" -L "C:\Qt\Tools\OpenSSL\Win_x64\lib" OPENSSL_LIBS="-llibssl -llibcrypto" -make libs -nomake tools -nomake examples -nomake tests "
 
 
 
 

                                                                  ~~~~~~~~~~~~^



