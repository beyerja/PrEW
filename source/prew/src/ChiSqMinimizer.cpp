#include <ChiSqMinimizer.h>

#include <cmath>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

ChiSqMinimizer::ChiSqMinimizer(FitContainer * container, MinuitFactory &factory) : 
  m_container(container) 
{
  this->update_chisq();
  m_minimizer = factory.create_minimizer();
}

//------------------------------------------------------------------------------
// get functions

double ChiSqMinimizer::get_chisq() const { return m_chisq; }

//------------------------------------------------------------------------------
// Core functionality

void ChiSqMinimizer::update_chisq() {
  /** Update the full chi-squared sum from the bins and parameter constraints
      given by the fit container.
  **/
  m_chisq = 0.0;
  for ( auto & bin : m_container->m_fit_bins ) {
    m_chisq += std::pow( ( bin.get_val_mst() - bin.get_val_prd() ) /  bin.get_val_unc() , 2 );
  }
  for ( auto & par : m_container->m_fit_pars ) {
    m_chisq += par.calc_constr_chisq();
  }
}

//------------------------------------------------------------------------------

}
}