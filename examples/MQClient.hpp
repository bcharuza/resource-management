#ifndef MQCLIENT_H
#define MQCLIENT_H
#include <string>
class MsgHandler;
struct MQClient {
  MQClient(std::string name):
    m_out{name}, m_name{name}
  {}
  void setHandler(MsgHandler* handler)
  { m_handler = handler; }
  void send(MessagePtr const& msg){
    trace("{(%i) %s}",++m_cnt, msg->text);
    m_out<<m_cnt<<msg->text<<std::endl;
  }
  void receive(MessagePtr const&);
private:
  MsgHandler* m_handler = nullptr;
  std::ofstream m_out;
  std::string m_name;
  int m_cnt = 0;
};
#endif /* MQCLIENT_H */
