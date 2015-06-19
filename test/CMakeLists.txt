find_package(Node REQUIRED)

include_directories(
    ${NODE_INCLUDE_DIRS}
)

file(
    GLOB
    TEST_FILES
    *.h *.cpp *.js
)

configure_file(test.js test.js)

add_library(
    v8-cpp-test SHARED
    ${TEST_FILES}
)

# This line sets the output binary name to "v8-cpp-exampleX.node"
set_target_properties(
    v8-cpp-test
    PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)
