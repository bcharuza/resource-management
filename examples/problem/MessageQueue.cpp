#include "MessageQueue.hpp"
#include "Subscription.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

void
Message::set(key_t const& key, val_t const& val)
{ m_props[key] = val; }

Message::val_t
Message::get(key_t const& key) const {
  auto it = m_props.find(key);
  return it != m_props.end() ? it->second : "";
}
string Message::str() const {
  string result = "[ ";
  for(auto const& p : m_props)
    ((result += p.first) += ' ') += p.second;
  return result += "]";
}
Message::Message(string const& str){
  istringstream iss{str};
  if(str.front() != '[' || str.back() != ']')
    throw runtime_error("Wrong message format");
  iss.get();
  string tag, val;
  while((iss >> tag >> val))
    m_props[tag]=val;
}
void MsgQueue::insert(MessagePtr msg){
  string s = msg->get("source");
  auto it = find_if(m_queue.begin(),m_queue.end(),
		    [&s](MessagePtr m){
		   return m->get("source") == s;});
  if(it != m_queue.end()) *it = msg;
  else m_queue.push_back(msg);
  stable_sort(m_queue.begin(),m_queue.end(),
      [](auto const& m1, auto const& m2){
	return m1->get("prio") < m2->get("prio");
      });
}
void MsgQueue::remove(std::string const& s){
  auto it = remove_if(m_queue.begin(),m_queue.end(),
		    [&s](MessagePtr m){
		      return m->get("source") == s;});
  m_queue.erase(it);
}
MessagePtr MsgQueue::top(){
  return m_queue.empty() ? nullptr : m_queue.front();
}
