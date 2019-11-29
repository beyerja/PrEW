#include <ChiSqMinimizer.h>

#include <cmath>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

ChiSqMinimizer::ChiSqMinimizer(FitContainer * container, MinuitFactory &factory) : 
  m_container(container) 
{
  m_chisq = this->calc_chisq();
  m_minimizer = factory.create_minimizer();
};

//------------------------------------------------------------------------------
// get functions

double ChiSqMinimizer::get_chisq() const { return m_chisq; }

//------------------------------------------------------------------------------
// Core functionality

double ChiSqMinimizer::calc_chisq() {
  double chi_sq {0.0};
  for ( auto & bin : m_container->m_fit_bins ) {
    chi_sq += std::pow( ( bin.get_val_mst() - bin.get_val_prd() ) /  bin.get_val_unc() , 2 );
  }
  return chi_sq;
}

//------------------------------------------------------------------------------

}
}