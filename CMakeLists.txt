cmake_minimum_required(VERSION 3.22.1)

project(vecrocks VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
include(cmake/rocksdb.cmake)
include(cmake/gtest.cmake)
include(cmake/faiss.cmake)
include(cmake/fmt.cmake)
include(cmake/nlohmann.cmake)


set(SOURCES
        src/vecrocks.cpp include/vecrocks.h src/codec.cpp src/codec.h)

set(TEST_SOURCES
        test/test.cpp)


# LIB
add_library(vecrocks STATIC ${SOURCES})
target_link_libraries(vecrocks PRIVATE rocksdb faiss fmt nlohmann_json)
target_include_directories(vecrocks PRIVATE
        ${rocksdb_SOURCE_DIR}/include
        ${faiss_SOURCE_DIR}/include
        ${vecrocks_SOURCE_DIR}/include
        ${fmt_SOURCE_DIR}/include
        ${nlohmann_json_SOURCE_DIR}/include)


# EXAMPLE
add_subdirectory(example)

# TEST

enable_testing()

add_executable(vecrocks_test ${TEST_SOURCES})
target_link_libraries(vecrocks_test PRIVATE vecrocks gtest_main rocksdb faiss fmt nlohmann_json)
target_include_directories(vecrocks_test PRIVATE
        ${vecrocks_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include
        ${rocksdb_SOURCE_DIR}/include
        ${faiss_SOURCE_DIR}/include
        ${fmt_SOURCE_DIR}/include
        ${nlohmann_json_SOURCE_DIR}/include)
include(GoogleTest)
gtest_discover_tests(vecrocks_test)