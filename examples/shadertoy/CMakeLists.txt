## Set variables
set(EXEC_TARGET_NAME Basil_example_shadertoy)

## Create executable
add_executable(${EXEC_TARGET_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

## Link with library
target_link_libraries(${EXEC_TARGET_NAME} PRIVATE ${BASIL_LIBRARIES})
target_include_directories(${EXEC_TARGET_NAME} PRIVATE ${BASIL_INCLUDE_DIRS})

## Set compile definitions
target_compile_definitions(
    ${EXEC_TARGET_NAME} PRIVATE
    BUILD_DIR="${PROJECT_BINARY_DIR}")

## Set warning level
if(MSVC)
    target_compile_options(${EXEC_TARGET_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${EXEC_TARGET_NAME} PRIVATE -Wall -Wextra -Werror -Wno-missing-field-initializers)
endif()