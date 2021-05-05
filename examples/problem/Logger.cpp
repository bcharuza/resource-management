#include "Logger.hpp"
#include <cstdio>
#include <cstdarg>

char const* s_category[] =
  {"FATAL","ERROR","WARNING","INFO"};

void trace(TraceLevel lvl,char const* fmt, ...)
{
  char buf[100];
  va_list argptr;
  va_start(argptr,fmt);
  sprintf(buf,"[%s] %s\n",s_category[lvl],fmt);
  vfprintf(stderr,buf,argptr);
  va_end(argptr);
}
