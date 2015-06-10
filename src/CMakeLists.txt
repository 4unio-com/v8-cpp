file(
    GLOB_RECURSE
    SRC_FILES
    *.h
)

add_custom_target(
  source_files
  SOURCES ${SRC_FILES}
)
