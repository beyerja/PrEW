#include <gtest/gtest.h>
#include <FitPar.h>

#include <string>  
#include <sstream>   

using namespace PREW::Fit;

TEST(TestFitPar, ReturnsCorrectIni) {
  FitPar fp1 (0.00055);
  FitPar fp2 (-200.5);
  EXPECT_EQ(fp1.get_val_ini(), 0.00055);
  EXPECT_EQ(fp1.m_val_mod, 0.00055);
  EXPECT_EQ(fp2.get_val_ini(), -200.5);
  EXPECT_EQ(fp2.m_val_mod, -200.5);
}

TEST(TestFitPar, ProperReset) {
  FitPar fp (4000.77);
  fp.m_val_mod = 20.5;
  fp.reset();
  EXPECT_EQ(fp.m_val_mod, 4000.77);
}

TEST(TestFitPar, ProperOStream) {
  FitPar fp (-30.27);
  fp.m_val_mod = 2.0;
  std::stringstream buffer;
  buffer << fp;
  std::string result = buffer.str();
  std::string expected = "Fit Parameter: initial value: -30.27 -> modified value: 2";
  EXPECT_STREQ(result.c_str(), expected.c_str());
}