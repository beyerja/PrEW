#include <gtest/gtest.h>
#include <ChiSqMinimizer.h>

using namespace PREW::Fit;

TEST(TestChiSqMinimizer, TrivialConstructor) {
  FitContainer container {};
  ChiSqMinimizer chi_sq_minimizer (&container);
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 0);
}

TEST(TestChiSqMinimizer, SimpleConstructor) {
  // Test with singular FitBin that should give chisq=1
  std::function<double()> trivial_prd = []() { return 0; }; // Prediction = 0
  FitBin fb (1.0, 1.0, trivial_prd); // Measurement = Unc = 1
  std::vector<FitPar> fit_pars {}; 
  std::vector<FitBin> fit_bins {fb};
  FitContainer container {};
  container.m_fit_pars = fit_pars;
  container.m_fit_bins = fit_bins;
  ChiSqMinimizer chi_sq_minimizer (&container);
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 1);
}

TEST(TestChiSqMinimizer, ManyBinsConstructor) {
  // Test with n FitBin's, range n from small to very large 
  // (testing mainly calc_chisq)
  std::function<double()> trivial_prd = []() { return 0; }; // Prediction = 0
  std::vector<int> n_bins = {20, 200, 2000, 20000, 200000};
  for ( auto n : n_bins ) {
    std::vector<FitPar> fit_pars {}; 
    std::vector<FitBin> fit_bins (n, FitBin(1.0, 1.0, trivial_prd));
    FitContainer container {};
    container.m_fit_pars = fit_pars;
    container.m_fit_bins = fit_bins;
    ChiSqMinimizer chi_sq_minimizer (&container);
    ASSERT_EQ(chi_sq_minimizer.get_chisq(), n);
  }
}

TEST(TestChiSqMinimizer, MultiBinPlusFitParConstructor) {
  // Test with two FitBin's and predicition that uses FitPar
  // => Is change of FitPar value properly reflected in construction?
  FitPar fp ("fp", 0.0);
  FitPar * fp_ptr = &fp;
  std::function<double()> bin_prd = [fp_ptr]() { return 2*fp_ptr->m_val_mod; };
  FitBin fb1 (1.0, 1.0, bin_prd); // Measurement = Unc = 1
  FitBin fb2 (-1.0, 1.0, bin_prd); // Measurement -1, Unc = 1
  std::vector<FitPar> fit_pars {}; 
  std::vector<FitBin> fit_bins {fb1, fb2};
  FitContainer container {};
  container.m_fit_pars = fit_pars;
  container.m_fit_bins = fit_bins;
  
  ChiSqMinimizer chi_sq_minimizer (&container);
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 2);
  
  fp.m_val_mod = 0.5;
  ChiSqMinimizer chi_sq_minimizer_mod (&container);
  // ((1-1)/1)^2 + ((1-(-1))/1)^2 = 4
  ASSERT_EQ(chi_sq_minimizer_mod.get_chisq(), 4);
}