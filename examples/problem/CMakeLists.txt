find_package(Threads REQUIRED)

add_executable( problem
  CfgReader.cpp
  CfgReader.hpp
  Logger.cpp
  Logger.hpp
  MessageQueue.cpp
  MessageQueue.hpp
  MessageHandler.hpp
  MessageHandler.cpp
  main.cpp
  Subscription.hpp
  )

target_link_libraries(problem PRIVATE Threads::Threads)
