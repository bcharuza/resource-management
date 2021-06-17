#include "Logger-fix.hpp"
#include "Resource.hpp"
#include "debug-mutex.hpp"
#include <thread>
using namespace std;
template <typename T> using DebugResource = ::Resource<T,DebugMutex<std::mutex>>;
DebugResource<int> res;
DebugResource<int> res2;

void shallow(){
  for(int i=0;i<10;++i,this_thread::sleep_for(100ms))
    res.critical_section([i](auto& x){x = i;});
}
void deep() try{
  for(int i=0;i<10;++i,this_thread::sleep_for(100ms))
    res2.critical_section([i](auto& x){x = i;shallow();});
 }catch(std::exception& e)
  { trace("Thrown: ",e.what()); }
  
int main(int argc, char *argv[]){
  trace("Run shallow");
  thread t1(shallow);
  thread t2(shallow);
  t1.join();t2.join();
  trace("Run deep");
  thread t3(deep);
  thread t4(deep);    
  t3.join();t4.join();
  return EXIT_SUCCESS;
}
