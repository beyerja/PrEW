#ifndef LIBRARY_PREDDISTR_H
#define LIBRARY_PREDDISTR_H

#include <CppUtils/Vec.h>
#include <Data/DistrInfo.h>

#include <vector>

namespace PrEW {
namespace Data {
  
  struct PredDistr {
    /** Class holding a predictions for differential distribution for one 
        chirality.
    **/
    
    DistrInfo m_info {}; // Info which identifies this distribution
    
    // Values of observables at bin center
    CppUtils::Vec::Matrix2D<double> m_bin_centers {}; 
    std::vector<double> m_sig_distr {}; // Predicted signal distribution
    std::vector<double> m_bkg_distr {}; // Predicted background distribution
  };
  
  typedef std::vector<PredDistr> PredDistrVec;
  
} 
}

#endif