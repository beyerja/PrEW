#include <gtest/gtest.h>
#include <Fit/FitResult.h>

using namespace PREW::Fit;

TEST(TestFitResult, EqualOperatorTest) {
  FitResult r1 {};
  ASSERT_EQ(r1 == r1, true);
  
  FitResult r2 {};
  r2.m_chisq_fin = 1.0;
  ASSERT_EQ(r1 == r2, false);
  
  FitResult r3 {};
  FitResult r4 {};
  r3.m_par_names = std::vector<std::string> {""};
  r4.m_par_names = std::vector<std::string> {"a"};
  ASSERT_EQ(r1 == r3, false);
  ASSERT_EQ(r3 == r4, false);
  
  FitResult r5 {};
  FitResult r6 {};
  r5.m_cov_matrix = std::vector<std::vector<double>> ( 4, std::vector<double>(4, 1.0) );
  r6.m_cov_matrix = std::vector<std::vector<double>> ( 4, std::vector<double>(4, 2.0) );
  ASSERT_EQ(r1 == r5, false);
  ASSERT_EQ(r5 == r6, false);
}