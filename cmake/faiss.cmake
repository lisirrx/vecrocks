include_guard()


include(FetchContent)

FetchContent_Declare(faiss
        GIT_REPOSITORY https://github.com/facebookresearch/faiss.git
        GIT_TAG v1.7.4
        )

if (NOT faiss_POPULATED)
    FetchContent_Populate(faiss)

    set(FAISS_ENABLE_GPU OFF CACHE INTERNAL "")
    set(FAISS_ENABLE_PYTHON OFF CACHE INTERNAL "")
    set(BUILD_TESTING OFF CACHE INTERNAL "")
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
    set(FAISS_ENABLE_C_API OFF CACHE INTERNAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE INTERNAL "")

    add_subdirectory(${faiss_SOURCE_DIR} ${faiss_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()