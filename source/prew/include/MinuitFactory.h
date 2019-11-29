#ifndef LIB_CHISQMINIMIZER_H
#define LIB_CHISQMINIMIZER_H 1

#include "Minuit2/Minuit2Minimizer.h"

namespace PREW {
namespace Fit {
  
  class MinuitFactory {
    /** Class that creates instances of the Minuit2Minimizer class with the 
        properties given at construction.
    **/
  
    ROOT::Minuit2::EMinimizerType m_type {ROOT::Minuit2::kMigrad}; // Type of Minimizer
    unsigned int m_max_fcn_calls {1000000}; // Maximum number of calls to the minized function
    unsigned int m_max_iters {1000000};     // Maximum number of iterations for minimization
    double m_tolerance {0.0001}; // Variation tolerated between iterations to stop minimization
    
    public:
      // Constructors
      MinuitFactory(ROOT::Minuit2::EMinimizerType type, unsigned int max_fcn_calls, unsigned int max_iters, double tolerance);
    
      std::unique_ptr<ROOT::Minuit2::Minuit2Minimizer> create_minimizer() const;
   // 
   // ROOT::Math::Functor f(&RosenBrock,2); 
   // double step[2] = {0.01,0.01};
   // double variable[2] = { -1.,1.2};
   // 
   // min.SetFunction(f);
   // 
   // // Set the free variables to be minimized!
   // min.SetVariable(0,"x",variable[0], step[0]);
   // min.SetVariable(1,"y",variable[1], step[1]);
   // 
   // min.Minimize(); 
   // 
    
  };
  
}
}

#endif