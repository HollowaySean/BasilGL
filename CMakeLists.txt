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


## Find dependencies

## OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

## fmt library
find_package(fmt QUIET CONFIG)
if(${fmt_FOUND})

    # fmt from installed binaries
    message("Found fmt as installed library.")
    set(FMT_LIBRARIES fmt::fmt)
else()

    # fmt from source
    message("Could not find fmt. Installing from local source.")
    set(FMT_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/extern/fmt")

    # Install Git submodule
    if(NOT EXISTS "${FMT_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- extern/fmt
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    add_subdirectory("${CMAKE_SOURCE_DIR}/extern/fmt")

    set(FMT_INCLUDE_DIRS "${FMT_SUBMODULE_DIR}/include")
    set(FMT_LIBRARIES fmt::fmt)
endif()

## GLEW
find_package(GLEW QUIET CONFIG)
if(${GLEW_FOUND})

    # GLEW from installed binaries
    message("Found GLEW as installed library.")
    set(GLEW_LIBRARIES
        GLEW::GLEW)
else()

    # GLEW from source
    message("Could not find GLEW. Installing from local source.")
    set(GLEW_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/extern/glew")

    # Install Git submodule
    if(NOT EXISTS "${GLEW_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- extern/glew
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    add_subdirectory("${GLEW_SUBMODULE_DIR}")
    set(GLEW_INCLUDE_DIRS
        "${GLEW_SUBMODULE_DIR}/include")
    set(GLEW_LIBRARIES libglew_static)
endif()

## GLFW
find_package(glfw3 QUIET CONFIG)
if(${glfw3_FOUND})

    # GLFW from installed binaries
    message("Found GLFW as installed library.")
    set(GLFW_LIBRARIES
        glfw)
else()

    # GLFW from source
    message("Could not find GLFW. Installing from local source.")
    set(GLFW_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/extern/glfw")

    # Install Git submodule
    if(NOT EXISTS "${GLFW_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- extern/glfw
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    # TODO(sholloway): Make platform independent
    set( GLFW_BUILD_X11 ON CACHE BOOL "Build for X11 Unix")

    set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
    set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
    add_subdirectory("${CMAKE_SOURCE_DIR}/extern/glfw")

    set(GLFW_INCLUDE_DIRS "${GLFW_SUBMODULE_DIR}/glfw/include")
    set(GLFW_LIBRARIES glfw)
endif()

## Library subdirectory
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

## Examples subdirectory
add_subdirectory("${CMAKE_SOURCE_DIR}/examples")

## Macros
macro(DEBUG_MESSAGE MESSAGE)
    if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
        message(${MESSAGE})
    endif()
endmacro()

## Testing setup
find_package(Catch2 2 QUIET)
if(${Catch2_FOUND})
    # Catch2 from installed binaries
    message("Found Catch2 as installed library.")
    set(CATCH2_LIBRARIES Catch2::Catch2)
else()
    #Catch2 from local source
    message("Could not find Catch2. Installing from local source.")
    set(CATCH2_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/extern/catch2")

    # Install Git submodule
    if(NOT EXISTS "${CATCH2_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- extern/catch2
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    add_subdirectory("${CATCH2_SUBMODULE_DIR}")
    set(CATCH2_LIBRARIES Catch2::Catch2)
    set(CATCH2_INCLUDE_DIRS "${CATCH2_SUBMODULE_DIR}/single_include/catch2")
endif()

include(Catch)
include(CTest)
enable_testing()

## Test subdirectory
add_subdirectory("${CMAKE_SOURCE_DIR}/tst")

# Debug build preprocessor options
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    add_compile_definitions(DEBUG_BUILD)
endif()

## Doxygen setup
# Only build documentation in Release build
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    add_subdirectory("${CMAKE_SOURCE_DIR}/docs")
endif()
