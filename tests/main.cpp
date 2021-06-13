#include "CfgReader.hpp"
#include "Logger.hpp"
#include "MessageHandler.hpp"
#include <thread>
#include <atomic>
#include <algorithm>
using namespace std;

void cyclicReader(MQClient* mq,MsgHandler* h){
  static atomic<int> s_id {0};
  int id = ++s_id;
  trace("Spawn thread: %i", id);
  h->setClient(mq);
  while(mq->receive())
    this_thread::sleep_for(100ms);
}
void printHandler(MsgHandler* h){
  this_thread::sleep_for(200ms);
  while(!h->isEmpty()){
    h->sendTop();
    this_thread::sleep_for(100ms);
  }
}
int main(int argc, char *argv[]){
  std::vector<std::unique_ptr<MQClient>> clients;
  trace("Running %s with args:",argv[0]);
  for(int i=0;i<argc;++i)
    trace("argv[%i]=%s",i,argv[i]);
  trace("Loading configuration from %s", argv[1]);
  CfgReader cfg;
  cfg.LoadCfg(argv[1]);
  trace("Configuration loaded");
  MsgHandler mux;
  mux.addSubscription(Subscription([](Message const& msg){trace("Received: %i %s",msg.prio,msg.text.c_str());}));
  trace("Spawning threads");
  vector<thread> threads;
  for(int i=2; i<argc; ++i){
    clients.push_back(make_unique<MQClient>(argv[i]));
    threads.push_back(thread(cyclicReader,clients.back().get(),&mux));
  }
  printHandler(&mux);
  trace("Joining threads");
  for_each(threads.begin(),threads.end(), mem_fn(&thread::join));
  return EXIT_SUCCESS;
}
