#ifndef LIB_FITPAR_H
#define LIB_FITPAR_H 1

#include <iostream>
#include <Fit/ParConstrGauss.h>

#include <vector>

namespace PREW {
namespace Fit {
  
  class FitPar {
    /** Class describing fit parameters that can be varied in a fit.
        The unique identifier for a fit parameter is its name string.
    **/
    
    std::string m_name {}; // Name (Unique identifier)
    
    double m_val_ini {}; // Initial value
    double m_unc_ini {}; // Initial guess of uncertainty -> Needed for Minuit
    
    bool m_is_fixed {};
    
    bool m_is_limited {false};
    double m_upper_lim {};
    double m_lower_lim {};
    
    bool m_is_constraint {false};
    ParConstrGauss m_constrgauss {};
    
    public:
      double m_val_mod {}; // Modified value => Publicly accessible
      
      // Constructors
      FitPar(
        std::string name,
        double val_ini,
        double unc_ini,
        bool is_fixed=false
      );
      
      const std::string & get_name() const; // Get name
      double get_val_ini() const;   // Get initial value
      double get_unc_ini() const;   // Get initial value
      bool is_fixed() const;   // Get info if parameter is fixed
      
      void set_limits(double lower_lim, double upper_lim);
      bool is_limited() const;
      double get_lower_lim() const;
      double get_upper_lim() const;
      
      void set_constrgauss(ParConstrGauss constrgauss);
      double calc_constr_chisq() const; // Chi-squared produced by constraint
      
      void fix();     // fix the parameter
      void release(); // release the parameter
      
      void reset(); // Reset to inital value
      
      // Operators
      bool operator==(const FitPar& fit_par) const;
      FitPar& operator=(const FitPar& other) = default;
  };
  
  typedef std::vector<FitPar> ParVec;
  
}
}

std::ostream& operator<<(std::ostream& os, const PREW::Fit::FitPar& fp);

#endif 