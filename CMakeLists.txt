cmake_minimum_required(VERSION 3.10)
project(rootoftrust_sim C)

set(CMAKE_C_STANDARD        99)
set(CMAKE_MODULE_PATH       ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(INTERFACE "Ethernet" CACHE STRING "")
set(ROOTDIR "${CMAKE_SOURCE_DIR}" CACHE STRING "")

set(SOURCES "" CACHE STRING "source files")
set(INCDIRS "" CACHE STRING "include dris")
set(LIBS "" CACHE STRING "libraries")
set(DEFINITIONS "" CACHE STRING "compiler definitions")

if("${INTERFACE}" STREQUAL "Serial")
    find_package(SerialPort REQUIRED)
    list(APPEND INCDIRS "${SERIALPORT_INCLUDE_DIR}")
    list(APPEND LIBS "${SERIALPORT_LIBRARY}")    
    list(APPEND SOURCES "src/interface/serial/serial.c")
    list(APPEND INCDIRS "src/interface/serial")
    set(DEFINITIONS "${DEFINITIONS} -DCONFIG_INTERFACE_SERIAL")
elseif("${INTERFACE}" STREQUAL "Ethernet")
    list(APPEND SOURCES "src/interface/ethernet/ethernet.c")
    list(APPEND INCDIRS "src/interface/ethernet")
    set(DEFINITIONS "${DEFINITIONS} -DCONFIG_INTERFACE_ETHERNET")
else()
    message(FATAL_ERROR "${INTERFACE} is not supported. The program only support: Serial or Ethernet")
endif()

find_package(Yder REQUIRED)
list(APPEND INCDIRS "${YDER_INCLUDE_DIR}")
list(APPEND LIBS "${YDER_LIBRARY}")

set(SHA3_SRC "${ROOTDIR}/lib/sha3/sha3.c")

set(ED25519_SRC "lib/ed25519/fe.c")
list(APPEND ED25519_SRC "lib/ed25519/fe.c")
list(APPEND ED25519_SRC "lib/ed25519/ge.c")
list(APPEND ED25519_SRC "lib/ed25519/fe.c")
list(APPEND ED25519_SRC "lib/ed25519/keypair.c")
list(APPEND ED25519_SRC "lib/ed25519/sc.c")
list(APPEND ED25519_SRC "lib/ed25519/sign.c")
list(APPEND ED25519_SRC "lib/ed25519/verify.c")

list(APPEND SOURCES "src/cmd_handler.c")
list(APPEND SOURCES "src/keycore/keycore.c")
list(APPEND SOURCES "src/keycore/keys.c")
list(APPEND SOURCES "src/utils.c")
list(APPEND SOURCES "src/io.c")
list(APPEND SOURCES "src/main.c")
list(APPEND SOURCES "${SHA3_SRC}")
list(APPEND SOURCES "${ED25519_SRC}")

list(APPEND INCDIRS "${ROOTDIR}")
list(APPEND INCDIRS "${ROOTDIR}/src")
list(APPEND INCDIRS "${ROOTDIR}/src/keycore")
list(APPEND INCDIRS "${ROOTDIR}/lib/sha3")
list(APPEND INCDIRS "${ROOTDIR}/lib/ed25519")

add_definitions(${DEFINITIONS})

include_directories(${INCDIRS})

add_executable(${PROJECT_NAME}  ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

message(STATUS "Build interface: ${INTERFACE}")

