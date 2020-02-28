#ifndef LIB_POLYNOMIAL_H
#define LIB_POLYNOMIAL_H 1

#include <vector>

namespace PREW {
namespace Fcts {
  
namespace Polynomial {
  /** Namespace for polynomial parametrisation functions.
      Must all follow structure:
        double fct_name (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/
  
  double constant_coef ( const std::vector<double>   &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  double constant_par ( const std::vector<double>   &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
                        
  double quadratic_1D ( const std::vector<double>   &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
                        
  double quadratic_3D_coeff ( const std::vector<double>   &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
                        
}
  
}
}

#endif