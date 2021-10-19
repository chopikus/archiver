cmake_minimum_required(VERSION 2.8)
project(archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O2")

add_executable(MAIN main.cpp)


add_library(READER reader/reader.cpp)
add_library(PRIORITY_QUEUE min_priority_queue/min_priority_queue.cpp)
add_library(TRIE trie/trie.cpp)
add_library(WRITER writer/writer.cpp)
add_library(ARCHIVER archiver/archiver.cpp)

target_link_libraries(MAIN ARCHIVER PRIORITY_QUEUE TRIE READER WRITER)


# Setup testing
link_directories(/usr/local/lib)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_custom_target(tests COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB TEST_FILES "tests/*.cpp")
add_executable("archiver_tests" EXCLUDE_FROM_ALL ${TEST_FILES})
target_link_libraries("archiver_tests" gtest pthread ARCHIVER PRIORITY_QUEUE TRIE READER WRITER)
add_dependencies(tests "archiver_tests")
add_test("archiver_tests" "./archiver_tests")
