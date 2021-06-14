#include "MessageHandler.hpp"
#include <algorithm>
using namespace std;
//MQClient
MQClient::MQClient(string const& arg):
  m_in{arg+".in"},m_out{arg+".out"},m_name{arg}
{}
void MQClient::setHandler(MsgHandler* handler){
  m_handler = handler;
}
void MQClient::send(MessagePtr const& msg){
  if(msg) m_out<<msg->text<<endl;
}
bool MQClient::receive(){
  auto msg = make_shared<Message>();
  if(!msg || !m_in.good()) return false;
  m_in >> msg->prio;
  getline(m_in, msg->text);
  if(!msg || !m_in.good()) return false;
  if(m_handler) m_handler->insert(msg);
  return true;
}
//Subscription
Subscription::Subscription(cb_t cb):
  m_cbFun{cb}, m_id{ getId() }
{}
uint32_t Subscription::getId() noexcept{
  static uint32_t id = 0;
  return ++id;
}
//MsgQueue
void MsgQueue::insert(MessagePtr const& msg){
  m_queue.push_back(msg);
  stable_sort(m_queue.begin(),m_queue.end(),
	      [](auto const& l,auto const& r){
		return l->prio < r->prio;
	      });
}
MessagePtr MsgQueue::top() const noexcept{
  return m_queue.empty()
    ? nullptr : m_queue.front();
}
void MsgQueue::popTop(){
  m_queue.erase(m_queue.begin());
}
//MsgHandler
MQClient* MsgHandler::m_client = nullptr;

void MsgHandler::setClient(MQClient* c) noexcept{
  m_client = c;
  m_client->setHandler(this);
}
uint32_t
MsgHandler::addSubscription(Subscription sub){
  lock_guard<mtx_t> lock{m_mtxLock};
  getSubscriptions().push_back(sub);
  return sub.m_id;
}
void
MsgHandler::removeSubscription(uint32_t id)const{
  lock_guard<mtx_t> lock{m_mtxLock};
  auto& subs = getSubscriptions();
  auto it = find_if(subs.begin(),subs.end(),
		    [id](auto& x){return x.m_id == id;});
  if(it != subs.end()) subs.erase(it);
}
void MsgHandler::insert(MessagePtr arg)const{
  lock_guard<mtx_t> lock{m_mtxLock};
  getMessageQueue().insert(arg);
  NotifyAll(*arg);
}
bool MsgHandler::isEmpty()const{
  lock_guard<mtx_t> lock{m_mtxLock};
  return getMessageQueue().top() == nullptr;
}
void MsgHandler::sendTop()const{
  lock_guard<mtx_t> lock{m_mtxLock};
  if(!m_client) return;
  m_client->send(getMessageQueue().top());
  getMessageQueue().popTop();
}
vector<Subscription>&
MsgHandler::getSubscriptions(){
  static vector<Subscription> m_subs;
  return m_subs;
}
MsgQueue& MsgHandler::getMessageQueue(){
  static MsgQueue m_subs;
  return m_subs;
}
void
MsgHandler::NotifyAll(Message const& msg)const{
  for(auto& sub : getSubscriptions())
    sub.m_cbFun(msg);
}
