#include "Logger-fix.hpp"
#include "CfgReader-fix.hpp"
#include <chrono>
#include <sstream>
using namespace std;
using namespace std::chrono;
void trace_sysinfo(ostream& o){
  auto r=getCfg().critical_section([](auto& x){;
      return make_pair(x["usr"],x["sys"])
	;});
  auto t=system_clock::now().time_since_epoch();
  auto c=duration_cast<microseconds>(t).count();
  trace_impl(o,c,' ', r.first,'@',r.second,": ");
}
