#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "Logger.hpp"
#include "resource/Resource.hpp"
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
void initializeMsgHandler(std::string);

using MessagePtr = std::shared_ptr<Message>;
struct MQClient {
  MQClient(std::string const&);
  void setHandler(std::string const&);
  void send(MessagePtr const&);
  void receive(MessagePtr const&);
private:
  std::shared_ptr<Resource<MsgHandler>> m_mux;
  std::ofstream m_out;
  std::string m_name;
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
  void setOutput(MQClient* client) noexcept;
private:
  void NotifyAll(Message const& msg) const;
  static subid_t genNewId() noexcept;
private:
  MQClient* m_client;
  std::map<subid_t,cb_t> m_subs;
  MsgQueue m_queue;
};
#endif /* MESSAGEHANDLER_H */
