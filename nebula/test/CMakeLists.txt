file(GLOB NEBULA_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")
######################################################################################################################
# DEPENDENCIES
######################################################################################################################

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# "make server client ..."
##########################################
foreach (nebula_test_source ${NEBULA_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(nebula_test_filename ${nebula_test_source} NAME)
    string(REPLACE ".cpp" "" nebula_test_name ${nebula_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${nebula_test_name} EXCLUDE_FROM_ALL ${nebula_test_source})
    add_dependencies(build-tests ${nebula_test_name})
    add_dependencies(check-tests ${nebula_test_name})

    target_link_libraries(${nebula_test_name} nebula_shared)

    # Set test target properties and dependencies.
    set_target_properties(${nebula_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${nebula_test_name}
    )

endforeach(nebula_test_source ${NEBULA_TEST_SOURCES})