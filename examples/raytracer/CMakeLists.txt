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

## Fetch skybox files
set(ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/assets")
set(SKYBOX_DIR "${ASSETS_DIR}/skybox")

if (NOT EXISTS ${SKYBOX_DIR})
    file(MAKE_DIRECTORY ${SKYBOX_DIR})

    message("Fetching skybox files...")
    set(SKYBOX_ZIP "${ASSETS_DIR}/skybox.zip")
    file(DOWNLOAD
        "https://learnopengl.com/img/textures/skybox.zip"
        ${SKYBOX_ZIP}
        STATUS FETCH_SKYBOX_STATUS)

    list(GET FETCH_SKYBOX_STATUS 0 FETCH_SKYBOX_STATUS_CODE)
    if (${FETCH_SKYBOX_STATUS_CODE} EQUAL 0)
        message("Successfully downloaded skybox files.")

        file(ARCHIVE_EXTRACT
            INPUT ${SKYBOX_ZIP}
            DESTINATION ${ASSETS_DIR})
        file(REMOVE ${SKYBOX_ZIP})

    else()
        message(WARNING "Unable to download skybox files.")
    endif()

endif()


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

## Set warning level
if(MSVC)
    target_compile_options(${EXEC_TARGET_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${EXEC_TARGET_NAME} PRIVATE -Wall -Wextra -Werror -Wno-missing-field-initializers -fdiagnostics-color=never)
endif()