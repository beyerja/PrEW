#include <gtest/gtest.h>
#include <Fit/FitPar.h>

#include <string>  
#include <sstream>   

using namespace PREW::Fit;

TEST(TestFitPar, ReturnsCorrectIni) {
  FitPar fp1 ("fp1", 0.00055, 0.01);
  FitPar fp2 ("fp2", -200.5, 0.01);
  
  ASSERT_EQ(fp1.get_val_ini(), 0.00055);
  ASSERT_EQ(fp1.get_unc_ini(), 0.01);
  ASSERT_EQ(fp1.m_val_mod, 0.00055);
  ASSERT_STREQ(fp1.get_name().c_str(), "fp1");
  
  ASSERT_EQ(fp2.get_val_ini(), -200.5);
  ASSERT_EQ(fp2.get_unc_ini(), 0.01);
  ASSERT_EQ(fp2.m_val_mod, -200.5);
  ASSERT_STREQ(fp2.get_name().c_str(), "fp2");
}

TEST(TestFitPar, ProperReset) {
  FitPar fp ("fp", 4000.77, 0.01);
  fp.m_val_mod = 20.5;
  fp.reset();
  ASSERT_EQ(fp.m_val_mod, 4000.77);
}

TEST(TestFitPar, ProperOStream) {
  FitPar fp ("fp", -30.27, 0.01);
  fp.m_val_mod = 2.0;
  std::stringstream buffer;
  buffer << fp;
  std::string result = buffer.str();
  std::string expected = "Fit Parameter: initial value: -30.27 -> modified value: 2";
  ASSERT_STREQ(result.c_str(), expected.c_str());
}

TEST(TestFitPar, EqualOperator) {
  FitPar fp1 ("fp1", 0, 0.01);
  FitPar fp2 ("fp2", 0, 0.01);
  FitPar fp1_copy ("fp1", 1, 0.01);
  
  ASSERT_EQ(fp1 == fp1, true);
  ASSERT_EQ(fp1 == fp2, false);
  ASSERT_EQ(fp1 == fp1_copy, true);
}

TEST(TestFitPar, ConstrCalcWithoutConstr) {
  FitPar fp ("fp", 2, 4);
  ASSERT_EQ(fp.calc_constr_chisq(), 0.0);
}

TEST(TestFitPar, GaussConstrCalc) {
  FitPar fp ("fp", 2, 4);
  fp.set_constrgauss(ParConstrGauss(1,2));
  // constr: ( (1-2)/2 )^2 = 0.25
  ASSERT_EQ(fp.calc_constr_chisq(), 0.25);
}

TEST(TestFitPar, GaussConstrModifiedValue) {
  FitPar fp ("fp", 2, 4);
  fp.set_constrgauss(ParConstrGauss(1,2));
  // constr: ( (1-2)/2 )^2 = 0.25
  ASSERT_EQ(fp.calc_constr_chisq(), 0.25);
  
  fp.m_val_mod = 5;
  // constr: ( (5-1)/2 )^2 = 4
  ASSERT_EQ(fp.calc_constr_chisq(), 4);
}
