## Set variables
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/")

## Add subdirectories
if(${BASIL_BUILD_EXAMPLES})

add_subdirectory("${EXAMPLE_DIR}/panes")
add_subdirectory("${EXAMPLE_DIR}/raytracer")
add_subdirectory("${EXAMPLE_DIR}/scratch")
add_subdirectory("${EXAMPLE_DIR}/shadertoy")

endif()