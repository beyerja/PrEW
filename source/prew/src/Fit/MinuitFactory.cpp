#include <Fit/MinuitFactory.h>


namespace PREW {
namespace Fit {
  
//------------------------------------------------------------------------------
// Constructors

MinuitFactory::MinuitFactory(ROOT::Minuit2::EMinimizerType type, unsigned int max_fcn_calls, unsigned int max_iters, double tolerance) :
  m_type(type), m_max_fcn_calls(max_fcn_calls), m_max_iters(max_iters), m_tolerance(tolerance) {}
  
//------------------------------------------------------------------------------
// Core functions

std::unique_ptr<ROOT::Minuit2::Minuit2Minimizer> MinuitFactory::create_minimizer() const {
  /** Create and return a Minuit2Minimizer with the options given at 
      construction of the factory.
      Must be returned as smart pointer due to ROOT ownership things...
  **/
  std::unique_ptr<ROOT::Minuit2::Minuit2Minimizer> minimizer = std::make_unique<ROOT::Minuit2::Minuit2Minimizer> (m_type);
  minimizer->SetMaxFunctionCalls(m_max_fcn_calls);
  minimizer->SetMaxIterations(m_max_iters);
  minimizer->SetTolerance(m_tolerance);
  return minimizer;
}

//------------------------------------------------------------------------------

}
}