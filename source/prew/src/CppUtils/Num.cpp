#include <CppUtils/Num.h>

#include <stdexcept>

namespace PrEW {
namespace CppUtils {
  
//------------------------------------------------------------------------------

double Num::log_factorial (int x) {
  /** Explicit implementation of the natural logarithm of the factorial of an 
      integer x:
        ln x!
      Uses the values the std::lgamma would give, but std::lgamma is not 
      thread-safe.
      Only defines the values up to 30 (for now).
  **/
  switch (x) {
    case 0: return 0.000000;
    case 1: return 0.000000;
    case 2: return 0.693147;
    case 3: return 1.791759;
    case 4: return 3.178054;
    case 5: return 4.787492;
    case 6: return 6.579251;
    case 7: return 8.525161;
    case 8: return 10.604603;
    case 9: return 12.801827;
    case 10: return 15.104413;
    case 11: return 17.502308;
    case 12: return 19.987214;
    case 13: return 22.552164;
    case 14: return 25.191221;
    case 15: return 27.899271;
    case 16: return 30.671860;
    case 17: return 33.505073;
    case 18: return 36.395445;
    case 19: return 39.339884;
    case 20: return 42.335616;
    case 21: return 45.380139;
    case 22: return 48.471181;
    case 23: return 51.606676;
    case 24: return 54.784729;
    case 25: return 58.003605;
    case 26: return 61.261702;
    case 27: return 64.557539;
    case 28: return 67.889743;
    case 29: return 71.257039;
    case 30: return 74.658236;
  }
  throw std::out_of_range("Num::log_factorial only accepts integers 0-30.");
  return -1; // Should never be reached
}

//------------------------------------------------------------------------------
  
} // Namespace CppUtils
} // Namespace PrEW