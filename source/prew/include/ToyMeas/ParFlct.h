#ifndef LIB_PARFLCT_H
#define LIB_PARFLCT_H 1

#include <Fit/FitPar.h>

namespace PrEW { 
namespace ToyMeas {

namespace ParFlct {
  /** Namespace for functions that can perform random fluctuations on fit 
      parameters.
  **/
  
  void fluctuate_constr ( Fit::FitPar & par );
  void fluctuate_constrs( Fit::ParVec & par_vec );
  
} // Namespace ParFlct
  
} // Namespace ToyMeas
} // Namespace PrEW

#endif