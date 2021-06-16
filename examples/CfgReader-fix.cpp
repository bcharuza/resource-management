#include "CfgReader-fix.hpp"
#include <fstream>
using namespace std;
void LoadCfg(string const& file){
  map<string,string> map;
  ifstream ifs {file};
  if(!ifs.is_open())
    throw runtime_error("Can't open: "+file);
  getline(ifs, map["sys"]);
  getline(ifs, map["usr"]);
  getCfg().critical_section([&map](auto& x){
			      x = move(map);});
 }
