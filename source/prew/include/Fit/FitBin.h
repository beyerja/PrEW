#ifndef LIB_FITBIN_H
#define LIB_FITBIN_H 1

#include <functional>

namespace PREW {
namespace Fit {
  
  class FitBin {
    /** Class describing the prediction for a bin depending on a set of fit parameters.
    **/
    
    double m_val_mst {}; // measured value
    double m_val_unc {}; // measurement uncertainty
    
    // Prediction function (includes dependece on fit parameters as pointers!!!)
    std::function<double()> m_prd_fct {};  
    
    public:
      // Constructors
      FitBin(double val_mst, double val_unc, std::function<double()> prd_fct);
      
      double get_val_mst() const; // Get measured value
      double get_val_unc() const; // Get measurement uncertainty
      
      double get_val_prd() const;
  };

  typedef std::vector<FitBin> BinVec;
  
}
}

#endif 