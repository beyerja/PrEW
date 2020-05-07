#ifndef LIB_CPPHELPVEC_H
#define LIB_CPPHELPVEC_H 1

#include <functional>
#include <vector>

namespace PrEW {
namespace CppUtils {

namespace Vec {
  template <class T> using Matrix2D = std::vector<std::vector<T>>;
  
  template <class T, class F>
  std::vector<T> subvec_by_condition( const std::vector<T> &vec, 
                                      F &condition );
  template <class T, class F>
  T element_by_condition( const std::vector<T> &vec, F &condition );
}

}
}

#include <CppUtils/Vec.tpp>

#endif