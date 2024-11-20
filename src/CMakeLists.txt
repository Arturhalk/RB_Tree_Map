cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
find_package(GTest REQUIRED)

add_executable(
  RB_Tree_tests
  tests.cpp
)
target_link_libraries(
  RB_Tree_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RB_Tree_tests)
