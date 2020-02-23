#ifndef LIB_FLCT_H
#define LIB_FLCT_H 1

#include <CppUtils/Num.h>
#include <CppUtils/Rnd.h>
#include <Fit/FitBin.h>

namespace PREW { 
namespace ToyMeas {
  
namespace Flct {
  /** Namespace for functions to fluctuate bins and distributions.
  **/
  
  void poisson_fluctuate(Fit::FitBin &bin);
}
  
}
}

#endif