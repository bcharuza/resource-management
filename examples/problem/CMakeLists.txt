find_package(Threads REQUIRED)

add_executable( problem
  main.cpp
  Logger.cpp
  Logger.hpp
  MessageQueue.cpp
  MessageQueue.hpp
  MessageHandler.hpp
  MessageHandler.cpp
  Subscription.hpp
  CfgReader.cpp
  CfgReader.hpp
  )

target_link_libraries(problem PRIVATE Threads::Threads)
