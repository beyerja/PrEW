#ifndef LIB_PARCONSTRGAUSS_H 
#define LIB_PARCONSTRGAUSS_H 1

namespace PREW {
namespace Fit {

  class ParConstrGauss {
    /** Class implementating soft gaussian contraints  for fit parameters
    **/
    
    double m_val {0.0}; // central value of constraint (measured value)
    double m_unc {1.0}; // uncertainty of constraint

    public:
      // Constructors
      ParConstrGauss() = default;
      ParConstrGauss(double val, double unc);
      
      double calc_chisq(double par_val) const;
  };
  
}
}


#endif