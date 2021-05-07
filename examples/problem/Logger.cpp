#include "Logger.hpp"
#include "CfgReader.hpp"
#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <thread>
#include <sstream>
char const* s_category[] =
  {"FATAL","ERROR","WARNING","INFO"};

void trace(TraceLevel lvl,char const* fmt, ...){
  char buf[100];
  va_list argptr;
  va_start(argptr,fmt);
  std::ostringstream id;
  id << std::this_thread::get_id();
  if(CfgReader::s_logTime){
    time_t t = time(nullptr);
    sprintf(buf,"%s(%s[%s])[%s] %s\n",ctime(&t),CfgReader::s_sysName.c_str(),id.str().c_str(),s_category[lvl],fmt);
  }else
    sprintf(buf,"(%s[%s])[%s] %s\n",CfgReader::s_sysName.c_str(),id.str().c_str(),s_category[lvl],fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
