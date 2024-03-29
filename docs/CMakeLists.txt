## Generate docs with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)

    set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/docs/Doxyfile_in")
    set(DOXYGEN_OUT "${CMAKE_BINARY_DIR}/docs/Doxyfile")

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    set(PROJECT_DOC_NAME Basil_docs)
    add_custom_target(${PROJECT_DOC_NAME} ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

else (DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)