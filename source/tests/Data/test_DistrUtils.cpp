#include <Data/DistrInfo.h>
#include <Data/DiffDistr.h>
#include <Data/DistrUtils.h>

#include <gtest/gtest.h>

#include <string>

using namespace PREW::Data;

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
