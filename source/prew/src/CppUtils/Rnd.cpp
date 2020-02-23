#include <CppUtils/Rnd.h>

#include <stdexcept>

namespace PREW {
namespace CppUtils {
  
//------------------------------------------------------------------------------

int Rnd::poisson_fluctuate(double mean) {
  /** Produce a random number from poisson distribution with given mean.
  **/
  if ( mean<0 ) { 
    throw std::invalid_argument("Poisson not defined for negativ mean!");
  }
  std::poisson_distribution<int> distribution(mean);
  return distribution(Rnd::rnd_gen);
}

//------------------------------------------------------------------------------

}
}