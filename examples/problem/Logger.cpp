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
  
  if(CfgReader::s_logTime){
    time_t t = time(nullptr);
    sprintf(buf,"%s(%s[t:%i])[%s] %s\n",ctime(&t),CfgReader::s_sysName.c_str(),CfgReader::threadId(),s_category[lvl],fmt);
  }else
    sprintf(buf,"(%s[t:%i])[%s] %s\n",CfgReader::s_sysName.c_str(),CfgReader::threadId(),s_category[lvl],fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
