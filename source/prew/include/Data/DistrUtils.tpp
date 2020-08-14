#ifndef LIB_DISTRUTILS_TPP
#define LIB_DISTRUTILS_TPP 1

#include <CppUtils/Vec.h>
#include <Data/DistrUtils.h>

#include "spdlog/spdlog.h"

#include <algorithm>

namespace PrEW {
namespace Data {

//------------------------------------------------------------------------------

template<class T>
std::vector<T> DistrUtils::subvec_energy_and_name(
  const std::vector<T>& vec, 
  int energy, 
  std::string distr_name
) {
  /** Find sub-vector of vector vec in which only distributions are contained 
      whose info contain the given energy and distr_name.
  **/
  
  // Lambda comparison checking energy and name match for single distribution
  auto info_comparison = [energy, distr_name](const T& distr) { 
                          return (distr.get_info().m_energy == energy) &&
                                 (distr.get_info().m_distr_name == distr_name);
                          };
  
  // Return all distributions where energy and name match
  return CppUtils::Vec::subvec_by_condition( vec, info_comparison ); 
}

//------------------------------------------------------------------------------

template<class T>
std::vector<T> DistrUtils::subvec_energy(
  const std::vector<T>& vec, 
  int energy
) {
  /** Find sub-vector of vector vec in which only distributions are contained 
      whose info contain the given energy.
  **/
  
  // Lambda comparison checking energy match for single distribution
  auto info_comparison =  [energy](const T& distr) { 
                            return (distr.get_info().m_energy == energy);
                          };
  
  // Return all distributions where energy match
  return CppUtils::Vec::subvec_by_condition( vec, info_comparison ); 
}

//------------------------------------------------------------------------------

template<class T>
std::vector<T> DistrUtils::subvec_pol( 
  const std::vector<T>& vec, 
  const std::string& pol_config
) {
  /** Find polarisation-specific sub-vector in distribution vector vec.
  **/
  
  // Lambda comparison checking polarisation match for single distribution
  auto info_comparison = [pol_config](const T& distr) { 
                            return (distr.get_info().m_pol_config == pol_config);
                          };
  
  // Return all matching distributions
  return CppUtils::Vec::subvec_by_condition( vec, info_comparison );
}

//------------------------------------------------------------------------------

template<class T>
T DistrUtils::element_pol( 
  const std::vector<T>& vec, 
  const std::string& pol_config
) {
  /** Find polarisation-specific element in distribution vector vec.
      Includes check that exactly one such element exists.
  **/
  T output_element {};
  
  // Get all elements that fulfill polarisation condition
  auto all_pol_elements = subvec_pol( vec, pol_config );
  
  // Check for how many are found
  if ( all_pol_elements.size() == 0 ) {
    spdlog::warn("Vector doesn't have element of pol. {}", pol_config);
  } else {
    output_element = all_pol_elements.at(0);
  }
  
  if ( all_pol_elements.size() > 1 ) {
    spdlog::warn(
      "Vector has more than one element of pol. {} , returning first", 
      pol_config
    );
  }
  
  return output_element;
}

//------------------------------------------------------------------------------

}
}

#endif