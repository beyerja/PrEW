#include <CppUtils/Num.h>
#include <Fcts/Physics.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PREW::CppUtils;
using namespace PREW::Fcts;

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
    Num::equal_to_eps(Physics::asymm_4chixs_a0({},c,p_ptrs), 1.021360921, 1e-9), 
    true 
  ) << "Expected " << 1.021360921 
  << " got " << Physics::asymm_4chixs_a0({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a1({},c,p_ptrs), 0.7796055619,1e-9), 
    true 
  ) << "Expected " << 0.7796055619 
  << " got " << Physics::asymm_4chixs_a1({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a2({},c,p_ptrs), 69.88593567, 1e-7), 
    true 
  ) << "Expected " << 69.88593567 
  << " got " << Physics::asymm_4chixs_a2({},c,p_ptrs);
  
  // Result is negative -> Does not make physical sense, but computational
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_4chixs_a3({},c,p_ptrs), -0.2344891008, 1e-9), 
    true 
  ) << "Expected " << -0.2344891008 
  << " got " << Physics::asymm_4chixs_a3({},c,p_ptrs);
}

//------------------------------------------------------------------------------
