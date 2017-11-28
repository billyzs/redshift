cmake_minimum_required(VERSION 3.2)

project(REDSHIFT)


subdirs(src)
message(${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
message(${CMAKE_MODULE_PATH})

# external dependencies
# glib
find_package (GLIB REQUIRED COMPONENT gio gobject gthread gmodule)
# XCB
find_package (XCB REQUIRED XCB RANDR RENDER)
# libdrm
find_package (Libdrm REQUIRED)

# set(CMAKE_BUILD_TYPE Debug)
set (UNIX_FLAGS "-DENABLE_RANDR -DENABLE_GEOCLUE2 -pthread")
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${UNIX_FLAGS}")
