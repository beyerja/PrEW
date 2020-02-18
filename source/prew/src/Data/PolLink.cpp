#include <Data/PolLink.h>

#include <stdexcept>
#include <string>

namespace PREW {
namespace Data {

//------------------------------------------------------------------------------
// Constructors

PolLink::PolLink(
  int energy, 
  std::string pol_config, 
  std::string eM_pol, std::string eP_pol, 
  std::string eM_sgn, std::string eP_sgn
) : 
  m_energy(energy),
  m_pol_config(pol_config),
  m_eM_pol(eM_pol),
  m_eP_pol(eP_pol),
  m_eM_sgn(eM_sgn),
  m_eP_sgn(eP_sgn)
{
  /** Construtor checks polarisation signs are valid.
      Sign can be:
        "+" => Take polarisation variable as is
        "-" => Take negative of polarisation variable
  **/
  if ( (m_eM_sgn != "+") && (m_eM_sgn != "-") ) {
    throw std::invalid_argument("Invalid e- pol. sign for : " + m_eM_pol);
  } 
  if ( (m_eP_sgn != "+") && (m_eP_sgn != "-") ) {
    throw std::invalid_argument("Invalid e+ pol. sign for : " + m_eP_pol);
  } 
}

//------------------------------------------------------------------------------
// Get functions

int PolLink::get_energy() const { return m_energy; }
std::string PolLink::get_pol_config() const { return m_pol_config; }
std::string PolLink::get_eM_pol() const { return m_eM_pol; }
std::string PolLink::get_eP_pol() const { return m_eP_pol; }

double PolLink::get_eM_sgn_factor() const {
  /** Get the factor (+1 or -1) associated with the e- polarisation sign.
  **/
  return (m_eM_sgn == "-") ? -1.0 : 1.0;
}

double PolLink::get_eP_sgn_factor() const {
  /** Get the factor (+1 or -1) associated with the e+ polarisation sign.
  **/
  return (m_eP_sgn == "-") ? -1.0 : 1.0;
}

//------------------------------------------------------------------------------

}
}