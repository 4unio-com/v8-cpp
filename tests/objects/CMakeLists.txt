# MODULE
add_library(
    module SHARED
    module.cpp
)

# This line sets the output binary name to "<target_name>.node"
set_target_properties(
    module
    PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)

# TEST
add_executable(
    test-objects
    test.h
    test.cpp
)

target_link_libraries(
    test-objects

    v8-cpp

    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${TEST_LDFLAGS}
)

add_test(
    test-objects
    test-objects
)
