#ifndef LIB_FITPAR_H
#define LIB_FITPAR_H 1

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
      
      double const get_val_ini(); // Get initial value
      
      void reset(); // Reset to inital value
  };
  
}
}

#endif 