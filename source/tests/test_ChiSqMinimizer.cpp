#include <gtest/gtest.h>
#include <ChiSqMinimizer.h>

using namespace PREW::Fit;

TEST(TestChiSqMinimizer, TrivialConstructor) {
  FitContainer container {};
  ChiSqMinimizer chi_sq_minimizer (&container);
}