#ifndef DEBUG_MUTEX_H
#define DEBUG_MUTEX_H
#include <stdexcept>
static int& accessCounter(){
  static thread_local int count=0;
  return count;
}
template<typename T>
struct DebugMutex : public T
{
  void lock(){
    if(accessCounter()) throw std::runtime_error("Double resource acquisition");
    ++accessCounter();
    T::lock();
  }
  void unlock(){
    T::unlock();
    --accessCounter();
  }
};
#endif /* DEBUG_MUTEX_H */
