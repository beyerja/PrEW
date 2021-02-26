#ifndef LIB_CPPHELPNUM_TPP
#define LIB_CPPHELPNUM_TPP 1

#include <CppUtils/Num.h>

// Standard library
#include <cmath>
#include <stdexcept>
#include <type_traits> // To check type of template class

namespace PrEW {
namespace CppUtils {

//------------------------------------------------------------------------------

template <class T>
bool Num::equal_to_eps(const T &v1, const T &v2, const T eps) {
  /** Compare two floating point numbers using a maximum distance epsilon.
   **/
  static_assert(std::is_floating_point<T>::value,
                "equal_to_eps can only be called with floating point types");

  return fabs(v1 - v2) < eps;
}

//------------------------------------------------------------------------------

template <class T>
bool Num::equal_to_eps(const std::vector<T> &v1, const std::vector<T> &v2,
                       const T eps) {
  /** Compare all the floating point numbers in the two vectors.
   **/
  if (v1.size() != v2.size()) {
    throw std::invalid_argument("Vectors must have equal size.");
  }
  bool are_same = true;
  for (size_t i=0; i<v1.size(); i++) {
    if (! equal_to_eps<T>(v1[i],v2[i],eps)) {
      are_same = false;
      break;
    }
  }
  return are_same;
}

//------------------------------------------------------------------------------

template <class T>
void Num::csum(T &a,T b,T &c) { //this function adds a and b, and passes c as a compensation term
    T y = b-c; //y is the correction of b argument
    b = a+y; //add corrected b argument to a argument. The output of the current summation
    c = (b-a)-y; //find new error to be passed as a compensation term
    a = b;
}

//------------------------------------------------------------------------------

} // namespace CppUtils
} // namespace PrEW

#endif