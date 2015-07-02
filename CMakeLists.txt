cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(v8-cpp CXX)

add_definitions(
    -std=c++11
    -fno-permissive
    -pedantic
    -Wall
    -Wextra
    -fPIC
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/include
)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/out/native/obj.target/tools/gyp
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(v8runner)
