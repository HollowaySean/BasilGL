## Macros
macro(DEBUG_MESSAGE MESSAGE)
    if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
        message(${MESSAGE})
    endif()
endmacro()

## Project settings
cmake_minimum_required(VERSION 3.0.0)
project(BlackHoleViz VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
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
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} GLEW::GLEW glfw)

## Testing setup
find_package(Catch2 REQUIRED)
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
    ${SRC_FILE_LIST}
)
target_include_directories(
    ${TEST_TARGET_NAME} PRIVATE
    ${TST_DIR_LIST}
    ${SRC_DIR_LIST}
    "${CMAKE_SOURCE_DIR}/external/catch2/"
)
target_link_libraries(${TEST_TARGET_NAME} Catch2::Catch2)
target_link_libraries(${TEST_TARGET_NAME} ${OPENGL_LIBRARIES} GLEW::GLEW glfw)

catch_discover_tests(${TEST_TARGET_NAME})

# Set test-related preprocessor options
target_compile_definitions(
    ${TEST_TARGET_NAME} PRIVATE
    TEST_BUILD
)


## Code coverage
# Only run in Debug build
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")

    if(CMAKE_COMPILER_IS_GNUCXX)

        include(CodeCoverage)
        append_coverage_compiler_flags()

        message("Running coverage for test: ${TEST_TARGET_NAME}")
        set(PROJECT_COVERAGE_NAME "${PROJECT_NAME}_coverage")
        setup_target_for_coverage_gcovr_html(
            NAME ${PROJECT_COVERAGE_NAME}
            EXECUTABLE ${TEST_TARGET_NAME}
            EXCLUDE "tst/*" "external/*" "include/*" "src/main.cpp"
        )

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
