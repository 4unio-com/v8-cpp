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

find_package(Node REQUIRED)

include_directories(
    ${NODE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_subdirectory(tests)
