include(FetchContent)
FetchContent_Declare(
    google_test
    GIT_REPOSITORY https://github.com/google/googletest.git
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/externals/gtest
    )

if(NOT google_test_POPULATED)
    FetchContent_Populate(google_test)
    add_subdirectory(${google_test_SOURCE_DIR} ${google_test_BINARY_DIR})
endif()

