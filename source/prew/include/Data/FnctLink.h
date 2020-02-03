#ifndef LIB_FNCTLINK_H
#define LIB_FNCTLINK_H 1

#include <string>
#include <vector>

namespace PREW {
namespace Data {
  
  struct FnctLink {
    /** Class that stores the names of parameters and coefficients 
        which are supposed to be used when using the given function.
    **/
    
    std::string m_fnct_name {};
    std::vector<std::string> m_pars {};
    std::vector<std::string> m_coefs {};
  };
  
  using FnctLinkVec = std::vector<FnctLink>;
}
}

#endif