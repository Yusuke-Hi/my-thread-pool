cmake_minimum_required(VERSION 3.22)
project(my-thread-pool CXX)

add_library(${PROJECT_NAME}
  src/my-thread-pool.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC include
)

add_subdirectory(test)
