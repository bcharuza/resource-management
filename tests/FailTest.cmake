message("TEST_EXE: ${TEST_EXE}")
message("ARG1: ${ARG1}")
message("ARG2: ${ARG2}")
message("ARG3: ${ARG3}")
message("EXPECTED_TIMEOUT: ${EXPECTED_TIMEOUT}")

execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXE} ${ARG1} ${ARG2} ${ARG3}
  TIMEOUT ${EXPECTED_TIMEOUT}
  RESULTS_VARIABLE result
  OUTPUT_QUIET
  ERROR_QUIET
  )

message("RESULT:" ${result})
if(result)
  message("Test Failed as expected")
else()
  message(FATAL_ERROR "Test PASSED wit code ${result} but was expected to Fail")
endif()
