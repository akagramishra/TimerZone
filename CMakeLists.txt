cmake_minimum_required(VERSION 3.15)
project(TimerZone VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

set(SOURCES
    main.cpp
    src/network.cpp
    src/app.cpp
    src/ui.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    CURL_STATICLIB
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    "C:/raylib/raylib/src"
    "C:/msys64/ucrt64/include"
)

target_link_directories(${PROJECT_NAME} PRIVATE
    "C:/raylib/raylib/src"
    "C:/msys64/ucrt64/lib"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    C:/msys64/ucrt64/lib/libcurl.a
    ssh2
    nghttp2
    nghttp3
    ngtcp2
    ngtcp2_crypto_ossl
    brotlidec
    brotlicommon
    zstd
    z
    ssl
    crypto
    idn2
    psl
    C:/msys64/ucrt64/lib/libunistring.a
    C:/msys64/ucrt64/lib/libiconv.a
    ws2_32
    wldap32
    crypt32
    bcrypt
    secur32
    mswsock
    iphlpapi
    opengl32
    gdi32
    winmm
)

target_link_options(${PROJECT_NAME} PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
)

file(COPY ${CMAKE_SOURCE_DIR}/Res DESTINATION ${CMAKE_BINARY_DIR})