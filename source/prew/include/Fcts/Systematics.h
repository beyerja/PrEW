#ifndef LIB_SYSTEMATICS_H
#define LIB_SYSTEMATICS_H 1

#include <Data/BinCoord.h>

// Standard library
#include <vector>

namespace PrEW {
namespace Fcts {
  
namespace Systematics {
  /** Namespace for parametrisation functions from systematic effects.
      Must all follow structure:
      double fct_name (const Data::BinCoord   &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
  **/
  
  //----------------------------------------------------------------------------
  
  double polarisation_factor (const Data::BinCoord        &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
  
  double luminosity_fraction (const Data::BinCoord        &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
                              
  //----------------------------------------------------------------------------
  
  double acceptance_box (const Data::BinCoord        &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
    
  //----------------------------------------------------------------------------
  
} // Namespace Systematics

} // Namespace Fcts
} // Namespace PrEW

#endif