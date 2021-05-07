#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H
#include <functional>

class Message;

struct Subscription{
  static uint32_t getId() noexcept{
    static uint32_t id = 0;
    return ++id;
  }
  explicit
  Subscription(std::function<void(Message const&)> cb):
    m_cbFun{cb}, m_id{ getId() }
  {}
  std::function<void(Message const&)> m_cbFun;
  uint32_t m_id;
};
#endif /* SUBSCRIPTION_H */