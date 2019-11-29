#include "ParConstrGauss.h"

#include <cmath>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

ParConstrGauss::ParConstrGauss(double val, double unc) :
  m_val(val), m_unc(unc) {};
      
//------------------------------------------------------------------------------
// Core functions

double ParConstrGauss::calc_chisq(double par_val) const {
  return std::pow( (par_val - m_val) / m_unc, 2 );
}

//------------------------------------------------------------------------------
  
}
}