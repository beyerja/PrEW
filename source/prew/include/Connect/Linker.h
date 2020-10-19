#ifndef LIB_LINKER_H
#define LIB_LINKER_H 1

#include <CppUtils/Vec.h>
#include <Data/BinCoord.h>
#include <Data/CoefDistr.h>
#include <Data/FctLink.h>
#include <Fit/FitPar.h>
#include <Fcts/FctMap.h>

#include <functional>
#include <string>

namespace PrEW {
namespace Connect {

  class Linker {
    /** Class that takes all the info about how functions for one particular
        distribution are supposed to be bound and can give the functions for 
        each bin of the distribution.
    **/
    
    Data::FctLinkVec m_fcts_links {};
    Data::CoordVec m_coords {};
    Data::CoefDistrVec m_coefs {};
    
    public:
      // Constructors
      Linker( Data::FctLinkVec fcts_links,
              Data::CoordVec coords,
              Data::CoefDistrVec coefs
             );
      
      // Core functionality
      std::vector<std::function<double()>> get_all_bonded_fcts_at_bin(
        size_t bin,
        Fit::ParVec *pars
      ) const;
      
    protected:
      std::function<double()> get_bonded_fct_at_bin(
        const Data::FctLink &fct_name,
        size_t bin,
        Fit::ParVec *pars
      ) const;
  };
  
}
}

#endif