#include "CfgReader.hpp"
#include "Logger.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
using namespace literals::chrono_literals;

int compute(int i)
{ this_thread::sleep_for(10ms); return ++i;}

int main(int argc, char* argv[]){
  if(argc != 2){
      trace(FATAL, "Configuration file must be passed as the argument");
      return EXIT_FAILURE;
    }
  vector<thread> threads;
  trace(INFO,"%s", "Enter main");
  for(int i=0;i<100; ++i)
    threads.push_back(thread([](int i){;
	  if(i%2) cout<<"Spawning proces "<<i<<": "<<i<<"+1="<<compute(i)<<endl;
	  else trace(INFO, "Spawning proces %i: %i+1=%i", i, i, compute(i)); }, i));
  for(auto& t : threads)
    t.join();
  return EXIT_SUCCESS;
}
