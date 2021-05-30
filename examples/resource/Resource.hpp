#ifndef RESOURCE_H
#define RESOURCE_H
#include <mutex>

template<typename T, typename M = std::mutex>
struct Resource{
  using value_t = T;
public:
  Resource(value_t const& arg): m_value{arg}
  { }
  Resource(value_t&& arg):m_value{std::move(arg)}
  { }
  template <typename... Args>
  Resource(Args&&... args): m_value{std::forward<Args>(args)...}
  { }
  template<typename FunT>
  decltype(std::declval<FunT>()(std::declval<value_t&>()))
  critical_section(FunT arg){
    std::lock_guard<M> lock{m_mutex};
    return arg(m_value);
  }
private:
  Resource(Resource const&) = delete;
  Resource(Resource&&) = delete;
  Resource& operator =(Resource const&) = delete;
  Resource& operator =(Resource&&) = delete;
private:
  M m_mutex;
  value_t m_value;
};
#endif /* RESOURCE_H */
