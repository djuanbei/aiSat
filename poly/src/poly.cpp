
#include<poly.hpp>
namespace aiSat {
using namespace std;


string trim(const string& str){
  
  size_t first = str.find_first_not_of(' ');
  if (string::npos == first)  {
    return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  std::vector<std::string> revec;
  
  for(vector<string>::iterator it=elems.begin(); it!= elems.end(); it++){
    string temp=trim(*it);
    if(!temp.empty()){
      revec.push_back(temp);
    }
  }
  
  return revec;
}


}
