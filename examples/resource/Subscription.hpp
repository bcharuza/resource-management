#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H
#include <functional>

class Message;
using cb_t = std::function<void(Message const&)>;

struct Subscription{
  static uint32_t getId() noexcept{
    static uint32_t id = 0;
    return ++id;
  }
  explicit
  Subscription(cb_t cb):
    m_cbFun{cb}, m_id{ getId() }
  {}
  cb_t m_cbFun;
  uint32_t m_id;
};
#endif /* SUBSCRIPTION_H */
