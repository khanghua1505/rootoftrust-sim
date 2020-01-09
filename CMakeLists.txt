cmake_minimum_required(VERSION 3.15)
project(rootoftrust_sim C)

set(CMAKE_C_STANDARD        99)
set(CMAKE_MODULE_PATH       ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(SerialPort REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/sha3
    ${CMAKE_SOURCE_DIR}/lib/ed25519
    ${CMAKE_SOURCE_DIR}/src
    ${SERIALPORT_INCLUDE_DIR})

set(SOURCES
    src/cmd_handler.c
    src/keycore.c
    src/keys.c
    src/main.c
    src/serial.c
    src/system.c
    src/utils.c)

set(LIB_SHA3_SRC
    lib/sha3/sha3.c)

set(LIB_ED25519_SRC
    lib/ed25519/fe.c
    lib/ed25519/ge.c
    lib/ed25519/keypair.c
    lib/ed25519/sc.c
    lib/ed25519/sign.c
    lib/ed25519/verify.c)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${LIB_SHA3_SRC}
    ${LIB_ED25519_SRC})

target_link_libraries(${PROJECT_NAME} ${SERIALPORT_LIBRARY})