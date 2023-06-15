# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

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
    set(WITH_CORE_TOOLS OFF CACHE INTERNAL "")
    set(WITH_TOOLS OFF CACHE INTERNAL "")
    set(WITH_SNAPPY OFF CACHE INTERNAL "")
    set(WITH_LZ4 OFF CACHE INTERNAL "")
    set(WITH_ZLIB OFF CACHE INTERNAL "")
    set(WITH_ZSTD OFF CACHE INTERNAL "")
    set(WITH_TOOLS OFF CACHE INTERNAL "")
    set(WITH_GFLAGS OFF CACHE INTERNAL "")
    set(WITH_TBB OFF CACHE INTERNAL "")
    set(USE_RTTI ON CACHE INTERNAL "")
    set(ROCKSDB_BUILD_SHARED OFF CACHE INTERNAL "")
    set(WITH_JEMALLOC OFF CACHE INTERNAL "")

    add_subdirectory(${rocksdb_SOURCE_DIR} ${rocksdb_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()