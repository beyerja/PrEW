#ifndef LIB_CPPHELPSTR_H
#define LIB_CPPHELPSTR_H 1

#include <string>
#include <vector>

namespace PrEW {
namespace CppUtils {

namespace Str {
  template<class T> T cast_string(const std::string & str);
  
  std::vector<std::string> string_to_vec( const std::string &str, 
                                          std::string delimiter=" ");
  
  template<class T>          
  std::string sci_string(T number, int precision=7);
  
  std::string get_dir_path_str( const std::string & file_path );
} // namespace Str

} // namespace CppUtils
} // namespace PrEW

#include <CppUtils/Str.tpp>

#endif
