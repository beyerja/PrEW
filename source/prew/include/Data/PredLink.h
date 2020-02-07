#ifndef LIB_PREDLINK_H
#define LIB_PREDLINK_H 1

#include <Data/DistrInfo.h>
#include <Data/FnctLink.h>

#include <vector>

namespace PREW {
namespace Data {
  
  struct PredLink {
    /** Class to store how the functions for a given distribution (identified by
        the info) are supposed to be linked to their parameters.
        => For both signal and background of the distribution.
    **/

    DistrInfo m_info {};
    FnctLinkVec m_fncts_links_sig {}; // Modification of signal Xsection
    FnctLinkVec m_fncts_links_bkg {}; // Modification of bkg Xsection 
    
  };
  
  using PredLinkVec = std::vector<PredLink>;
}
}

#endif