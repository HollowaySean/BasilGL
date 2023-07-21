## Project settings
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
project(BlackHoleViz VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
enable_testing()

## Main executable
add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/lib")

## OpenGL setup
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
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
add_executable(${PROJECT_TEST_NAME} "${CMAKE_SOURCE_DIR}/tst/test.cpp")
find_package(Catch2 REQUIRED)
target_link_libraries(BlackHoleViz_test Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(BlackHoleViz_test)

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
endif()

## Bugfixes
# Only enable CUDA after including CodeCoverage to prevent error 
enable_language(CUDA)