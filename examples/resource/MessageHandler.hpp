#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "MessageQueue.hpp"
#include "Resource.hpp"
#include <mutex>

struct MQClient
{
  MQClient(std::string in,std::string out);
  void send(MessagePtr);
  MessagePtr receive();
private:
  std::shared_ptr<std::ifstream> m_in;
  std::shared_ptr<std::ofstream> m_out;
};
class MsgHandler
{
  static Resource<std::unique_ptr<MQClient>>&
  accessClient() noexcept{
    static Resource<std::unique_ptr<MQClient>> c;
    return c;
  }
  static Resource<MsgQueue>&
  accessQueue() noexcept{
    static Resource<MsgQueue> q;
    return q;
  }
  static
  Resource<std::vector<Subscription>>&
  accessSubs() noexcept{
    static Resource<std::vector<Subscription>> s;
    return s;
  }
public:
  static
  void setClient(std::string const& in,
		 std::string const& out){
    auto mq = new MQClient(in,out);
    accessClient().critical_section([mq](auto& x){x.reset(mq);});
  }
  static
  void send(MessagePtr msg){
    accessClient().critical_section([msg](auto& x){if(x) x->send(msg);});
  }
  MessagePtr receive(){
    return accessClient()
      .critical_section([](auto& x){
			  return x ? x->receive()
			    : MessagePtr{new Message{""}}; });
  }
  static uint32_t addSubscription(Subscription);
  static bool checkSubscription(uint32_t);
  static void removeSubscription(uint32_t);
  static void insert(MessagePtr);
  static MessagePtr top();
  static void remove(MessagePtr);
public:
  MsgHandler();
private:
  static void NotifyAll(Message const&);
};
using MsgHandlerPtr =std::shared_ptr<MsgHandler>;
#endif /* MESSAGEHANDLER_H */
