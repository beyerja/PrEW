#ifndef LIB_PHYSICS_H
#define LIB_PHYSICS_H 1

#include <Data/BinCoord.h>

// Standard library
#include <vector>

namespace PrEW {
namespace Fcts {
  
namespace Physics {
  /** Namespace for parametrisation functions from statistics.
      Must all follow structure:
        double fct_name (const Data::BinCoord   &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  **/

  //----------------------------------------------------------------------------
  
  double asymm_2chixs_a0 (const Data::BinCoord        &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
                          
  double asymm_2chixs_a1 (const Data::BinCoord        &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
                          
  //----------------------------------------------------------------------------
  
  double asymm_3chixs_a0 (const Data::BinCoord        &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  
  double asymm_3chixs_a1 (const Data::BinCoord        &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  
  double asymm_3chixs_a2 (const Data::BinCoord        &x,
                          const std::vector<double>   &c,
                          const std::vector<double*>  &p);
  
  //----------------------------------------------------------------------------
  
  double asymm_Af_2f_LR (const Data::BinCoord         &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  double asymm_Af_2f_RL (const Data::BinCoord         &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  //----------------------------------------------------------------------------
  
  double general_2f_param_LR (const Data::BinCoord         &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
  
  double general_2f_param_RL (const Data::BinCoord         &x,
                              const std::vector<double>   &c,
                              const std::vector<double*>  &p);
                              
  double unpol_2f_param (const Data::BinCoord         &x,
                         const std::vector<double>   &c,
                         const std::vector<double*>  &p);
  
  //----------------------------------------------------------------------------
}
  
}
}

#endif