cmake_minimum_required(VERSION 3.22)

include(FetchContent)
FechContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(my-thread-pool-test
  my-thread-pool-test.cpp
)

target_link_libraries(my-thread-pool-test
  PRIVATE
  my-thread-pool
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(my-thread-pool-test)
