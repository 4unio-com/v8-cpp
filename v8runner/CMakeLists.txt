cmake_minimum_required(VERSION 2.8)

project(v8runner CXX)

add_definitions(
    -std=c++11
    -fno-permissive
    -pedantic
    -Wall
#    -Wextra
    -fPIC
)

# make deps - Download and build dependencies
add_custom_target(deps COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps/build-deps.sh)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8
)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/out/native/obj.target/tools/gyp
)

add_subdirectory(src)
