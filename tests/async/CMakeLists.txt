find_package(Node REQUIRED)

include_directories(
    ${NODE_INCLUDE_DIRS}
)

# MODULE
add_library(
    test-async-module SHARED
    module.cpp
)

# This line sets the output binary name to "<target_name>.node"
set_target_properties(
    test-async-module
    PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)

# TEST
add_executable(
    test-async
    test.h
    test.cpp
)

target_link_libraries(
    test-async

    v8-cpp

    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${TEST_LDFLAGS}
)

add_test(
    test-async
    test-async
)
