## CMake version requirements
cmake_minimum_required(VERSION 3.12...3.28)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

## Project settings
project(Basil VERSION 1.0
    DESCRIPTION "C++ library for simplified setup & management of OpenGL shader-based texture rendering"
    LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

## Add CMake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

## Testing options
include(Catch)
include(CTest)

## Build options
option(BASIL_INCLUDE_IMGUI "Build support for ImGui" ON)

## Set paths
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include" )
set(SRC_DIR     "${CMAKE_SOURCE_DIR}/src"     )
set(TST_DIR     "${CMAKE_SOURCE_DIR}/tst"     )
set(EXTERN_DIR  "${CMAKE_SOURCE_DIR}/extern"  )
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples")

## Add subdirectories
add_subdirectory(${EXTERN_DIR})
add_subdirectory(${SRC_DIR})
add_subdirectory(${EXAMPLE_DIR})
add_subdirectory(${TST_DIR})

## Doxygen setup
# Only build documentation in Release build
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    add_subdirectory("${CMAKE_SOURCE_DIR}/docs")
endif()
