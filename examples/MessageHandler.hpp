#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "Logger.hpp"
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
class MsgHandler;
struct Message {
  int prio;
  std::string text;
};
using MessagePtr = std::shared_ptr<Message>;
struct MQClient {
  MQClient(std::string const&);
  void setHandler(MsgHandler*);
  void send(MessagePtr const&);
  void receive(MessagePtr const&);
private:
  MsgHandler* m_handler = nullptr;
  std::ofstream m_out;
  std::string m_name;
};
using cb_t = std::function<void(Message const&)>;
struct Subscription {
  static uint32_t getId() noexcept;
  explicit Subscription(cb_t);
  cb_t m_cbFun;
  uint32_t m_id;
};
struct MsgQueue {
  void insert(MessagePtr const& msg);
  MessagePtr top() const noexcept;
  void popTop();
private:
  std::vector<MessagePtr> m_queue;
};
class MsgHandler
{
  using mtx_t = std::mutex;  
public:
  uint32_t addSubscription(Subscription sub);
  void insert(MessagePtr arg)const;
  bool isEmpty()const;
  void removeSubscription(uint32_t id)const;
  void sendTop()const;
  void setClient(MQClient* client) noexcept;
private:
  void NotifyAll(Message const& msg) const;
  static std::vector<Subscription>& getSubscriptions();
  static MsgQueue& getMessageQueue();
private:
  mutable mtx_t m_mtxLock;
  static MQClient* m_client;
};
using MsgHandlerPtr =std::shared_ptr<MsgHandler>;
#endif /* MESSAGEHANDLER_H */
