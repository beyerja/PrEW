#include <CppUtils/Num.h>
#include <Fncts/Polynomial.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PREW::CppUtils;
using namespace PREW::Fncts;

//------------------------------------------------------------------------------
// Check that polynomial functions are correctly implemented

TEST(TestPolynomial, Gaussian1D) {
  std::vector<double> c {};
  std::vector<double> p_vals {  1.0, // Offset
                                2.0, // Linear parameter
                                0.5  // Quadratic parameter
                              };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }

  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::quadratic_1D( {-1.} ,c,p_ptrs), -0.5, 1e-9), 
    true 
  ) << "Expected " << -0.5 
    << " got " << Polynomial::quadratic_1D({-1.},c,p_ptrs);
  ASSERT_EQ(
    Num::equal_to_eps( Polynomial::quadratic_1D( {0.0} ,c,p_ptrs), 1.0, 1e-9),
    true
  ) << "Expected " << 1.0 
    << " got " << Polynomial::quadratic_1D({0.0},c,p_ptrs);
  ASSERT_EQ(
    Num::equal_to_eps( Polynomial::quadratic_1D( {1.0} ,c,p_ptrs), 3.5, 1e-9),
    true
  ) << "Expected " << 3.5 
    << " got " << Polynomial::quadratic_1D({1.0},c,p_ptrs);
  ASSERT_EQ(
    Num::equal_to_eps( Polynomial::quadratic_1D( {1.5} ,c,p_ptrs), 5.125, 1e-9),
    true
  ) << "Expected " << 5.125 
    << " got " << Polynomial::quadratic_1D({1.5},c,p_ptrs);
}

//------------------------------------------------------------------------------