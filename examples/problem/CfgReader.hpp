#ifndef CFGREADER_H
#define CFGREADER_H
#include <fstream>

struct CfgReader{
  static bool s_logTime;
  static std::string s_sysName;
  static std::string s_msgInput;
  static std::string s_msgOutput;
public:
  void LoadCfg(std::string const& file);
  static int threadId() noexcept;
};
#endif /* CFGREADER_H */
