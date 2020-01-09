#
# \file         FindSerialPort.cmake
#
# \brief        Try to find the low-level libserialport library.
#               Once done this will define:
#               <ul>
#                   <li>SERIALPORT_FOUND - System has libserialport.</li>
#                   <li>SERIALPORT_INCLUDE_DIR - The libserialport include directory.</li>
#                   <li>SERIALPORT_LIBRARY - The libraries need to use libserialport.<li>
#               </ul>
# Copyright (c) 2019, DESLAB, Khang Hua, khanghua1505@gmail.com
#

if (SERIALPORT_INCLUDE_DIR AND SERIALPORT_LIBRARY)
    set(SERIALPORT_FOUND TRUE)
else()
    if (WIN32)
        message(FATAL_ERROR "This script is not supported for Windows")
    else()
        find_package(PkgConfig REQUIRED)
        if (PKG_CONFIG_FOUND)
            pkg_check_modules(_SERIALPORT QUIET serialport)
        endif()  # PKG_CONFIG_FOUND

        find_path(SERIALPORT_INCLUDE_DIR libserialport.h
                ${_SERIALPORT_INCLUDE_DIRS}
                "/usr/include"
                "/usr/local/include")

        find_library(SERIALPORT_LIBRARY NAMES serialport
                PATHS
                ${_SERIALPORT_LIBRARIES}
                "/usr/lib"
                "/usr/local/lib")

        if (SERIALPORT_INCLUDE_DIR AND SERIALPORT_LIBRARY)
            set(SERIALPORT_FOUND TRUE)
        endif()  # SERIALPORT_INCLUDE_DIR AND SERIALPORT_LIBRARIES

        if (SERIALPORT_FOUND)
            if (!Serialport_FIND_QUIETLY)
                message(STATUS "Found libserialport: ${SERIALPORT_LIBRARY}")
            endif()  # NOT SERIALPORT_FIND_QUIETLY
        else()
            if (Serialport_FIND_REQUIRED)
                message(FATAL_ERROR "Could not find SERIALPORT")
            endif()  # SERIALPORT_FIND_REQUIRED
        endif()  # SERIALPORT_FOUND
        mark_as_advanced(SERIALPORT_INCLUDE_DIR SERIALPORT_LIBRARY)
    endif()  # WIN32
endif()  # SERIALPORT_INCLUDE_DIR AND SERIALPORT_LIBRARY
