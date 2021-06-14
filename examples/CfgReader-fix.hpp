#ifndef CFGREADER_H
#define CFGREADER_H
#include "Resource.hpp"
#include <string>
#include <map>
inline auto& getCfg(){
  using namespace std;
  static Resource<map<string,string>> s_cfg;
  return s_cfg;
}
extern void LoadCfg(std::string const& file);
#endif /* CFGREADER_H */
