#ifndef LIB_CPPHELPFNCT_H
#define LIB_CPPHELPFNCT_H 1

#include <functional>
#include <vector>

namespace PREW {
namespace CppUtils {

namespace Fct {
  /** Namespace for C++ helper functions and classes related to std::function.
  **/
  
  // Function which has three input parameter vectors,
  // In PrEW like: coordinates,parameters,coefficients
  using ParametrisationFct = std::function<double(const std::vector<double> &,
                                                   const std::vector<double> &,
                                                   const std::vector<double*> &)
                                            >;
}

}
}

#endif