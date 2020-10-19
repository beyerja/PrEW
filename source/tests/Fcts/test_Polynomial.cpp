#include <CppUtils/Num.h>
#include <Data/BinCoord.h>
#include <Fcts/Polynomial.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;
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

  std::vector<double> x_vals {-1., 0.0, 1.0, 1.5};
  std::vector<double> pred {-0.5 , 1.0, 3.5, 5.125};
  
  for (size_t b=0; b<x_vals.size(); b++) {
    BinCoord x ({x_vals[b]},{x_vals[b]},{x_vals[b]});
    ASSERT_EQ( 
      Num::equal_to_eps( Polynomial::quadratic_1D( {x},c,p_ptrs), pred[b], 1e-9), 
      true 
    ) << "Expected " << pred[b] 
    << " got " << Polynomial::quadratic_1D(x,c,p_ptrs);
  }
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