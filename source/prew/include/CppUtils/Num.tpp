#ifndef LIB_CPPHELPNUM_TPP
#define LIB_CPPHELPNUM_TPP 1

#include <CppUtils/Num.h>

#include <type_traits> // To check type of template class
#include <cmath>

namespace PREW {
namespace CppUtils {

//------------------------------------------------------------------------------

template<class T> 
bool Num::equal_to_eps(const T & v1, const T & v2, const T eps) {
  /** Compare two floating point numbers using a maximum distance epsilon.
  **/
  static_assert(std::is_floating_point<T>::value,
    "equal_to_eps can only be called with floating point types");

  return fabs(v1 - v2) < eps;
}

//------------------------------------------------------------------------------

}
}

#endif