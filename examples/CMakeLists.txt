## Set variables
set(EXEC_TARGET_NAME Basil_example)
set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/")

## Create executable
add_executable(${EXEC_TARGET_NAME} "${EXAMPLE_DIR}/BasilTest.cpp")

## Link with library
target_link_libraries(${EXEC_TARGET_NAME} PRIVATE ${BASIL_LIBRARIES})
target_include_directories(${EXEC_TARGET_NAME} PRIVATE ${BASIL_INCLUDE_DIRS})