#ifndef LIB_POLYNOMIAL_H
#define LIB_POLYNOMIAL_H 1

#include <Data/BinCoord.h>

// Standard library
#include <vector>

namespace PrEW {
namespace Fcts {
  
namespace Polynomial {
  /** Namespace for polynomial parametrisation functions.
      Must all follow structure:
      double fct_name (const Data::BinCoord   &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
  **/
  
  double constant_coef ( const Data::BinCoord        &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  double constant_par ( const Data::BinCoord        &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
                        
  double linear_3D_coeff ( const Data::BinCoord        &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
                        
  double quadratic_1D ( const Data::BinCoord        &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
                        
  double quadratic_3D_coeff ( const Data::BinCoord        &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
                        
}
  
}
}

#endif