#include <gtest/gtest.h>
#include <Fit/ChiSqMinimizer.h>

#include <random>

using namespace PREW::Fit;

TEST(TestChiSqMinimizer, TrivialConstructor) {
  FitContainer container {};
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  ChiSqMinimizer chi_sq_minimizer (&container, factory);
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 0);
  ASSERT_EQ(chi_sq_minimizer.get_result() == FitResult(), true);
}

TEST(TestChiSqMinimizer, SimpleConstructor) {
  // Test with singular FitBin that should give chisq=1
  std::function<double()> trivial_prd = []() { return 0; }; // Prediction = 0
  FitBin fb (1.0, 1.0, trivial_prd); // Measurement = Unc = 1
  ParVec fit_pars {}; 
  BinVec fit_bins {fb};
  FitContainer container {};
  container.m_fit_pars = fit_pars;
  container.m_fit_bins = fit_bins;
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  ChiSqMinimizer chi_sq_minimizer (&container, factory);
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 1);
}

TEST(TestChiSqMinimizer, ManyBinsConstructor) {
  // Test with n FitBin's, range n from small to very large 
  // (testing mainly calc_chisq)
  std::function<double()> trivial_prd = []() { return 0; }; // Prediction = 0
  std::vector<int> n_bins = {20, 200, 2000, 20000, 200000};
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  for ( auto n : n_bins ) {
    ParVec fit_pars {}; 
    BinVec fit_bins (n, FitBin(1.0, 1.0, trivial_prd));
    FitContainer container {};
    container.m_fit_pars = fit_pars;
    container.m_fit_bins = fit_bins;
    ChiSqMinimizer chi_sq_minimizer (&container, factory);
    ASSERT_EQ(chi_sq_minimizer.get_chisq(), n);
  }
}

TEST(TestChiSqMinimizer, MultiBinPlusFitParConstructor) {
  // Test with two FitBin's and predicition that uses FitPar
  // => Is change of FitPar value properly reflected in construction?
  FitPar fp ("fp", 0.0, 0.01);
  FitPar * fp_ptr = &fp;
  std::function<double()> bin_prd = [fp_ptr]() { return 2*fp_ptr->m_val_mod; };
  FitBin fb1 (1.0, 1.0, bin_prd); // Measurement = Unc = 1
  FitBin fb2 (-1.0, 1.0, bin_prd); // Measurement -1, Unc = 1
  ParVec fit_pars {}; 
  BinVec fit_bins {fb1, fb2};
  FitContainer container {};
  container.m_fit_pars = fit_pars;
  container.m_fit_bins = fit_bins;
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  
  ChiSqMinimizer chi_sq_minimizer (&container, factory);
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 2);
  
  fp.m_val_mod = 0.5;
  ChiSqMinimizer chi_sq_minimizer_mod (&container, factory);
  // ((1-1)/1)^2 + ((1-(-1))/1)^2 = 4
  ASSERT_EQ(chi_sq_minimizer_mod.get_chisq(), 4);
}

TEST(TestChiSqMinimizer, ChiSqWithBinsAndConstr) {
  // Test with one bin and one parameter constraint
  std::function<double()> trivial_prd = []() { return 0; }; // Prediction = 0
  FitContainer container {};
  
  FitBin fb (1.0, 1.0, trivial_prd); // Measurement = Unc = 1
  BinVec fit_bins {fb};
  container.m_fit_bins = fit_bins;
  
  FitPar fp ("fp", 0, 0.1); // Parameter starts at 0
  fp.set_constrgauss(ParConstrGauss(1,2)); // Constraint to 0 with unc of 1
  ParVec fit_pars {fp}; 
  container.m_fit_pars = fit_pars;
  
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  ChiSqMinimizer chi_sq_minimizer (&container, factory);
  
  // Chi-sq = ((1-0)/1)^2 + ((0-1)/2)^2 = 1.25
  ASSERT_EQ(chi_sq_minimizer.get_chisq(), 1.25);
}



TEST(TestChiSqMinimizer, SecondOrderPolynomialFit) {
  /** Test if I manage to properly fit a parabola from ten gauss-fluctuated 
      points.
  **/
  
  double true_a = 2.5;
  double true_b = -0.3;
  double true_c = 4.3;
  auto true_parabola = [true_a, true_b, true_c](double x) { return true_a*std::pow(x,2) + true_b* x + true_c; };
  
  FitContainer container {};
  
  container.m_fit_pars = ParVec {
    FitPar ("a", 2.0, 0.5),
    FitPar ("b", -0.5, 0.1),
    FitPar ("c", 5, 0.2) 
  };

  auto full_prediction = [](double* a, double* b, double* c, double x) { return (*a)*std::pow(x,2) + (*b)* x + (*c); }; 
  
  // For bins with random gaussian fluctuation
  std::mt19937 gen{1}; // Random seed = 1
  double fluctuation = 0.1;
  
  unsigned int n_bins = 10;
  BinVec fit_bins {};
  double x_start = -5.0;
  for (auto i_bin=0; i_bin<n_bins; i_bin++) {
    double x_bin = -5.0 + double(i_bin);
    std::function<double()> connected_prediction = std::bind(full_prediction, &container.m_fit_pars[0].m_val_mod, &container.m_fit_pars[1].m_val_mod, &container.m_fit_pars[2].m_val_mod, x_bin);
    
    std::normal_distribution<> measurement_func{true_parabola(x_bin),fluctuation};
    double measurement = measurement_func(gen);
    
    fit_bins.push_back( FitBin( measurement, fluctuation, connected_prediction ) );
  }
  
  container.m_fit_bins = fit_bins;

  // Create Minimizer and minimize
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05); // Simple Factory
  ChiSqMinimizer chi_sq_minimizer (&container, factory);
  
  // Here we go...
  chi_sq_minimizer.minimize();
  
  // Test the result 
  // Fixed the differences to what the results came out for me with random seed 1
  // => If they throw failures at first compile adjust the errors accordingly
  // Important: Should be consistent between compilations on same system (-> Minimizer self-consistent)
  auto const result = chi_sq_minimizer.get_result();
  double result_a = result.m_pars_fin[0];
  double result_b = result.m_pars_fin[1];
  double result_c = result.m_pars_fin[2];
  EXPECT_EQ( fabs(result_a-true_a)<0.002, true) << "True a: " << true_a << " , Result a: " << result_a;
  EXPECT_EQ( fabs(result_b-true_b)<0.003, true) << "True b: " << true_b << " , Result b: " << result_b;
  EXPECT_EQ( fabs(result_c-true_c)<0.03, true) << "True c: " << true_c << " , Result c: " << result_c;
  
  double result_chisq = result.m_chisq_fin;
  int n_dof = n_bins - 3; // 3->#parameters
  EXPECT_EQ( fabs(result_chisq/double(n_bins)-1.0)<0.2, true) << "Result chi^2/n_dof: " << result_chisq/double(n_bins);
  
  // Fix parameter "a" to true value and test if all works well 
  // and that "a" does not change
  container.m_fit_pars[0].m_val_mod = true_a;
  container.m_fit_pars[0].fix();
  
  // Here we go again...
  chi_sq_minimizer.minimize();
  auto const new_result = chi_sq_minimizer.get_result();
  double new_result_a = new_result.m_pars_fin[0];
  double new_result_b = new_result.m_pars_fin[1];
  double new_result_c = new_result.m_pars_fin[2];
  EXPECT_EQ( fabs(new_result_a-true_a)<1e-9, true) << "True a: " << true_a << " , Result a: " << new_result_a;
  EXPECT_EQ( fabs(new_result_b-true_b)<0.03, true) << "True b: " << true_b << " , Result b: " << new_result_b;
  EXPECT_EQ( fabs(new_result_c-true_c)<0.03, true) << "True c: " << true_c << " , Result c: " << new_result_c;
}