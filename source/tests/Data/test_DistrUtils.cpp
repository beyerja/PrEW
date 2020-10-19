#include <CppUtils/Num.h>
#include <CppUtils/Vec.h>
#include <Data/BinCoord.h>
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

TEST(TestDistrUtils, SubVecInfo) {
  // Test if distributions are correctly extracted by their info
  DistrInfo info1 {"D1","LR",200};
  DistrInfo info2 {"D2","RL",400};
  
  DiffDistrVec vec {
    {info1, {}, {}},
    {info1, {}, {}},
    {info1, {}, {}},
    {info2, {}, {}},
    {info2, {}, {}},
  };
  
  // Should extract exactly 3 distributions named "d1" with energy 200
  ASSERT_EQ(DistrUtils::subvec_info(vec,info1).size(), 3);
}

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
  CoordVec coords{
      BinCoord({0.0, -10.0, 0.5}, {-0.5, -15.0, 0.1}, {1.5, 0.0, 0.9}),
      BinCoord({1.0, 20.0, 0.5}, {1.5, 0.0, 0.1}, {2.5, 40.0, 0.9}),
      BinCoord({2.0, -20.0, 0.5}, {2.5, -30.0, 0.1}, {3.5, -15, 0.9})};

  BinCoord middle_expected({1.0, 0.0, 0.5}, {-0.5, -30.0, 0.1},
                           {3.5, 40.0, 0.9});
  auto middle_found = DistrUtils::bin_middle(coords);
  
  ASSERT_EQ(middle_expected, middle_found);
}

//------------------------------------------------------------------------------

TEST(TestDistrUtils, CombineDiffDistr) {
  DiffDistr distr {
    {"DistrName", "PolConfigName", 1000},
    { BinCoord({0.0, 25.0}, {-0.5,20.0}, {0.5,30.0}), 
      BinCoord({-1.0, 15.0}, {-1.5,10.0}, {-0.5,20.0}) }, // Bin centers
    { {7.0, 3.0},  {10.0, 4.0} }  // (Value, Uncertainty) pairs
  };
  
  // Expected results
  BinCoord bin_middle ({-0.5, 20.0}, {-1.5,10.0}, {0.5,30});
  double val_sum = 17.0;
  double unc_rms = 5.0;
  
  // Combined distribution
  auto comb_distr = DistrUtils::combine_bins(distr);
  auto coords = comb_distr.m_coords[0];
  double comb_val = comb_distr.m_distribution[0].get_val_mst();
  double comb_unc = comb_distr.m_distribution[0].get_val_unc();
  
  EXPECT_EQ( comb_distr.m_coords.size(), 1 );
  EXPECT_EQ( comb_distr.m_coords[0], bin_middle );
  
  EXPECT_TRUE( Num::equal_to_eps( val_sum, comb_val ) ) 
    << "Expected " << val_sum << " got " << comb_val;
  
  EXPECT_TRUE( Num::equal_to_eps( unc_rms, comb_unc ) ) 
    << "Expected " << unc_rms << " got " << comb_unc;
}

TEST(TestDistrUtils, CombinePredDistr) {
  PredDistr distr {
    {"DistrName", "PolConfigName", 1000},
    { BinCoord({0.0, 25.0}, {-0.5,20.0}, {0.5,30.0}), 
      BinCoord({-1.0, 15.0}, {-1.5,10.0}, {-0.5,20.0}) }, // Bin centers
    { 2.0, 4.0 }, // Signal values
    { 0.5, 1.5 } // Background values
  };
  
  // Expected results
  BinCoord bin_middle ({-0.5, 20.0}, {-1.5,10.0}, {0.5,30});
  double sig_sum = 6.0;
  double bkg_sum = 2.0;
  
  // Combined distribution
  auto comb_distr = DistrUtils::combine_bins(distr);
  auto coords = comb_distr.m_coords[0];
  double comb_sig = comb_distr.m_sig_distr[0];
  double comb_bkg = comb_distr.m_bkg_distr[0];
  
  EXPECT_EQ( comb_distr.m_coords.size(), 1 );
  EXPECT_EQ( comb_distr.m_coords[0], bin_middle );
  
  ASSERT_EQ( Num::equal_to_eps( sig_sum, comb_sig ), true ) 
    << "Expected " << sig_sum << " got " << comb_sig;
  
  ASSERT_EQ( Num::equal_to_eps( bkg_sum, comb_bkg ), true  ) 
    << "Expected " << bkg_sum << " got " << comb_bkg;
  
  // ****
  // Exact same test for a vector of distribtions
  PredDistrVec distrs {distr, distr};
  auto comb_distrs = DistrUtils::combine_bins(distrs);
  for (const auto &_comb_distr: comb_distrs) {
    auto _coords = _comb_distr.m_coords[0];
    double _comb_sig = _comb_distr.m_sig_distr[0];
    double _comb_bkg = _comb_distr.m_bkg_distr[0];
    
    EXPECT_EQ( comb_distr.m_coords.size(), 1 );
    EXPECT_EQ( comb_distr.m_coords[0], bin_middle );
    EXPECT_EQ( Num::equal_to_eps( sig_sum, _comb_sig ), true ) 
      << "Expected " << sig_sum << " got " << _comb_sig;
    EXPECT_EQ( Num::equal_to_eps( bkg_sum, _comb_bkg ), true  ) 
      << "Expected " << bkg_sum << " got " << _comb_bkg;
  }

}

//------------------------------------------------------------------------------