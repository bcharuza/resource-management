#include "CfgReader.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cctype>
#include <map>
using namespace std;

bool CfgReader::s_logTime {false};
string CfgReader::s_sysName {};
string CfgReader::s_msgInput {};
string CfgReader::s_msgOutput {};

CfgReader::TransformType
CfgReader::s_msgFmt = NONE;

inline string lower(string str){
  transform(str.begin(),str.end(), str.begin(),
	    [](char x){ return tolower(x);});
  return str;
}
static
CfgReader::TransformType
parse(string str){
  if(str == "none")
    return CfgReader::TransformType::NONE;
  if(str == "lower")
    return CfgReader::TransformType::LOWER;
  if(str == "upper")
    return CfgReader::TransformType::UPPER;
  if(str == "pokemon")
    return CfgReader::TransformType::POKEMON;
  throw runtime_error(str + " not recognized");
}
void
CfgReader::LoadCfg(string const& file) try{
  ifstream ifs {file};
  map<string,string> cfg;
  string tag,val;
  while((ifs >> tag >> val))
    cfg.emplace(tag,val);
  s_logTime = lower(cfg["logTime"]) == "enable";
  s_sysName = cfg["sysName"];
  s_msgFmt = parse(lower(cfg["msgFmt"]));
  s_msgInput = cfg["msgInput"];
  s_msgOutput = cfg["msgOutput"];
 }catch(exception& e){
  trace(TraceLevel::FATAL, "%s", e.what());
 }
