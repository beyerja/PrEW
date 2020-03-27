#ifndef LIB_CPPHELPSTR_TPP
#define LIB_CPPHELPSTR_TPP 1

#include <CppUtils/Str.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace PREW {
namespace CppUtils {
  
template<class T>          
std::string Str::sci_string(T number, int precision){
  /** Return a string of the given number in scientific notation.
  **/
  std::ostringstream stream;
 
  // Switch to scientific notation with given precision and add double to stream
  stream << std::scientific << std::setprecision(precision) << number;
   
  // Get string from output string stream
  return stream.str(); 
}

}
}

#endif
