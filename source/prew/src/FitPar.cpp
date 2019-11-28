#include <FitPar.h>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors


FitPar::FitPar(std::string name, double val_ini) : 
  m_name(name), m_val_ini(val_ini), m_val_mod(val_ini) {};

//------------------------------------------------------------------------------
// get functions

std::string FitPar::get_name() const { return m_name; }
double FitPar::get_val_ini() const { return m_val_ini; }
  
//------------------------------------------------------------------------------
// Modifying functions

void FitPar::reset() { m_val_mod = m_val_ini; }

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
