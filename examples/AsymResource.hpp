#ifndef ASYMRESOURCE_H
#define ASYMRESOURCE_H
#include <shared_mutex>
#include <utility>
template<typename T>
struct AsymResource{
  using mutex_t = std::shared_timed_mutex;
  template <typename... Args>
  AsymResource(Args&&... args):
    m_value{std::forward<Args>(args)...}
  { }
  template<typename F>
  auto critical_section_read(F&& arg){
    std::shared_lock<mutex_t> slk(m_mutex);
    return arg(const_cast<T const&>(m_value));
  }
  template<typename F>
  auto critical_section_write(F&& arg){
    std::lock_guard<mutex_t> lock{m_mutex};
    return arg(m_value);
  }
  AsymResource(AsymResource const&) = delete;
  AsymResource(AsymResource&&) = delete;
  AsymResource& operator =(AsymResource const&) = delete;
  AsymResource& operator =(AsymResource&&) = delete;
private:
  mutex_t m_mutex;
  T m_value;
};
#endif /* ASYMRESOURCE_H */
