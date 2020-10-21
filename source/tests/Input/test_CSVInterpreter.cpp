#include <gtest/gtest.h>

// Inputs from PrEW
#include <Data/DistrInfo.h>
#include <GlobalVar/Chiral.h>
#include <Input/CSVInterpreter.h>

// Standard library
#include <string>

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
}

//------------------------------------------------------------------------------
