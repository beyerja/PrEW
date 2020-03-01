#ifndef LIB_CPPHELPSTR_H
#define LIB_CPPHELPSTR_H 1

#include <string>
#include <vector>

namespace PREW {
namespace CppUtils {

namespace Str {
  std::vector<std::string> string_to_vec( const std::string &str, 
                                          std::string delimiter=" ");
  
  template<class T>          
  std::string sci_string(T number, int precision=7);
}

}
}

#include <CppUtils/Str.tpp>

#endif
