#include "Logger.hpp"
#include "CfgReader.hpp"
#include <cstdarg>
#include <cstdio>

char const* s_category[] =
  {"FATAL","ERROR","WARNING","INFO"};

void trace(TraceLevel lvl,char const* fmt, ...){
  char buf[100];
  va_list argptr;
  va_start(argptr,fmt);
  if(CfgReader::s_logTime){
    time_t t = time(nullptr);
    sprintf(buf,"%s [%s] %s\n",ctime(&t),s_category[lvl],fmt);
  }else
    sprintf(buf,"[%s] %s\n",s_category[lvl],fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
