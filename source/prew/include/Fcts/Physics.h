#ifndef LIB_PHYSICS_H
#define LIB_PHYSICS_H 1

#include <vector>

namespace PrEW {
namespace Fcts {
  
namespace Physics {
  /** Namespace for parametrisation functions from statistics.
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
  
  double asymm_2chixs_a0 (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
                          
  double asymm_2chixs_a1 (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
                          
  //----------------------------------------------------------------------------
  
  double asymm_3chixs_a0 (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  
  double asymm_3chixs_a1 (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  
  double asymm_3chixs_a2 (const std::vector<double>   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  
  //----------------------------------------------------------------------------
  
  double asymm_Af_2f_LR (const std::vector<double>   &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  double asymm_Af_2f_RL (const std::vector<double>   &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  //----------------------------------------------------------------------------
}
  
}
}

#endif