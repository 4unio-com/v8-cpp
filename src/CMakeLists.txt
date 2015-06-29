file(
    GLOB_RECURSE
    SRC_FILES
    *.h
)

add_library(
    v8-cpp SHARED
    ${SRC_FILES}
)

set_target_properties(
    v8-cpp PROPERTIES
    LINKER_LANGUAGE CXX
)

target_link_libraries(
    v8-cpp

    v8_base
    v8_libbase
    v8_snapshot

    dl
    pthread
)
