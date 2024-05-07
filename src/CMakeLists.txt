## Set variables
set(LIB_TARGET_NAME Basil_lib)
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

## Search for required files
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS
    "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HEADER_LIST CONFIGURE_DEPENDS
    "${SRC_DIR}/*.hpp")
file(GLOB_RECURSE PUBLIC_HEADER_LIST CONFIGURE_DEPENDS
    "${INCLUDE_DIR}/*.hpp")

## Create library
add_library(${LIB_TARGET_NAME} STATIC
    ${SOURCE_LIST})

## Export variables
set(BASIL_LIBRARIES
    ${LIB_TARGET_NAME}
    ${BASIL_DEPENDENCY_LIBRARIES}
    PARENT_SCOPE)

set(BASIL_INCLUDE_DIRS
    ${BASIL_DEPENDENCY_INCLUDE_DIRS}
    PARENT_SCOPE)

## Preprocessor directives
target_compile_definitions(
    ${LIB_TARGET_NAME} PRIVATE
    SOURCE_DIR="${SRC_DIR}"
)

## Link libraries and include directories
target_link_libraries(${LIB_TARGET_NAME}
    PRIVATE
    ${BASIL_DEPENDENCY_LIBRARIES})

target_include_directories(${LIB_TARGET_NAME}
    PUBLIC
        ${INCLUDE_DIR}
        ${SRC_DIR}
    PRIVATE
        ${BASIL_DEPENDENCY_INCLUDE_DIRS})
