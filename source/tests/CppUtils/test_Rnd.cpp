#include <gtest/gtest.h>
#include <CppUtils/Num.h>
#include <CppUtils/Rnd.h>

using namespace PREW::CppUtils;

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
