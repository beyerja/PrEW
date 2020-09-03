#include <Fit/PoissonNLLMinimizer.h>
#include <CppUtils/Num.h>

#define _USE_MATH_DEFINES // To access mathematical constants such as pi
#include <cmath>
#include <limits> // For numerical limits (e.g. infinity)

// External 
#include "Math/Functor.h"
#include "spdlog/spdlog.h"


namespace PrEW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

PoissonNLLMinimizer::PoissonNLLMinimizer(FitContainer * container, const MinuitFactory &factory) : 
  m_container(container) 
{
  this->update_nll();
  m_minimizer = factory.create_minimizer();
}

//------------------------------------------------------------------------------
// get functions

double PoissonNLLMinimizer::get_nll() const { return m_nll; }
const FitResult& PoissonNLLMinimizer::get_result() const { return m_result; }

//------------------------------------------------------------------------------
// Log Likelihood functions

double PoissonNLLMinimizer::nll_poisson(int n, double mu) const { 
  /** Negative log likelihood (including factor -2) of poissonian probability.
      n ... Number of measured events
      mu ... expected value
  **/
  return - 2.0 * ( - CppUtils::Num::log_factorial(n) - mu + double(n) * std::log(mu) );
}
  
double PoissonNLLMinimizer::nll_gaussian(double x, double mu, double sigma) const { 
  /** Negative log likelihood (including factor -2) of gaussian probability.
      x ... measured value
      mu ... expected value
      sigma ... uncertainty on x
  **/
  static const double log_2pi = std::log( 2.0 * M_PI );
  return log_2pi + 2.0 * std::log( sigma ) + std::pow( (x - mu) / sigma , 2);
}

//------------------------------------------------------------------------------
// Core functionality

void PoissonNLLMinimizer::update_nll() {
  /** Update the poissonian negative log-likelihood.
      All measurement bins are assumed to have an positive integer value (>=0).
  
      Bins with a measurement below 25 use the poissonian log-likelihood:
        NLL_bin = - 2.0 * ( - ln(n!) - mu + n * ln(mu) )
        with the measured bin value n and it's prediction mu.
        Since nothing keeps mu from being negative that case is treated:
          for n == 0: NLL_bin (mu < 0) = 0
          for n > 0:  NLL_bin (mu < 0) = inf
        which continues the value NLL_bin would have at mu = 0.
        
      For bins with a measurement above 25 a gaussian approximation is used to 
      avoid problems with logarithms of large numbers:
        NLL_bin = ln( 2*pi * sigma^2) + (x - mu)^2 / sigma^2
        where 
          x = n ... measured bin value value
          sigma = sqrt(mu) ... bin value uncertainty
        
      The gaussian NLL is also used for soft constraints on the parameters.
      In that case:
        x ... measured parameter value
        mu ... current parameter value
        sigma ... uncertainty on measured parameter value
  **/
  
  m_nll = 0.0; // Reset NLL before summing it up again
  
  // Find log-likelihood contributions from bin values
  for ( const auto & bin : m_container->m_fit_bins ) {
    int n = int( bin.get_val_mst() ); // Measurements have to be integer
    double mu = bin.get_val_prd();    // Prediction
    
    // Handle all possible cases of n and mu
    if ( n == 0 ) {
      if ( mu > 0 ) {
        // Poisson NLL behaves well under these conditions
        m_nll += this->nll_poisson(n, mu);
      } else {
        // Poisson NLL goes to zero for mu->0 for n==0, continue at 0 for
        // mu < 0 as well.
        // => Negative pred. not punished, assume will be fixed by other bins
        m_nll += 0;
      }
    } else if ( n <= 25 ) {
      if ( mu > 0 ) {
        // Poisson NLL behaves well under these conditions
        m_nll += this->nll_poisson(n, mu);
      } else {
        // Poisson NLL goes to infinity for mu->0 for n>0, continue at inf. for
        // mu < 0 as well.
        m_nll = std::numeric_limits<double>::infinity();
        return; // No need to add any more
      }
    } else {
      // Measured value big enough that a gaussian can be assumed
      if ( mu > 0 ) {
        // Gaussian well behaved
        m_nll += this->nll_gaussian(double(n), mu, std::sqrt(mu));
      } else {
        // Gaussian assumption leads to infinity
        m_nll = std::numeric_limits<double>::infinity();
        return; // No need to add any more
      }
    }    
    
  } // End bin loop
  
  // Find log-likelihood contributions from parameter constraints
  for ( const auto & par : m_container->m_fit_pars ) {
    if ( (! par.is_fixed()) && par.has_constraint()) { 
      // Parameter constraints are assumed to be gaussian
      m_nll += 
        this->nll_gaussian( 
          par.m_val_mod, 
          par.get_constr_val(), 
          par.get_constr_unc()
        );
    }
  }
}

void PoissonNLLMinimizer::minimize() {
  /** Perform the actual negative log-likelihood minimization using Minuit2.
      Will modify the m_val_mod of all parameters in the container!
  **/
  
  // Set minimizer strategy to high accuracy
  // -> Want precision results, if that takes longer it takes longer.
  m_minimizer->SetStrategy(2); 
  
  // Tell minimizer to perform hessian error-calculation for accurate errors
  m_minimizer->SetValidError(true); 
  
  // Create a vector holding the addresses of the parameter values
  // => Minimizer will directly change parameter values by changing the 
  //    values at the addresses
  const unsigned int n_pars = m_container->m_fit_pars.size();
  std::vector<double*> pars(n_pars);
  for ( unsigned int i=0; i<n_pars; i++ ){
    pars[i] = &(m_container->m_fit_pars[i].m_val_mod);
  }
    
  // Thing that minimizer performs NLL minimization on
  const ROOT::Math::Functor recalc_nll (
    // Lambda function for the minimizer:
    // Update the parameter set, then recalculate and return new NLL
    [pars, n_pars, this](const double * _pars) { 
      for ( unsigned int i=0; i<n_pars; i++ ) { *(pars[i]) = _pars[i]; }
      this->update_nll();
      return this->get_nll();
    },
    // Needs to know the correct number of parameters
    n_pars
  );
  
  // Set up minimizer by telling about function and parameters
  m_minimizer->SetFunction(recalc_nll);
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
  
  // Check if any parameters were limited and if so reset parameters as free and 
  // re-perform error calculation.
  // This is recommended for Minuit to get more precise errors because without 
  // limits no internal parameter transformations have to be performed.
  bool was_limited = false;
  for ( unsigned int i_par=0; i_par<n_pars; i_par++ ){
    if ( m_container->m_fit_pars[i_par].is_limited() ) {
      m_minimizer->SetVariable(
        i_par, 
        m_minimizer->VariableName(i_par), 
        m_minimizer->X()[i_par],
        m_minimizer->Errors()[i_par]
      );
      was_limited = true;
    }
  }
  if ( was_limited ) { 
    spdlog::debug("Minimisation used parameter limits, recalculating error without limits for accuracy.");
    m_minimizer->Hesse(); 
  }
  
  // Form a usable output collection
  this->collect_par_names();
  this->update_result();
}

//------------------------------------------------------------------------------
// Result collecting

void PoissonNLLMinimizer::collect_par_names() {
  unsigned int n_pars = m_container->m_fit_pars.size();
  m_result.m_par_names.resize(n_pars);
  for ( unsigned int i_par=0; i_par<n_pars; i_par++ ){
    m_result.m_par_names[i_par] = m_container->m_fit_pars[i_par].get_name();
  }
}

void PoissonNLLMinimizer::update_result() {
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