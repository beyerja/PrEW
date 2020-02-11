#ifndef LIB_FNCTMAP_H
#define LIB_FNCTMAP_H 1

#include <Fncts/Physics.h>
#include <Fncts/Polynomial.h>
#include <Fncts/Statistic.h>
#include <CppUtils/Fnct.h>

#include <map>
#include <string>

namespace PREW {
namespace Fncts {
  // Kind of map pointing from a string (function-ID) to a function 
  // which has three input parameter vectors: coordinate,parameters,coefficients
  using FnctMap = std::map<std::string, CppUtils::Fnct::ParametrisationFnct>;
        
  // This fixes the association of a funtion-ID string to an actual function.
  // All parameterisation functions must have their own unique ID!
  static const FnctMap prew_fnct_map = {
    // Polynomials
    {"Constant", Polynomial::constant_par},
    {"Quadratic1DPolynomial", Polynomial::quadratic_1D},
    // Statisticals
    {"Gaussian1D", Statistic::gaussian_1D},
    // Physis motivated
    {"PolarisationFactor", Physics::polarisation_factor}
  };

  
}
}

#endif