## Set variables
set(EXEC_TARGET_NAME Basil_example_panes)

## Create executable
add_executable(${EXEC_TARGET_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

## Link with library
target_link_libraries(${EXEC_TARGET_NAME} PRIVATE ${BASIL_LIBRARIES})
target_include_directories(${EXEC_TARGET_NAME} PRIVATE ${BASIL_INCLUDE_DIRS})
