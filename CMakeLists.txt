cmake_minimum_required(VERSION 3.3)
cmake_policy(SET CMP0054 NEW)
include("CMakeHelpers.cmake")


# ===== Build options ===== #
set(PROJECT_BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
option(BUILD_SHARED_LIBS "Build shared libs" OFF)


# ===== Project ===== #
project("bf" LANGUAGES C)
add_compile_options(-g)


# ===== Third Party libraries ===== #


# ===== Source files ===== #
list_sources(SOURCES "src")


# ===== Include directories ===== #
list(APPEND INCLUDES "src")
include_directories(${INCLUDES})


# ===== Link libraries ===== #
list(APPEND LINK_LIBRARIES "")
link_libraries(${LINK_LIBRARIES})


# ===== Targets ===== #
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BUILD_DIRECTORY}/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BUILD_DIRECTORY}/release"
)
