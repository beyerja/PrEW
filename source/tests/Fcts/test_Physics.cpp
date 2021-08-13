#include <CppUtils/Num.h>
#include <Data/BinCoord.h>
#include <Fcts/Physics.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmetryFactors2Allowed) {
  // Test the factors on the chiral cross section describing a shift of the 
  // asymmetry when two chiral cross sections are allowed.
  double sigma_LR = 4699.82;
  double sigma_RL = 43.50;
  double sigma_0 = (sigma_LR + sigma_RL);
  double A_LR = (sigma_LR - sigma_RL) / sigma_0;
  double d_A_LR = -0.1;
  
  std::vector<double> c { sigma_LR, sigma_RL }; 
  std::vector<double> p_vals { -0.1 }; // Asymmetry shift
  std::vector<double*> p_ptrs { &d_A_LR };
  // for (double & p: p_vals) { p_ptrs.push_back(&p); }

  double factor_LR = Physics::asymm_2chixs_a0({},c,p_ptrs);
  double factor_RL = Physics::asymm_2chixs_a1({},c,p_ptrs);
  
  double sigma_LR_new = sigma_LR * factor_LR;
  double sigma_RL_new = sigma_RL * factor_RL;
  double sigma_0_new = sigma_LR_new + sigma_RL_new;
  double A_LR_new = (sigma_LR_new - sigma_RL_new) / sigma_0_new;
  
  ASSERT_TRUE(Num::equal_to_eps(A_LR + d_A_LR, A_LR_new))
      << "Expected " << A_LR + d_A_LR << " got " << A_LR_new;
  ASSERT_TRUE(Num::equal_to_eps(sigma_0, sigma_0_new))
      << "Expected " << sigma_0 << " got " << sigma_0_new;
}

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmetryFactors3Allowed) {
  // Test the factors on the chiral cross section describing a shift of the 
  // asymmetry when three chiral cross sections are allowed.
  
  // Random test cross section values and asymmetry shifts
  std::vector<double> c {
    2400.1, // E.g. LR
    570.4, // E.g. RL
    5.7   // E.g. LL
  }; 
  std::vector<double> p_vals { 
    -0.05, // Shift in asymmetry AI
    0.1    // Shift in asymmetry AII`
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  // Expected results
  double res_0 = 1.1240031665347276;
  double res_1 = 1.2608870967741936;
  double res_2 = -77.321052631578978;
  
  // Test function against values I got from Python
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_3chixs_a0({},c,p_ptrs), res_0, 1e-9), 
    true 
  ) << "Expected " << res_0 
  << " got " << Physics::asymm_3chixs_a0({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_3chixs_a1({},c,p_ptrs), res_1, 1e-9), 
    true 
  ) << "Expected " << res_1 
  << " got " << Physics::asymm_3chixs_a1({},c,p_ptrs);
  
  ASSERT_EQ(
    Num::equal_to_eps(Physics::asymm_3chixs_a2({},c,p_ptrs), res_2, 1e-9), 
    true 
  ) << "Expected " << res_2 
  << " got " << Physics::asymm_3chixs_a2({},c,p_ptrs);
  
}

//------------------------------------------------------------------------------

TEST(TestPhysics, AsymmFactors_Af_2f) {
  // Test the factors introduced by the 2-fermion final state asymmetry
  BinCoord x {{0.3},{0.3},{0.3}}; // Test value for cos(theta)
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
  
  ASSERT_TRUE(Num::equal_to_eps(Physics::asymm_Af_2f_LR({x},c,p_ptrs), res_LR))
    << "Expected " << res_LR << " got " << Physics::asymm_Af_2f_LR(x,c,p_ptrs);
  
  ASSERT_TRUE(Num::equal_to_eps(Physics::asymm_Af_2f_RL({x},c,p_ptrs), res_RL))
    << "Expected " << res_RL << " got " << Physics::asymm_Af_2f_RL(x,c,p_ptrs);
}

//------------------------------------------------------------------------------

TEST(TestPhysics, General2fParam) {
  // Test the general 2f parametrisation
  BinCoord x {{0.45}, {0.4}, {0.5}};
  std::vector<double> c {2.5e4,2.7e7,0.3e7,0};
  std::vector<double> p_vals {1.1,0.7,0.3,-0.3,0.1,-0.01};
  double res_LR = 57.04879124999998;
  double res_RL = 6.08689125;
  
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  ASSERT_TRUE(Num::equal_to_eps(Physics::general_2f_param_LR(x,c,p_ptrs), res_LR))
    << "Expected " << res_LR << " got " << Physics::general_2f_param_LR(x,c,p_ptrs);
  
  ASSERT_TRUE(Num::equal_to_eps(Physics::general_2f_param_RL(x,c,p_ptrs), res_RL))
    << "Expected " << res_RL << " got " << Physics::general_2f_param_RL(x,c,p_ptrs);
}

//------------------------------------------------------------------------------
