add_test(NAME problem-test1
  COMMAND problem ${CMAKE_CURRENT_LIST_DIR}/test1.cfg
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )

set_tests_properties(problem-test1
  PROPERTIES
  TIMEOUT 10)
message("Test: problem-test1")
