#ifndef LIB_FITCONTAINER_H
#define LIB_FITCONTAINER_H 1

#include <Fit/FitPar.h>
#include <Fit/FitBin.h>

#include <vector>

namespace PREW {
namespace Fit {
  
  // TODO => Still useful to store the everything that is used for ONE SPECIFIC FIT!
  // -> Each fit should have own container which is handed to ChiSqMinimizer

  struct FitContainer {
    /** Class to hold complete set of bins and corresponding fit parameters that
        are to be used in the chi-squared minimization.
    **/
  
    std::vector<FitPar> m_fit_pars {}; // Fit parameters 
    std::vector<FitBin> m_fit_bins {}; // Bins (whose prediction is connected to the parameters)
    
    // public:
      
  };

}
}

#endif 