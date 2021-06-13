#include "CfgReader.hpp"
#include <fstream>
using namespace std;

string CfgReader::s_sysName {""};
string CfgReader::s_userName {""};

void
CfgReader::LoadCfg(string const& file){
  ifstream ifs {file};
  if(!ifs.is_open())
    throw runtime_error("Can't open: "+file);
  std::getline(ifs, s_sysName);
  std::getline(ifs, s_userName);
 }
