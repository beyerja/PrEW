#ifndef LIBRARY_CHIRAL_H
#define LIBRARY_CHIRAL_H 1

#include <string>

namespace PREW {
namespace GlobalVar {
/** Careful with this namespace, only for static const variables!
**/
    
  namespace Chiral {
    /** Common strings to mark generator level chiral distributions.
    **/
    
    static const std::string eLpR = "PrEW-internal-GenLevel-ElectronL-PositronR";
    static const std::string eRpL = "PrEW-internal-GenLevel-ElectronR-PositronL";
    static const std::string eLpL = "PrEW-internal-GenLevel-ElectronL-PositronL";
    static const std::string eRpR = "PrEW-internal-GenLevel-ElectronR-PositronR";
  }
    
}
}

#endif