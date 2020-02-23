#include <Data/PolLink.h>

#include <gtest/gtest.h>

#include <string>

using namespace PREW::Data;

//------------------------------------------------------------------------------
// Tests for polarisation instruction class

TEST(TestPolLink, DefaultConstructor) {
  PolLink pol_link {};
  
  ASSERT_EQ(pol_link.get_energy(),0);
  ASSERT_STREQ(pol_link.get_pol_config().c_str(),"");
  ASSERT_STREQ(pol_link.get_eM_pol().c_str(),"");
  ASSERT_STREQ(pol_link.get_eP_pol().c_str(),"");
  ASSERT_EQ(pol_link.get_eM_sgn_factor(),+1.0);
  ASSERT_EQ(pol_link.get_eP_sgn_factor(),+1.0);
}

TEST(TestPolLink, SimpleConstructor) {
  PolLink pol_link {500,"e-p-","ePol","pPol","-","-"};
  
  ASSERT_EQ(pol_link.get_energy(),500);
  ASSERT_STREQ(pol_link.get_pol_config().c_str(),"e-p-");
  ASSERT_STREQ(pol_link.get_eM_pol().c_str(),"ePol");
  ASSERT_STREQ(pol_link.get_eP_pol().c_str(),"pPol");
  ASSERT_EQ(pol_link.get_eM_sgn_factor(),-1.0);
  ASSERT_EQ(pol_link.get_eP_sgn_factor(),-1.0);
}

//------------------------------------------------------------------------------
