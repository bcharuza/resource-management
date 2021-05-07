#include "CfgReader.hpp"
#include "Logger.hpp"
#include "MessageHandler.hpp"
#include "Subscription.hpp"
#include <iostream>
#include <thread>
using namespace std;
using namespace literals::chrono_literals;

static MsgHandler s_handler;

void reader(MQClient* client)
{
  trace(INFO,"Reader started");
  MessagePtr msg = client->receive();
  for(;!msg->empty(); msg = client->receive()){
    s_handler.insert(msg);
    this_thread::sleep_for(1s);
  }
  trace(INFO,"Reader finished");
}
void sender(Message const& msg){
  trace(INFO,"Sender started");
  s_handler.send(make_shared<Message>(msg));
  trace(INFO,"Sender finished");
}
void doDiagnostic(){
  MsgHandler handler1;
  auto mq1 = new MQClient{"", "log"};
  handler1.setClient(mq1);
  handler1.addSubscription(Subscription{sender});
  MessagePtr msg {new Message()};
  msg->set("source", "seq1");
  msg->set("prio", "9");
  msg->set("text", "Some diagnostic stuff");
  handler1.send(msg);
}
int main(int argc, char* argv[]) try{
  trace(INFO, "Running: %s", argv[0]);
  //configuration
  if(argc != 2)
    throw std::runtime_error("Expected 1 argument with configuration file.");
  trace(INFO, "Load configuration: %s", argv[1]);
  CfgReader cfg;
  cfg.LoadCfg(argv[1]);
  //Setup s_handler
  auto mq2 = new MQClient{CfgReader::s_msgInput, CfgReader::s_msgOutput};
  s_handler.setClient(mq2);
  s_handler.addSubscription(Subscription{sender});
  std::thread t1 (reader, mq2);
  std::thread t2 (doDiagnostic);
  t1.join();
  t2.join();
  delete mq2;
  return EXIT_SUCCESS;
}catch(exception const& e){
  trace(FATAL, e.what());
  return EXIT_FAILURE;
 }catch(...){
  trace(FATAL, "Unknown error");
  return EXIT_FAILURE;
 }
