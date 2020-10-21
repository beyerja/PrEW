#include <gtest/gtest.h>

#include <CppUtils/Num.h>
#include <Data/BinCoord.h>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;

//------------------------------------------------------------------------------
// Tests for BinCoord class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Constructor tests

TEST(TestBinCoord, TrivialConstructor) {
  BinCoord trivial_coord{};
  std::vector<double> empty{};
  ASSERT_EQ(trivial_coord.get_dim(), 0);
  ASSERT_EQ(trivial_coord.get_center(), empty);
  ASSERT_EQ(trivial_coord.get_edge_low(), empty);
  ASSERT_EQ(trivial_coord.get_edge_up(), empty);
}

TEST(TestBinCoord, SimpleConstructor) {
  std::vector<double> center{0.1, 0.0, -0.4};
  std::vector<double> edge_low{0.05, -4, -0.5};
  std::vector<double> edge_up{0.15, 5, -0.3};
  BinCoord simple_coord{center, edge_low, edge_up};

  ASSERT_EQ(simple_coord.get_dim(), 3);
  ASSERT_EQ(simple_coord.get_center(), center);
  ASSERT_EQ(simple_coord.get_edge_low(), edge_low);
  ASSERT_EQ(simple_coord.get_edge_up(), edge_up);
}

TEST(TestBinCoord, FalseConstructor) {
  ASSERT_THROW(BinCoord({0}, {0}, {0, 1}), std::invalid_argument);
  ASSERT_THROW(BinCoord({0}, {0, 1}, {0}), std::invalid_argument);
  ASSERT_THROW(BinCoord({0, 1}, {0}, {0}), std::invalid_argument);
}

//------------------------------------------------------------------------------
// Operator tests

TEST(TestBinCoord, Operators) {
  BinCoord coord1{{1.0, -2.0}, {0.5, -4.0}, {1.0, 0.0}};
  BinCoord coord2{{0.5, 7.0}, {-1.0, 4.5}, {2.0, 8.5}};

  ASSERT_EQ(coord1, coord1);
  ASSERT_NE(coord1, coord2);
}

//------------------------------------------------------------------------------