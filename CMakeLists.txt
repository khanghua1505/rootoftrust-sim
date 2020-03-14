################################################################################
# BASIC SETUP
################################################################################

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

################################################################################
# BUILD SHA3 LIBRARY
################################################################################

set(SHA3_SOURCE_DIR    ${ROOTDIR}/lib/sha3)
set(SHA3_SOURCE_FILES  ${SHA3_SOURCE_DIR}/sha3.c)

add_library(libsha3 ${SHA3_SOURCE_FILES})

target_include_directories(libsha3 PUBLIC ${SHA3_SOURCE_DIR})

################################################################################
# BUILD SHA3 LIBRARY
################################################################################

set(ED25519_SOURCE_DIR    ${ROOTDIR}/lib/ed25519)
set(ED25519_SOURCE_FILES  "")

list(APPEND ED25519_SOURCE_FILES  ${ED25519_SOURCE_DIR}/fe.c)
list(APPEND ED25519_SOURCE_FILES  ${ED25519_SOURCE_DIR}/ge.c)
list(APPEND ED25519_SOURCE_FILES  ${ED25519_SOURCE_DIR}/keypair.c)
list(APPEND ED25519_SOURCE_FILES  ${ED25519_SOURCE_DIR}/sc.c)
list(APPEND ED25519_SOURCE_FILES  ${ED25519_SOURCE_DIR}/sign.c)
list(APPEND ED25519_SOURCE_FILES  ${ED25519_SOURCE_DIR}/verify.c)

add_library(libed25519 ${ED25519_SOURCE_FILES})

target_include_directories(libed25519 PUBLIC ${ED25519_SOURCE_DIR})

################################################################################
# GENERATE KEY PROGRAM
################################################################################

set(GENERATEKEY_SOURCE_DIR    ${ROOTDIR}/tool/generate_key)
set(GENERATEKEY_SOURCE_FILES  ${GENERATEKEY_SOURCE_DIR}/main.c)

add_executable(generate_key  ${GENERATEKEY_SOURCE_FILES})
target_link_libraries(generate_key libsha3 libed25519)

################################################################################
# BUILDTRUST SIMULATION PROGRAM
################################################################################

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

message(STATUS "Build interface: ${INTERFACE}")

add_definitions(${DEFINITIONS})
include_directories(${INCDIRS})

add_executable(rootoftrust_sim  ${SOURCES})
target_link_libraries(rootoftrust_sim libsha3 libed25519)

if (WIN32) 
    set(COPY_CMD    "copy")
else()
    set(COPY_CMD    "cp")
endif()

add_custom_command(
    TARGET rootoftrust_sim
    POST_BUILD
    COMMAND ${COPY_CMD} ${ROOTDIR}/tool/run.sh ${CMAKE_BINARY_DIR}
)



