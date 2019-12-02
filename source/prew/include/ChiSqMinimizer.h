#ifndef LIB_CHISQMINIMIZER_H
#define LIB_CHISQMINIMIZER_H 1

#include <FitContainer.h>
#include <MinuitFactory.h>

#include <memory>

#include "Minuit2/Minuit2Minimizer.h"

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
  std::unique_ptr<ROOT::Minuit2::Minuit2Minimizer> m_minimizer; // Minimizer created by factory
  
  // Output
  double m_chisq {};
  
  // Internal functions
  void update_chisq();
  
  public:
    // Constructors
    ChiSqMinimizer(FitContainer * container, MinuitFactory &factory);
    
    void minimize();
    
    // Get function
    double get_chisq() const;
  };
  
}
}


#endif