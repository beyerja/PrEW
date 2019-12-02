#include <FitPar.h>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors


FitPar::FitPar(std::string name, double val_ini, double unc_ini) : 
  m_name(name), m_val_ini(val_ini), m_unc_ini(unc_ini), m_val_mod(val_ini) {}

//------------------------------------------------------------------------------
// get functions

std::string FitPar::get_name() const { return m_name; }
double FitPar::get_val_ini() const { return m_val_ini; }
  
//------------------------------------------------------------------------------
// Constraints

void FitPar::set_constrgauss(ParConstrGauss constrgauss) { 
  /** Set a Gaussian constraint for this parameter.
  **/
  m_is_constraint = true;
  m_constrgauss = constrgauss; 
}

double FitPar::calc_constr_chisq() const {
  /** Calculate chi-squared term produced by external parameter constraint.
  **/
  double chisq=0;
  if ( m_is_constraint ) chisq += m_constrgauss.calc_chisq(m_val_mod);
  return chisq; 
}

//------------------------------------------------------------------------------
// Modifying functions

void FitPar::reset() { m_val_mod = m_val_ini; }

//------------------------------------------------------------------------------
// Operators

bool FitPar::operator==(const FitPar& fit_par) const {
  /** Unique identifir is fit parameter.
      Changing the values won't change identity of parameter.
  **/
  return fit_par.get_name() == m_name;
}

//------------------------------------------------------------------------------

}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const PREW::Fit::FitPar& fp) {
  /** Define what happens when "<<" is called on class
  **/
  os << "Fit Parameter: initial value: " << fp.get_val_ini()
     << " -> modified value: " << fp.m_val_mod;
  return os;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
