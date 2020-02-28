#ifndef LIB_PHYSICS_H
#define LIB_PHYSICS_H 1

#include <vector>

namespace PREW {
namespace Fcts {
  
namespace Physics {
  /** Namespace for parametrisation functions from statistics.
      Must all follow structure:
        double fct_name (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/
  
  double polarisation_factor (const std::vector<double>   &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
  
  double luminosity_fraction (const std::vector<double>   &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
}
  
}
}

#endif