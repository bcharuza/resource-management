#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "Resource.hpp"
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <deque>
class MsgHandler;
struct Message{
  int prio;
  std::string text;
};
std::shared_ptr<Resource<MsgHandler>> accessMsgHandler(std::string const&);
void setupMsgHandler(std::string const&);

using MessagePtr = std::shared_ptr<Message>;
struct MQClient {
  MQClient(std::string const&,std::shared_ptr<Resource<MsgHandler>> const&);
  void send(MessagePtr const&);
  bool receive();
private:
  std::shared_ptr<Resource<MsgHandler>> m_mux;
  Resource<std::ifstream> m_in;
  Resource<std::ofstream> m_out;
};
struct MsgQueue {
  void insert(MessagePtr const& msg);
  MessagePtr top() const noexcept;
  void popTop();
private:
  std::deque<MessagePtr> m_queue;
};
struct MsgHandler
{
  using subid_t = uint32_t;
  using cb_t=std::function<void(Message const&)>;
  subid_t subscribe(cb_t);
  void insert(MessagePtr const& arg);
  bool isEmpty()const;
  void unsubscribe(subid_t);
  void sendTop();
  void setOutput(std::shared_ptr<MQClient>const&)noexcept;
private:
  void NotifyAll(Message const& msg) const;
  static subid_t genNewId() noexcept;
private:
  std::shared_ptr<MQClient> m_client;
  std::map<subid_t,cb_t> m_subs;
  MsgQueue m_queue;
};
#endif /* MESSAGEHANDLER_H */
