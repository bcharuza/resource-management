#ifndef RESOURCE_H
#define RESOURCE_H
#include <mutex>

template<typename T>
struct Resource{
  Resource(T const& arg): m_value{arg}
  { }
  Resource(T&& arg): m_value{std::move(arg)}
  { }
  template <typename... Args>
  Resource(Args... args): m_value{args...}
  { }
  template<typename FunT>
  decltype(std::declval<FunT>()(std::declval<T&>()))
  critical_section(FunT arg){
    std::lock_guard<std::mutex> lock{m_mutex};
    return arg(m_value);
  }
private:
  Resource(Resource const&) = delete;
  Resource(Resource&&) = delete;
  Resource& operator =(Resource const&) = delete;
  Resource& operator =(Resource&&) = delete;
private:
  std::mutex m_mutex;
  T m_value;
};
#endif /* RESOURCE_H */
