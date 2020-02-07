#include <Connect/Linker.h>
#include <Connect/LinkHelp.h>
#include <Data/CoefDistr.h>
#include <Data/FnctLink.h>
#include <GlobalVar/Chiral.h>

namespace PREW {
namespace Connect {

//------------------------------------------------------------------------------

std::function<double()> LinkHelp::get_polfactor_lambda(
  const std::string                         & chirality, 
  const std::pair<std::string, std::string> & pol_pair, 
  Fit::ParVec *pars
) {
  /** Get lambda function for the polarisation factor associated with a chiral
      cross section. Lambda function output will be dependent on polarisation
      fit parameters (whose names are in the pol pair).
      In pol_pair: First string is electron polarisation parameter name, 
                   second is positron polarisation parameter name.
      Underlying equation:
        (1 + e-_chirality * P_e-) * (1 + e+_chirality * P_e+) / 4
  **/
  
  // Individual beam polarisation fit parameter names
  std::string e_pol = pol_pair.first;
  std::string p_pol = pol_pair.second;
  
  // Sign associated with a given chirality
  Data::CoefDistrVec pol_coefs {
    {"L", {}, {{-1}}},  // Left handed
    {"R", {}, {{+1}}}   // Right handed
  };
  
  // Chirality considered in the current cross section
  std::string e_chirality {}, p_chirality {};
  if ( chirality == GlobalVar::Chiral::eLpR ) {
    e_chirality = "L";
    p_chirality = "R";
  } else if ( chirality == GlobalVar::Chiral::eRpL ) {
    e_chirality = "R";
    p_chirality = "L";
  } else if ( chirality == GlobalVar::Chiral::eLpL ) {
    e_chirality = "L";
    p_chirality = "L";
  } else if ( chirality == GlobalVar::Chiral::eRpR ) {
    e_chirality = "R";
    p_chirality = "R";
  } else {
    throw std::invalid_argument("Unknown chirality" + chirality);
  }
  
  // Instruction class for how to build lambda function
  Data::FnctLinkVec pol_fnct_link {
    {"PolarisationFactor", {e_pol,p_pol}, {e_chirality,p_chirality} } };
    
  // Use Linker class to get function (Need one dummy 0 bin)
  auto pol_factor = 
    Connect::Linker(pol_fnct_link, {{0}}, pol_coefs)
    .get_bonded_fnct_at_bin("PolarisationFactor",0,pars);

  return pol_factor;
}

//------------------------------------------------------------------------------

std::function<double()> LinkHelp::get_modified_sigma(
  double sigma,
  const std::vector<std::function<double()>>& alphas
) {
  /** Take a cross section (sigma) and alpha factor functions and return a 
      function that gives the modified cross section value.
  **/
  auto sigma_mod_fct = 
    [sigma,alphas](){
      double sigma_mod = sigma;
      for (const auto & alpha: alphas) {sigma_mod *= alpha();}
      return sigma_mod;
    };
  return sigma_mod_fct;
}

//------------------------------------------------------------------------------

}
}
