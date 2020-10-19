#include <Fcts/Polynomial.h>

#include <math.h>

namespace PrEW {
namespace Fcts {

//------------------------------------------------------------------------------

double Polynomial::constant_coef ( 
  const Data::BinCoord &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &/*p*/
) {
  /** Simple constant factor, no variation with parameters.
      Can be used to scale cross sections.
  **/
  return c[0];
}

//------------------------------------------------------------------------------

double Polynomial::constant_par ( 
  const Data::BinCoord &/*x*/,
  const std::vector<double>   &/*c*/,
  const std::vector<double*>  &p
) {
  /** Simple constant dependence on first parameter p[0].
      No bin centers or coefficients required.
  **/
  return (*(p[0]));
}

//------------------------------------------------------------------------------

double Polynomial::linear_3D_coeff ( 
  const Data::BinCoord &/*x*/,
  const std::vector<double> &c,
  const std::vector<double*> &p
) {
  /** Linear polynomial in 3D.
      Parameters: p[0-2] - variables of polynomial
      Coefficients: c[0] - offset
                    c[1-3] - linear coeffs
  **/
  return c[0] + c[1]* (*(p[0])) + c[2]* (*(p[1])) + c[3]* (*(p[2]));
}

//------------------------------------------------------------------------------

double Polynomial::quadratic_1D ( 
  const Data::BinCoord &x,
  const std::vector<double> &/*c*/,
  const std::vector<double*> &p
) {
  /** Quadratic polynomial in x (1D coordinate) with parameters as constants.
      Parameters: p[0] - offset
                  p[1] - linear coeff
                  p[2] - quadratic coeff
      No coefficients required.
  **/
  
  return (*(p[0])) + (*(p[1])) * x.get_center()[0] 
                   + (*(p[2])) * std::pow( x.get_center()[0], 2);
}

//------------------------------------------------------------------------------

double Polynomial::quadratic_3D_coeff ( 
  const Data::BinCoord &/*x*/,
  const std::vector<double> &c,
  const std::vector<double*> &p
) {
  /** Cubic polynomial in 1D.
      Parameters: p[0-2] - variables of polynomial
      Coefficients: c[0] - offset
                    c[1-3] - linear coeffs
                    c[4-6] - pure quadratic coeff
                    c[7-9] - mixed quadratic coeff
  **/
  
  return  c[0]
          + c[1]* (*(p[0])) + c[2]* (*(p[1])) + c[3]* (*(p[2]))
          + c[4]* std::pow((*(p[0])),2) + c[5]* std::pow((*(p[1])),2) 
                                        + c[6]* std::pow((*(p[2])),2)
          + c[7]* (*(p[0])) * (*(p[1])) + c[8]* (*(p[0])) * (*(p[2])) 
                                        + c[9]* (*(p[1])) * (*(p[2]));
}

//------------------------------------------------------------------------------

}
}