#include <Connect/DataConnector.h>
#include <CppUtils/Num.h>
#include <CppUtils/Vec.h>
#include <Data/CoefDistr.h>
#include <Data/PredDistr.h>
#include <Data/PredLink.h>
#include <Data/PolLink.h>
#include <Fit/FitPar.h>
#include <GlobalVar/Chiral.h>
#include <ToyMeas/ToyGen.h>

#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

using namespace PrEW::Connect;
using namespace PrEW::CppUtils;
using namespace PrEW::Data;
using namespace PrEW::Fit;
using namespace PrEW::GlobalVar;
using namespace PrEW::ToyMeas;

//------------------------------------------------------------------------------
// Check that statistical functions are correctly implemented

TEST(TestToyGen, SimpleConstructor) {
  /** Copy-paste of DataConnector test with slightly varied values.
      Test all functionalities of toy generator on very simple use case.
  **/
  DistrInfo info_pol {"test", "e-p+", 500};
  DistrInfo info_LR {"test", Chiral::eLpR, 500};
  DistrInfo info_RL {"test", Chiral::eRpL, 500};
  DistrInfo info_LL {"test", Chiral::eLpL, 500};
  DistrInfo info_RR {"test", Chiral::eRpR, 500};
  CoordVec coords = {{{0},{0},{0}}, {{1},{1},{1}}};
  PredDistrVec pred_distrs { 
    { info_LR, coords, {1}, {0} },
    { info_RL, coords, {1}, {0} },
    { info_LL, coords, {1}, {0} },
    { info_RR, coords, {1}, {0} },
  };
  ParVec input_pars { 
    {"A_pol", 2, 0},
    {"A_LR", 1, 0},
    {"mu", 0, 0},
    {"sigma", 0.5, 0},
    {"ePol", 0.60, 0},
    {"pPol", 0.20, 0}
  };
  CoefDistrVec coef_distrs {};
  PredLinkVec  pred_links {
    { info_LR, { {"Gaussian1D", {"A_LR", "mu", "sigma"}} }, {} },
    { info_RL, {}, {} },
    { info_LL, {}, {} },
    { info_RR, {}, {} },
    { info_pol, { {"Gaussian1D", {"A_pol", "mu", "sigma"}} }, {} }
  };
  PolLinkVec   pol_links {
    PolLink(500, "e-p+", "ePol", "pPol", "-", "+")
  };
  
  DataConnector connector {pred_distrs,coef_distrs,pred_links,pol_links};
  
  ToyGen test_gen = ToyGen( connector, input_pars );
                
  // Do I get correctly sized distributions?
  auto expected_distrs = test_gen.get_expected_distrs(500);
  ASSERT_EQ(expected_distrs.size(), 1);
  ASSERT_EQ(expected_distrs[0].m_distribution.size(), 2);
  
  // Is the distribution content accurate?
  double bin0_expected = 1.44095492471;
  double bin0_content = expected_distrs[0].m_distribution[0].get_val_mst();
  ASSERT_EQ( Num::equal_to_eps(bin0_content, bin0_expected, 1e-9), true )
    << "Expected " << bin0_expected << " got " << bin0_content;
    
  //----------------------------------------------------------------------------
  // Do toy fluctuations work?
  auto fluctuated_distrs = test_gen.get_fluctuated_distrs(500);
  ASSERT_EQ(fluctuated_distrs.size(), 1);
  ASSERT_EQ(fluctuated_distrs[0].m_distribution.size(), 2);
  double bin0_fluctuated = fluctuated_distrs[0].m_distribution[0].get_val_mst();
  ASSERT_EQ( Num::equal_to_eps(bin0_fluctuated, bin0_expected, 1e-9), false );
  
  //----------------------------------------------------------------------------
  // Does modification and resetting of parameters work?
  test_gen.modify_par("A_pol", 1.0);
  auto mod_distrs = test_gen.get_expected_distrs(500);
  double bin0_content_mod = mod_distrs[0].m_distribution[0].get_val_mst();
  ASSERT_EQ(Num::equal_to_eps(bin0_content_mod, bin0_expected/2.0, 1e-9), true)
    << "Parameter modification didn't change bin value correctly! "
    << "Expected " << bin0_expected/2.0 << " got " << bin0_content_mod;

  test_gen.reset_par("A_pol");
  auto res_distrs = test_gen.get_expected_distrs(500);
  double bin0_content_res = res_distrs[0].m_distribution[0].get_val_mst();
  ASSERT_EQ( Num::equal_to_eps(bin0_content_res, bin0_expected, 1e-9), true )
    << "Parameter resetting didn't change bin value correctly! "
    << "Expected " << bin0_expected << " got " << bin0_content_res;
  
  // Does modifcation / resetting of unknown parameter cause exception?
  ASSERT_THROW(test_gen.modify_par("wrong", 1.0), std::invalid_argument);
  ASSERT_THROW(test_gen.reset_par("wrong"), std::invalid_argument);
  //----------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
