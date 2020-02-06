#ifndef LIB_POLLINK_H
#define LIB_POLLINK_H 1

#include <CppUtils/Map.h>

namespace PREW {
namespace Data {

  struct PolLink {
    /** Class that stores which polarisation configuration couples which 
        individual beam polarisations together.
    **/
    
    int m_energy {}; // This depends on the energy!
    
    // Map from polarisation setup to individual beam polarisation names
    // => Rule: First electron polarisation, second positron polarisation
    CppUtils::Map::StrToStrPairMap m_config_pol_links {};
  };

  using PolLinkVec = std::vector<PolLink>;
}
}

#endif