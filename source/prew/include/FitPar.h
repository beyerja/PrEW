#ifndef LIB_FITPAR_H
#define LIB_FITPAR_H 1

#include <iostream>

namespace PREW {
namespace Fit {
  
  class FitPar {
    /** Class describing fit parameters that can be varied in a fit.
    **/
    
    double m_val_ini {}; // Initial value
    
    public:
      double m_val_mod {}; // Modified value => Publicly accessible
      
      // Constructors
      FitPar(double val_ini);
      
      double get_val_ini() const; // Get initial value
      
      void reset(); // Reset to inital value
  };
  
}
}

std::ostream& operator<<(std::ostream& os, const PREW::Fit::FitPar& fp);

#endif 