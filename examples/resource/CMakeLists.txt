find_package(Threads REQUIRED)

add_executable( resource
  CfgReader.cpp
  CfgReader.hpp
  Logger.cpp
  Logger.hpp
  MessageHandler.cpp
  MessageHandler.hpp
  MessageQueue.cpp
  MessageQueue.hpp
  Resource.hpp
  Subscription.hpp
  main.cpp
  )

target_link_libraries(resource PRIVATE Threads::Threads)
