#ifndef LIB_CHISQMINIMIZER_H
#define LIB_CHISQMINIMIZER_H 1

#include <FitContainer.h>

namespace PREW {
namespace Fit {
  
  class ChiSqMinimizer {
    /** Class that performs a generic chi-squared minimization.
        Takes the fit parameters and the bins (whose prediction is connected
        to the fit parameters).
        Bins cannot (!) be correlated except through common fit parameters.
    **/
  
  // Input
  FitContainer * m_container {}; // Container with bins and parameters
  
  // Output
  double m_chisq {};
  
  // Internal functions
  double calc_chisq();
  
  public:
    // Constructors
    ChiSqMinimizer(FitContainer * container);
    
    double get_chisq() const;
  };
  
}
}


#endif