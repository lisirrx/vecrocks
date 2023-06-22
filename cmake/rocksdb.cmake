include_guard()


include(FetchContent)

FetchContent_Declare(rocksdb
        GIT_REPOSITORY https://github.com/facebook/rocksdb.git
        GIT_TAG v8.1.1
        )

if (NOT rocksdb_POPULATED)
    FetchContent_Populate(rocksdb)

    set(FAIL_ON_WARNINGS OFF CACHE INTERNAL "")
    set(WITH_TESTS OFF CACHE INTERNAL "")
    set(WITH_BENCHMARK_TOOLS OFF CACHE INTERNAL "")
    set(WITH_CORE_TOOLS ON CACHE INTERNAL "")
    set(WITH_TOOLS ON CACHE INTERNAL "")
    set(WITH_SNAPPY OFF CACHE INTERNAL "")
    set(WITH_LZ4 OFF CACHE INTERNAL "")
    set(WITH_ZLIB OFF CACHE INTERNAL "")
    set(WITH_ZSTD OFF CACHE INTERNAL "")
    set(WITH_GFLAGS OFF CACHE INTERNAL "")
    set(WITH_TBB OFF CACHE INTERNAL "")
    set(USE_RTTI ON CACHE INTERNAL "")
    set(ROCKSDB_BUILD_SHARED OFF CACHE INTERNAL "")
    set(WITH_JEMALLOC OFF CACHE INTERNAL "")

    add_subdirectory(${rocksdb_SOURCE_DIR} ${rocksdb_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()