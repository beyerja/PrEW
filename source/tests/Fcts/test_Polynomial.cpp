#include <CppUtils/Num.h>
#include <Fcts/Polynomial.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------
// Check that polynomial functions are correctly implemented
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

TEST(TestPolynomial, ConstantPar) {
  // Test function that simply mirrors the parameter
  std::vector<double> c {};
  std::vector<double> p_vals {1.0};
  std::vector<double*> p_ptrs {&(p_vals[0])};

  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::constant_par({},c,p_ptrs), 1.0, 1e-9), true 
  ) << "Expected " << 1.0 << " got " << Polynomial::constant_par({},c,p_ptrs);
  
  p_vals[0] = -0.5;
  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::constant_par({},c,p_ptrs), -0.5, 1e-9), true 
  ) << "Expected " << -0.5 << " got " << Polynomial::constant_par({},c,p_ptrs);
  
  p_vals[0] = 2000.;
  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::constant_par({},c,p_ptrs), 2000., 1e-9), true 
  ) << "Expected " << 2000. << " got " << Polynomial::constant_par({},c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestPolynomial, ConstantCoef) {
  // Test function that simply mirrors the parameter
  std::vector<double> c {-2.5};

  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::constant_coef({},c,{}), -2.5, 1e-9), true 
  ) << "Expected " << -2.5 << " got " << Polynomial::constant_par({},c,{});
}

//------------------------------------------------------------------------------

TEST(TestPolynomial, Linear3DCoeff) {
  // Test 3D linear function
  std::vector<double> c {1.0, 2.0, -2.0, 2.0};
  std::vector<double> p_vals { 1.0, 3.0, 4.0 };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::linear_3D_coeff({},c,p_ptrs), 5.0, 1e-9), 
    true 
  ) << "Expected " << 5.0 
    << " got " << Polynomial::linear_3D_coeff({},c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestPolynomial, Quadratic1D) {
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

TEST(TestPolynomial, Quadratic3DCoeff) {
  // Test 3D quadratic function that includes mixed terms
  std::vector<double> c {1.0, 2.0, -2.0, 2.0, -0.5, 0.5, -0.5, 1.5, 2, 2.5};
  std::vector<double> p_vals { 1.0, 3.0, 4.0 };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }

  ASSERT_EQ( 
    Num::equal_to_eps( Polynomial::quadratic_3D_coeff({},c,p_ptrs), 43.5, 1e-9), 
    true 
  ) << "Expected " << 43.5 
    << " got " << Polynomial::quadratic_3D_coeff({},c,p_ptrs);
}

//------------------------------------------------------------------------------