#ifndef LIB_FNCTMAP_H
#define LIB_FNCTMAP_H 1

#include <Fcts/Physics.h>
#include <Fcts/Polynomial.h>
#include <Fcts/Statistic.h>
#include <CppUtils/Fct.h>

#include <map>
#include <string>

namespace PREW {
namespace Fcts {
  // Kind of map pointing from a string (function-ID) to a function 
  // which has three input parameter vectors: coordinate,parameters,coefficients
  using FctMap = std::map<std::string, CppUtils::Fct::ParametrisationFct>;
        
  // This fixes the association of a funtion-ID string to an actual function.
  // All parameterisation functions must have their own unique ID!
  static const FctMap prew_fct_map = {
    // Polynomials
    {"Constant", Polynomial::constant_par},
    {"ConstantCoef", Polynomial::constant_coef},
    {"Quadratic1DPolynomial", Polynomial::quadratic_1D},
    // Statisticals
    {"Gaussian1D", Statistic::gaussian_1D},
    // Physis motivated
    {"PolarisationFactor", Physics::polarisation_factor}
  };

  
}
}

#endif