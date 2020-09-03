#include <CppUtils/Num.h>
#include <Fcts/Statistic.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------
// Check that statistical functions are correctly implemented

TEST(TestStatistic, Gaussian1D) {
  std::vector<double> c {};
  std::vector<double> p_vals {  1.5, // Amplitude
                                0.5, // Mean
                                2.0  // Sigma
                              };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }

  ASSERT_EQ( 
    Num::equal_to_eps( Statistic::gaussian_1D( {-1.} ,c,p_ptrs), 0.225853, 1e-5), 
    true 
  ) << "Expected " << 0.225853 
    << " got " << Statistic::gaussian_1D({-1.},c,p_ptrs);
  ASSERT_EQ(
    Num::equal_to_eps( Statistic::gaussian_1D( {0.0} ,c,p_ptrs), 0.290001, 1e-5),
    true
  ) << "Expected " << 0.290001 
    << " got " << Statistic::gaussian_1D({0.0},c,p_ptrs);
  ASSERT_EQ(
    Num::equal_to_eps( Statistic::gaussian_1D( {1.0} ,c,p_ptrs), 0.290001, 1e-5),
    true
  ) << "Expected " << 0.290001 
    << " got " << Statistic::gaussian_1D({1.0},c,p_ptrs);
  ASSERT_EQ(
    Num::equal_to_eps( Statistic::gaussian_1D( {1.5} ,c,p_ptrs), 0.264049, 1e-5),
    true
  ) << "Expected " << 0.264049 
    << " got " << Statistic::gaussian_1D({1.5},c,p_ptrs);
}

//------------------------------------------------------------------------------