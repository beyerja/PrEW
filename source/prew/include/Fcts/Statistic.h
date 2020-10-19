#ifndef LIB_STATISTIC_H
#define LIB_STATISTIC_H 1

#include <Data/BinCoord.h>

// Standard library
#include <vector>

namespace PrEW {
namespace Fcts {
  
namespace Statistic {
  /** Namespace for parametrisation functions from statistics.
      Must all follow structure:
      double fct_name (const Data::BinCoord   &x,
                        const std::vector<double>   &c,
                        const std::vector<double*>  &p);
  **/
  
  double gaussian_1D (const Data::BinCoord        &x,
                      const std::vector<double>   &c,
                      const std::vector<double*>  &p);
}
  
}
}

#endif