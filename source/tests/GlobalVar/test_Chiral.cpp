#include <gtest/gtest.h>

#include <GlobalVar/Chiral.h>

using namespace PrEW::GlobalVar;

//------------------------------------------------------------------------------
// Tests for helper functions for chiral (PrEW-)global variables

TEST(TestChiral, ChiralityIntToStr) {
  // Test the conversion of chiral integers to the combined string
  ASSERT_EQ(Chiral::transform(+1, +1),Chiral::eRpR);
  ASSERT_EQ(Chiral::transform(-1, +1),Chiral::eLpR);
  ASSERT_EQ(Chiral::transform(+1, -1),Chiral::eRpL);
  ASSERT_EQ(Chiral::transform(-1, -1),Chiral::eLpL);
  ASSERT_THROW(Chiral::transform(0, 0), std::invalid_argument);
}

//------------------------------------------------------------------------------