## Macros
macro(DEBUG_MESSAGE MESSAGE)
    if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
        message(${MESSAGE})
    endif()
endmacro()

## Project settings
cmake_minimum_required(VERSION 3.5.0)
project(BasilGL VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

## Find source files and folders
file(GLOB SRC_CHILDREN
    "${CMAKE_SOURCE_DIR}/src/*")
set(SRC_DIR_LIST "")
set(SRC_FILE_LIST "")
foreach(CHILD ${SRC_CHILDREN})
    if(IS_DIRECTORY "${CHILD}")
        debug_message("Found source subdirectory: ${CHILD}")
        list(APPEND SRC_DIR_LIST ${CHILD})
        file(GLOB SRC_CHILDREN_FILES
            "${CHILD}/*.cpp")
        foreach(GRANDCHILD ${SRC_CHILDREN_FILES})
            debug_message("Found source file: ${GRANDCHILD}")
            list(APPEND SRC_FILE_LIST "${GRANDCHILD}")
        endforeach()
    endif()
endforeach()

## Main executable
add_executable(${PROJECT_NAME}
    ${SRC_FILE_LIST}
    "${CMAKE_SOURCE_DIR}/src/main.cpp")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    ${SRC_DIR_LIST})


## OpenGL setup
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENGL_LIBRARIES})

## GLEW setup
find_package(GLEW QUIET CONFIG)
if(${GLEW_FOUND})
    # GLEW from installed binaries
    message("Found GLEW as installed library.")
    set(GLEW_LIBRARIES
        GLEW::GLEW)
else()
    # GLEW from source
    message("Could not find GLEW. Installing from local source.")
    set(GLEW_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/vendor/glew")

    # Install Git submodule
    if(NOT EXISTS "${GLEW_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- vendor/glew
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    add_subdirectory("${GLEW_SUBMODULE_DIR}")
    set(GLEW_INCLUDE_DIRS
        "${GLEW_SUBMODULE_DIR}/include")
    set(GLEW_LIBRARIES libglew_static)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLEW_LIBRARIES})


## GLFW setup
find_package(glfw3 QUIET CONFIG)
if(${glfw3_FOUND})
    # GLFW from installed binaries
    message("Found GLFW as installed library.")
    set(GLFW_LIBRARIES
        glfw)
else()
    # GLFW from source
    message("Could not find GLFW. Installing from local source.")
    set(GLFW_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw")

    # Install Git submodule
    if(NOT EXISTS "${GLFW_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- vendor/glfw
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
    set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
    add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/glfw")

    set(GLFW_INCLUDE_DIRS "${GLFW_SUBMODULE_DIR}/glfw/include")
    set(GLFW_LIBRARIES glfw)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLFW_LIBRARIES})


## Format library
find_package(fmt QUIET CONFIG)
if(${fmt_FOUND})
    # fmt from installed binaries
    message("Found fmt as installed library.")
    set(FMT_LIBRARIES fmt::fmt)
else()
    # fmt from source
    message("Could not find fmt. Installing from local source.")
    set(FMT_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/vendor/fmt")

    # Install Git submodule
    if(NOT EXISTS "${FMT_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- vendor/fmt
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/fmt")

    set(FMT_INCLUDE_DIRS "${FMT_SUBMODULE_DIR}/include")
    set(FMT_LIBRARIES fmt::fmt)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${FMT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${FMT_LIBRARIES})


## Testing setup
find_package(Catch2 2 QUIET)
if(${Catch2_FOUND})
    # Catch2 from installed binaries
    message("Found Catch2 as installed library.")
    set(CATCH2_LIBRARIES Catch2::Catch2)
else()
    #Catch2 from local source
    message("Could not find Catch2. Installing from local source.")
    set(CATCH2_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/vendor/catch2")

    # Install Git submodule
    if(NOT EXISTS "${CATCH2_SUBMODULE_DIR}/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive -- vendor/catch2
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
    endif()

    add_subdirectory("${CATCH2_SUBMODULE_DIR}")
    set(CATCH2_LIBRARIES Catch2::Catch2)
    set(CATCH2_INCLUDE_DIRS "${CATCH2_SUBMODULE_DIR}/single_include/catch2")
endif()

include(Catch)
include(CTest)

# Find test files and folders
file(GLOB TST_CHILDREN
    "${CMAKE_SOURCE_DIR}/tst/*")
set(TST_DIR_LIST "")
set(TST_FILE_LIST "")
foreach(CHILD ${TST_CHILDREN})
    if(IS_DIRECTORY "${CHILD}")
        debug_message("Found test subdirectory: ${CHILD}")
        list(APPEND TST_DIR_LIST ${CHILD})
        file(GLOB TST_CHILDREN_FILES
            "${CHILD}/*.cpp")
        foreach(GRANDCHILD ${TST_CHILDREN_FILES})
            debug_message("Found test file: ${GRANDCHILD}")
            list(APPEND TST_FILE_LIST "${GRANDCHILD}")
        endforeach()
    endif()
endforeach()

# Build test executable
set(TEST_TARGET_NAME "${PROJECT_NAME}_test")
add_executable(
    ${TEST_TARGET_NAME}
    "${CMAKE_SOURCE_DIR}/tst/main.test.cpp"
    ${TST_FILE_LIST}
    ${SRC_FILE_LIST})

target_include_directories(
    ${TEST_TARGET_NAME} PRIVATE
    ${TST_DIR_LIST}
    ${SRC_DIR_LIST}
    ${CATCH2_INCLUDE_DIRS}
    ${FMT_INCLUDE_DIRS})
target_link_libraries(${TEST_TARGET_NAME} PRIVATE
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${CATCH2_LIBRARIES}
    ${FMT_LIBRARIES})

catch_discover_tests(${TEST_TARGET_NAME})


## Preprocessor directives
# Set preprocessor options for main executable
target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    SOURCE_DIR="${CMAKE_SOURCE_DIR}/src"
)

# Set test-related preprocessor options
target_compile_definitions(
    ${TEST_TARGET_NAME} PRIVATE
    TEST_BUILD
    SOURCE_DIR="${CMAKE_SOURCE_DIR}/src"
    TEST_DIR="${CMAKE_SOURCE_DIR}/tst"
)

# Debug build preprocessor options
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    add_compile_definitions(DEBUG_BUILD)
endif()


## Code coverage
# Only run in Debug build
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")

    if(CMAKE_COMPILER_IS_GNUCXX)

        include(CodeCoverage)
        append_coverage_compiler_flags_to_target(${TEST_TARGET_NAME})

        message("Running coverage for test: ${TEST_TARGET_NAME}")
        set(PROJECT_COVERAGE_NAME "${PROJECT_NAME}_coverage")
        setup_target_for_coverage_gcovr_html(
            NAME ${PROJECT_COVERAGE_NAME}
            EXECUTABLE ${TEST_TARGET_NAME}
            EXCLUDE "tst/*" "vendor/*" "include/*" "src/main.cpp"
        )
    else()
        message("GNUCXX compiler required to run code coverage. Skipping.")
    endif(CMAKE_COMPILER_IS_GNUCXX)

endif()


## Doxygen setup
# Only build documentation in Release build
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")

    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/docs/Doxyfile_in")
        set(DOXYGEN_OUT "${CMAKE_BINARY_DIR}/docs/Doxyfile")

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        set(PROJECT_DOC_NAME "${PROJECT_NAME}_docs")
        add_custom_target(${PROJECT_DOC_NAME} ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)

    else (DOXYGEN_FOUND)
        message("Doxygen needs to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)

endif()
