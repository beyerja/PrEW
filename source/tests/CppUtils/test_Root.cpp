#include <gtest/gtest.h>

#include <CppUtils/Num.h>
#include <CppUtils/Root.h>
#include <CppUtils/Vec.h>

using namespace PrEW::CppUtils;

//------------------------------------------------------------------------------
// Tests for equal_to_eps

TEST(TestRoot, TMatrixTConversion) {
  // Test function that transforms a TMatrixT into a vector of vectors
  const double data [6] = { 0.0, 0.1, 0.2, 1.0, 1.1, 1.2 };
  TMatrixT<double> root_mtx (2, 3, data);
  Vec::Matrix2D<double> vec_mtx = Root::matrix2D_from_TMatrixT( root_mtx );
  
   // Comparinsons allow small unequality due to numerics of doubles
  ASSERT_EQ( Num::equal_to_eps(vec_mtx[1][2], 1.2), true );
  ASSERT_EQ( Num::equal_to_eps(root_mtx[1][2], vec_mtx[1][2]), true );
  ASSERT_EQ( Num::equal_to_eps(root_mtx[0][1], vec_mtx[0][1]), true );
}

//------------------------------------------------------------------------------
