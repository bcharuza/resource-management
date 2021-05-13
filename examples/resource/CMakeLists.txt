find_package(Threads REQUIRED)

add_executable( resource
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
  Resource.hpp
  )

target_link_libraries(resource PRIVATE Threads::Threads)
