#ifndef CFGREADER_H
#define CFGREADER_H
#include <string>
struct CfgReader{
  static std::string s_sysName;
  static std::string s_userName;
  void LoadCfg(std::string const& file);
};
#endif /* CFGREADER_H */
