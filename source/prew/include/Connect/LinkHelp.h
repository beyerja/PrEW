#ifndef LIB_LINKHELP_H
#define LIB_LINKHELP_H 1

#include <Data/PolLink.h>
#include <Fit/FitPar.h>

#include <functional>
#include <string>
#include <vector>

namespace PrEW {
namespace Connect {
  
namespace LinkHelp {
  /** Functions that help with the linking of predictions.
  **/
  
  std::function<double()> get_polfactor_lambda(
    const std::string   & chirality, 
    const Data::PolLink & pol_link, 
    Fit::ParVec *pars
  );
  
  std::function<double()> get_modified_sigma(
    double sigma,
    const std::vector<std::function<double()>>& alphas
  );
}

}
}

#endif