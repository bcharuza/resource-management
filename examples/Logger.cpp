#include "Logger.hpp"
#include "CfgReader.hpp"
#include <cstdarg>
void trace(char const* fmt,...){
  char buf[128];
  va_list argptr;
  va_start(argptr,fmt);
  time_t t = time(nullptr);
  sprintf(buf, "%s %s@%s: %s",
	  ctime(&t),
	  CfgReader::s_userName.c_str(),
	  CfgReader::s_sysName.c_str(),
	  fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
