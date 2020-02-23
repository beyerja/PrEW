#include <Connect/DataConnector.h>
#include <Connect/Linker.h>
#include <CppUtils/Map.h>
#include <Data/DistrInfo.h>
#include <Data/DistrUtils.h>
#include <GlobalVar/Chiral.h>
#include <ToyMeas/Flct.h>
#include <ToyMeas/ToyGenerator.h>

#include "spdlog/spdlog.h"

#include <algorithm>
#include <cmath>
#include <exception>

namespace PREW {
namespace ToyMeas {

//------------------------------------------------------------------------------
// Constructors

ToyGenerator::ToyGenerator(
  const Connect::DataConnector & connector,
  const Fit::ParVec & pars
) : m_connector(connector), m_pars(pars) 
{
  // TODO DESCRIPTION!!!
  
  if (m_connector.get_pred_distrs().size() == 0) {
    spdlog::warn("No predicted distributions supplied!");
  }
  
  // First figure energies and available distributions and polarisation configs
  // for each energy
  std::set<int> energies {};
  CppUtils::Map::IntToStrSetMap distr_per_energies {};
  CppUtils::Map::IntToStrSetMap pol_configs_per_energies {};
  for (const auto & distr: m_connector.get_pred_distrs()) {
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
    // TODO Map function to check if element exists TODO
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
        
        m_diff_distrs.push_back(distribution);
      } // End pol config loop
    } // End diff distr loop
  } // End energy loop
}

//------------------------------------------------------------------------------
// Functions to get distributions which were generated from predictions

Data::DiffDistrVec ToyGenerator::get_expected_distrs ( int energy ) const {
  // TODO TODO TODO COMMENT!!!
  auto distrs = Data::DistrUtils::subvec_energy(m_diff_distrs, energy);
  
  Data::DiffDistrVec output_distrs {};
  for ( const auto & distr: distrs ) {
    Data::DiffDistr output_distr {
      distr.m_info,
      distr.m_bin_centers,
      {} // Bin contents to be set now
    };
    
    for ( const auto & bin : distr.m_distribution ) {
      output_distr.m_distribution.push_back(
        Fit::FitBin(bin.get_val_prd(), std::sqrt(bin.get_val_prd()))
      );
    }
    output_distrs.push_back(output_distr);
  }
  
  return output_distrs;
}

Data::DiffDistrVec ToyGenerator::get_fluctuated_distrs ( int energy ) const {
  /** Get poisson fluctuated versions of the expected distributions at the given
      energy.
  **/
  Data::DiffDistrVec distrs = this->get_expected_distrs(energy);
  
  // Fluctuate each bin with a Poisson distribution on its value and adjust unc.
  for (auto & distr: distrs) {
    for (auto & bin: distr.m_distribution) {
      Flct::poisson_fluctuate(bin);
    }
  }
  return distrs;
}

//------------------------------------------------------------------------------
  
}
}