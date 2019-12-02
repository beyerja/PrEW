#include <gtest/gtest.h>
#include <ParConstrGauss.h>

#include <math.h>

using namespace PREW::Fit;

TEST(TestParConstrGauss, DefaultConstructor) {
  ParConstrGauss constr {}; // Gaussian constr. around 0 with unc. of 1
  EXPECT_EQ(constr.calc_chisq(0), 0);
}

TEST(TestParConstrGauss, SimpleChiSq) {
  ParConstrGauss constr (0.0, 1.0); // Gaussian constr. around 0 with unc. of 1
  EXPECT_EQ(constr.calc_chisq(0), 0);
  EXPECT_EQ(constr.calc_chisq(1), 1);
  EXPECT_EQ(constr.calc_chisq(2), 4);
}

TEST(TestParConstrGauss, PrecisionChiSq) {
  // Test that precision is held over orders of magnitude
  ParConstrGauss constr (200000.05, 50000.05);
  EXPECT_EQ( fabs(constr.calc_chisq(250000.1) - 1.0) < 0.00001, true);
}