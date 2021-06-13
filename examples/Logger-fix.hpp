#ifndef LOGGER_H
#define LOGGER_H
#include <sstream>
extern void trace_sysinfo(std::ostream& oss);
template<typename T>
void trace_print(std::ostream& o,T const& a){
  o << a;
}
namespace {
  template<typename T> inline
  void trace_impl(std::ostream& o,T const& h){
    trace_print(o,h);
  }
  template<typename T,typename...Args> inline
  void trace_impl(std::ostream& o,T const& h,
		  Args const&... args){
    trace_print(o,h);
    trace_impl(o,args...);
  }
}
template<typename... A> inline
void trace(std::ostream& out, A const&...args){
  std::ostringstream oss;
  trace_sysinfo(oss);
  trace_impl(args...);
  out<<oss.str();
}
#endif /* LOGGER_H */
