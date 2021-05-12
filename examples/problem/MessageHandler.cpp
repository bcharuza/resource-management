#include "MessageHandler.hpp"
#include "Logger.hpp"
#include "Subscription.hpp"
#include <fstream>
#include <algorithm>
using namespace std;

MQClient*
MsgHandler::m_client = nullptr;

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
  std::lock_guard<mtx_t> lock{m_mux};
  *m_out << msg << endl;
}
MessagePtr
MQClient::receive(){
  if(!m_in) return MessagePtr{new Message{""}};
  std::string msg;
  std::lock_guard<mtx_t> lock{m_mux};
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
  Subscription sub {cb};
  lock_guard<mtx_t> lock{m_mtxLock};
  getSubscriptions().push_back(sub);
  trace(INFO, "Subscribed: %i", (int)sub.m_id);
  return sub.m_id;
}
bool
MsgHandler::checkSubscription(uint32_t id)const{
  lock_guard<mtx_t> lock{m_mtxLock};
  auto& subs = getSubscriptions();
  auto it = find_if(subs.begin(),subs.end(),
		    [id](auto& x){return x.m_id == id;});
  return it != subs.end();
}
void
MsgHandler::removeSubscription(uint32_t id)const{
  lock_guard<mtx_t> lock{m_mtxLock};
  auto& subs = getSubscriptions();
  auto it = find_if(subs.begin(),subs.end(),
		    [id](auto& x){return x.m_id == id;});
  if(it != subs.end()){
    trace(INFO, "Remove Subscription: %i", (int)id);
    subs.erase(it);
  }}
void
MsgHandler::NotifyAll(Message const& msg)const{
  trace(INFO, "Enter Notify All");
  lock_guard<mtx_t> lock{m_mtxLock};
  for(auto& sub : getSubscriptions())
    sub.m_cbFun(msg);
}
std::vector<Subscription>&
MsgHandler::getSubscriptions(){
  static std::vector<Subscription> m_subs;
  return m_subs;
}
MsgQueue&
MsgHandler::getMessageQueue(){
  static MsgQueue m_subs;
  return m_subs;
}
void
MsgHandler::insert(MessagePtr arg) const {
  lock_guard<mtx_t> lock{m_mtxLock};
  trace(INFO, "Inserting %s", arg->get("source").c_str());
  getMessageQueue().insert(arg);
  NotifyAll(*arg);
}
MessagePtr
MsgHandler::top() const{
    lock_guard<mtx_t> lock{m_mtxLock};
    return getMessageQueue().top();
}
void
MsgHandler::remove(MessagePtr msg)const{
  lock_guard<mtx_t> lock{m_mtxLock};
  trace(INFO, "Removing %s", msg->get("source").c_str());
  getMessageQueue().remove(msg->get("source"));
  NotifyAll(*msg);
}
