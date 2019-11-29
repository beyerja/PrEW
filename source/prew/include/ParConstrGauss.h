#ifndef LIB_PARCONSTRGAUSS_H 
#define LIB_PARCONSTRGAUSS_H 1

namespace PREW {
namespace Fit {

  class ParConstrGauss {
    /** Class implementating soft gaussian contraints  for fit parameters
    **/
    
    double m_val {}; // central value of constraint (measured value)
    double m_unc {}; // uncertainty of constraint

    public:
      // Constructors
      ParConstrGauss(double val, double unc);
      
      double calc_chisq(double par_val) const;
  };
  
}
}


#endif