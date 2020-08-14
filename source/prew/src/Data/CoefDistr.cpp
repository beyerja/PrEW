#include <Data/CoefDistr.h>

#include <stdexcept>

namespace PrEW {
namespace Data {

//------------------------------------------------------------------------------
// Constructors

CoefDistr::CoefDistr() {}

CoefDistr::CoefDistr(const std::string &coef_name, const DistrInfo &info,
                     const std::vector<double> &coefficients)
    : m_coef_name(coef_name), m_info(info), m_coefficients(coefficients),
      m_is_global(false) {
  /** Constructor using differential coefficients (different for each bin).
   **/
}

CoefDistr::CoefDistr(const std::string &coef_name, const DistrInfo &info,
                     double coefficient)
    : m_coef_name(coef_name), m_info(info), m_coefficient(coefficient),
      m_is_global(true) {
  /** Constructor using a global coefficient (same for each bin).
   **/
}

//------------------------------------------------------------------------------
// Access functions

const std::string &CoefDistr::get_coef_name() const { return m_coef_name; }
const DistrInfo &CoefDistr::get_info() const { return m_info; }

double CoefDistr::get_coef(int bin) const {
  /** Returns the coefficient for the given bin of the distribution.
   **/
  if (!m_is_global) {
    if (bin >= int(m_coefficients.size())) {
      throw std::out_of_range("CoefDistr: requested bin out of range: " +
                              std::to_string(bin));
    }
  }
  return m_is_global ? m_coefficient : m_coefficients[bin];
}

//------------------------------------------------------------------------------

} // Namespace Data
} // Namespace PrEW