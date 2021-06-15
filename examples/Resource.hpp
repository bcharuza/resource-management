#ifndef RESOURCE_H
#define RESOURCE_H
#include <mutex>
template<typename T, typename M = std::mutex>
struct Resource{
  template <typename... Args>
  Resource(Args&&... args):
    m_value{std::forward<Args>(args)...}
  { }
  template<typename F>
  auto critical_section(F&& arg){
    std::lock_guard<M> lock{m_mutex};
    return arg(m_value);
  }
  Resource(Resource const&) = delete;
  Resource(Resource&&) = delete;
  Resource& operator =(Resource const&) = delete;
  Resource& operator =(Resource&&) = delete;
private:
  M m_mutex;
  T m_value;
};
#endif /* RESOURCE_H */
