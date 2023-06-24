cmake_minimum_required(VERSION 3.16)

project(klib VERSION 0.1.0 LANGUAGES C)

include_directories("src")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(${PROJECT_NAME} ${SOURCES})

if (${BUILD_TESTS})
    add_subdirectory("tests")
endif()
