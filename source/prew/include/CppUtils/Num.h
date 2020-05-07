#ifndef LIB_CPPHELPNUM_H
#define LIB_CPPHELPNUM_H 1

namespace PrEW {
namespace CppUtils {

namespace Num {
  template<class T> bool equal_to_eps(const T & v1, const T & v2, const T eps=1e-9);
}

}
}

#include <CppUtils/Num.tpp>

#endif
