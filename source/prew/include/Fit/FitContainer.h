#ifndef LIB_FITCONTAINER_H
#define LIB_FITCONTAINER_H 1

#include <Fit/FitBin.h>
#include <Fit/FitPar.h>

#include <vector>

namespace PrEW {
namespace Fit {
  
  struct FitContainer {
    /** Class to hold complete set of bins and corresponding fit parameters that
        are to be used in the chi-squared minimization.
    **/
  
    // Fit parameters 
    ParVec m_fit_pars {}; 
    // Bins (whose prediction is connected to the parameters and coefficients)
    BinVec m_fit_bins {}; 
  };

}
}

#endif 