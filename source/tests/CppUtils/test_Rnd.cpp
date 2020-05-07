#include <gtest/gtest.h>
#include <CppUtils/Num.h>
#include <CppUtils/Rnd.h>

#include <numeric>
#include <vector>

using namespace PrEW::CppUtils;

//------------------------------------------------------------------------------
// Tests for random number generation

TEST(TestRnd, PoissonRange) {
  ASSERT_THROW(Rnd::poisson_fluctuate(-1.0), std::invalid_argument);
}

TEST(TestRnd, PoissonMeanTests) {
  std::vector<double> means {0.5, 2.5, 15.0};
  int n_vals = 10000;
  for (const auto & mean: means){
    double sum = 0;
    for (int i=0; i<n_vals; i++) { sum += Rnd::poisson_fluctuate(mean); }
    double result_mean = sum / double(n_vals);
    ASSERT_EQ(Num::equal_to_eps(result_mean, mean, 5e-2*mean), true)
      << "Expected mean: " << mean << " , got: " << result_mean;
  }
}

//------------------------------------------------------------------------------

TEST(TestRnd, GaussianRange) {
  ASSERT_THROW(Rnd::gauss_fluctuate(0.0, -1.0), std::invalid_argument);
}

TEST(TestRnd, GaussianMeanAndWidth) {
  std::vector<double> means {-4.5, 0.0, 2.5, 15.0};
  std::vector<double> widths {0.01, 3, 50.0};
  int n_vals = 10000;
  // For each mean,width-combination test if distribution ends up as expected
  for (const auto & mean: means){
  for (const auto & width: widths){
    std::vector<double> rnd_vals (n_vals);
    for ( int i=0; i<n_vals; i++ ) { 
      rnd_vals[i] = Rnd::gauss_fluctuate(mean,width); 
    }
    
    // Calculate mean and standard deviation
    double result_sum = std::accumulate(rnd_vals.begin(), rnd_vals.end(), 0.0);
    double result_mean = result_sum / double(n_vals);

    double result_sq_sum = 
      std::inner_product(rnd_vals.begin(),rnd_vals.end(),rnd_vals.begin(),0.0);
    double result_width = 
      std::sqrt(result_sq_sum / double(n_vals) - result_mean * result_mean);
    
    EXPECT_EQ(Num::equal_to_eps(result_mean, mean, 0.1*width), true)
      << "Expected mean: " << mean << " , got: " << result_mean;
    EXPECT_EQ(Num::equal_to_eps(result_width, width, 5e-2*width), true)
      << "Expected width: " << width << " , got: " << result_width;
  } // Loop widths
  } // Loop means
}

//------------------------------------------------------------------------------
