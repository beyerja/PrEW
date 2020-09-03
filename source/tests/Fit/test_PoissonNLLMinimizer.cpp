#include <gtest/gtest.h>

#include <CppUtils/Num.h>
#include <Fit/PoissonNLLMinimizer.h>

#include <functional>
#include <limits>
#include <map>
#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Fit;

//------------------------------------------------------------------------------

TEST(TestPoissonNLLMinimizer, TrivialConstructor) {
  FitContainer container {};
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  PoissonNLLMinimizer pnll_minimizer (&container, factory);
  ASSERT_EQ(pnll_minimizer.get_nll(), 0);
  ASSERT_EQ(pnll_minimizer.get_result() == FitResult(), true);
}

//------------------------------------------------------------------------------

TEST(TestPoissonNLLMinimizer, TestAllCases) {
  // Test all six possible (n,mu) cases for a bin
  int n_0 = 0;
  int n_low = 3;
  int n_high = 50;
  
  double pred_neg = -1;
  double pred_low = 2;
  double pred_high = 45;
  
  // Values for mu>0 for comparison calculated in python with scipy.stats
  std::map<std::pair<int,double>,double> n_mu_res_vec = 
  {
    { {n_0, pred_neg},    0.0 },
    { {n_0, pred_low},    4.0 },
    { {n_low, pred_neg},  std::numeric_limits<double>::infinity() },
    { {n_low, pred_low},  3.4246358550964384 },
    { {n_high, pred_neg}, std::numeric_limits<double>::infinity() },
    { {n_high, pred_high},6.20009511173522 }
  };
  
  for (const auto & n_mu_res: n_mu_res_vec) {
    int n = n_mu_res.first.first;
    double mu = n_mu_res.first.second;
    double res = n_mu_res.second;
    
    std::function<double()> prd = [mu]() { return mu; };
    FitBin fb (n, 0, prd); // Measurement = 1 (uncertainty ignored)
    FitContainer container {};
    container.m_fit_bins = {fb};
    MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
    PoissonNLLMinimizer pnll_minimizer (&container, factory);
    
    if ( std::isinf(res) ) {
      EXPECT_EQ( std::isinf(pnll_minimizer.get_nll()), true ) 
        << "Expected infinite result";
    } else {
      EXPECT_EQ(
        Num::equal_to_eps( pnll_minimizer.get_nll(), res, 1e-6 ),
        true
      ) << "Got " << pnll_minimizer.get_nll() << " expected " << res;
    }
    
  }
}

//------------------------------------------------------------------------------

TEST(TestPoissonNLLMinimizer, ManyBinsConstructor) {
  // Test with n FitBin's, range n from small to very large 
  std::function<double()> trivial_prd = []() { return 0.5; }; // Prediction = 0
  std::vector<int> n_bins = {20, 200, 2000, 20000, 200000};
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  
  double one_bin_res = 2.386294361119891;
  
  for ( int n : n_bins ) {
    FitContainer container {};
    container.m_fit_bins = BinVec(n, FitBin(1.0, 1.0, trivial_prd));
    PoissonNLLMinimizer pnll_minimizer (&container, factory);
    EXPECT_EQ(
      Num::equal_to_eps(pnll_minimizer.get_nll(), double(n)*one_bin_res, 1e-5),
      true
    ) << "Got " << pnll_minimizer.get_nll() 
      << " expected " << double(n)*one_bin_res;
  }
}

//------------------------------------------------------------------------------