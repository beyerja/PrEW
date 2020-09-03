#include <gtest/gtest.h>

#include <CppUtils/Num.h>
#include <Data/CoefDistr.h>
#include <Data/DistrInfo.h>

#include <string>
#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;

//------------------------------------------------------------------------------
// Tests for DistrInfo class

TEST(TestCoefDistr, TrivialConstructor) {
  CoefDistr coefs {};
  ASSERT_STREQ(coefs.get_coef_name().c_str(), "");
  ASSERT_EQ(coefs.get_info(), DistrInfo());
  ASSERT_THROW(coefs.get_coef(0),std::out_of_range);
}

//------------------------------------------------------------------------------

TEST(TestCoefDistr, ChangeInfo) {
  CoefDistr coefs {};
  ASSERT_EQ(coefs.get_info(), DistrInfo());
  DistrInfo test_info = {"Distr", "PolConfig", 1000};
  coefs.set_info(test_info);
  ASSERT_EQ(coefs.get_info(), test_info);
}

//------------------------------------------------------------------------------

TEST(TestCoefDistr, GlobalAndDiffCoefs) {
  std::string test_name = "Name";
  DistrInfo test_info = {"Distr", "PolConfig", 1000};
  
  double test_val = 1.25;
  std::vector<double> test_vals {-0.234, 213213, 444, 222.004};
  
  CoefDistr coef(test_name,test_info,test_val);
  CoefDistr coefs(test_name,test_info,test_vals);
  
  ASSERT_STREQ(coef.get_coef_name().c_str(), test_name.c_str());
  ASSERT_STREQ(coefs.get_coef_name().c_str(), test_name.c_str());
  ASSERT_EQ(coef.get_info(), test_info);
  ASSERT_EQ(coefs.get_info(), test_info);
  
  for (size_t b=0; b<test_vals.size(); b++) {
    ASSERT_EQ( Num::equal_to_eps(coef.get_coef(b), test_val), true )
      << "Got: " << coef.get_coef(b) << ", expected: " << test_val;
    ASSERT_EQ( Num::equal_to_eps(coefs.get_coef(b), test_vals[b]), true )
      << "Got: " << coefs.get_coef(b) << ", expected: " << test_vals[b];
  }
}

//------------------------------------------------------------------------------

TEST(TestCoefDistr, Operators) {
  DistrInfo info_A = {"DistrA", "PolConfigA", 1000};
  DistrInfo info_B = {"DistrB", "PolConfigB", 1000};
  CoefDistr coef1("CoefA",info_A,0);
  CoefDistr coef2("CoefA",info_A,{1,2,3});
  CoefDistr coef3("CoefA",info_B,0);
  CoefDistr coef4("CoefB",info_A,0);
  
  ASSERT_EQ(coef1, coef2);
  ASSERT_NE(coef1, coef3);
  ASSERT_NE(coef1, coef4);
}

//------------------------------------------------------------------------------
