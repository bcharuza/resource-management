#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "MessageQueue.hpp"
#include <mutex>

struct MQClient
{
  using mtx_t = std::mutex;
  MQClient(std::string in,std::string out);
  void send(MessagePtr);
  MessagePtr receive();
private:
  mtx_t m_mux;
  std::shared_ptr<std::ifstream> m_in;
  std::shared_ptr<std::ofstream> m_out;
};
class MsgHandler
{
  using mtx_t = std::mutex;  
  mutable mtx_t m_mtxLock;
public:
  MsgHandler();
  void setClient(MQClient* client) noexcept
  { m_client = client; }
public:
  void send(MessagePtr msg) const
  {  if(m_client) m_client->send(msg); }
  uint32_t addSubscription(Subscription cb);
  bool checkSubscription(uint32_t) const;
  void removeSubscription(uint32_t) const;
public:
  void insert(MessagePtr arg) const;
  MessagePtr top() const;
  void remove(MessagePtr msg) const;
private:
  static std::vector<Subscription>& getSubscriptions();
  static MsgQueue& getMessageQueue();
private:
  void NotifyAll(Message const&) const;
private:
  static MQClient* m_client;
};
using MsgHandlerPtr =std::shared_ptr<MsgHandler>;
#endif /* MESSAGEHANDLER_H */
