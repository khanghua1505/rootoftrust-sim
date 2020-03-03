#
# \file         FindYder.cmake
#
# \brief        Try to find the low-level libyder library.
#               Once done this will define:
#               <ul>
#                   <li>YDER_FOUND - System has libyder.</li>
#                   <li>YDER_INCLUDE_DIR - The libyder include directory.</li>
#                   <li>YDER_LIBRARY - The libraries need to use libyder.<li>
#               </ul>
# Copyright (c) 2019, DESLAB, Khang Hua, khanghua1505@gmail.com
#

if (YDER_INCLUDE_DIR AND YDER_LIBRARY)
    set(YDER_FOUND TRUE)
else()
    if (WIN32)
        message(FATAL_ERROR "This script is not supported for Windows")
    else()
        find_package(PkgConfig REQUIRED)
        if (PKG_CONFIG_FOUND)
            pkg_check_modules(_YDER QUIET libyder)
        endif()
        
        find_path(YDER_INCLUDE_DIR yder.h
            ${_YDER_INCLUDE_DIRS}
            "/usr/include"
            "/usr/local/include")
        
        find_library(YDER_LIBRARY NAMES yder
            PATHS
              ${_YDER_LIBRARIES}
              "/usr/lib"
              "/usr/local/lib")
              
        if(YDER_INCLUDE_DIR AND YDER_LIBRARY)
            set(YDER_FOUND TRUE)
        endif()
        
        if(YDER_FOUND)
            if(Yder_FIND_REQUIRED)
                message(STATUS "Found yder library: ${YDER_LIBRARY}")
            endif()
        else()
            if(Yder_FIND_REQUIRED)
                message(FATAL_ERROR "Could not find libyder")
            endif()
        endif()
        mark_as_advanced(YDER_INCLUDE_DIR YDER_LIBRARY) 
    endif()
endif()
