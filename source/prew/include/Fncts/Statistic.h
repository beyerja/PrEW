#ifndef LIB_STATISTIC_H
#define LIB_STATISTIC_H 1

#include <vector>

namespace PREW {
namespace Fncts {
  
namespace Statistic {
  /** Namespace for parametrisation functions from statistics.
      Must all follow structure:
        double fnct_name (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/
  
  double gaussian_1D (const std::vector<double>   &x,
                      const std::vector<double>   &c,
                      const std::vector<double*>  &p);
}
  
}
}

#endif