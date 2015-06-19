add_executable(
    v8runner
    v8runner.cpp
)

target_link_libraries(
    v8runner

    v8_base
    v8_libbase
    v8_snapshot

    dl
    pthread
)
