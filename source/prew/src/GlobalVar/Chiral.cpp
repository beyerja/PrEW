#include <GlobalVar/Chiral.h>

// Standard library
#include <stdexcept>

namespace PrEW {
namespace GlobalVar {

//------------------------------------------------------------------------------

std::string Chiral::transform(int eM_chirality, int eP_chirality) {
  /** Transform a given e-e+ chirality combination into he corresponding string.
   **/
  std::string output{};
  if ((eM_chirality == -1) && (eP_chirality == -1)) {
    output = eLpL;
  } else if ((eM_chirality == -1) && (eP_chirality == +1)) {
    output = eLpR;
  } else if ((eM_chirality == +1) && (eP_chirality == -1)) {
    output = eRpL;
  } else if ((eM_chirality == +1) && (eP_chirality == +1)) {
    output = eRpR;
  } else {
    throw std::invalid_argument(
        "Asking for invalid chirality string: " + std::to_string(eM_chirality) +
        " " + std::to_string(eP_chirality));
  }
  return output;
}

//------------------------------------------------------------------------------

} // namespace GlobalVar
} // namespace PrEW