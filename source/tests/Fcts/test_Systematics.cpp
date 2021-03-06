#include <CppUtils/Num.h>
#include <Data/BinCoord.h>
#include <Fcts/Systematics.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------

TEST(TestSystematics, PolarisationFactor) {
  // Test the polarisation factor function.
  std::vector<double> c {
    +1, // e- chirality = R
    -1, // e+ chirality = L
    -1, // e- beam polarisation sign
    +1  // e- beam polarisation sign
  };
  std::vector<double> p_vals {  
    0.2, // e- beam polarisation amplitude
    0.6  // e+ beam polarisation amplitude
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  // (1+(+1)*(-0.2)) * (1+(-1)*(0.6)) / 4.0 = 0.08
  ASSERT_EQ( 
    Num::equal_to_eps( Systematics::polarisation_factor({},c,p_ptrs), 0.08, 1e-9), 
    true 
  ) << "Expected " << 0.08 
    << " got " << Systematics::polarisation_factor({},c,p_ptrs);
}

TEST(TestSystematics, LuminosityFraction) {
  // Test the luminosity fraction function.
  std::vector<double> c { 0.25 };
  std::vector<double> p_vals { 2000 };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  ASSERT_EQ( 
    Num::equal_to_eps( Systematics::luminosity_fraction({},c,p_ptrs), 500.0, 1e-9), 
    true 
  ) << "Expected " << 500.0
    << " got " << Systematics::luminosity_fraction({},c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestSystematics, AcceptanceBox) {
  // Test if box acceptance function works properly
  BinCoord x0 ({-0.6}, {-0.6}, {-0.6}); // Outside acceptance
  BinCoord x1 ({0.1}, {0.1}, {0.1}); // Fully contained in acceptance
  BinCoord x2 ({-0.45}, {-0.45}, {-0.45}); // Contains lower limit
  BinCoord x3 ({0.575}, {0.575}, {0.575}); // Contains upper limit
  std::vector<double> c {
    0,  // Index of relevant coordinate
    0.1 // Bin width
  };
  std::vector<double> p_vals { 
    0.05, // Box center
    1.0,   // box width
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  double pred0 = 0.0;
  double pred1 = 1.0;
  double pred2 = 0.5;
  double pred3 = 0.25;
  
  double res0 = Systematics::acceptance_box({x0},c,p_ptrs);
  double res1 = Systematics::acceptance_box({x1},c,p_ptrs);
  double res2 = Systematics::acceptance_box({x2},c,p_ptrs);
  double res3 = Systematics::acceptance_box({x3},c,p_ptrs);
  
  ASSERT_EQ( Num::equal_to_eps( pred0, res0, 1e-9), true ) 
    << "(0)Expected " << pred0 << " got " << res0;
  ASSERT_EQ( Num::equal_to_eps( pred1, res1, 1e-9), true ) 
    << "(1)Expected " << pred1 << " got " << res1;
  ASSERT_EQ( Num::equal_to_eps( pred2, res2, 1e-9), true ) 
    << "(2)Expected " << pred2 << " got " << res2;
  ASSERT_EQ( Num::equal_to_eps( pred3, res3, 1e-9), true ) 
    << "(3)Expected " << pred3 << " got " << res3;
}

//------------------------------------------------------------------------------

TEST(TestSystematics, AcceptanceBoxPolynomial) {
  // Test box acceptance function that uses 2nd order polynomial
  BinCoord x ({-0.6}, {-0.65}, {-0.55}); // Bin coordinate (has no influence)
  std::vector<double> c {
    0.7,   // constant polynomial term
    10.0,  // linear polyn. term in center deviation
    12.5,  // linear polyn. term in width deviation
    -50.5, // quadratic polyn. term in center deviation
    -30.5, // quadratic polyn. term in width deviation
    -4.5   // mixed polynomial term in width and center deviations
  };
  std::vector<double> p_vals { 
    0.05, // Box center deviation
    -0.01,   // Box width deviation
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  double pred = 0.94795;
  double res = Systematics::acceptance_box_polynomial({x},c,p_ptrs);
  
  ASSERT_TRUE( Num::equal_to_eps( pred, res, 1e-9) ) 
    << "Expected " << pred << " got " << res;
}

//------------------------------------------------------------------------------