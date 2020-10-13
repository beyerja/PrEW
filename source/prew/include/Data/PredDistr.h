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
    
    const DistrInfo & get_info() const { return m_info; }
    
    bool operator==(const PredDistr &other) const {
      return (m_info == other.m_info) && 
             (m_bin_centers == other.m_bin_centers) &&
             (m_sig_distr == other.m_sig_distr) &&
             (m_bkg_distr == other.m_bkg_distr);
    }
  };
  
  typedef std::vector<PredDistr> PredDistrVec;
  
} 
}

#endif