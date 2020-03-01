#include <CppUtils/Str.h>

#include <gtest/gtest.h>

#include <string>

using namespace PREW::CppUtils;

//------------------------------------------------------------------------------
// Tests string helper functions

TEST(TestStr, StringSplitting) {
  // Test splitting of string into vector using a delimiter
  
  std::string str = "12 ab,c 3de, 45 fg";
  
  auto space_split = Str::string_to_vec(str, " ");
  auto comma_split = Str::string_to_vec(str, ",");
  
  ASSERT_EQ(space_split.size(), 5);
  ASSERT_EQ(comma_split.size(), 3);
  ASSERT_STREQ(space_split[0].c_str(), "12");
  ASSERT_STREQ(comma_split[0].c_str(), "12 ab");
}

//------------------------------------------------------------------------------
