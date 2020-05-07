#ifndef LIB_CPPHELPVEC_TPP
#define LIB_CPPHELPVEC_TPP 1

#include <CppUtils/Vec.h>

#include <algorithm>

namespace PrEW {
namespace CppUtils {

//------------------------------------------------------------------------------

template <class T, class F>
std::vector<T> Vec::subvec_by_condition( 
  const std::vector<T> &vec, 
  F &condition
) {
  /** Find a subvector of vec in which all elements return true on the given 
      condition function.
  **/
  std::vector<T> subvec (vec.size());
  // Copy all elements that fulfill the condition
  auto it = std::copy_if( vec.begin(), vec.end(), subvec.begin(), 
                          condition );
  subvec.resize(std::distance(subvec.begin(),it)); // Crop empty end of subvec
  return subvec;
}

//------------------------------------------------------------------------------

template <class T, class F>
T Vec::element_by_condition( const std::vector<T> &vec, F &condition ) {
  /** Find and return the first element in the vector that fulfills the given
      condition (i.e. returns true when condition function is called).
  **/
  auto it = std::find_if( vec.begin(), vec.end(), condition );
  return *it;
}

//------------------------------------------------------------------------------

}
}

#endif