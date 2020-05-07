#include <CppUtils/Rnd.h>

#include <stdexcept>

namespace PrEW {
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

double Rnd::gauss_fluctuate(double mean, double width) {
  /** Produce a random number from gaussian distribution with given mean and 
      width.
  **/
  if ( ! (width>0) ) { 
    throw std::invalid_argument("Gaussian width must be greater than zero!");
  }
  std::normal_distribution<> distribution(mean,width);
  return distribution(Rnd::rnd_gen);
}

//------------------------------------------------------------------------------

}
}