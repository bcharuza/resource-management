#include "CfgReader.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cctype>
#include <map>
using namespace std;

bool CfgReader::s_logTime {true};
string CfgReader::s_sysName {""};
string CfgReader::s_msgInput {""};
string CfgReader::s_msgOutput {""};

string lower(string str){
  transform(str.begin(),str.end(), str.begin(),
	    [](char x){ return tolower(x);});
  return str;
}
void
CfgReader::LoadCfg(string const& file){
  ifstream ifs {file};
  map<string,string> cfg;
  string tag,val;
  while((ifs >> tag >> val))
    cfg.emplace(tag,val);
  s_logTime = lower(cfg["logTime"]) == "enable";
  s_sysName = cfg["sysName"];
  s_msgInput = cfg["msgInput"];
  s_msgOutput = cfg["msgOutput"];
 }
