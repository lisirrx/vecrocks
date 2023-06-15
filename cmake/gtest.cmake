include_guard()

include(FetchContent)

FetchContent_Declare(gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.13.0
        )

if (NOT gtest_POPULATED)
    FetchContent_Populate(gtest)

    set(BUILD_GMOCK OFF)
    set(INSTALL_GTEST OFF)

    add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()



