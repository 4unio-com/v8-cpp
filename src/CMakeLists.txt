file(
    GLOB_RECURSE
    SRC_FILES
    *.h
)

# this allows us to see our source files in QtCreator
add_custom_target(
    source_files
    SOURCES ${SRC_FILES}
)
