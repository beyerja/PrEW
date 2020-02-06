#include <Fncts/Physics.h>

namespace PREW {
namespace Fncts {

//------------------------------------------------------------------------------

double Physics::polarisation_factor ( 
  const std::vector<double> &/*x*/,
  const std::vector<double> &c,
  const std::vector<double*> &p
) {
  /** Calculate polarisation factor for given chirality (handled by 
      coeffficients) and polarisations (handled by parameters).
      Coefficients: c[0] - electron chirality (-1 = L , +1 = R)
                    c[1] - positron chirality (-1 = L , +1 = R)
      Parameters: p[0] - electron beam polarisation
                  p[1] - positron beam polarisation
  **/
  
  return 0.25 * ( 1 + c[0] * (*(p[0])) ) * ( 1 + c[1] * (*(p[1])) );
}

}
}