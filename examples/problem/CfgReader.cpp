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
  if(!ifs.is_open())
    throw runtime_error("Can't open: "+file);
  map<string,string> cfg;
  string tag,val;
  while((ifs >> tag >> val))
    cfg.emplace(tag,val);
  s_logTime = lower(cfg["logTime"]) == "true";
  s_sysName = cfg["sysName"];
  s_msgInput = cfg["msgInput"];
  s_msgOutput = cfg["msgOutput"];
 }
int
CfgReader::threadId() noexcept{
  static int s_counter = 0;
  static thread_local int s_id = ++s_counter;
  return s_id;
 }
