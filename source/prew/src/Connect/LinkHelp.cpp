#include <Connect/Linker.h>
#include <Connect/LinkHelp.h>
#include <Data/CoefDistr.h>
#include <Data/FctLink.h>
#include <GlobalVar/Chiral.h>

namespace PrEW {
namespace Connect {

//------------------------------------------------------------------------------

std::function<double()> LinkHelp::get_polfactor_lambda(
  const std::string   & chirality, 
  const Data::PolLink & pol_link, 
  Fit::ParVec *pars
) {
  /** Get lambda function for the polarisation factor associated with a chiral
      cross section. Lambda function output will be dependent on polarisation
      fit parameters (given in the pol_link).
      Underlying equation:
          (1 + e-_chirality * sgn(P_e-) * |P_e-|) / 2
        * (1 + e+_chirality * sgn(P_e+) * |P_e+|) / 2
        
  **/
  
  // Coefficients to be used in function building
  Data::CoefDistrVec pol_coefs {
    {"L", {}, {{-1}}},  // Left handed
    {"R", {}, {{+1}}},  // Right handed
    {"e-sign", {}, {{pol_link.get_eM_sgn_factor()}}}, // Sign of e- polarisation
    {"e+sign", {}, {{pol_link.get_eP_sgn_factor()}}}  // Sign of e+ polarisation
  };
  
  // Chirality considered in the current cross section
  std::string eM_chirality {}, eP_chirality {};
  if ( chirality == GlobalVar::Chiral::eLpR ) {
    eM_chirality = "L";
    eP_chirality = "R";
  } else if ( chirality == GlobalVar::Chiral::eRpL ) {
    eM_chirality = "R";
    eP_chirality = "L";
  } else if ( chirality == GlobalVar::Chiral::eLpL ) {
    eM_chirality = "L";
    eP_chirality = "L";
  } else if ( chirality == GlobalVar::Chiral::eRpR ) {
    eM_chirality = "R";
    eP_chirality = "R";
  } else {
    throw std::invalid_argument("Unknown chirality" + chirality);
  }
  
  // Instruction class for how to build lambda function
  Data::FctLinkVec pol_fct_link {
    {"PolarisationFactor", 
      {pol_link.get_eM_pol(), pol_link.get_eP_pol()}, // Parameters to use
      {eM_chirality,eP_chirality,"e-sign","e+sign"}   // Coefficients to use
    } 
  };
    
  // Use Linker class to get function (Need one dummy 0 bin)
  auto pol_factor = 
    Connect::Linker(pol_fct_link, {{0}}, pol_coefs)
    .get_bonded_fct_at_bin("PolarisationFactor",0,pars);

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
