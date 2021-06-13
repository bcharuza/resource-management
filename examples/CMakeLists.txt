option(BUILD_TEST "Build tests" ON)

add_subdirectory(problem)
add_subdirectory(resource)

find_package(Threads REQUIRED)

add_library( example STATIC
  CfgReader.cpp
  CfgReader.hpp
  Logger.cpp
  Logger.hpp
  MessageHandler.cpp
  MessageHandler.hpp
  )
target_link_libraries(example PRIVATE Threads::Threads)

add_library( example-fix STATIC
  CfgReader-fix.cpp
  CfgReader-fix.hpp
  Logger-fix.cpp
  Logger-fix.hpp
  MessageHandler-fix.cpp
  MessageHandler-fix.hpp
  )
target_link_libraries(example-fix PRIVATE Threads::Threads)

