#ifndef LIB_CHISQMINIMIZER_H
#define LIB_CHISQMINIMIZER_H 1

#include <Fit/FitContainer.h>
#include <Fit/MinuitFactory.h>
#include <Fit/FitResult.h>

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
  FitResult m_result {};
  
  // Internal functions
  void update_chisq();
  
  void collect_par_names();
  void update_result();
  
  public:
    // Constructors
    ChiSqMinimizer(FitContainer * container, const MinuitFactory &factory);
    
    void minimize();
    
    // Get function
    double get_chisq() const;
    const FitResult& get_result() const;
  };
  
}
}


#endif