#ifndef LIBRARY_DISTR1D_H
#define LIBRARY_DISTR1D_H 1

#include <Fit/FitBin.h>
#include <CppUtils/Vec.h>


namespace PREW {
namespace Data {
  
  struct Distr1D {
    /** Class holding a differential distribution of a process for a 
        polarisation combination.
        -> Save all (necessary) information directly with distribution
           (Distributions with common arguments can be found easily)
    **/
    
    std::string m_process_name {};  // Name of process
    std::string m_distr_name {};    // Name of distribution
    std::string m_pol_config {};    // Name of polarisation setting
    double      m_energy {};        // Center of mass energy
    
    // Values of observables at bin center
    // => Are not stored with the bin because underlying fit framework 
    //    doesn't need it
    CppUtils::Vec::Matrix2D<double> m_bin_centers {}; 
    Fit::BinVec m_distribution {}; // Observable values (e.g. cross section)
  };
  
  typedef std::vector<Distr1D> Distr1DVec;
}
}

#endif