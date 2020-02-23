#include <Connect/LinkHelp.h>
#include <CppUtils/Num.h>
#include <Data/PolLink.h>
#include <Fit/FitPar.h>
#include <GlobalVar/Chiral.h>

#include <gtest/gtest.h>

#include <functional>
#include <string>

using namespace PREW::Connect;
using namespace PREW::CppUtils;
using namespace PREW::Data;
using namespace PREW::Fit;
using namespace PREW::GlobalVar;

//------------------------------------------------------------------------------
// Tests for helper functions for prediction linking

TEST(TestLinkHelp, PolFactorLambdas) {
  // Test function that returns lambda for polarisation factor
  ParVec pars {
    {"ePol", 1.0, 0.0}, // e- Pol.: 100% -> Sign in PolLink (-)
    {"pPol", 1.0, 0.0}, // e+ Pol.: 100% -> Sign in PolLink (+)
  };
  PolLink pol_link {500, "test", "ePol", "pPol", "-", "+"};
  
  auto factor_LR = LinkHelp::get_polfactor_lambda(Chiral::eLpR,pol_link,&pars);
  auto factor_RL = LinkHelp::get_polfactor_lambda(Chiral::eRpL,pol_link,&pars);
  auto factor_LL = LinkHelp::get_polfactor_lambda(Chiral::eLpL,pol_link,&pars);
  auto factor_RR = LinkHelp::get_polfactor_lambda(Chiral::eRpR,pol_link,&pars);
  
  ASSERT_EQ(factor_LR(),1);
  ASSERT_EQ(factor_RL(),0);
  ASSERT_EQ(factor_LL(),0);
  ASSERT_EQ(factor_RR(),0);
  
  // Change polarisations and check that factors changed
  pars[0].m_val_mod = 0.8; // e- Pol.: -80%
  pars[1].m_val_mod = 0.3; // e+ Pol.: +30%
  
  ASSERT_EQ(Num::equal_to_eps(factor_LR(), 0.585, 1e-9), true)
    << "Got " << factor_LR() << " expected " << 0.585;
  ASSERT_EQ(Num::equal_to_eps(factor_RL(), 0.035, 1e-9), true)
    << "Got " << factor_RL() << " expected " << 0.035;
  ASSERT_EQ(Num::equal_to_eps(factor_LL(), 0.315, 1e-9), true)
    << "Got " << factor_LL() << " expected " << 0.315;
  ASSERT_EQ(Num::equal_to_eps(factor_RR(), 0.065, 1e-9), true)
    << "Got " << factor_RR() << " expected " << 0.065;
}

TEST(TestLinkHelp, SigmaModification) {
  /** Test function that returns lambda for value multiplied with output of 
      functions.
  **/
  std::vector<std::function<double()>> alpha_fcts {
    [](){return 2.5;},
    [](){return 3.0;}
  };
  double val = 2.0;

  auto mod_val = LinkHelp::get_modified_sigma(val,alpha_fcts);
  // 2.5 * 3 * 2 = 15
  ASSERT_EQ(Num::equal_to_eps(mod_val(), 15.0, 1e-9), true)
    << "Got " << mod_val() << " expected " << 15.0;
}
//------------------------------------------------------------------------------
