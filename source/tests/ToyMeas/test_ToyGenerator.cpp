#include <Connect/DataConnector.h>
#include <CppUtils/Num.h>
#include <CppUtils/Vec.h>
#include <Data/CoefDistr.h>
#include <Data/PredDistr.h>
#include <Data/PredLink.h>
#include <Data/PolLink.h>
#include <Fit/FitPar.h>
#include <GlobalVar/Chiral.h>
#include <ToyMeas/ToyGenerator.h>

#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

using namespace PREW::Connect;
using namespace PREW::CppUtils;
using namespace PREW::Data;
using namespace PREW::Fit;
using namespace PREW::GlobalVar;
using namespace PREW::ToyMeas;

//------------------------------------------------------------------------------
// Check that statistical functions are correctly implemented

TEST(TestToyGenerator, SimpleConstructor) {
  /** Copy-paste of DataConnector test with slightly varied values,
      tests that bin predicitions are in the end properly transformed into bin
      content of a faked differential distribution.
  **/
  DistrInfo info_pol {"test", "e-p+", 500};
  DistrInfo info_LR {"test", Chiral::eLpR, 500};
  DistrInfo info_RL {"test", Chiral::eRpL, 500};
  DistrInfo info_LL {"test", Chiral::eLpL, 500};
  DistrInfo info_RR {"test", Chiral::eRpR, 500};
  Vec::Matrix2D<double> bin_centers = {{0}, {1}};
  PredDistrVec pred_distrs { 
    { info_LR, bin_centers, {1}, {0} },
    { info_RL, bin_centers, {1}, {0} },
    { info_LL, bin_centers, {1}, {0} },
    { info_RR, bin_centers, {1}, {0} },
  };
  ParVec input_pars { 
    {"A_pol", 2, 0},
    {"A_LR", 1, 0},
    {"mu", 0, 0},
    {"sigma", 0.5, 0},
    {"ePol", -0.60, 0},
    {"pPol", +0.20, 0}
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
    {500, { {"e-p+", {"ePol", "pPol"}} }}
  };
  
  DataConnector connector {pred_distrs,coef_distrs,pred_links,pol_links};
  
  ToyGenerator test_gen = ToyGenerator( connector, input_pars );
                
  // Do I get correctly sized distributions?
  auto expected_distrs = test_gen.get_expected_distrs(500);
  ASSERT_EQ(expected_distrs.size(), 1);
  ASSERT_EQ(expected_distrs[0].m_distribution.size(), 2);
  
  // Is the distribution content accurate?
  double bin0_content = expected_distrs[0].m_distribution[0].get_val_mst();
  ASSERT_EQ( Num::equal_to_eps(bin0_content, 1.44095492471, 1e-9), true )
    << "Expected " << 1.44095492471 << " got " << bin0_content;
}

//------------------------------------------------------------------------------