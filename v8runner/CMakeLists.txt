file(
    GLOB
    SRC_FILES
    *.h
    *.cpp
    *.js
)

add_executable(
    v8runner
    ${SRC_FILES}
)

target_link_libraries(
    v8runner

    v8_base
    v8_libbase
    v8_snapshot

    dl
    pthread
)
