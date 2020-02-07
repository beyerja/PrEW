#include <Fit/ParConstrGauss.h>
#include <CppUtils/Num.h>

#include <cmath>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

ParConstrGauss::ParConstrGauss(double val, double unc) :
  m_val(val), m_unc(unc) {}
      
//------------------------------------------------------------------------------
// Core functions

double ParConstrGauss::calc_chisq(double par_val) const {
  return std::pow( (par_val - m_val) / m_unc, 2 );
}

//------------------------------------------------------------------------------
// Reader functions

double ParConstrGauss::get_val() const { return m_val; }
double ParConstrGauss::get_unc() const { return m_unc; }

//------------------------------------------------------------------------------
// Operators

bool ParConstrGauss::operator==(const ParConstrGauss& other) const {
  /** Compare the value and uncertainty using 10_{-9} precision.
  **/
  return CppUtils::Num::equal_to_eps(m_val, other.get_val(), 1e-9) &&
         CppUtils::Num::equal_to_eps(m_unc, other.get_unc(), 1e-9);
}

//------------------------------------------------------------------------------
  
}
}