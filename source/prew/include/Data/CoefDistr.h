#ifndef LIBRARY_COEFDISTR_H
#define LIBRARY_COEFDISTR_H 1

#include <Data/DistrInfo.h>

#include <string>
#include <vector>

namespace PREW {
namespace Data {

  struct CoefDistr {
    /** Class holding a coefficient for parameterised prediction of a 
        distribution for one process & polarisation/chirality.
    **/
    
    std::string m_coef_name {}; // Name that identifies this coefficient
    DistrInfo m_info {}; // Info which identifies the corresponding distribution
    
    // Coefficient for each bin
    std::vector<double> m_coefficients {};
  };
  
  typedef std::vector<CoefDistr> CoefDistrVec;
    
}
}

#endif