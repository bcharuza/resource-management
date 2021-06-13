option(BUILD_TEST "Build tests" ON)

find_package(Threads REQUIRED)

add_library( example STATIC
  CfgReader.cpp
  CfgReader.hpp
  Logger.cpp
  Logger.hpp
  MessageHandler.cpp
  MessageHandler.hpp
  )
target_link_libraries(example PUBLIC Threads::Threads)
target_include_directories(example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library( example-fix STATIC
  CfgReader-fix.cpp
  CfgReader-fix.hpp
  Logger-fix.cpp
  Logger-fix.hpp
  MessageHandler-fix.cpp
  MessageHandler-fix.hpp
  Resource.hpp
  )
target_link_libraries(example-fix PUBLIC Threads::Threads)
target_include_directories(example-fix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
