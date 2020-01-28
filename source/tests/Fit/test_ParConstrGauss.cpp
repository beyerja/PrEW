#include <Fit/ParConstrGauss.h>
#include <CppUtils/Num.h>

#include <gtest/gtest.h>

#include <math.h>

using namespace PREW::CppUtils;
using namespace PREW::Fit;

TEST(TestParConstrGauss, DefaultConstructor) {
  ParConstrGauss constr {}; // Gaussian constr. around 0 with unc. of 1
  ASSERT_EQ(constr.get_val(), 0);
  ASSERT_EQ(constr.get_unc(), 1);
  ASSERT_EQ(constr.calc_chisq(0), 0);
}

TEST(TestParConstrGauss, SimpleChiSq) {
  ParConstrGauss constr (0.0, 1.0); // Gaussian constr. around 0 with unc. of 1
  ASSERT_EQ(constr.get_val(), 0);
  ASSERT_EQ(constr.get_unc(), 1);
  ASSERT_EQ(constr.calc_chisq(0), 0);
  ASSERT_EQ(constr.calc_chisq(1), 1);
  ASSERT_EQ(constr.calc_chisq(2), 4);
}

TEST(TestParConstrGauss, PrecisionChiSq) {
  // Test that precision is held over orders of magnitude
  ParConstrGauss constr (200000.05, 50000.05);
  ASSERT_EQ(Num::equal_to_eps(constr.calc_chisq(250000.1), 1.0, 0.00001), true);
}

TEST(TestParConstrGauss, EqualOperator) {
  ParConstrGauss constr1 (0, 0.01);
  ParConstrGauss constr2 (0, 0.01);
  ParConstrGauss constr3 (1, 0.01);
  ParConstrGauss constr4 (0, 0.05);
  
  ASSERT_EQ(constr1 == constr1, true);
  ASSERT_EQ(constr1 == constr2, true);
  ASSERT_EQ(constr1 == constr3, false);
  ASSERT_EQ(constr1 == constr4, false);
}

TEST(TestParConstrGauss, CopyAssignmentOperator) {
  ParConstrGauss constr_1 (0, 0.01);
  ParConstrGauss constr_2 (1, 0.05);
  ParConstrGauss constr_2_copy = constr_2;
  ASSERT_EQ(constr_1 == constr_2, false);
  ASSERT_EQ(constr_2 == constr_2_copy, true);
  
  constr_2 = constr_1; // Overwrite constr_1
  ASSERT_EQ(constr_1 == constr_2, true);
  ASSERT_EQ(constr_2 == constr_2_copy, false);
}