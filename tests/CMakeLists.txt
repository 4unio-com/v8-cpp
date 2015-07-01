include(FindGMock)

include_directories(
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${TEST_INCLUDE_DIRS}
)

add_subdirectory(functions)
add_subdirectory(members)
add_subdirectory(methods)
add_subdirectory(objects)
