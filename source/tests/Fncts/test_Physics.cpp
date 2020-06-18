#include <CppUtils/Num.h>
#include <Fcts/Physics.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------
// Check that polynomial functions are correctly implemented

TEST(TestPhysics, PolarisationFactor) {
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
    Num::equal_to_eps( Physics::polarisation_factor({},c,p_ptrs), 0.08, 1e-9), 
    true 
  ) << "Expected " << 0.08 
    << " got " << Physics::polarisation_factor({},c,p_ptrs);
}

TEST(TestPhysics, LuminosityFraction) {
  // Test the luminosity fraction function.
  std::vector<double> c { 0.25 };
  std::vector<double> p_vals { 2000 };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  ASSERT_EQ( 
    Num::equal_to_eps( Physics::luminosity_fraction({},c,p_ptrs), 500.0, 1e-9), 
    true 
  ) << "Expected " << 500.0
    << " got " << Physics::luminosity_fraction({},c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmetryFactors2Allowed) {
  // Test the factors on the chiral cross section describing a shift of the 
  // asymmetry when two chiral cross sections are allowed.
  std::vector<double> c { // Random test cross section values
    145.7, // E.g. LR
    63.4   // E.g. RL
  }; 
  std::vector<double> p_vals { 0.1 }; // Asymmetry shift
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }

  // Test function against values I got from hand-calculator
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_2chixs_a0({},c,p_ptrs), 1.071757035, 1e-9), 
    true 
  ) << "Expected " << 1.071757035 
    << " got " << Physics::asymm_2chixs_a0({},c,p_ptrs);
    
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_2chixs_a1({},c,p_ptrs), 0.8350946372, 1e-9), 
    true 
  ) << "Expected " << 0.8350946372 
    << " got " << Physics::asymm_2chixs_a1({},c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmetryFactors3Allowed) {
  // Test the factors on the chiral cross section describing a shift of the 
  // asymmetry when three chiral cross sections are allowed.
  double AI_sm  = 0.2;    // Random values for the asymmetries for this test
  double AII_sm = -0.01;
  double deltaAI = -0.05;
  double deltaAII = 0.1;
  
  // The three functions need different input coefficients and paramaters
  std::vector<double> c0 { AII_sm }; 
  std::vector<double> c1 { AI_sm }; 
  std::vector<double> c2 { AI_sm, AII_sm }; 
  std::vector<double> p0_vals { deltaAII };
  std::vector<double> p1_vals { deltaAI };
  std::vector<double> p2_vals { deltaAI, deltaAII };
  std::vector<double*> p0_ptrs {}, p1_ptrs {}, p2_ptrs {};
  for (double & p: p0_vals) { p0_ptrs.push_back(&p); }
  for (double & p: p1_vals) { p1_ptrs.push_back(&p); }
  for (double & p: p2_vals) { p2_ptrs.push_back(&p); }
  
  // Expected results
  double res_0 = 1.202020202020202;
  double res_1 = 1.125;
  double res_2 = -0.4285714285714288;
  
  // Test function against values I got from Python
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_3chixs_a0({},c0,p0_ptrs), res_0, 1e-9), 
    true 
  ) << "Expected " << res_0 
  << " got " << Physics::asymm_3chixs_a0({},c0,p0_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_3chixs_a1({},c1,p1_ptrs), res_1, 1e-9), 
    true 
  ) << "Expected " << res_1 
  << " got " << Physics::asymm_3chixs_a1({},c1,p1_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_3chixs_a2({},c2,p2_ptrs), res_2, 1e-9), 
    true 
  ) << "Expected " << res_2 
  << " got " << Physics::asymm_3chixs_a2({},c2,p2_ptrs);
  
}

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmetryFactors4Allowed) {
  // Test the factors on the chiral cross section describing a shift of the 
  // asymmetry when all four chiral cross sections are allowed.
  std::vector<double> c { // Random test cross section values
    5938.7, // E.g. LR
    1233.4, // E.g. RL
    3.42,   // E.g. LL
    73.4    // E.g. RR
  }; 
  std::vector<double> p_vals { // Asymmetry shift
    0.1,
    -0.04,
    0.07
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  // Test function against values I got from hand-calculator
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a0({},c,p_ptrs), 1.039670281, 1e-9), 
    true 
  ) << "Expected " << 1.039670281 
  << " got " << Physics::asymm_4chixs_a0({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a1({},c,p_ptrs), 0.6914477866,1e-9), 
    true 
  ) << "Expected " << 0.6914477866 
  << " got " << Physics::asymm_4chixs_a1({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a2({},c,p_ptrs), 38.0924269, 1e-9), 
    true 
  ) << "Expected " << 38.0924269 
  << " got " << Physics::asymm_4chixs_a2({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a3({},c,p_ptrs), 1.24689782, 1e-9), 
    true 
  ) << "Expected " << 1.24689782 
  << " got " << Physics::asymm_4chixs_a3({},c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmFactors_Af_2f) {
  // Test the factors introduced by the 2-fermion final state asymmetry
  std::vector<double> x {0.3}; // Test value for cos(theta)
  std::vector<double> c {
    20.5, // Total SM chiral cross section
    0.5,  // SM chiral cross section in this cos(theta) bin
    0     // Index of the cos(theta) coordinate in coordinate array
  }; 
  std::vector<double> p_vals {
    0.02 // DeltaAf value
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  // expected results (Calculated with Python)
  double res_LR = 1.1845;
  double res_RL = 0.8155;
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_Af_2f_LR(x,c,p_ptrs), res_LR), true
  ) << "Expected " << res_LR 
    << " got " << Physics::asymm_Af_2f_LR(x,c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_Af_2f_RL(x,c,p_ptrs), res_RL), true
  ) << "Expected " << res_RL 
    << " got " << Physics::asymm_Af_2f_RL(x,c,p_ptrs);
}

//------------------------------------------------------------------------------
