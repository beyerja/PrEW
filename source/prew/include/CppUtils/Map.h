#ifndef LIB_CPPHELPMAP_H
#define LIB_CPPHELPMAP_H 1

#include <map>
#include <set>
#include <vector>

namespace PrEW {
namespace CppUtils {

namespace Map {
  using StrToStrVecMap  = std::map<std::string,std::vector<std::string>>;
  using StrToStrPairMap = 
    std::map<std::string, std::pair<std::string, std::string>>;
  
  using DoubleToStrVecMap = std::map<double, std::vector<std::string>>;
  using IntToStrVecMap    = std::map<int, std::vector<std::string>>;
  
  using IntToStrSetMap = std::map<int, std::set<std::string>>;
}

}
}

#endif