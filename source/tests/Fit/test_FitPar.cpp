#include <Fit/FitPar.h>
#include <CppUtils/Num.h>

#include <gtest/gtest.h>

#include <string>  
#include <sstream>   

using namespace PREW::Fit;
using namespace PREW::CppUtils;

TEST(TestFitPar, ReturnsCorrectIni) {
  FitPar fp1 ("fp1", 0.00055, 0.01);
  FitPar fp2 ("fp2", -200.5, 0.01, true);
  
  ASSERT_EQ(fp1.get_val_ini(), 0.00055);
  ASSERT_EQ(fp1.get_unc_ini(), 0.01);
  ASSERT_EQ(fp1.m_val_mod, 0.00055);
  ASSERT_STREQ(fp1.get_name().c_str(), "fp1");
  ASSERT_EQ(fp1.is_fixed(), false);
  
  ASSERT_EQ(fp2.get_val_ini(), -200.5);
  ASSERT_EQ(fp2.get_unc_ini(), 0.01);
  ASSERT_EQ(fp2.m_val_mod, -200.5);
  ASSERT_STREQ(fp2.get_name().c_str(), "fp2");
  ASSERT_EQ(fp2.is_fixed(), true);
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

TEST(TestFitPar, CopyAssignmentOperator) {
  FitPar fp1 ("fp1", 0, 0.01);
  FitPar fp2 ("fp2", 1, 0.05, true);
  FitPar fp2_copy = fp2;
  ASSERT_EQ(fp1 == fp2, false); // Compares only name string
  ASSERT_EQ(fp2 == fp2_copy, true);
  ASSERT_EQ(Num::equal_to_eps(fp2.get_val_ini(), fp2_copy.get_val_ini()), true);
  ASSERT_EQ(Num::equal_to_eps(fp2.get_unc_ini(), fp2_copy.get_unc_ini()), true);
  ASSERT_EQ(fp2.is_fixed(), fp2_copy.is_fixed());
  
  fp2 = fp1; // Overwrite fp1
  ASSERT_EQ(fp1 == fp2, true);
  ASSERT_EQ(fp2 == fp2_copy, false);
  ASSERT_EQ(Num::equal_to_eps(fp2.get_val_ini(), fp2_copy.get_val_ini()), false);
  ASSERT_EQ(Num::equal_to_eps(fp2.get_unc_ini(), fp2_copy.get_unc_ini()), false);
  ASSERT_EQ(fp2.is_fixed() == fp2_copy.is_fixed(), false);
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

TEST(TestFitPar, ParFixing) {
  FitPar fp ("fp", 0, 0, false); // Not fixed
  ASSERT_EQ(fp.is_fixed(), false);
  
  fp.fix(); 
  ASSERT_EQ(fp.is_fixed(), true);
  fp.release(); 
  ASSERT_EQ(fp.is_fixed(), false);
}

TEST(TestFitPar, LimitSetting) {
  FitPar fp ("fp", 0, 0); // Parameter without limits
  ASSERT_EQ(fp.is_limited(), false);
  ASSERT_EQ(fp.get_upper_lim(), 0);
  ASSERT_EQ(fp.get_lower_lim(), 0);
  
  fp.set_limits(-1.5, 1000);
  ASSERT_EQ(fp.get_lower_lim(), -1.5);
  ASSERT_EQ(fp.get_upper_lim(), 1000);
}