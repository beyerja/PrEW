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
  std::string file_path = "~/home/test/blabla/get.out";
  ASSERT_STREQ(Str::get_dir_path_str(file_path).c_str(), "~/home/test/blabla");
}

//------------------------------------------------------------------------------
