#include <CppUtils/Num.h>
#include <CppUtils/Rnd.h>
#include <Fit/FitBin.h>
#include <ToyMeas/Flct.h>

#include <cmath>

namespace PREW {
namespace ToyMeas {
  
//------------------------------------------------------------------------------

void Flct::poisson_fluctuate(Fit::FitBin &bin) {
  /** Poisson fluctuate the measured value in the bin and adjust the 
      uncertainty.
  **/
  bin.set_val_mst( CppUtils::Rnd::poisson_fluctuate(bin.get_val_mst()) );
  
  // If content zero keep previous uncertainty, else poisson uncertainty
  if ( ! CppUtils::Num::equal_to_eps(bin.get_val_mst(),0.0) ) { 
    bin.set_val_unc( std::sqrt(bin.get_val_mst()) );
  }
}

//------------------------------------------------------------------------------

}
}