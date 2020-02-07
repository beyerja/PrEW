#include <CppUtils/Str.h>

namespace PREW {
namespace CppUtils {

//------------------------------------------------------------------------------


std::vector<std::string> Str::string_to_vec( 
  const std::string &str, 
  std::string delimiter ) 
{
  /** Splits string into vector of substrings.
      Splitting is defined by the given delimiter (default=whitespace).
  **/
  std::vector<std::string> substrings {};
  size_t last = 0; size_t next = 0;
  std::string substring {};
  while ( (next = str.find(delimiter, last) ) != std::string::npos ) {
    substring = str.substr(last, next-last);
    last = next + 1; 
    if ( substring != "" ) { substrings.push_back(substring); }
  }
  substring = str.substr(last);
  if ( substring != "" ) { substrings.push_back(substring); }
  
  return substrings;
}


//------------------------------------------------------------------------------
  
}
}