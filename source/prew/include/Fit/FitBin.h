#ifndef LIB_FITBIN_H
#define LIB_FITBIN_H 1

#include <functional>
#include <vector>

namespace PREW {
namespace Fit {
  
  class FitBin {
    /** Class describing the prediction for a bin depending on a set of fit 
        parameters.
    **/
    
    double m_val_mst {}; // measured value
    double m_val_unc {}; // measurement uncertainty
    
    // Prediction function (includes dependece on fit parameters as pointers!!!)
    std::function<double()> m_prd_fct {};  
    
    public:
      // Constructors
      FitBin(
        double val_mst=0, 
        double val_unc=0, 
        std::function<double()> prd_fct=NULL
      );
      
      void set_val_mst(double val_mst);
      void set_val_unc(double val_unc);
      void set_prd_fct(std::function<double()> prd_fct); // Set prediction fct.
      
      double get_val_mst() const; // Get measured value
      double get_val_unc() const; // Get measurement uncertainty
      
      double get_val_prd() const;
  };

  typedef std::vector<FitBin> BinVec;
  
}
}

#endif 