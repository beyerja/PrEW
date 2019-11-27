#include <gtest/gtest.h>
#include <test.h>

TEST(TestTest, ReturnsOneTest) {
  EXPECT_EQ(test::test(), 1);
}