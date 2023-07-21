## Project settings
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
project(BlackHoleViz VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
enable_testing()

## Main executable
add_executable(${PROJECT_NAME} 
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/frame/framecontroller.cpp"
    "${CMAKE_SOURCE_DIR}/src/window/windowview.cpp")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/include" 
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/src/frame"
    "${CMAKE_SOURCE_DIR}/src/window")

## OpenGL setup
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENGL_INCLUDE_DIRS} 
    ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} GLEW::GLEW glfw)

## CUDA setup
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
# Test output
#add_executable(cudaTest "${CMAKE_SOURCE_DIR}/src/test.cu")
#set_target_properties(cudaTest PROPERTIES CUDA_ARCHITECTURES "75")
#set_target_properties(cudaTest PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

# Testing setup
set(PROJECT_TEST_NAME "${PROJECT_NAME}_test")
add_executable(${PROJECT_TEST_NAME} 
    "${CMAKE_SOURCE_DIR}/src/frame/framecontroller.cpp"
    "${CMAKE_SOURCE_DIR}/src/window/windowview.cpp"
    "${CMAKE_SOURCE_DIR}/tst/main.test.cpp")
find_package(Catch2 REQUIRED)
target_link_libraries(BlackHoleViz_test Catch2::Catch2)
target_include_directories(${PROJECT_TEST_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/tst")

include(CTest)
include(Catch)
catch_discover_tests(BlackHoleViz_test)


# Only run code coverage in Debug build
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")

    ## Code coverage
    if(CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        append_coverage_compiler_flags()
        set(PROJECT_COVERAGE_NAME "${PROJECT_NAME}_coverage")
        setup_target_for_coverage_gcovr_html(
            NAME ${PROJECT_COVERAGE_NAME}
            EXECUTABLE ${PROJECT_TEST_NAME}
            EXCLUDE "tst/*" "include/*"
        )
    endif(CMAKE_COMPILER_IS_GNUCXX)

endif()

# Only build documentation in Release build
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")

    ## Doxygen setup
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

## Bugfixes
# Only enable CUDA after including CodeCoverage to prevent error 
enable_language(CUDA)