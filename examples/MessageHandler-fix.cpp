#include "MessageHandler-fix.hpp"
#include <algorithm>
#include <atomic>
using namespace std;
//global functions
Resource<map<string,shared_ptr<Resource<MsgHandler>>>>&
getMsgHandlerMap(){
  using val_t = shared_ptr<Resource<MsgHandler>>;
  static Resource<map<string,val_t>> s_resource;
  return s_resource;
}
std::shared_ptr<Resource<MsgHandler>>
accessMsgHandler(string const& n){
  return getMsgHandlerMap()
    .critical_section([&n](auto& x){;
	auto it = x.find(n);
	return it == x.end()? nullptr : it->second;
	});
}
void registerMsgHandler(string n){
  if(auto i = make_unique<Resource<MsgHandler>>())
    return getMsgHandlerMap()
      .critical_section([&n,&i](auto& x){;
	  x[n].reset(i.release());});
}
//MQClient
MQClient::MQClient(string const& name):
  m_out{name}, m_name{name}
{}
void MQClient::setHandler(string const& handler){
  m_mux = accessMsgHandler(handler);
  if(m_mux)
    m_mux->critical_section([this](auto& x){;
	x.setOutput(this);});
}
void MQClient::send(MessagePtr const& msg){
  m_out<<msg->text<<endl;
}
void MQClient::receive(MessagePtr const& msg){
  if(m_mux)
    m_mux->critical_section([&msg](auto& x){;
	x.insert(msg);});
}
//MsgQueue
void MsgQueue::insert(MessagePtr const& msg){
  m_queue.push_back(msg);
  stable_sort(m_queue.begin(),m_queue.end(),
	      [](auto const& l,auto const& r){
		return l->prio < r->prio;});
}
MessagePtr MsgQueue::top() const noexcept{
  return m_queue.empty() ? nullptr : m_queue.front();
}
void MsgQueue::popTop(){
  m_queue.erase(m_queue.begin());
}
//MsgHandler
MsgHandler::subid_t MsgHandler::genNewId() noexcept{
  static std::atomic<uint32_t> id {0};
  return ++id;
}
void MsgHandler::setOutput(MQClient* client) noexcept{
  m_client = client;
}
MsgHandler::subid_t
MsgHandler::subscribe(cb_t cb){
  if(cb == nullptr) return 0;
  auto id = genNewId();
  return m_subs.emplace(id,cb).second ? id : 0;
}
void
MsgHandler::unsubscribe(subid_t id){
  auto it = m_subs.find(id);
  if(it != m_subs.end()) m_subs.erase(it);
}
void MsgHandler::insert(MessagePtr const& arg){
  m_queue.insert(arg);
  NotifyAll(*arg);
}
bool MsgHandler::isEmpty()const{
  return m_queue.top() != nullptr;
}
void MsgHandler::sendTop(){
  if(!m_client) return;
  m_client->send(m_queue.top());
  m_queue.popTop();
}
void MsgHandler::NotifyAll(Message const& msg) const{
  for(auto& sub : m_subs)
    sub.second(msg);
}

