#ifndef LIB_CPPHELPNUM_H
#define LIB_CPPHELPNUM_H 1

// Standard library
#include <vector>

namespace PrEW {
namespace CppUtils {

namespace Num {
template <class T>
bool equal_to_eps(const T &v1, const T &v2, const T eps = T(1e-9));
template <class T>
bool equal_to_eps(const std::vector<T> &v1, const std::vector<T> &v2,
                  const T eps = T(1e-9));

double log_factorial(int x);

template <class T>
void csum(T &a,T b,T &c);
//  { //this function adds a and b, and passes c as a compensation term
//     double y = b-c; //y is the correction of b argument
//     b = a+y; //add corrected b argument to a argument. The output of the current summation
//     c = (b-a)-y; //find new error to be passed as a compensation term
//     a = b;
// }
} // namespace Num

} // namespace CppUtils
} // namespace PrEW

#include <CppUtils/Num.tpp>

#endif
