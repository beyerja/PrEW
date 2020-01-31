#include <gtest/gtest.h>
#include <Fit/FitBin.h>

using namespace PREW::Fit;

TEST(TestFitbin, TrivialConstructor) {
  FitBin fb {};
  ASSERT_EQ(fb.get_val_mst(), 0);
  ASSERT_EQ(fb.get_val_unc(), 0);
}
  
TEST(TestFitbin, ReturnsCorrectVals) {
  FitBin fb1 (0.0035, 200.0);
  FitBin fb2 (-1110.022, 0.0);
  ASSERT_EQ(fb1.get_val_mst(), 0.0035);
  ASSERT_EQ(fb1.get_val_unc(), 200.0);
  ASSERT_EQ(fb2.get_val_mst(), -1110.022);
  ASSERT_EQ(fb2.get_val_unc(), 0.0);
}

TEST(TestFitbin, CorrectTrivialPrediction) {
  std::function<double()> trivial_prd = []() { return 2.1; };
  FitBin fb (0, 0, trivial_prd);
  ASSERT_EQ(fb.get_val_prd(), 2.1);
}

TEST(TestFitbin, CorrectChangedPrediction) {
  /** Test that if I connect a function that depends on a pointer the 
      prediction changes with pointer content.
  **/
  double x = 1.0;
  double * x_ptr = &x;
  std::function<double()> trivial_prd = [x_ptr]() { return *x_ptr; };
  FitBin fb (0, 0, trivial_prd);
  x = -3.5;
  ASSERT_EQ(fb.get_val_prd(), -3.5);
}