#ifndef LIBRARY_CPPHELPROOT_H
#define LIBRARY_CPPHELPROOT_H 1

#include <CppUtils/Vec.h>
#include "TMatrixT.h"

namespace PREW {
namespace CppUtils {
  
namespace Root {
  template<class Type> 
  Vec::Matrix2D<Type> matrix2D_from_TMatrixT(const TMatrixT <Type>& matrix);
}
  
}
}

#include <CppUtils/Root.tpp>

#endif