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
  std::string sysName;
  bool logTime;
  std::tie(logTime,sysName) =
    getGlobalCfg().critical_section([](auto const& x){;
	return std::make_pair(x.m_logTime,x.m_sysName);
				    });
  if(logTime){
    time_t t = time(nullptr);
    sprintf(buf,"%s(%s[t:%i])[%s] %s\n",ctime(&t),sysName.c_str(),getThreadId(),s_category[lvl],fmt);
  }else
    sprintf(buf,"(%s[t:%i])[%s] %s\n",sysName.c_str(),getThreadId(),s_category[lvl],fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
