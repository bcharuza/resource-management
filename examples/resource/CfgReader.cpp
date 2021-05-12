#include "CfgReader.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cctype>
#include <map>
using namespace std;

string lower(string str){
  transform(str.begin(),str.end(), str.begin(),
	    [](char x){ return tolower(x);});
  return str;
}
void
CfgReader::LoadCfg(string const& file){
  ifstream ifs {file};
  if(!ifs.is_open())
    throw runtime_error("Can't open: "+file);
  map<string,string> cfg;
  string tag,val;
  while((ifs >> tag >> val))
    cfg.emplace(tag,val);
  m_logTime = lower(cfg["logTime"]) == "true";
  m_sysName = cfg["sysName"];
  m_msgInput = cfg["msgInput"];
  m_msgOutput = cfg["msgOutput"];
 }
