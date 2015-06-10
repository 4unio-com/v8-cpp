cmake_minimum_required(VERSION 2.8)

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
    /usr/local/include/node
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(src)
add_subdirectory(test)
