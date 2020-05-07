#ifndef LIB_CPPHELPROOT_TPP
#define LIB_CPPHELPROOT_TPP 1

#include <CppUtils/Root.h>

namespace PrEW {
namespace CppUtils {

//------------------------------------------------------------------------------

template<class Type> 
Vec::Matrix2D<Type> Root::matrix2D_from_TMatrixT(const TMatrixT <Type>& matrix) {
  /** Take a TMatrixT and convert it to vector<vector<>> structure.
  **/
  Vec::Matrix2D<Type> output(matrix.GetNrows(), std::vector<Type>(matrix.GetNcols()));
  for(int row = 0; row < matrix.GetNrows(); row++){
    for(int col = 0; col < matrix.GetNcols(); col++){
      output[row][col] = matrix[row][col];
    }
  }
  return output;
}

//------------------------------------------------------------------------------

}
}

#endif