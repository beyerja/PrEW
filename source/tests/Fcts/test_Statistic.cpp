#include <CppUtils/Num.h>
#include <Data/BinCoord.h>
#include <Fcts/Statistic.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------
// Check that statistical functions are correctly implemented

TEST(TestStatistic, Gaussian1D) {
  std::vector<double> c{};
  std::vector<double> p_vals{
      1.5, // Amplitude
      0.5, // Mean
      2.0  // Sigma
  };
  std::vector<double *> p_ptrs{};
  for (double &p : p_vals) {
    p_ptrs.push_back(&p);
  }

  std::vector<double> x_vals{-1., 0.0, 1.0, 1.5};
  std::vector<double> pred{0.225853, 0.290001, 0.290001, 0.264049};

  for (size_t b = 0; b < x_vals.size(); b++) {
    BinCoord x({x_vals[b]}, {x_vals[b]}, {x_vals[b]});
    ASSERT_TRUE(Num::equal_to_eps(Statistic::gaussian_1D({x}, c, p_ptrs),
                                  pred[b], 1e-6))
        << "Expected " << pred[b] << " got "
        << Statistic::gaussian_1D(x, c, p_ptrs);
  }
}

//------------------------------------------------------------------------------