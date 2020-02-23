#include <Connect/Linker.h>
#include <CppUtils/Vec.h>
#include <Fcts/FctMap.h>

#include "spdlog/spdlog.h"

#include <functional>
#include <exception>
#include <string>
#include <vector>

namespace PREW {
namespace Connect {

//------------------------------------------------------------------------------
// Constructors

Linker::Linker( Data::FctLinkVec fcts_links,
                CppUtils::Vec::Matrix2D<double> bin_centers,
                Data::CoefDistrVec coefs 
) : m_fcts_links(fcts_links),
    m_bin_centers(bin_centers),
    m_coefs(coefs) 
{}

//------------------------------------------------------------------------------

std::function<double()> Linker::get_bonded_fct_at_bin (
  const std::string &fct_name,
  size_t bin,
  Fit::ParVec *pars
) const {
  /** Return the requested parameterisation function in which the values of
      bin centers and coefficients are fixed (to their values at the bin),
      and for which the parameters are connected to the given pointers.
      This means that if parameters are changed the output of the bounded 
      function will change.
  **/
  
  if (bin > m_bin_centers.size()) {
    throw std::out_of_range("Asking for function for non-existing bin!");
  }
  std::vector<double> bin_center = m_bin_centers[bin];

  // Find link of function (names of pars and coefs to be used)
  auto name_condition = 
    [fct_name](const Data::FctLink &link) {
      return link.m_fct_name == fct_name;
    };
  Data::FctLink fct_link = 
    CppUtils::Vec::element_by_condition(m_fcts_links, name_condition);

  // Find needed coefficient values
  spdlog::debug("Looking for {} coefficients.", fct_link.m_coefs.size());
  std::vector<double> bin_coefs {};
  for ( const auto & coef_name: fct_link.m_coefs ) {
    // Find coefficient distribution with given name
    auto name_condition = [coef_name](const Data::CoefDistr& coef_distr) 
                            {return coef_distr.m_coef_name==coef_name;};
    auto coefs = CppUtils::Vec::element_by_condition(m_coefs, name_condition);
    // Choose coeffient value at bin
    bin_coefs.push_back(coefs.m_coefficients[bin]);
  }
  spdlog::debug("Found {} coefficients.", bin_coefs.size());
  
  // Find pointers to needed parameters
  spdlog::debug("Looking for {} parameters.", fct_link.m_pars.size());
  std::vector<double*> bin_pars {};
  for ( auto & par_name: fct_link.m_pars ) {
    // Find the index of the correct parameter and connect the modifiable 
    // parameter value in with the function
    for (size_t i_par=0; i_par<pars->size(); i_par++) {
      if ( (*pars)[i_par].get_name() == par_name ) {
        // Found correct parameter, use pointer to its modifiable value
        bin_pars.push_back( & ((*pars)[i_par].m_val_mod) );
        continue;
      }
    }
  }
  spdlog::debug("Found {} parameters.", bin_pars.size());
  
  // Check if requested function exists
  if ( Fcts::prew_fct_map.find(fct_name) == Fcts::prew_fct_map.end() ) {
    throw std::invalid_argument("Function not known: " + fct_name);
  }
  
  // Fix the arguments of the requested function:
  // Bin center and coefficient values are fixed, parameter pointers are fixed.
  std::function<double()> bound_fct = 
   std::bind( 
     Fcts::prew_fct_map.at(fct_name),
     bin_center,
     bin_coefs,
     bin_pars
   );

  return bound_fct;
}

//------------------------------------------------------------------------------

std::vector<std::function<double()>> Linker::get_all_bonded_fcts_at_bin(
  size_t bin,
  Fit::ParVec *pars
) const {
  /** Get all bonded parametrisation functions for the given bin.
      (More details in get_bonded_fct_at_bin)
  **/
  
  std::vector<std::function<double()>> bonded_fcts_at_bin {};
  for (const auto & fct_link: m_fcts_links) {
    bonded_fcts_at_bin.push_back(
      get_bonded_fct_at_bin(fct_link.m_fct_name, bin, pars)
    );
  }
  
  return bonded_fcts_at_bin;
}

//------------------------------------------------------------------------------

}
}