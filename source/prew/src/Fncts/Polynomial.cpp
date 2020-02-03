#include <Fncts/Polynomial.h>

#include <math.h>

namespace PREW {
namespace Fncts {

//------------------------------------------------------------------------------

double Polynomial::quadratic_1D ( 
  const std::vector<double> &x,
  const std::vector<double> &/*c*/,
  const std::vector<double*> &p
) {
  /** Gaussian function in 1D.
      Parameters: p[0] - offset
                  p[1] - linear coeff
                  p[2] - quadratic coeff
      No coefficients required.
  **/
  
  return (*(p[0])) + (*(p[1])) * x[0] + (*(p[2])) * std::pow( x[0], 2);
}

}
}