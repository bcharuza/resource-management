#ifndef LOGGER_H
#define LOGGER_H

enum TraceLevel{FATAL, ERROR, WARNING, INFO};

extern
void trace(TraceLevel,char const* fmt, ...);
#endif /* LOGGER_H */
