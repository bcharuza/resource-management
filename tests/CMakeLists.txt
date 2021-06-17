add_executable( example-test main.cpp)
target_link_libraries( example-test PRIVATE example)

add_executable( example-fix-test main-fix.cpp)
target_link_libraries( example-fix-test PRIVATE example-fix)

add_executable( example-debug-mutex main-extra-debug.cpp)
target_link_libraries( example-debug-mutex PRIVATE example-fix)

add_executable( example-asymetric main-asymetric.cpp)
target_link_libraries( example-asymetric PRIVATE example-fix)

add_test(NAME copyCfg
  COMMAND ${CMAKE_COMMAND} -E copy config.cfg ${CMAKE_CURRENT_BINARY_DIR}/
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME copyInput
  COMMAND ${CMAKE_COMMAND} -E copy test1.in test2.in ${CMAKE_CURRENT_BINARY_DIR}/
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(copyCfg copyInput PROPERTIES FIXTURES_SETUP testEnv)

message("Test: example-test")
add_test(NAME example-test
  COMMAND example-test config.cfg test1 test2
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
message("Test: example-fix-test")
add_test(NAME example-fix-test
  COMMAND example-fix-test config.cfg test1 test2
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
message("Test: example-debug-mutex")
add_test(NAME example-debug-mutex
  COMMAND example-debug-mutex
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
message("Test: example-asymetric")
add_test(NAME example-asymetric
  COMMAND example-asymetric
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties(example-test example-fix-test example-debug-mutex
  PROPERTIES
  TIMEOUT 10
  FIXTURES_REQUIRED testEnv
  )
