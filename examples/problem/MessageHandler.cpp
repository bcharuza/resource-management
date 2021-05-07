#include "MessageHandler.hpp"
#include "Logger.hpp"
#include "Subscription.hpp"
#include <fstream>
#include <algorithm>
using namespace std;

MQClient*
MsgHandler::m_client = nullptr;

MQClient::MQClient(string in, string out):
  m_in{ new ifstream{in}},
  m_out{ new ofstream{out}}
{ }
void MQClient::send(MessagePtr m){
  MessagePtr result{ new Message };
  std::string msg = m->str();
  trace(INFO, "Sending: %s", msg);
  std::lock_guard<mutex> lock{m_mux};
  *m_out << msg << endl;
}
MessagePtr MQClient::receive(){
  std::string msg;
  std::lock_guard<mutex> lock{m_mux};
  if(!(*m_in >> msg) || msg != "[")
    throw runtime_error("Wrong format");
  do msg += m_in->get();
  while(m_in && msg.back() != ']');
  trace(INFO, "Received: %s", msg);
  return MessagePtr{new Message{msg}};
}
void MsgHandler::send(MessagePtr msg) const {
  if(m_client)
    m_client->send(msg);
}
uint32_t MsgHandler::addSubscription(Subscription cb){
  Subscription sub {cb};
  lock_guard<mutex> lock{m_mtxLock};
  getSubscriptions().push_back(sub);
  return sub.m_id;
}
bool MsgHandler::checkSubscription(uint32_t id) const {
  lock_guard<mutex> lock{m_mtxLock};
  auto& subs = getSubscriptions();
  auto it = find_if(subs.begin(),subs.end(),
		    [id](auto& x){return x.m_id == id;});
  return it != subs.end();
}
void MsgHandler::removeSubscription(uint32_t id) const {
  lock_guard<mutex> lock{m_mtxLock};
  auto& subs = getSubscriptions();
  auto it = find_if(subs.begin(),subs.end(),
		    [id](auto& x){return x.m_id == id;});
  if(it != subs.end()) subs.erase(it);
}
void MsgHandler::NotifyAll(Message const& msg) const {
  lock_guard<mutex> lock{m_mtxLock};
  for(auto const& sub : getSubscriptions())
    sub.m_cbFun(msg);
}
std::vector<Subscription>&
MsgHandler::getSubscriptions(){
  static std::vector<Subscription> m_subs;
  return m_subs;
}