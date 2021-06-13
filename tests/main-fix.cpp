#include "CfgReader-fix.hpp"
#include "Logger-fix.hpp"
#include "MessageHandler-fix.hpp"
#include <thread>
#include <atomic>
#include <algorithm>
#include <vector>
using namespace std;

void cyclicReader(MQClient* mq){
  static atomic<int> s_id {0};
  int id = ++s_id;
  trace("Spawn thread: ",id);
  mq->setHandler("mainMux");
  while(mq->receive())
    this_thread::sleep_for(100ms);
}
void printHandler(){
  this_thread::sleep_for(200ms);
  auto mux = accessMsgHandler("mainMux");
  while(mux->critical_section([](auto& x){;
	if(x.isEmpty()) return false;
	x.sendTop();
	return true;}))
    this_thread::sleep_for(100ms);
}
int main(int argc, char *argv[]){
  std::vector<std::unique_ptr<MQClient>> clients;
  trace("Running with args: ",argv[0]);
  for(int i=0;i<argc;++i)
    trace("argv[",i,"]=",argv[i]);
  trace("Loading configuration from ", argv[1]);
  LoadCfg(argv[1]);
  trace("Configuration loaded");
  setupMsgHandler("mainMux");
  accessMsgHandler("mainMux")->critical_section([](auto& x){;
      x.subscribe([](Message const& msg){trace("Received: ",msg.prio," ",msg.text);});
						});
  trace("Spawning threads");
  vector<thread> threads;
  for(int i=2; i<argc; ++i){
    clients.push_back(make_unique<MQClient>(argv[i]));
    threads.push_back(thread(cyclicReader,clients.back().get()));
  }
  printHandler();
  trace("Joining threads");
  for_each(threads.begin(),threads.end(), mem_fn(&thread::join));
  return EXIT_SUCCESS;
}
