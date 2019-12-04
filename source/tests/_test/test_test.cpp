#include <gtest/gtest.h>
#include <_test/test.h>

TEST(TestTest, ReturnsOneTest) {
  EXPECT_EQ(test::test(), 1);
}