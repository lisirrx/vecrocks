cmake_minimum_required(VERSION 3.22.1)

project(vecrocks VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

include(cmake/rocksdb.cmake)
include(cmake/gtest.cmake)


set(SOURCES
        src/vecrocks.cpp)

set(TEST_SOURCES
        test/test.cpp)


# LIB
add_library(vecrocks STATIC ${SOURCES})
target_link_libraries(vecrocks PRIVATE rocksdb)
target_include_directories(vecrocks PRIVATE ${rocksdb_SOURCE_DIR}/include)


# EXAMPLE
add_subdirectory(example)

# TEST

enable_testing()

add_executable(vecrocks_test ${TEST_SOURCES})
target_link_libraries(vecrocks_test PRIVATE vecrocks gtest_main rocksdb)
target_include_directories(vecrocks_test PRIVATE ${gtest_SOURCE_DIR}/include ${rocksdb_SOURCE_DIR}/include)
include(GoogleTest)
gtest_discover_tests(vecrocks_test)