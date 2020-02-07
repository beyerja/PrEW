#include <Connect/DataConnector.h>
#include <CppUtils/Num.h>
#include <CppUtils/Vec.h>
#include <Data/CoefDistr.h>
#include <Data/DistrInfo.h>
#include <Data/PolLink.h>
#include <Data/PredDistr.h>
#include <Fit/FitBin.h>
#include <Fit/FitContainer.h>
#include <Fit/FitPar.h>
#include <GlobalVar/Chiral.h>

#include <gtest/gtest.h>

using namespace PREW::Connect;
using namespace PREW::CppUtils;
using namespace PREW::Data;
using namespace PREW::Fit;
using namespace PREW::GlobalVar;

//------------------------------------------------------------------------------
// Tests for DataConnector class that links prediction

TEST(TestDataConnector, EmptyConstructor) {
  // DataConnector needs at least polarisation links and predicitions
  ASSERT_THROW(
    DataConnector(),
    std::invalid_argument
  );
}

TEST(TestDataConnector, ReadFunctions) {
  // Simple sanity check that read functions return correct vectors
  DistrInfo info_LR {"test", Chiral::eLpR, 500};
  DistrInfo info_RL {"test", Chiral::eRpL, 500};
  Vec::Matrix2D<double> bin_centers = {{0}, {1}};
  PredDistrVec pred_distrs { 
    { info_LR, bin_centers, {1}, {0} }, 
    { info_LR, bin_centers, {2}, {0} } 
  };
  CoefDistrVec coef_distrs {};
  PredLinkVec  pred_links {
    { info_LR, { {"Gaussian1D", {"A_LR", "mu", "sigma"}} }, {} },
  };
  PolLinkVec   pol_links { {500, { {"e-p+", {"ePol", "pPol"}} }} };
  
  DataConnector connector {pred_distrs,coef_distrs,pred_links,pol_links};

  // Here simple size check
  ASSERT_EQ( connector.get_pred_distrs().size(), 2 );
  ASSERT_EQ( connector.get_coef_distrs().size(), 0 );
  ASSERT_EQ( connector.get_pred_links().size(), 1 );
  ASSERT_EQ( connector.get_pol_links().size(), 1 );
}

TEST(TestDataConnector, DistrFilling) {
  // Test on a simple example that filling of a bin vector works properly
  
  // Create some distributions with predictions to test bin connecting
  DistrInfo info_pol {"test", "e-p+", 500};
  DistrInfo info_LR {"test", Chiral::eLpR, 500};
  DistrInfo info_RL {"test", Chiral::eRpL, 500};
  DistrInfo info_LL {"test", Chiral::eLpL, 500};
  DistrInfo info_RR {"test", Chiral::eRpR, 500};
  Vec::Matrix2D<double> bin_centers = {{0}, {1}};
  DiffDistr diff_distr { info_pol, bin_centers, {{0.8,0.2},{1,0.2}} };
  PredDistrVec pred_distrs { 
    { info_LR, bin_centers, {1}, {0} },
    { info_RL, bin_centers, {1}, {0} },
    { info_LL, bin_centers, {1}, {0} },
    { info_RR, bin_centers, {1}, {0} },
  };
  ParVec pars { 
    {"A_pol", 1, 0},
    {"A_LR", 1, 0},
    {"mu", 0, 0},
    {"sigma", 0.5, 0},
    {"ePol", -0.80, 0},
    {"pPol", +0.30, 0}
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
  
  // Connector has task to connect all of that
  DataConnector connector {pred_distrs,coef_distrs,pred_links,pol_links};
  
  // Test on single distribution
  BinVec bins {};
  connector.fill_bins(diff_distr, &pars, &bins);
  
  double bin0_val = bins[0].get_val_mst();
  double bin0_unc = bins[0].get_val_unc();
  double bin0_pred = bins[0].get_val_prd();
  ASSERT_EQ( Num::equal_to_eps(bin0_val, 0.8, 1e-9), true );
  ASSERT_EQ( Num::equal_to_eps(bin0_unc, 0.2, 1e-9), true );
  ASSERT_EQ( Num::equal_to_eps(bin0_pred, 0.70354465956, 1e-9), true )
    << "Expected " << 0.70354465956 << " got " << bin0_pred;
    
  // Test on distribution vector (=> fills fit container)
  DiffDistrVec distr_vec {diff_distr};
  FitContainer fit_container {};
  connector.fill_fit_container( distr_vec, pars, &fit_container );
  ASSERT_EQ( fit_container.m_fit_pars.size(), 6 );
  ASSERT_EQ( fit_container.m_fit_bins.size(), 2 );
}

//------------------------------------------------------------------------------