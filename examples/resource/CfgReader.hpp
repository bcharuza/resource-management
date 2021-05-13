#ifndef CFGREADER_H
#define CFGREADER_H
#include "Resource.hpp"
#include <string>

struct CfgReader{
  bool m_logTime;
  std::string m_sysName;
  std::string m_msgInput;
  std::string m_msgOutput;
public:
  void LoadCfg(std::string const& file);
};
inline Resource<CfgReader>& getGlobalCfg(){
  static Resource<CfgReader> s_config {
     .m_logTime = false,
     .m_sysName = "abc",
     .m_msgInput = "",
     .m_msgOutput = ""};
  return s_config;
}
inline int getThreadId() noexcept{
  static int s_counter = 0;
  static thread_local int s_id = ++s_counter;
  return s_id;
}

#endif /* CFGREADER_H */
