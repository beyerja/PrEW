#include <Fit/FitBin.h>

namespace PrEW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

FitBin::FitBin(double val_mst, double val_unc, std::function<double()> prd_fct) : 
  m_val_mst(val_mst), m_val_unc(val_unc), m_prd_fct(prd_fct) {}

//------------------------------------------------------------------------------
// set functions

void FitBin::set_val_mst(double val_mst) { m_val_mst = val_mst; }
void FitBin::set_val_unc(double val_unc) { m_val_unc = val_unc; }
void FitBin::set_prd_fct(std::function<double()> prd_fct) {m_prd_fct = prd_fct;}

//------------------------------------------------------------------------------
// get functions

double FitBin::get_val_mst() const { return m_val_mst; }
double FitBin::get_val_unc() const { return m_val_unc; }

//------------------------------------------------------------------------------
// Envoking the prediction function

double FitBin::get_val_prd() const { return m_prd_fct(); }

//------------------------------------------------------------------------------

}
}