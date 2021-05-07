#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "MessageQueue.hpp"
#include <mutex>

struct MQClient
{
  MQClient(std::string in,std::string out);
  void send(MessagePtr);
  MessagePtr receive();
private:
  std::mutex m_mux;
  std::shared_ptr<std::istream> m_in;
  std::shared_ptr<std::ostream> m_out;
};
class MsgHandler
{
  mutable std::mutex m_mtxLock;
public:
  void setClient(MQClient* client) noexcept
  { m_client = client; }
public:
  void send(MessagePtr) const;
  uint32_t addSubscription(Subscription cb);
  bool checkSubscription(uint32_t) const;
  void removeSubscription(uint32_t) const;
private:
  static std::vector<Subscription>& getSubscriptions();
  static MsgQueue& getMessageQueue();
private:
  void NotifyAll(Message const&) const;
private:
  static MQClient* m_client;
};
using MsgHandlerPtr = std::shared_ptr<MsgHandler>;

#endif /* MESSAGEHANDLER_H */
