#ifndef LIB_SYSTEMATICS_H
#define LIB_SYSTEMATICS_H 1

#include <vector>

namespace PrEW {
namespace Fcts {
  
namespace Systematics {
  /** Namespace for parametrisation functions from systematic effects.
      Must all follow structure:
        double fct_name (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/
  
  //----------------------------------------------------------------------------
  
  double polarisation_factor (const std::vector<double>   &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
  
  double luminosity_fraction (const std::vector<double>   &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
                              
  //----------------------------------------------------------------------------
  
  double acceptance_box (const std::vector<double>   &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
    
  //----------------------------------------------------------------------------
  
} // Namespace Systematics

} // Namespace Fcts
} // Namespace PrEW

#endif