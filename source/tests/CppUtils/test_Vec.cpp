#include <CppUtils/Vec.h>

#include <gtest/gtest.h>

#include <vector>
#include <string>

using namespace PrEW::CppUtils;

//------------------------------------------------------------------------------
// Tests for vector helpers

TEST(TestVec, SubVecFinding) {
  /** Test finding of a subvector using a condition function.
  **/
  // Original vector 
  std::vector<double> vec = {-1, 234234, 0, -45, 0.333, -3, -1e9};
  
  // Condition function: Only negative values
  auto condition = [](double val) { return val < 0; };
  
  // Negative value vector
  std::vector<double> subvec = Vec::subvec_by_condition(vec, condition);
  
  // Expectation:
  std::vector<double> expected = {-1, -45, -3, -1e9};
  
  ASSERT_EQ(subvec, expected);
}

TEST(TestVec, ElementFinding) {
  /** Test finding of an element using a condition function.
  **/
  // Original vector 
  std::vector<std::string> vec = {"a", "A", "abc", "0sd9"};
  
  // Condition function: string must be three letters long
  auto condition = [](std::string str) { return str.size() == 3; };
  
  // Corresponding element
  std::string element = Vec::element_by_condition(vec, condition);
  
  ASSERT_EQ(element, "abc");
}

//------------------------------------------------------------------------------
