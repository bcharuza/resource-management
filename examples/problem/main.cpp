#include "Logger.hpp"

#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std::literals::chrono_literals;

struct MainClass
{ MainClass(){ trace(INFO, "Created main class");} };

int compute(int i){ std::this_thread::sleep_for(10ms); return ++i;}

int main(int, char*[])
{
  static MainClass x;
  std::vector<std::thread> threads;
  trace(INFO,"%s", "Enter main");
  for(int i=0;i<100; ++i)
    threads.push_back(std::thread([](int i){;
	  if(i%2) std::cout<<"Spawning proces "<<i<<": "<<i<<"+1="<<compute(i)<<std::endl;
	  else
	    trace(INFO, "Spawning proces %i: %i+1=%i", i, i, compute(i)); }, i));
  for(auto& t : threads)
    t.join();
  return 0;
}
