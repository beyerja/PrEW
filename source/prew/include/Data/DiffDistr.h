#ifndef LIBRARY_DIFFDISTR_H
#define LIBRARY_DIFFDISTR_H 1

#include <CppUtils/Vec.h>
#include <Data/BinCoord.h>
#include <Data/DistrInfo.h>
#include <Fit/FitBin.h>


namespace PrEW {
namespace Data {
  
  struct DiffDistr {
    /** Class holding a differential distribution for one polarisation 
        combination.
        Differential distributions can only be distributions of numbers of 
        events in the context of this framework. Everything else (e.g cross 
        section) would carry a systematic error that this framework is not made
        to account for.
    **/
    
    DistrInfo m_info {}; // Info which identifies this distribution
    
    // Values of observables at bin center
    // => Are not stored with the bin because underlying fit framework 
    //    doesn't need it
    CoordVec m_coords {}; 
    Fit::BinVec m_distribution {}; // Distribution of number of events
    
    const DistrInfo & get_info() const { return m_info; }
  };
  
  typedef std::vector<DiffDistr> DiffDistrVec;
}
}

#endif