cmake_minimum_required(VERSION 3.16)
project(memutils)

include(${PROJECT_SOURCE_DIR}/externals.cmake)

add_library(memory_pool INTERFACE)
target_include_directories(memory_pool INTERFACE include/)

if (NOT CMAKE_CROSSCOMPILING)
    enable_testing()
    add_subdirectory(tests)
endif()
