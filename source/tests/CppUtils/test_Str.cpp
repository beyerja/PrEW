#include <CppUtils/Str.h>

#include <gtest/gtest.h>

#include <string>

using namespace PrEW::CppUtils;

//------------------------------------------------------------------------------
// Tests string helper functions

TEST(TestStr, StringSplitting) {
  // Test splitting of string into vector using a delimiter
  
  std::string str = "12 ab,c 3=!=de, 45 fg";
  
  auto space_split = Str::string_to_vec(str, " ");
  auto comma_split = Str::string_to_vec(str, ",");
  auto multisign_split = Str::string_to_vec(str, "=!=");
  
  ASSERT_EQ(space_split.size(), 5);
  ASSERT_EQ(comma_split.size(), 3);
  ASSERT_EQ(multisign_split.size(), 2);
  
  ASSERT_STREQ(space_split[0].c_str(), "12");
  ASSERT_STREQ(space_split[1].c_str(), "ab,c");
  ASSERT_STREQ(comma_split[0].c_str(), "12 ab");
  ASSERT_STREQ(comma_split[1].c_str(), "c 3=!=de");
  ASSERT_STREQ(multisign_split[0].c_str(), "12 ab,c 3");
  ASSERT_STREQ(multisign_split[1].c_str(), "de, 45 fg");
}

TEST(TestStr, ScientificNotationString) {
  // Test conversion of number to string in scientific notation
  double d = 123456789.098765e8;
  float  f = 0.00000006574839;
  int    i = -987654321;

  ASSERT_STREQ(Str::sci_string(d,2).c_str(), "1.23e+16");
  ASSERT_STREQ(Str::sci_string(d,9).c_str(), "1.234567891e+16");
  ASSERT_STREQ(Str::sci_string(d,15).c_str(), "1.234567890987650e+16");
  ASSERT_STREQ(Str::sci_string(f,1).c_str(), "6.6e-08");
  ASSERT_STREQ(Str::sci_string(f,3).c_str(), "6.575e-08");
  ASSERT_STREQ(Str::sci_string(f,6).c_str(), "6.574839e-08");

  // Special case: There is no scientific notation for integers in C++ 
  ASSERT_STREQ(Str::sci_string(i,5).c_str(), "-987654321");
}

TEST(TestStr, DirectoryFromPath) {
  // Test getting the directory path string from a full file path string
  std::string f1_path = "~/home/test/blabla/get.out";
  std::string f2_path = "get.out";
  std::string f3_path = "~/home/test/blabla/";
  ASSERT_STREQ(Str::get_dir_path_str(f1_path).c_str(), "~/home/test/blabla");
  ASSERT_STREQ(Str::get_dir_path_str(f2_path).c_str(), "");
  ASSERT_STREQ(Str::get_dir_path_str(f3_path).c_str(), "~/home/test/blabla");
}

//------------------------------------------------------------------------------
