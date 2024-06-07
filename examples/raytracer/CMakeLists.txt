## Set variables
set(EXEC_TARGET_NAME Basil_example_raytracer)

## Find/install GLM
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/glm/.git")
    message("Could not find GLM. Installing from local source.")
    find_package(Git REQUIRED)
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --init --recursive -- ${SUBMODULE_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY)
else()
    message("Found GLM in local directory.")
endif()

## Add GLM to CMake
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glm")

## Create executable
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

add_executable(${EXEC_TARGET_NAME}
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    ${SOURCE_LIST})

## Link with library
target_link_libraries(
    ${EXEC_TARGET_NAME} PRIVATE
    ${BASIL_LIBRARIES}
    glm::glm)
target_include_directories(
    ${EXEC_TARGET_NAME} PRIVATE
    ${BASIL_INCLUDE_DIRS})

## Set compile definitions
target_compile_definitions(
    ${EXEC_TARGET_NAME} PRIVATE
    BUILD_DIR="${PROJECT_BINARY_DIR}")