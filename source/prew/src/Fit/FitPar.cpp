#include <Fit/FitPar.h>

#include <cmath>

namespace PrEW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors


FitPar::FitPar(
  std::string name,
  double val_ini,
  double unc_ini,
  bool is_fixed
) : m_name(name), m_val_ini(val_ini), m_unc_ini(unc_ini), m_is_fixed(is_fixed) 
{
  m_val_mod = m_val_ini;
}

FitPar FitPar::clone(const std::string &name) const {
  /** Clone this FitPar into a new one that has all the same values and only
      changes the name.
   **/
  auto new_par = FitPar(name, m_val_ini, m_unc_ini, m_is_fixed);
  new_par.m_val_mod = m_val_mod;
  if (this->is_limited()) {
    new_par.set_limits(this->get_lower_lim(),this->get_upper_lim());
  }
  
  if (this->has_constraint()) {
    new_par.set_constrgauss(this->get_constr_val(),this->get_constr_unc());
  }
  return new_par;
}

//------------------------------------------------------------------------------
// get functions

const std::string & FitPar::get_name() const { return m_name; }
double FitPar::get_val_ini() const { return m_val_ini; }
double FitPar::get_unc_ini() const { return m_unc_ini; }
bool FitPar::is_fixed() const { return m_is_fixed; }
bool FitPar::is_limited() const { return m_is_limited; }
double FitPar::get_lower_lim() const { return m_lower_lim; }
double FitPar::get_upper_lim() const { return m_upper_lim; }
  
//------------------------------------------------------------------------------
// Constraints

void FitPar::set_constrgauss(double constr_val, double constr_unc) {
  /** Set a Gaussian constraint for this parameter.
  **/
  m_has_constraint = true;
  m_constr_val = constr_val;
  m_constr_unc = constr_unc;
}

bool FitPar::has_constraint() const { return m_has_constraint; }
double FitPar::get_constr_val() const { return m_constr_val; }
double FitPar::get_constr_unc() const { return m_constr_unc; }

double FitPar::calc_constr_chisq() const {
  /** Return the Chi^2 resulting from the parameter constraint (if available).
  **/
  double chisq=0;
  if ( m_has_constraint ) {
    chisq = std::pow( (m_val_mod - m_constr_val) / m_constr_unc, 2 );
  }
  return chisq; 
}

//------------------------------------------------------------------------------
// Modifying functions

void FitPar::set_limits(double lower_lim, double upper_lim) {
  /** Set upper and lower limit for allowed parameter space.
  **/
  m_is_limited = true;
  m_lower_lim = lower_lim;
  m_upper_lim = upper_lim;
}

void FitPar::fix()     { m_is_fixed=true; }
void FitPar::release() { m_is_fixed=false; }

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

std::ostream& operator<<(std::ostream& os, const PrEW::Fit::FitPar& fp) {
  /** Define what happens when "<<" is called on class
  **/
  os << "Fit Parameter: initial value: " << fp.get_val_ini()
     << " -> modified value: " << fp.m_val_mod;
  return os;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
