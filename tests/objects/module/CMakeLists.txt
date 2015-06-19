add_library(
    test-objects-module SHARED
    module.cpp
)

# This line sets the output binary name to "v8-cpp-exampleX.node"
set_target_properties(
    test-objects-module
    PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)
