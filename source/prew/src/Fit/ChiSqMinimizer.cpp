#include <Fit/ChiSqMinimizer.h>

#include <cmath>

// External 
#include "Math/Functor.h"
#include "spdlog/spdlog.h"


namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

ChiSqMinimizer::ChiSqMinimizer(FitContainer * container, const MinuitFactory &factory) : 
  m_container(container) 
{
  this->update_chisq();
  m_minimizer = factory.create_minimizer();
}

//------------------------------------------------------------------------------
// get functions

double ChiSqMinimizer::get_chisq() const { return m_chisq; }
const FitResult& ChiSqMinimizer::get_result() const { return m_result; }

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
    if (par.is_fixed()) { continue; } // Skip fixed parameters
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
    m_minimizer->SetVariable( i_par, par.get_name(), par.m_val_mod, par.get_unc_ini() );
    // Check if parameter is fixed or limited
    if (par.is_fixed()) {
      m_minimizer->FixVariable(i_par); 
    } else if (par.is_limited()) {
      m_minimizer->SetVariableLimits( 
        i_par, par.get_upper_lim(), par.get_lower_lim());
    }
  }
  
  // -------------------------------------------------------------------------//
  // --------------------------------ACTION!----------------------------------//
  // -------------------------------------------------------------------------//
  m_minimizer->Minimize();
  // -------------------------------------------------------------------------//
  // -------------------------------------------------------------------------//
  
  // Form a usable output collection
  this->collect_par_names();
  this->update_result();
}

//------------------------------------------------------------------------------
// Result collecting

void ChiSqMinimizer::collect_par_names() {
  unsigned int n_pars = m_container->m_fit_pars.size();
  m_result.m_par_names.resize(n_pars);
  for ( unsigned int i_par=0; i_par<n_pars; i_par++ ){
    m_result.m_par_names[i_par] = m_container->m_fit_pars[i_par].get_name();
  }
}

void ChiSqMinimizer::update_result() {
  /** Write the result of the minimization procedure into a output container.
  **/
  
  if (m_result != FitResult()) {
    spdlog::debug("FitResult not empty, will be overwritten.");
  }
  
  unsigned int n_pars = m_container->m_fit_pars.size();
  m_result.m_cov_matrix = std::vector<std::vector<double>>( n_pars, std::vector<double>(n_pars) );
  m_result.m_cor_matrix = std::vector<std::vector<double>>( n_pars, std::vector<double>(n_pars) );
  for ( unsigned int i_par=0; i_par<n_pars; i_par++ ){
    for ( unsigned int j_par=0; j_par<n_pars; j_par++ ){
      m_result.m_cov_matrix[i_par][j_par] = m_minimizer->CovMatrix(i_par, j_par);
      m_result.m_cor_matrix[i_par][j_par] = m_minimizer->Correlation(i_par, j_par);
    }
  }
  
  m_result.m_pars_fin = std::vector<double>( m_minimizer->X(), m_minimizer->X()+n_pars );
  m_result.m_uncs_fin = std::vector<double>( m_minimizer->Errors(), m_minimizer->Errors()+n_pars );
  
  m_result.m_n_bins = m_container->m_fit_bins.size();
  m_result.m_n_free_pars = m_minimizer->NFree();
  
  // Minimization process information
  m_result.m_n_fct_calls = m_minimizer->NCalls();
  m_result.m_n_iters = m_minimizer->NIterations();
  
  m_result.m_chisq_fin = m_minimizer->MinValue(); 
  m_result.m_edm_fin = m_minimizer->Edm();
  m_result.m_min_status = m_minimizer->Status();
  m_result.m_cov_status = m_minimizer->CovMatrixStatus();
}

//------------------------------------------------------------------------------

}
}