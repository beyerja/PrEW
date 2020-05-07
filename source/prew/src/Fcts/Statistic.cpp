#include <Fcts/Statistic.h>

#include <exception>
#include <math.h>

namespace PrEW {
namespace Fcts {

//------------------------------------------------------------------------------

double Statistic::gaussian_1D ( 
  const std::vector<double> &x,
  const std::vector<double> &/*c*/,
  const std::vector<double*> &p
) {
  /** Gaussian function in 1D.
      Parameters: p[0] - amplitude
                  p[1] - mean
                  p[2] - width
      No coefficients required.
  **/
  
  return (*(p[0])) / (  (*(p[2])) * std::sqrt(2.0*M_PI) ) 
         * std::exp( -0.5 * std::pow( ( x[0] - (*(p[1])) ) / (*(p[2])) ,2) );
}

}
}