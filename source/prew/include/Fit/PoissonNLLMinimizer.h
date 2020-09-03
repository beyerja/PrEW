#ifndef LIB_POISSONNLLMINIMIZER_H
#define LIB_POISSONNLLMINIMIZER_H 1

#include <Fit/FitContainer.h>
#include <Fit/MinuitFactory.h>
#include <Fit/FitResult.h>

#include <memory>

#include "Minuit2/Minuit2Minimizer.h"

namespace PrEW {
namespace Fit {
  
  class PoissonNLLMinimizer {
    /** Class that performs minimization of a negative Log-Likelihood (NLL)
          - 2 * ln L({p}|{x})
        in which the Likelihood L is poissonian.
        For bins with an measurement above 25 a gaussian approximation is used.
        Takes the fit parameters and the bins (whose prediction is connected
        to the fit parameters).
        The uncertainty of the measurement bins is not explicitely used 
        because the Likelihood assumes a Poissonian fluctuation.
    **/
  
    // Input
    FitContainer * m_container {}; // Container with bins and parameters
    std::unique_ptr<ROOT::Minuit2::Minuit2Minimizer> m_minimizer; // Minimizer created by factory
    
    // Output
    double m_nll {}; // Current value of the negative log-likelihood
    FitResult m_result {};
    
    // Internal functions
    double nll_poisson(int n, double mu) const;
    double nll_gaussian(double x, double mu, double sigma) const;
    void update_nll();
    
    void collect_par_names();
    void update_result();
    
    public:
      // Constructors
      PoissonNLLMinimizer(FitContainer * container, const MinuitFactory &factory);
      
      void minimize();
      
      // Get function
      double get_nll() const;
      const FitResult& get_result() const;
  };
  
}
}


#endif