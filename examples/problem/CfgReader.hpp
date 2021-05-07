#ifndef CFGREADER_H
#define CFGREADER_H
#include <fstream>

struct CfgReader{
  enum TransformType { NONE, LOWER, UPPER, POKEMON };
public:
  static bool s_logTime;
  static std::string s_sysName;
  static TransformType s_msgFmt;
  static std::string s_msgInput;
  static std::string s_msgOutput;
public:
  void LoadCfg(std::string const& file);
};
#endif /* CFGREADER_H */
