## Set variables
set(TEST_TARGET_NAME Basil_test)

## Search for source files
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS
    "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE TEST_LIST CONFIGURE_DEPENDS
    "${TST_DIR}/*.cpp")

## Create executable
add_executable(${TEST_TARGET_NAME}
    ${SOURCE_LIST}
    ${TEST_LIST}
    ${BASIL_DEPENDENCY_SOURCES})

## Link and build
target_include_directories(${TEST_TARGET_NAME} PRIVATE
    ${SRC_DIR}
    ${TST_DIR}
    ${BASIL_DEPENDENCY_INCLUDE_DIRS}
    ${BASIL_TEST_DEPENDENCY_INCLUDE_DIRS})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE
    ${BASIL_LIBRARIES}
    ${BASIL_DEPENDENCY_LIBRARIES}
    ${BASIL_TEST_DEPENDENCY_LIBRARIES})

catch_discover_tests(${TEST_TARGET_NAME})

## Preprocessor directives
target_compile_definitions(
    ${TEST_TARGET_NAME} PRIVATE
    TEST_BUILD
    SOURCE_DIR="${SRC_DIR}"
    TEST_DIR="${TST_DIR}"
    "$<$<CONFIG:Debug>:DEBUG_BUILD>")

# Build options
if (${BASIL_INCLUDE_IMGUI})
    target_compile_definitions(
        ${TEST_TARGET_NAME} PRIVATE
        BASIL_INCLUDE_IMGUI=1)
else()
    target_compile_definitions(
        ${TEST_TARGET_NAME} PRIVATE
        BASIL_INCLUDE_IMGUI=0)
endif()

## Code coverage
# Only run in Debug build
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")

    if(CMAKE_COMPILER_IS_GNUCXX)

        include(CodeCoverage)
        append_coverage_compiler_flags_to_target(${TEST_TARGET_NAME})

        message("Setting up coverage for ${TEST_TARGET_NAME}")
        set(PROJECT_COVERAGE_NAME Basil_coverage)
        setup_target_for_coverage_gcovr_html(
            NAME ${PROJECT_COVERAGE_NAME}
            EXECUTABLE ${TEST_TARGET_NAME}
            EXCLUDE "tst/*" "extern/*" "include/*" "examples/*"
        )
    else()
        message("GNUCXX compiler required to run code coverage. Skipping.")
    endif(CMAKE_COMPILER_IS_GNUCXX)

endif()
