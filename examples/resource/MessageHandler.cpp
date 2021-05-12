#include "MessageHandler.hpp"
#include "Logger.hpp"
#include "Subscription.hpp"
#include <fstream>
#include <algorithm>
using namespace std;

MQClient::MQClient(string in, string out):
  m_in{ in.empty() ? nullptr : new ifstream{in}},
  m_out{ out.empty() ? nullptr : new ofstream{out}}
{
  if(m_in && !m_in->is_open())
    throw runtime_error("Can't open '" + in + "'");
  if(m_out && !m_out->is_open())
    throw runtime_error("Can't open '" + out + "'");
}
void
MQClient::send(MessagePtr m){
  if(!m_out) return;
  MessagePtr result{ new Message };
  std::string msg = m->str();
  trace(INFO, "Sending: %s", msg.c_str());
  *m_out << msg << endl;
}
MessagePtr
MQClient::receive(){
  if(!m_in) return MessagePtr{new Message{""}};
  std::string msg;
  *m_in >> msg;
  if(msg.empty()) return MessagePtr{new Message{""}};
  if(msg != "[") throw runtime_error("Wrong format");
  do msg += m_in->get();
  while(m_in && msg.back() != ']');
  trace(INFO, "Received: %s", msg.c_str());
  return MessagePtr{new Message{msg}};
}
MsgHandler::MsgHandler(){
  trace(INFO, "Initialized MsgHandler");
}
uint32_t
MsgHandler::addSubscription(Subscription cb){
  accessSubs().critical_section([&cb](auto& x){;
      x.push_back(cb);});
  trace(INFO, "Subscribed: %i", (int)cb.m_id);
  return cb.m_id;
}
bool
MsgHandler::checkSubscription(uint32_t id){
  return accessSubs().
    critical_section([id](auto const& subs){;
	auto it = find_if(subs.begin(), subs.end(), [id](auto& s){return s.m_id == id;});
	return it != subs.end(); });
}
void
MsgHandler::removeSubscription(uint32_t id){
  auto result = accessSubs()
    .critical_section([id](auto& s){;
	auto it = find_if(s.begin(), s.end(), [id](auto const& x){return x.m_id == id;});
	if(it == s.end()) return false;
	s.erase(it);
	return true;});
  if(result)
    trace(INFO, "Removed Subscription: %i", (int)id);
}
void
MsgHandler::NotifyAll(Message const& msg){
  trace(INFO, "Enter Notify All");
  accessSubs().critical_section([&msg](auto& s){;
      for(auto& sub : s) sub.m_cbFun(msg); });
}
void
MsgHandler::insert(MessagePtr arg){
  trace(INFO, "Inserting %s", arg->get("source").c_str());
  accessQueue().critical_section([&arg](auto& x){;
      x.insert(arg); });
  NotifyAll(*arg);
}
MessagePtr
MsgHandler::top(){
  return accessQueue()
    .critical_section([](auto const& x){;
	return x.top(); });
}
void
MsgHandler::remove(MessagePtr msg){
  trace(INFO, "Removing %s", msg->get("source").c_str());
  auto src = msg->get("source");
  accessQueue().critical_section([&msg,src](auto& x){;
      x.remove(src); });
  NotifyAll(*msg);
}
