#include "CfgReader.hpp"
#include "Logger.hpp"
#include "MessageHandler.hpp"
#include "Subscription.hpp"
#include <iostream>
#include <thread>
using namespace std;
using namespace chrono;
using namespace literals::chrono_literals;

static MsgHandler s_handler;

void ping_race(MsgHandler& h,steady_clock::time_point end,std::string name){
  while(steady_clock::now() < end){
    MessagePtr msg{ new Message{} };
    msg->set("source",name);
    msg->set("prio","5");
    msg->set("text","ping");
    h.insert(msg);
    this_thread::sleep_for(10ms);
    h.remove(msg);
    this_thread::sleep_for(10ms);
  }}
void reader(){
  trace(INFO,"Reader started");
  MessagePtr msg = s_handler.receive();
  for(;!msg->empty(); msg = s_handler.receive()){
    s_handler.insert(msg);
    this_thread::sleep_for(1s);
  }
  trace(INFO,"Reader finished");
  ping_race(s_handler, steady_clock::now()+3s, "a-pong");
}
void sender(Message const& msg){
  trace(INFO,"Sender started");
  s_handler.send(make_shared<Message>(msg));
  trace(INFO,"Sender finished");
}
void doDiagnostic(){
  MsgHandler handler1;
  trace(INFO,"Setup diag MQClient");
  handler1.setClient("","log.log");
  handler1.addSubscription(Subscription{sender});
  trace(INFO,"Create diag message");
  MessagePtr msg {new Message()};
  msg->set("source", "seq1");
  msg->set("prio", "9");
  msg->set("text", "Some-diagnostic-stuff");
  trace(INFO,"Sending diag message");
  handler1.insert(msg);
  ping_race(handler1, steady_clock::now()+5s, "b-ping");
}
int main(int argc, char* argv[]) try{
  trace(INFO, "Running: %s", argv[0]);
  //configuration
  if(argc != 2)
    throw std::runtime_error("Expected 1 argument with configuration file.");
  trace(INFO, "Load configuration: %s", argv[1]);
  CfgReader cfg = getGlobalCfg()
    .critical_section([argv](auto& x){;
	x.LoadCfg(argv[1]);
	return x;});
  trace(INFO, "logTime: %i", cfg.m_logTime);
  trace(INFO, "msgInput: %s", cfg.m_msgInput);
  trace(INFO, "msgOutput: %s", cfg.m_msgOutput);
  trace(INFO, "sysName: %s", cfg.m_msgOutput);
  //Setup s_handler
  s_handler.setClient(cfg.m_msgInput, cfg.m_msgOutput);
  s_handler.addSubscription(Subscription{sender});
  std::thread t1 (reader);
  std::thread t2 (doDiagnostic);
  t1.join();
  t2.join();
  trace(INFO, "Print queue");
  for(MessagePtr top = s_handler.top();
      top && !top->empty();
      top = s_handler.top()){
    s_handler.remove(top);
  }
  return EXIT_SUCCESS;
}catch(exception const& e){
  trace(FATAL, e.what());
  return EXIT_FAILURE;
 }catch(...){
  trace(FATAL, "Unknown error");
  return EXIT_FAILURE;
 }
