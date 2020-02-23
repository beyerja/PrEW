#include <Connect/DataConnector.h>
#include <Connect/Linker.h>
#include <Connect/LinkHelp.h>
#include <Data/DistrUtils.h>
#include <GlobalVar/Chiral.h>

#include "spdlog/spdlog.h"

#include <exception>
#include <string>

namespace PREW {
namespace Connect {

//------------------------------------------------------------------------------
// Constructors

DataConnector::DataConnector (
  Data::PredDistrVec pred_distrs,
  Data::CoefDistrVec coef_distrs,
  Data::PredLinkVec  pred_links ,
  Data::PolLinkVec   pol_links  
) : 
  m_pred_distrs(pred_distrs), 
  m_coef_distrs(coef_distrs), 
  m_pred_links(pred_links), 
  m_pol_links(pol_links)
{
  // Check that necessary inputs are there
  if (m_pred_distrs.size() == 0) {
    throw std::invalid_argument("DataConnector needs predicted distributions!");
  }
  if (m_pol_links.size() == 0) {
    throw std::invalid_argument("DataConnector needs polarisation links!");
  }
  
  // Check if optional inputs are there
  if (m_coef_distrs.size() == 0) {
    spdlog::debug("No coefficients supplied to DataConnector.");
  }
  if (m_pred_links.size() == 0) {
    spdlog::debug("No prediction links supplied to DataConnector.");
  }
}

//------------------------------------------------------------------------------
// Read functions

const Data::PredDistrVec& DataConnector::get_pred_distrs() const { 
  return m_pred_distrs; 
}
const Data::CoefDistrVec& DataConnector::get_coef_distrs() const { 
  return m_coef_distrs; 
}
const Data::PredLinkVec&  DataConnector::get_pred_links()  const { 
  return m_pred_links; 
}
const Data::PolLinkVec&   DataConnector::get_pol_links()   const { 
  return m_pol_links; 
}

//------------------------------------------------------------------------------
// Core functionality

void DataConnector::fill_bins(
  const Data::DiffDistr & diff_distr,
  Fit::ParVec *pars,
  Fit::BinVec *bins
) const {
  /** Set bin prediction functions for all bins of the distribution.
      Predictions will be correctly connected to the given input parameters.
      
      A full prediction for a single bin is calculated as:
        f_pol,1 * ... * f_pol,n *
        ( (1-Pe-)*)(1+Pe+)/4 * f_LR,1 * ... * f_LR,n * pred_LR +
          ... +
          (1+Pe-)*)(1+Pe+)/4 * f_RR,1 * ... * f_LR,n * pred_RR
        )
        {Calculated for signal and background separately and then added up.}
      Where:
        f_pol,i : factor functions applied to the prediction for a given 
                  polarisation configuration
        f_LR/LL/...,i : factor functions applied to the prediction for a given 
                        incoming particle helicity 
        Pe- : electron polarisation
        Pe+ : positron polarisation
  **/
  
  // Information of the given distribution
  std::string distr_name = diff_distr.m_info.m_distr_name;
  std::string pol_config = diff_distr.m_info.m_pol_config;
  int energy             = diff_distr.m_info.m_energy;
  
  CppUtils::Vec::Matrix2D<double> bin_centers = diff_distr.m_bin_centers;
  
  // Find polarisation link for this energy
  auto energy_pol_condition = 
    [energy,pol_config](const Data::PolLink& link) {
      return (link.get_energy()==energy) && (link.get_pol_config()==pol_config);
    };
  Data::PolLink pol_link = 
    CppUtils::Vec::element_by_condition(m_pol_links, energy_pol_condition);

  // Find corresponding predicted distributions, links and coefficients
  Data::PredDistrVec predictions  = 
    Data::DistrUtils::subvec_energy_and_name(m_pred_distrs, energy, distr_name);
  Data::CoefDistrVec coefficients = 
    Data::DistrUtils::subvec_energy_and_name(m_coef_distrs, energy, distr_name);
  Data::PredLinkVec links         = 
    Data::DistrUtils::subvec_energy_and_name(m_pred_links, energy, distr_name);

  // --- Get chiral predictions, links and coefficients ------------------------
  auto pred_LR = Data::DistrUtils::element_pol( predictions, 
                                                GlobalVar::Chiral::eLpR);
  auto pred_RL = Data::DistrUtils::element_pol( predictions, 
                                                GlobalVar::Chiral::eRpL);
  auto pred_LL = Data::DistrUtils::element_pol( predictions, 
                                                GlobalVar::Chiral::eLpL);
  auto pred_RR = Data::DistrUtils::element_pol( predictions, 
                                                GlobalVar::Chiral::eRpR);

  auto links_LR = Data::DistrUtils::element_pol(links, GlobalVar::Chiral::eLpR);
  auto links_RL = Data::DistrUtils::element_pol(links, GlobalVar::Chiral::eRpL);
  auto links_LL = Data::DistrUtils::element_pol(links, GlobalVar::Chiral::eLpL);
  auto links_RR = Data::DistrUtils::element_pol(links, GlobalVar::Chiral::eRpR);

  auto coefs_LR = Data::DistrUtils::subvec_pol( coefficients, 
                                                GlobalVar::Chiral::eLpR);
  auto coefs_RL = Data::DistrUtils::subvec_pol( coefficients, 
                                                GlobalVar::Chiral::eRpL);
  auto coefs_LL = Data::DistrUtils::subvec_pol( coefficients, 
                                                GlobalVar::Chiral::eLpL);
  auto coefs_RR = Data::DistrUtils::subvec_pol( coefficients, 
                                                GlobalVar::Chiral::eRpR);
  // ---------------------------------------------------------------------------

  // --- Get linkers for chiral alpha functions ------- ------------------------
  Connect::Linker linker_sig_LR = 
    Connect::Linker(links_LR.m_fcts_links_sig, bin_centers, coefs_LR);
  Connect::Linker linker_bkg_LR = 
    Connect::Linker(links_LR.m_fcts_links_bkg, bin_centers, coefs_LR);

  Connect::Linker linker_sig_RL = 
    Connect::Linker(links_RL.m_fcts_links_sig, bin_centers, coefs_RL);
  Connect::Linker linker_bkg_RL = 
    Connect::Linker(links_RL.m_fcts_links_bkg, bin_centers, coefs_RL);

  Connect::Linker linker_sig_LL = 
    Connect::Linker(links_LL.m_fcts_links_sig, bin_centers, coefs_LL);
  Connect::Linker linker_bkg_LL = 
    Connect::Linker(links_LL.m_fcts_links_bkg, bin_centers, coefs_LL);

  Connect::Linker linker_sig_RR = 
    Connect::Linker(links_RR.m_fcts_links_sig, bin_centers, coefs_RR);
  Connect::Linker linker_bkg_RR = 
    Connect::Linker(links_RR.m_fcts_links_bkg, bin_centers, coefs_RR);
  // ---------------------------------------------------------------------------

  // --- Get linkers for polarised alpha functions -----------------------------
  auto links_pol = Data::DistrUtils::element_pol(links, pol_config);
  auto coefs_pol = Data::DistrUtils::subvec_pol(coefficients, pol_config);

  Connect::Linker linker_sig_pol = 
    Connect::Linker(links_pol.m_fcts_links_sig, bin_centers, coefs_pol);
  Connect::Linker linker_bkg_pol = 
    Connect::Linker(links_pol.m_fcts_links_bkg, bin_centers, coefs_pol);
  // ---------------------------------------------------------------------------

  // --- Get polarisation factor alpha functions -------------------------------
  auto pol_factor_LR = 
    LinkHelp::get_polfactor_lambda(GlobalVar::Chiral::eLpR, pol_link, pars);
  auto pol_factor_RL = 
    LinkHelp::get_polfactor_lambda(GlobalVar::Chiral::eRpL, pol_link, pars);
  auto pol_factor_LL = 
    LinkHelp::get_polfactor_lambda(GlobalVar::Chiral::eLpL, pol_link, pars);
  auto pol_factor_RR = 
    LinkHelp::get_polfactor_lambda(GlobalVar::Chiral::eRpR, pol_link, pars);
  // ---------------------------------------------------------------------------

  // Set the prediction of each distribution
  for ( size_t bin=0; bin<bin_centers.size(); bin++ ) {
    spdlog::debug("Binding functions for bin {}.", bin);
    
    // -------------------- Get chiral signal prediction -----------------------
    spdlog::debug("Getting chiral signal predictions.");
    double sigma_sig_LR = pred_LR.m_sig_distr[bin];
    double sigma_sig_RL = pred_RL.m_sig_distr[bin];
    double sigma_sig_LL = pred_LL.m_sig_distr[bin];
    double sigma_sig_RR = pred_RR.m_sig_distr[bin];

    auto alphas_sig_LR = linker_sig_LR.get_all_bonded_fcts_at_bin(bin, pars);
    auto alphas_sig_RL = linker_sig_RL.get_all_bonded_fcts_at_bin(bin, pars);
    auto alphas_sig_LL = linker_sig_LL.get_all_bonded_fcts_at_bin(bin, pars);
    auto alphas_sig_RR = linker_sig_RR.get_all_bonded_fcts_at_bin(bin, pars);

    auto sigma_sig_LR_mod =
        LinkHelp::get_modified_sigma(sigma_sig_LR, alphas_sig_LR);
    auto sigma_sig_RL_mod =
        LinkHelp::get_modified_sigma(sigma_sig_RL, alphas_sig_RL);
    auto sigma_sig_LL_mod =
        LinkHelp::get_modified_sigma(sigma_sig_LL, alphas_sig_LL);
    auto sigma_sig_RR_mod =
        LinkHelp::get_modified_sigma(sigma_sig_RR, alphas_sig_RR);
    // -------------------------------------------------------------------------

    // -------------------- Get chiral background prediction -------------------
    spdlog::debug("Getting chiral background predictions.");
    double sigma_bkg_LR = pred_LR.m_bkg_distr[bin];
    double sigma_bkg_RL = pred_RL.m_bkg_distr[bin];
    double sigma_bkg_LL = pred_LL.m_bkg_distr[bin];
    double sigma_bkg_RR = pred_RR.m_bkg_distr[bin];

    auto alphas_bkg_LR = linker_bkg_LR.get_all_bonded_fcts_at_bin(bin, pars);
    auto alphas_bkg_RL = linker_bkg_RL.get_all_bonded_fcts_at_bin(bin, pars);
    auto alphas_bkg_LL = linker_bkg_LL.get_all_bonded_fcts_at_bin(bin, pars);
    auto alphas_bkg_RR = linker_bkg_RR.get_all_bonded_fcts_at_bin(bin, pars);

    auto sigma_bkg_LR_mod =
        LinkHelp::get_modified_sigma(sigma_bkg_LR, alphas_bkg_LR);
    auto sigma_bkg_RL_mod =
        LinkHelp::get_modified_sigma(sigma_bkg_RL, alphas_bkg_RL);
    auto sigma_bkg_LL_mod =
        LinkHelp::get_modified_sigma(sigma_bkg_LL, alphas_bkg_LL);
    auto sigma_bkg_RR_mod =
        LinkHelp::get_modified_sigma(sigma_bkg_RR, alphas_bkg_RR);
    // -------------------------------------------------------------------------

    // -------------------- Get polarised signal prediction --------------------
    spdlog::debug("Getting polarised signal predictions.");
    auto alphas_sig_pol = linker_sig_pol.get_all_bonded_fcts_at_bin(bin, pars);

    // No longer sigma because includes lumi => #Events
    auto pred_sig_pol =
      [ sigma_sig_LR_mod, pol_factor_LR,
        sigma_sig_RL_mod, pol_factor_RL,
        sigma_sig_LL_mod, pol_factor_LL,
        sigma_sig_RR_mod, pol_factor_RR,
        alphas_sig_pol
      ] () {
        double sigma_mod =  pol_factor_LR() * sigma_sig_LR_mod() +
                            pol_factor_RL() * sigma_sig_RL_mod() +
                            pol_factor_LL() * sigma_sig_LL_mod() +
                            pol_factor_RR() * sigma_sig_RR_mod();
        for (const auto & alpha: alphas_sig_pol) {sigma_mod *= alpha();}
        return sigma_mod;
      };
    // -------------------------------------------------------------------------

    // -------------------- Get polarised background prediction ----------------
    spdlog::debug("Getting polarised background predictions.");
    auto alphas_bkg_pol = linker_bkg_pol.get_all_bonded_fcts_at_bin(bin, pars);

    // No longer sigma because includes lumi => #Events
    auto pred_bkg_pol =
      [ sigma_bkg_LR_mod,pol_factor_LR,
        sigma_bkg_RL_mod,pol_factor_RL,
        sigma_bkg_LL_mod,pol_factor_LL,
        sigma_bkg_RR_mod,pol_factor_RR,
        alphas_bkg_pol
      ] () {
        double sigma_mod =  pol_factor_LR() * sigma_bkg_LR_mod() +
                            pol_factor_RL() * sigma_bkg_RL_mod() +
                            pol_factor_LL() * sigma_bkg_LL_mod() +
                            pol_factor_RR() * sigma_bkg_RR_mod();
        for (const auto & alpha: alphas_bkg_pol) {sigma_mod *= alpha();}
        return sigma_mod;
      };
    // -------------------------------------------------------------------------

    // -------------------- Get total polarised prediction ---------------------
    spdlog::debug("Getting total polarised predictions.");
    std::function<double()> pred_pol  =
      [pred_sig_pol,pred_bkg_pol]() { return pred_sig_pol() + pred_bkg_pol(); };
    // -------------------------------------------------------------------------

    // -------------------- Set bin prediction function ------------------------
    // Copy the bin values from the one in the distribution
    Fit::FitBin connected_bin = diff_distr.m_distribution.at(bin);
    connected_bin.set_prd_fct(pred_pol); // Set the prediction
    bins->push_back(connected_bin);
    // -------------------------------------------------------------------------
  }
}


//------------------------------------------------------------------------------

void DataConnector::fill_fit_container(
  const Data::DiffDistrVec & diff_distrs,
  const Fit::ParVec        & pars,
  Fit::FitContainer *fit_container 
) const {
  
  if (  (fit_container->m_fit_pars.size() != 0) ||
        (fit_container->m_fit_bins.size() != 0)
  ) {
    throw std::invalid_argument("Can't fill non-empty fit container!");
  }
  
  // Parameters are just to be copied, are created from diff. distrs. with 
  // proper linking to the parameters in the fit container
  fit_container->m_fit_pars = pars;
  for ( const auto & distr : diff_distrs ) {
    this->fill_bins(  
      distr,
      &(fit_container->m_fit_pars),
      &(fit_container->m_fit_bins)
    );
  }
}

//------------------------------------------------------------------------------

}
}