include(FetchContent)
if (NOT CMAKE_CROSSCOMPILING)
    FetchContent_Declare(
        google_test
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/externals/gtest
        )

    if(NOT google_test_POPULATED)
        FetchContent_Populate(google_test)
        add_subdirectory(${google_test_SOURCE_DIR} ${google_test_BINARY_DIR})
    endif()
endif()

