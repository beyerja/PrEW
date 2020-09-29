#ifndef LIB_CPPHELPSTR_TPP
#define LIB_CPPHELPSTR_TPP 1

#include <CppUtils/Str.h>

// Standard library
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

namespace PrEW {
namespace CppUtils {

//------------------------------------------------------------------------------

template <class T> T Str::cast_string(const std::string &str) {
  /** Try casting the given string to the requested type.
   **/
  if (typeid(T).name() == typeid(double(0.0)).name()) {
    return T(std::stod(str));
  } else if (typeid(T).name() == typeid(float(0.0)).name()) {
    return T(std::stof(str));
  } else if (typeid(T).name() == typeid(int(0)).name()) {
    return T(std::stoi(str));
  } else {
    throw std::invalid_argument("Requested cast to unknown type: " +
                                std::string(typeid(T).name()));
  }
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

} // namespace CppUtils
} // namespace PrEW

#endif
