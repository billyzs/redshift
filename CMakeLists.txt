cmake_minimum_required(VERSION 3.2)

project(REDSHIFT)


subdirs(src)
set(CMAKE_MODULE_PATH CMakeModules)

# external dependencies
# glib
find_package (GLIB REQUIRED COMPONENT gio gobject gthread gmodule)
# XCB
find_package (XCB REQUIRED XCB RANDR RENDER)
# libdrm
find_package (Libdrm REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set (UNIX_FLAGS "-DENABLE_RANDR -DENABLE_GEOCLUE2 -pthread")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNIX_FLAGS}")
