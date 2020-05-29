#include <CppUtils/Num.h>
#include <CppUtils/Vec.h>
#include <Data/DistrInfo.h>
#include <Data/DiffDistr.h>
#include <Data/DistrUtils.h>

#include <gtest/gtest.h>

#include <cmath>
#include <string>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;

//------------------------------------------------------------------------------
// Tests for distribution helper functions

TEST(TestDistrUtils, SubVecEnergyName) {
  // Test if distributions are correctly extracted by their name and energy
  std::string good_name = "d1";
  DistrInfo info_good1 {good_name,"LR",200};
  DistrInfo info_good2 {good_name,"RL",200};
  DistrInfo info_bad1 {"d2","LR",200};
  DistrInfo info_bad2 {good_name,"LR",400};
  
  DiffDistrVec vec {
    {info_good1, {}, {}},
    {info_good1, {}, {}},
    {info_good2, {}, {}},
    {info_bad1, {}, {}},
    {info_bad2, {}, {}},
  };
  
  // Should extract exactly 3 distributions named "d1" with energy 200
  ASSERT_EQ(DistrUtils::subvec_energy_and_name(vec,200,good_name).size(), 3);
}

TEST(TestDistrUtils, SubVecEnergy) {
  // Test if distributions are correctly extracted by energy
  DistrInfo info_good1 {"d1","LR",200};
  DistrInfo info_good2 {"d2","RL",200};
  DistrInfo info_bad1 {"d1","LR",400};
  
  DiffDistrVec vec {
    {info_good1, {}, {}},
    {info_good2, {}, {}},
    {info_bad1, {}, {}},
  };
  
  // Should extract exactly 2 distributions with energy 200
  ASSERT_EQ(DistrUtils::subvec_energy(vec,200).size(), 2);
}

TEST(TestDistrUtils, SubVecPol) {
  // Test if distributions are correctly extracted by polarisation
  DistrInfo info_good1 {"d1","LR",200};
  DistrInfo info_good2 {"d2","LR",400};
  DistrInfo info_bad1 {"d1","RL",200};
  
  DiffDistrVec vec {
    {info_good1, {}, {}},
    {info_good2, {}, {}},
    {info_bad1, {}, {}},
  };
  
  // Should extract exactly 2 distributions with polarisation "LR"
  std::string good_pol = "LR";
  ASSERT_EQ(DistrUtils::subvec_pol(vec,good_pol).size(), 2);
}

TEST(TestDistrUtils, ElementPol) {
  // Test if element with given polarisation is correctly extracted
  std::string pol_good = "LR";
  std::string pol_bad = "RL";
  DistrInfo info_good {"d1",pol_good,200};
  DistrInfo info_bad {"d1",pol_bad,200};
  
  DiffDistrVec vec {
    {info_good, {}, {}},
    {info_bad, {}, {}},
    {info_bad, {}, {}},
  };
  
  ASSERT_EQ(DistrUtils::element_pol(vec,pol_good).m_info,info_good);
}

//------------------------------------------------------------------------------

TEST(TestDistrUtils, FindBinMiddle) {
  // Find middle of all bins
  Vec::Matrix2D<double> bin_centers {
    {0.0, -10.0, 0.5, 0.002},
    {1.0,  20.0, 0.5, 0.001},
    {2.0, -20.0, 0.5, 0.002}
  };
  
  std::vector<double> middle_expected { 1.0, 0.0, 0.5, 0.0015 };
  std::vector<double> middle_found = DistrUtils::bin_middle(bin_centers);
  
  for (size_t d=0; d<middle_expected.size(); d++) {
    ASSERT_EQ( Num::equal_to_eps( middle_expected[d], middle_found[d]), true )
      << "Expected " << middle_expected[d] << " got " << middle_found[d];
  } 
}

//------------------------------------------------------------------------------

TEST(TestDistrUtils, CombineDiffDistr) {
  DiffDistr distr {
    {"DistrName", "PolConfigName", 1000},
    { {0.0, 25.0}, {-1.0, 15.0}, {3.0, 5.0} }, // Bin centers
    { {7.0, 9.0},  {10.0, 6.0},  {8.0, 2.0} }  // (Value, Uncertainty) pairs
  };
  
  // Expected results
  std::vector<double> bin_middle = {1.0, 15.0};
  double val_sum = 25.0;
  double unc_rms = 11.0;
  
  // Combined distribution
  auto comb_distr = DistrUtils::combine_bins(distr);
  auto bin_centers = comb_distr.m_bin_centers[0];
  double comb_val = comb_distr.m_distribution[0].get_val_mst();
  double comb_unc = comb_distr.m_distribution[0].get_val_unc();
  
  for (size_t d=0; d<bin_middle.size(); d++) {
    EXPECT_EQ( Num::equal_to_eps( bin_middle[d], bin_centers[d] ), true  ) 
    << "Expected " << bin_middle[d] << " got " << bin_centers[d];
  } 
  
  EXPECT_EQ( Num::equal_to_eps( val_sum, comb_val ), true ) 
    << "Expected " << val_sum << " got " << comb_val;
  
  EXPECT_EQ( Num::equal_to_eps( unc_rms, comb_unc ), true  ) 
    << "Expected " << unc_rms << " got " << comb_unc;
}

TEST(TestDistrUtils, CombinePredDistr) {
  PredDistr distr {
    {"DistrName", "PolConfigName", 1000},
    { {0.0, 25.0}, {-1.0, 15.0}, {3.0, 5.0} }, // Bin centers
    { 2.0, 4.0, 205.0 }, // Signal values
    { 0.5, 1.5,  90.0 } // Background values
  };
  
  // Expected results
  std::vector<double> bin_middle = {1.0, 15.0};
  double sig_sum = 211.0;
  double bkg_sum = 92.0;
  
  // Combined distribution
  auto comb_distr = DistrUtils::combine_bins(distr);
  auto bin_centers = comb_distr.m_bin_centers[0];
  double comb_sig = comb_distr.m_sig_distr[0];
  double comb_bkg = comb_distr.m_bkg_distr[0];
  
  for (size_t d=0; d<bin_middle.size(); d++) {
    EXPECT_EQ( Num::equal_to_eps( bin_middle[d], bin_centers[d] ), true  ) 
    << "Expected " << bin_middle[d] << " got " << bin_centers[d];
  } 
  
  EXPECT_EQ( Num::equal_to_eps( sig_sum, comb_sig ), true ) 
    << "Expected " << sig_sum << " got " << comb_sig;
  
  EXPECT_EQ( Num::equal_to_eps( bkg_sum, comb_bkg ), true  ) 
    << "Expected " << bkg_sum << " got " << comb_bkg;
}

//------------------------------------------------------------------------------