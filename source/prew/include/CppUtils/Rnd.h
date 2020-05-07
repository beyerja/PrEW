#ifndef LIB_RND_H
#define LIB_RND_H 1

#include <random>

namespace PrEW { 
namespace CppUtils {
  
namespace Rnd {
  /** Namespace for random number generation / fluctuation.
  **/
  
  // Global random number generator to avoid duplication of random numbers
  static thread_local std::random_device rnd_device;
  static thread_local std::mt19937 rnd_gen(rnd_device());
  
  int poisson_fluctuate(double mean);
  double gauss_fluctuate(double mean, double width);
}
  
}
}

#endif