#include <gtest/gtest.h>

#include <Data/DistrInfo.h>

using namespace PREW::Data;

//------------------------------------------------------------------------------
// Tests for DistrInfo class

TEST(TestData, EqualOperator) {
  /** Test that equal operator works properly.
  **/
  DistrInfo info1 {"a", "LR", 100};
  DistrInfo info1_clone {"a", "LR", 100};
  DistrInfo info2 {"b", "LR", 100};
  DistrInfo info3 {"a", "LL", 100};
  DistrInfo info4 {"a", "LR", 200};
  
  
  ASSERT_EQ(info1 == info1_clone, true);
  ASSERT_EQ(info1 == info2, false);
  ASSERT_EQ(info1 == info3, false);
  ASSERT_EQ(info1 == info4, false);
}

//------------------------------------------------------------------------------
