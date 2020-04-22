#include <CppUtils/Rnd.h>
#include <ToyMeas/ParFlct.h>

#include "spdlog/spdlog.h"

namespace PREW {
namespace ToyMeas {

//------------------------------------------------------------------------------

void ParFlct::fluctuate_constr ( Fit::FitPar & par ) {
  /** Fluctuate the gaussian constraint of a given parameter within its 
      uncertainty.
  **/
  par.set_constrgauss(
    CppUtils::Rnd::gauss_fluctuate(par.get_constr_val(), par.get_constr_unc()),
    par.get_constr_unc()
  );
}

void ParFlct::fluctuate_constrs ( Fit::ParVec & par_vec ) {
  /** Fluctuate the mean of all the gauss constraints of the parameters (if they
      have one) within the constrain-uncertainty.
      Does not change the uncertainty itself.
  **/
  for (auto & par: par_vec) {
    if (par.has_constraint()) {
      fluctuate_constr(par);
    }
  }
}

//------------------------------------------------------------------------------

} // Namespace ToyMeas
} // Namespace PREW