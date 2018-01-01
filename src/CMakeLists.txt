#[[
   CmakeList.txt - CMake build instructions for the core libraries
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
include_directories(include .)

set(REDSHIFT_BASE_INCLUDE_DIR "${GLIBCONFIG_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS} ${Libdrm_INCLUDE_DIRS}")
# single-file libraries

# TODO clean up the mess
#[[include_directories(${REDSHIFT_BASE_INDLUCDE_DIR})
    add_library(solar       SHARED solar.c)
    add_library(config_ini  SHARED config-ini.c)
    add_library(system_time SHARED systemtime.c)
    add_library(hooks       SHARED hooks.c)
    add_library(signals     SHARED signals.c)
    add_library(pipeutils   SHARED pipeutils.c)
    add_library(colorramp   SHARED colorramp.c)
    add_library(gamma_dummy SHARED gamma-dummy.c )
    add_library(gamma_drm   SHARED gamma-drm.c )
    add_library(gamma_randr SHARED gamma-randr.c )
    add_library(loation_manual    SHARED location-manual.c )
    add_library(location_geoclue2 SHARED location-geoclue2.c)
    target_include_directories(location_geoclue2 PUBLIC ${GLIB_INCLUDE_DIRS})]]

# file(GLOB SOURCES "./*.c")
set (SOURCES colorramp.cpp
        config-ini.c gamma-dummy.c gamma-drm.c gamma-randr.c hooks.c location-geoclue2.c location-manual.cpp options.cpp pipeutils.c redshift.cpp signals.c  solar.cpp systemtime.cpp location-provider-impl.cpp)
set (REDSHIFT_LIBS ${Libdrm_LIBRARIES} ${XCB_LIBRARIES} ${GLIB_LIBRARIES}
        ${GLIB_GOBJECT_LIBRARIES} ${GLIB_GTHREAD_LIBRARIES} ${GLIB_GMODULE_LIBRARIES}
        ${GLIB_GIO_LIBRARIES} m)


set(LIBS ${Libdrm_LIBRARIES} ${XCB_LIBRARIES} ${GLIB_LIBRARIES})
SET(CMAKE_BUILD_TYPE Debug)
# executable
add_executable(redshift redshift.cpp ${SOURCES})
target_include_directories(redshift PUBLIC ${GLIB_INCLUDE_DIRS} ${Libdrm_INCLUDE_DIRS} ${GLIBCONFIG_INCLUDE_DIR} ${XCB_INCLUDE_DIRS})
target_link_libraries(redshift LINK_PUBLIC ${Libdrm_LIBRARIES} ${XCB_LIBRARIES} ${GLIB_LIBRARIES}
                                           ${GLIB_GOBJECT_LIBRARIES} ${GLIB_GTHREAD_LIBRARIES} ${GLIB_GMODULE_LIBRARIES}
                                           ${GLIB_GIO_LIBRARIES} m)

