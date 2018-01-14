# Locate libwlroots
#
# This module defines
# - WLROOTS_FOUND
# - WLROOTS_LIBRARY
# - WLROOTS_INCLUDE_DIR
#
# Author: Simon Zeni
# email: simonzeni@gmail.com 
#

find_path(WLROOTS_INCLUDE_DIR
    NAMES
        wlr/
    HINTS
        "${WLROOTS_LOCATION}/include"
    PATHS
        /usr/local/include/
        /usr/include/
    DOC
        "wlroots include directory"
)

find_library(WLROOTS_LIBRARY
    NAMES
        wlroots
    HINTS
        "${WLROOTS_LOCATION}/lib"
    PATHS
        /usr/local/lib
        /usr/local/lib64
        /usr/lib/
        /usr/lib64/
    DOC
        "libwlroots path"
)

if(WLROOTS_LIBRARY AND WLROOTS_INCLUDE_DIR)
    set(WLROOTS_FOUND TRUE)
    message("-- Found WLROOTS_LIBRARY: ${WLROOTS_LIBRARY}")
else()
    set(WLROOTS_FOUND FALSE)
    if(wlroots_FIND_REQUIRED)
        message(SEND_ERROR "Unable to find wlroots library")
    endif()
endif()
