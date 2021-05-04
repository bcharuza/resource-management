set(name google-test)
if(BUILD_TEST)
  message("Target: external::${name}")
  include(FetchContent)
  message(STATUS "Unpacking local GTest repository")
  FetchContent_Declare( GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
    )
  FetchContent_MakeAvailable(GTest)
  add_library(${name} INTERFACE)
  target_link_libraries(${name} INTERFACE gmock gmock_main gtest gtest_main)
  add_library(external::${name} ALIAS ${name})
else()
  message("Target: google-test (disabled)")
endif()
