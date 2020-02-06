#ifndef LIB_LINKHELP_H
#define LIB_LINKHELP_H 1

#include <Fit/FitPar.h>

#include <functional>
#include <string>
#include <vector>

namespace PREW {
namespace Connect {
  
namespace LinkHelp {
  /** Functions that help with the linking of predictions.
  **/
  
  std::function<double()> get_polfactor_lambda(
    const std::string                         & chirality, 
    const std::pair<std::string, std::string> & pol_pair, 
    Fit::ParVec *pars
  );
  
  double get_modified_sigma(
    double sigma,
    const std::vector<std::function<double()>>& alphas
  );
}

}
}

#endif