add_executable(
  v8-cpp-unit-tests
  test.cpp
)

target_link_libraries(
  v8-cpp-unit-tests
  ${GTEST_BOTH_LIBRARIES}
  ${GMOCK_LIBRARIES}
  ${TEST_LDFLAGS}
)

add_test(
  v8-cpp-unit-tests
  v8-cpp-unit-tests
)
