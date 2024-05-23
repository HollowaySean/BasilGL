## Set variables
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/")

## Add subdirectories
if(${BASIL_BUILD_EXAMPLES})

add_subdirectory("${EXAMPLE_DIR}/panes")
add_subdirectory("${EXAMPLE_DIR}/raytracer")
add_subdirectory("${EXAMPLE_DIR}/shadertoy")

## Subdirectory for gitignore'd scratch file
if(EXISTS "${EXAMPLE_DIR}/scratch" AND EXISTS "${EXAMPLE_DIR}/scratch/CMakeLists.txt")
    add_subdirectory("${EXAMPLE_DIR}/scratch")
endif()

endif()