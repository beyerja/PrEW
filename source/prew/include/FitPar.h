#ifndef LIB_FITPAR_H
#define LIB_FITPAR_H 1

#include <iostream>
#include <ParConstrGauss.h>

namespace PREW {
namespace Fit {
  
  class FitPar {
    /** Class describing fit parameters that can be varied in a fit.
        The unique identifier for a fit parameter is its name string.
    **/
    
    std::string m_name {}; // Name (Unique identifier)
    
    double m_val_ini {}; // Initial value
    double m_unc_ini {}; // Initial guess of uncertainty -> Needed for Minuit
    
    bool m_is_constraint {false};
    ParConstrGauss m_constrgauss {};
    
    public:
      double m_val_mod {}; // Modified value => Publicly accessible
      
      // Constructors
      FitPar(std::string name, double val_ini, double unc_ini);
      
      std::string get_name() const; // Get name
      double get_val_ini() const;   // Get initial value
      double get_unc_ini() const;   // Get initial value
      
      void set_constrgauss(ParConstrGauss constrgauss);
      double calc_constr_chisq() const; // Chi-squared produced by constraint
      
      void reset(); // Reset to inital value
      
      // Operators
      bool operator==(const FitPar& fit_par) const;
  };
  
}
}

std::ostream& operator<<(std::ostream& os, const PREW::Fit::FitPar& fp);

#endif 