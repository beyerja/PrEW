#ifndef LIB_CPPHELPNUM_H
#define LIB_CPPHELPNUM_H 1

namespace PrEW {
namespace CppUtils {

namespace Num {
  template<class T> bool equal_to_eps(const T & v1, const T & v2, const T eps=T(1e-9));
  
  double log_factorial (int x);
}

}
}

#include <CppUtils/Num.tpp>

#endif
