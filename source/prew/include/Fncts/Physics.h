#ifndef LIB_PHYSICS_H
#define LIB_PHYSICS_H 1

#include <vector>

namespace PREW {
namespace Fncts {
  
namespace Physics {
  /** Namespace for parametrisation functions from statistics.
      Must all follow structure:
        double fnct_name (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/
  
  double polarisation_factor (const std::vector<double>   &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
}
  
}
}

#endif