#ifndef LIB_PARAMETRISATIONFCT_H
#define LIB_PARAMETRISATIONFCT_H 1

#include <Data/BinCoord.h>

// Standard library
#include <functional>
#include <vector>

namespace PrEW {
namespace Fcts {
// Function which has three input parameter vectors,
// In PrEW like: coordinates,parameters,coefficients
using ParametrisationFct = std::function<double(const Data::BinCoord &,
                                                const std::vector<double> &,
                                                const std::vector<double *> &)>;
} // namespace Fcts
} // namespace PrEW

#endif