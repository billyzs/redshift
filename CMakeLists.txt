#[[
   CmakeList.txt - CMake build instructions for the the entire project
   This file is part of Yellowstone.

   Redshift is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Redshift is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Redshift.  If not, see <http://www.gnu.org/licenses/>.

   Copyright (c) 2017-2018  Billy Zhou <billyzs.728@gmail.com>
]]

cmake_minimum_required(VERSION 3.2)

project(yellowstone)


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
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${UNIX_FLAGS} --std=c++17 -Wall -Werror")
