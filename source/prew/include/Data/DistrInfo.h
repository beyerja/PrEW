#ifndef LIB_DISTRINFO_H
#define LIB_DISTRINFO_H 1

#include <string>

namespace PREW {
namespace Data {

  struct DistrInfo {
    /** Identifier holding all the info that uniquely identifies a distribution.
    **/
    std::string m_distr_name {};    // Name of distribution
    std::string m_pol_config {};    // Name of polarisation setting
    int         m_energy {};        // Center of mass energy
    
    bool operator==(const DistrInfo& other) const;
  };
  
}
}

#endif 