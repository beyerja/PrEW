#ifndef LIB_LINKER_H
#define LIB_LINKER_H 1

#include <CppUtils/Vec.h>
#include <Data/CoefDistr.h>
#include <Data/FnctLink.h>
#include <Fit/FitPar.h>
#include <Fncts/FnctMap.h>

#include <functional>
#include <string>

namespace PREW {
namespace Connect {

  class Linker {
    /** Class that takes all the info about how functions for one particular
        distribution are supposed to be bound and can give the functions for 
        each bin of the distribution.
    **/
    
    Data::FnctLinkVec m_fncts_links {};
    CppUtils::Vec::Matrix2D<double> m_bin_centers {};
    Data::CoefDistrVec m_coefs {};
    
    public:
      // Constructors
      Linker( Data::FnctLinkVec fncts_links,
              CppUtils::Vec::Matrix2D<double> bin_centers,
              Data::CoefDistrVec coefs
             );
      
      // Core functionality
      std::function<double()> get_bonded_fnct_at_bin(
        const std::string &fnct_name,
        size_t bin,
        Fit::ParVec *pars
      ) const;
      
      std::vector<std::function<double()>> get_all_bonded_fncts_at_bin(
        size_t bin,
        Fit::ParVec *pars
      ) const;
  };
  
}
}

#endif