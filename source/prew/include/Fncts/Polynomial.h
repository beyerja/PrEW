#ifndef LIB_POLYNOMIAL_H
#define LIB_POLYNOMIAL_H 1

#include <vector>

namespace PREW {
namespace Fncts {
  
namespace Polynomial {
  /** Namespace for polynomial parametrisation functions.
      Must all follow structure:
        double fnct_name (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/
  
  double quadratic_1D ( const std::vector<double>   &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
                        
}
  
}
}

#endif