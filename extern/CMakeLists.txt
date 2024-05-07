## Define macro for conditional loading of libraries
function(install_dependency
    DEP_VARIABLE_NAME
    DEP_PACKAGE_NAME
    DEP_SUBMODULE_PATH
    DEP_INSTALLED_LIB_NAME
    DEP_LOCAL_LIB_NAME
    DEP_INCLUDE_PATH_EXTENSION )

    # Attempt to find with find_package
    find_package("${DEP_PACKAGE_NAME}" QUIET CONFIG)
    if("${${DEP_PACKAGE_NAME}_FOUND}")

        # Use installed binaries
        message("Found ${DEP_PACKAGE_NAME} as installed library.")
        set("${DEP_VARIABLE_NAME}_LIBRARIES" "${DEP_INSTALLED_LIB_NAME}")
    else()

        # Search for submodule directory
        set("SUBMODULE_DIR" "${EXTERN_DIR}/${DEP_SUBMODULE_PATH}")
        if(NOT EXISTS "${SUBMODULE_DIR}/CMakeLists.txt")
            message("Could not find ${DEP_PACKAGE_NAME}. Installing from local source.")
            find_package(Git REQUIRED)
            execute_process(COMMAND ${GIT_EXECUTABLE}
                submodule update --init --recursive -- ${SUBMODULE_DIR}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND_ERROR_IS_FATAL ANY)
        else()
            message("Found ${DEP_PACKAGE_NAME} in extern directory.")
        endif()

        # Add CMake subdirectory
        if(EXISTS "${SUBMODULE_DIR}/CMakeLists.txt")
            add_subdirectory("${SUBMODULE_DIR}")
        endif()

        # Export variables
        set("${DEP_VARIABLE_NAME}_INCLUDE_DIRS"
            "${SUBMODULE_DIR}/${DEP_INCLUDE_PATH_EXTENSION}"
            PARENT_SCOPE)
        set("${DEP_VARIABLE_NAME}_LIBRARIES"
            "${DEP_LOCAL_LIB_NAME}"
            PARENT_SCOPE)
    endif()
endfunction(install_dependency)

## Install dependencies

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# {FMT} library
install_dependency("FMT" "fmt" "fmt" "fmt::fmt" "fmt::fmt" "include")

# STB library
install_dependency("STB" "STB" "stb" "" "" "")

# GLEW
install_dependency("GLEW" "GLEW" "glew" "GLEW::GLEW" "libglew_static" "include")

# GLFW
# TODO(sholloway): Make platform independent
set( GLFW_BUILD_X11 ON CACHE BOOL "Build for X11 Unix")
set( GLFW_BUILD_WAYLAND OFF CACHE BOOL "Do not build for Wayland Unix")
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
install_dependency("GLFW" "glfw3" "glfw" "glfw" "glfw" "include")

# Catch2
install_dependency("CATCH2" "Catch2" "catch2" "Catch2::Catch2" "Catch2::Catch2" "single_include/catch2")

## Export directories and libraries to parent scope
set(BASIL_DEPENDENCY_LIBRARIES
    ${FMT_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    PARENT_SCOPE)

set(BASIL_DEPENDENCY_INCLUDE_DIRS
    ${FMT_INCLUDE_DIRS}
    ${STB_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    PARENT_SCOPE)

set(BASIL_TEST_DEPENDENCY_LIBRARIES
    ${CATCH2_LIBRARIES}
    PARENT_SCOPE)

set(BASIL_TEST_DEPENDENCY_INCLUDE_DIRS
    ${CATCH2_INCLUDE_DIRS}
    PARENT_SCOPE)