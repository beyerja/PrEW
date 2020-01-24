#ifndef LIBRARY_PREDDISTR_H
#define LIBRARY_PREDDISTR_H

#include <CppUtils/Vec.h>
#include <Data/DistrInfo.h>

#include <vector>

namespace PREW {
namespace Data {
  
  struct PredDistr {
    /** Class holding a predictions for differential distribution of a process
        for one chirality.
    **/
    
    DistrInfo m_info {}; // Info which identifies this distribution
    
    // Values of observables at bin center
    CppUtils::Vec::Matrix2D<double> m_bin_centers {}; 
    std::vector<double> m_distribution {}; // Predicted distribution
  };
  
  typedef std::vector<PredDistr> PredDistrVec;
  
} 
}

#endif