#ifndef LIB_PREDLINK_H
#define LIB_PREDLINK_H 1

#include <Data/DistrInfo.h>
#include <Data/FctLink.h>

#include <vector>

namespace PrEW {
namespace Data {
  
  struct PredLink {
    /** Class to store how the functions for a given distribution (identified by
        the info) are supposed to be linked to their parameters.
        => For both signal and background of the distribution.
    **/

    DistrInfo m_info {};
    FctLinkVec m_fcts_links_sig {}; // Modification of signal Xsection
    FctLinkVec m_fcts_links_bkg {}; // Modification of bkg Xsection 
    
  };
  
  using PredLinkVec = std::vector<PredLink>;
}
}

#endif