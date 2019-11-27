#include <FitPar.h>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors


FitPar::FitPar(double val_ini) : m_val_ini(val_ini), m_val_mod(val_ini) {};

//------------------------------------------------------------------------------
// get functions

double const FitPar::get_val_ini() { return m_val_ini; }
  
//------------------------------------------------------------------------------
// Modifying functions

void FitPar::reset() { m_val_mod = m_val_ini; }

//------------------------------------------------------------------------------

}
}