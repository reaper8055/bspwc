cmake_minimum_required(VERSION 3.10)

project(bway)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wextra -Wfatal-errors -Wpedantic -ggdb")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wno-warnings")

find_package(wlroots REQUIRED)

include_directories(
    include/
    ${WLROOTS_INCLUDE_DIR}    
)

add_executable(bway
    bway/main.c
    bway/config.c
    bway/server.c
)

set_property(TARGET bway PROPERTY C_STANDARD 11)

target_link_libraries(bway
    ${WLROOTS_LIBRARY}
)
