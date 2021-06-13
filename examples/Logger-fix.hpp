#ifndef LOGGER_H
#define LOGGER_H
#include <sstream>
#include <iostream>
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
void trace(A const&...args){
  std::ostringstream oss;
  trace_sysinfo(oss);
  trace_impl(oss,args...);
  oss<<'\n';
  std::cout<<oss.str()<<std::flush;
}
#endif /* LOGGER_H */
