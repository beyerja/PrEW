#include <ChiSqMinimizer.h>

#include <cmath>

#include "Math/Functor.h"


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

void ChiSqMinimizer::minimize() {
  /** Perform the actual chi-squared minimization using Minuit2.
      Will modify the m_val_mod of all parameters in the container!
  **/
  
  // Create a vector holding the addresses of the parameter values
  // => Minimizer will directly change parameter values by changing the 
  //    values at the addresses
  unsigned int n_pars = m_container->m_fit_pars.size();
  std::vector<double*> pars(n_pars);
  for ( unsigned int i=0; i<n_pars; i++ ){
    pars[i] = &(m_container->m_fit_pars[i].m_val_mod);
  }
    
  // Thing that minimizer performs chi-squared minimization on
  const ROOT::Math::Functor recalc_chisq (
    // Lambda function for the minimizer:
    // Update the parameter set, then recalculate and return new chi-squared
    [pars, n_pars, this](const double * _pars) { 
      for ( unsigned int i=0; i<n_pars; i++ ) { *(pars[i]) = _pars[i]; }
      this->update_chisq();
      return this->get_chisq();
    },
    // Needs to know the correct number of parameters
    n_pars
  );
  
  // Set up minimizer by telling about function and parameters
  m_minimizer->SetFunction(recalc_chisq);
  for ( unsigned int i_par=0; i_par<n_pars; i_par++ ){
    FitPar par = m_container->m_fit_pars[i_par];
    m_minimizer->SetVariable( i_par, par.get_name(), par.get_val_ini(), par.get_unc_ini() );
  }
  
  // -------------------------------------------------------------------------//
  // --------------------------------ACTION!----------------------------------//
  // -------------------------------------------------------------------------//
  m_minimizer->Minimize();
  // -------------------------------------------------------------------------//
  // -------------------------------------------------------------------------//
}


//------------------------------------------------------------------------------

}
}