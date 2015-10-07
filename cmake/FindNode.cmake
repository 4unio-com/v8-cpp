# vim:expandtab:shiftwidth=2:tabstop=2:

# Copyright (C) 2015 Canonical Ltd.

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

set(NODE_EXECUTABLE_NAMES iojs node)

if (DEFINED ENV{NODE_ROOT_DIR})
  find_program (NODE_EXECUTABLE NAMES ${NODE_EXECUTABLE_NAMES}
    PATHS
    $ENV{NODE_ROOT_DIR}
    NO_DEFAULT_PATH
    )
else($ENV{NODE_ROOT_DIR})
  find_program (NODE_EXECUTABLE NAMES ${NODE_EXECUTABLE_NAMES}
    HINTS
    $ENV{NODE_DIR}
    PATH_SUFFIXES bin
    DOC "Node.js interpreter"
    )
endif()

include (FindPackageHandleStandardArgs)

if (NOT DEFINED ENV{NODE_ROOT_DIR})
    find_path (NODE_ROOT_DIR "node/node.h" "src/node.h"
      PATHS /usr/include/nodejs /usr/local/include/nodejs)
else()
    set(NODE_ROOT_DIR $ENV{NODE_ROOT_DIR})
endif()

set (NODE_INCLUDE_DIRS
  ${NODE_ROOT_DIR}/src
  ${NODE_ROOT_DIR}/node
  ${NODE_ROOT_DIR}/deps/v8/include
  ${NODE_ROOT_DIR}/deps/uv/include
)

find_package_handle_standard_args (Node DEFAULT_MSG
    NODE_EXECUTABLE
    NODE_INCLUDE_DIRS
)

if (NODE_EXECUTABLE)
    execute_process(COMMAND ${NODE_EXECUTABLE} --version
                    OUTPUT_VARIABLE _VERSION
                    RESULT_VARIABLE _NODE_VERSION_RESULT)
    execute_process(COMMAND ${NODE_EXECUTABLE} -e "console.log(process.versions.v8)"
                    OUTPUT_VARIABLE _V8_VERSION
                    RESULT_VARIABLE _V8_RESULT)
    if (NOT _NODE_VERSION_RESULT AND NOT _V8_RESULT)
        string (REPLACE "v" "" NODE_VERSION_STRING "${_VERSION}")
        string (REPLACE "." ";" _VERSION_LIST "${NODE_VERSION_STRING}")
        list (GET _VERSION_LIST 0 NODE_VERSION_MAJOR)
        list (GET _VERSION_LIST 1 NODE_VERSION_MINOR)
        list (GET _VERSION_LIST 2 NODE_VERSION_PATCH)
        set (V8_VERSION_STRING ${_V8_VERSION})
        string (REPLACE "." ";" _V8_VERSION_LIST "${_V8_VERSION}")
        string (REPLACE "." "" V8_DEFINE_STRING "${_V8_VERSION}")
        string (STRIP ${V8_DEFINE_STRING} V8_DEFINE_STRING)
        list (GET _V8_VERSION_LIST 0 V8_VERSION_MAJOR)
        list (GET _V8_VERSION_LIST 1 V8_VERSION_MINOR)
        list (GET _V8_VERSION_LIST 2 V8_VERSION_PATCH)
        # we end up with a nasty newline so strip everything that isn't a number
        string (REGEX MATCH "^[0-9]*" V8_VERSION_PATCH ${V8_VERSION_PATCH})
    else ()
        set (NODE_VERSION_STRING "0.10.30")
        set (NODE_VERSION_MAJOR "0")
        set (NODE_VERSION_MINOR "10")
        set (NODE_VERSION_PATCH "30")
        set (V8_VERSION_MAJOR "3")
        set (V8_VERSION_MINOR "28")
        set (V8_VERSION_PATCH "72")
        set (V8_VERSION_STRING "3.28.72")
        message ("defaulted to node 0.10.30")
    endif ()
    message ("INFO - Node version is " ${NODE_VERSION_STRING})
    message ("INFO - Node using v8 " ${V8_VERSION_STRING})
endif ()

mark_as_advanced (NODE_EXECUTABLE)
