#include <gtest/gtest.h>

// Inputs from PrEW
#include <CppUtils/Num.h>
#include <Data/DistrInfo.h>
#include <GlobalVar/Chiral.h>
#include <Input/CSVInterpreter.h>

// Standard library
#include <string>

using namespace PrEW::CppUtils;
using namespace PrEW::Data;
using namespace PrEW::GlobalVar;
using namespace PrEW::Input;

//------------------------------------------------------------------------------
// Tests for CSVInterpreter class

TEST(TestCSVInterpreter, TestDataReading) {
  CSVInterpreter interpreter("../testdata/test.csv");
  auto info = interpreter.get_info();
  auto pred_distr = interpreter.get_pred_distr();
  auto coef_distrs = interpreter.get_coef_distrs();
  
  // Check if info is correct info
  DistrInfo info_pred {"test", Chiral::eLpR, 250};
  ASSERT_EQ(info, info_pred);
  
  // Contains 4 coefficients (2 differential + 2 global)
  ASSERT_EQ(coef_distrs.size(), 4);
  // Number of bins is 5, bin dimension is 2
  ASSERT_EQ(pred_distr.m_coords.size(), 5);
  ASSERT_EQ(pred_distr.m_coords.at(0).get_dim(), 2);
  // Check the first direction of the first coordinate
  auto first_coord = pred_distr.m_coords.at(0);
  ASSERT_TRUE( Num::equal_to_eps(first_coord.get_center()[0], 0.01) );
  ASSERT_TRUE( Num::equal_to_eps(first_coord.get_edge_low()[0], 0.005) );
  ASSERT_TRUE( Num::equal_to_eps(first_coord.get_edge_up()[0], 0.015) );
}

//------------------------------------------------------------------------------
