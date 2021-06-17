#include "Logger-fix.hpp"
#include "AsymResource.hpp"
#include "debug-mutex.hpp"
#include <thread>
using namespace std;
AsymResource<int> res {0};

void read(const char* name){
  for(int i=0;i<10;++i,this_thread::sleep_for(100ms))
    trace(name," = ",res.critical_section_read([i](auto const& x){return x;}));
}
void write(){
  for(int i=0;i<10;++i,this_thread::sleep_for(100ms))
    res.critical_section_write([i](auto& x){x = i;});
 }  
int main(int argc, char *argv[]){
  trace("Run test");
  thread t1(read,"read1"),t2(read,"read2"),t3(write);
  t1.join();t2.join();t3.join();
  return EXIT_SUCCESS;
}
