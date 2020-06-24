#ifndef LIB_FNCTMAP_H
#define LIB_FNCTMAP_H 1

#include <Fcts/Physics.h>
#include <Fcts/Polynomial.h>
#include <Fcts/Statistic.h>
#include <CppUtils/Fct.h>

#include <map>
#include <string>

namespace PrEW {
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
    {"Quadratic3DPolynomial_Coeff", Polynomial::quadratic_3D_coeff},
    // Statisticals
    {"Gaussian1D", Statistic::gaussian_1D},
    // Physis motivated
    {"PolarisationFactor", Physics::polarisation_factor},
    {"LuminosityFraction", Physics::luminosity_fraction},
    {"AsymmFactor0_2allowed", Physics::asymm_2chixs_a0},
    {"AsymmFactor1_2allowed", Physics::asymm_2chixs_a1},
    {"AsymmFactor0_3allowed", Physics::asymm_3chixs_a0},
    {"AsymmFactor1_3allowed", Physics::asymm_3chixs_a1},
    {"AsymmFactor2_3allowed", Physics::asymm_3chixs_a2},
    {"AsymmFactorLR_Af_2f", Physics::asymm_Af_2f_LR},
    {"AsymmFactorRL_Af_2f", Physics::asymm_Af_2f_RL}
  };

  
}
}

#endif