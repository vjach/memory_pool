cmake_minimum_required(VERSION 3.16)
project(memutils)

include(externals.cmake)

add_library(memory_pool INTERFACE)
target_include_directories(memory_pool INTERFACE include/)
target_compile_features(memory_pool INTERFACE cxx_std_17)

if (NOT CMAKE_CROSSCOMPILING)
    enable_testing()
    add_subdirectory(tests)
endif()
