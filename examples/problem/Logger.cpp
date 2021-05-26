#include "Logger.hpp"
#include "CfgReader.hpp"
#include <cstdarg>

static char const*
cat(TraceLevel arg) noexcept{
  static char const* l[] =
    {"FATAL","ERROR","WARNING","INFO","<NULL>"};
  static const size_t s = sizeof(l)/sizeof(char*);
  return l[std::min((size_t)arg, s)];
}
void trace(TraceLevel lvl,char const* fmt,...){
  char buf[64];
  va_list argptr;
  va_start(argptr,fmt);
  if(CfgReader::s_logTime){
    time_t t = time(nullptr);
    sprintf(buf,"%s(%s[t:%i])[%s] %s\n",ctime(&t),CfgReader::s_sysName.c_str(),CfgReader::threadId(),cat(lvl),fmt);
  }else
    sprintf(buf,"(%s[t:%i])[%s] %s\n",CfgReader::s_sysName.c_str(),CfgReader::threadId(),cat(lvl),fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
