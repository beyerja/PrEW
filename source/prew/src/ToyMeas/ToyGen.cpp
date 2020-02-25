#include <Connect/DataConnector.h>
#include <CppUtils/Map.h>
#include <CppUtils/Rnd.h>
#include <Data/DistrInfo.h>
#include <Data/DistrUtils.h>
#include <ToyMeas/ToyGen.h>

#include "spdlog/spdlog.h"

#include <algorithm>
#include <cmath>
#include <exception>

namespace PREW {
namespace ToyMeas {

//------------------------------------------------------------------------------
// Constructors

ToyGen::ToyGen(
  const Connect::DataConnector & connector,
  const Fit::ParVec & pars
) : m_connector(connector), m_pars(pars)
{
  /** Toy generator constructor sets up the distributions with linked 
      predictions for every predicted distribution in the connector.
      These linked predictions are later used to create toy measurements by
      fluctuating the prediction in each bin.
  **/
  
  if (m_connector.get_pred_distrs().size() == 0) {
    spdlog::warn("No predicted distributions supplied!");
  }
  
  // First figure energies and available distributions and polarisation configs
  // for each energy
  std::set<int> energies {};
  CppUtils::Map::IntToStrSetMap distr_per_energies {};
  CppUtils::Map::IntToStrSetMap pol_configs_per_energies {};
  for (const auto & distr: m_connector.get_pred_distrs()) {
    // TODO Split: Find energies => Split off finding of polarisation configs
    int energy = distr.m_info.m_energy;
    energies.insert(energy);
    distr_per_energies[energy].insert(distr.m_info.m_distr_name);
    
    // Find polarisation link for this energy 
    // => which polarisations exist?
    auto energy_condition = 
      [energy](const Data::PolLink& link) {return link.get_energy()==energy;};
    Data::PolLinkVec pol_links = 
      CppUtils::Vec::subvec_by_condition( 
        m_connector.get_pol_links(), energy_condition 
      );
      
    // Find all available polarisations at energy from polarisation link
    for (const auto& pol_link: pol_links) {
      std::string pol_config = pol_link.get_pol_config();
      pol_configs_per_energies[energy].insert(pol_config);
    }
  }
  
  // Check what was found
  for (const int & energy: energies) {
    if (pol_configs_per_energies.find(energy)==pol_configs_per_energies.end()) {
      spdlog::warn("For E={} no pol. config. found!", energy);
      continue;
    }
    spdlog::debug(
      "For E={} found {} distributions and {} pol. configurations.",
      energy,
      distr_per_energies.at(energy).size(), 
      pol_configs_per_energies.at(energy).size()
    );
  }
  

  // Loop over all distributions found at their respective energies
  spdlog::debug("Start setting up basic toy distributions.");
  for (const int & energy: energies) {
    for (const auto & distr_name: distr_per_energies.at(energy)) {
      // Get distribution setup bin centers from first prediction
      auto predictions = 
        Data::DistrUtils::subvec_energy_and_name(m_connector.get_pred_distrs(),energy,distr_name);
      auto bin_centers = predictions.at(0).m_bin_centers;
        
      // Loop over all polarisations for the given energy
      for (const auto& pol_config: pol_configs_per_energies.at(energy)) {
        spdlog::debug( 
          "Setting up distribution for : {}, {}, {}.",
          energy, distr_name, pol_config
        );
        
        // Create a differential distribution for this polarisation config.
        // Fit bins empty for now, prediction to be set
        Data::DistrInfo current_info { distr_name, pol_config, energy };
        Data::DiffDistr distribution {};
        distribution.m_info = current_info;
        distribution.m_bin_centers = bin_centers;
        distribution.m_distribution = Fit::BinVec(bin_centers.size());
        
        // Fix the bin predictions and link them to the given fit parameters
        Fit::BinVec connected_bins {};
        m_connector.fill_bins(
          distribution,
          &m_pars,
          &connected_bins
        );
        distribution.m_distribution = connected_bins;
        
        // Set measurement uncertainty to poissonian uncertainty
        // -> Should not be modified because unc. uses expectation (not truth)
        for ( auto & bin : distribution.m_distribution ) {
          bin.set_val_unc( std::sqrt(bin.get_val_prd()) );
        }
        
        m_diff_distrs.push_back(distribution);
      } // End pol config loop
    } // End diff distr loop
  } // End energy loop
}

//------------------------------------------------------------------------------
// Internal functions

Fit::FitPar * ToyGen::find_par( const std::string & par_name ) {
  /**
  **/
  auto name_cond = 
    [par_name](const Fit::FitPar& par) {return par.get_name()==par_name;};
  auto par_it = std::find_if( m_pars.begin(), m_pars.end(), name_cond );
  
  if (par_it == m_pars.end()) {
    throw std::invalid_argument("Unknown parameter: " + par_name);
  } 
  
  return &(*par_it);
}

Data::DiffDistrVec ToyGen::get_distrs ( int energy, bool fluctuated ) const {
  // TODO TODO TODO Properly document, right now still old documentation!
  /** Get the toy distributions at a given energy.
      Sets the measured value either to the predicted value (with current 
      parameters) or a poisson fluctuated version of it.
      Afterwards removes the prediction function because it is connected to 
      internal members variables of the toy generator.
  **/
  // Get the distributions at this energy (with measurement = prediction)
  auto distrs = Data::DistrUtils::subvec_energy(m_diff_distrs, energy);
  
  // Modify bin measured value and remove prediction function
  for (auto & distr: distrs) {
    for (auto & bin: distr.m_distribution) {
      if ( fluctuated ) {
        // Fluctuate measured bin value around (potentially modified) prediction
        bin.set_val_mst( CppUtils::Rnd::poisson_fluctuate(bin.get_val_prd()) );
      } else {
        // Set bin value to (potentially modified) prediction
        bin.set_val_mst( bin.get_val_prd() );
      }
      
      bin.set_prd_fct({}); // Remove toy gen internal prediction function
    }
  }
  return distrs;
}


//------------------------------------------------------------------------------
// Modifying parameter values

void ToyGen::modify_par ( const std::string & par_name, double val_mod ) {
  /** Modify the named parameter to a new value.
      Will lead to different measured value.
      Uncertainty is not touched because it is determined by the original 
      expectation and not the true parameter value.
  **/
  this->find_par(par_name)->m_val_mod = val_mod;
}

void ToyGen::reset_par  ( const std::string & par_name ) {
  /** Reset the named parameter to its original value.
  **/
  this->find_par(par_name)->reset();
}

void ToyGen::reset_pars () {
  /** Reset all parameters to their original values.
  **/
  for (auto & par: m_pars) { par.reset(); }
}

//------------------------------------------------------------------------------
// Functions to get distributions which were generated from predictions

Data::DiffDistrVec ToyGen::get_expected_distrs ( int energy ) const {
  /** Get all expected distributions at the given energy.
      The measured values are determined from the current parameters.
      The uncertainties use the original parameters.
  **/
  return this->get_distrs(energy,false);
}

Data::DiffDistrVec ToyGen::get_fluctuated_distrs ( int energy ) const {
  /** Get poisson fluctuated versions of the expected distributions at the given
      energy.
  **/
  return this->get_distrs(energy,true);
}

//------------------------------------------------------------------------------
  
}
}