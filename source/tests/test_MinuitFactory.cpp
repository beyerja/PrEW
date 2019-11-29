#include <gtest/gtest.h>
#include <MinuitFactory.h>

#include "Math/Functor.h"

#include <math.h>

using namespace PREW::Fit;

TEST(TestMinuitFactory, SimpleMinimizerCreation) {
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 100, 200, 0.05);
  auto minimizer = factory.create_minimizer();
  ASSERT_EQ(minimizer->MaxFunctionCalls(), 100);
  ASSERT_EQ(minimizer->MaxIterations(), 200);
  ASSERT_EQ(minimizer->Tolerance(), 0.05);
}

TEST(TestMinuitFactory, CreateAllMinimizerTypes) {
  // Check that all minimizers are available
  std::vector<ROOT::Minuit2::EMinimizerType> types {
    ROOT::Minuit2::kMigrad, 
    ROOT::Minuit2::kSimplex, 
    ROOT::Minuit2::kCombined, 
    ROOT::Minuit2::kScan, 
    ROOT::Minuit2::kFumili, 
    ROOT::Minuit2::kMigradBFGS 
  };
  for ( auto type : types ) {
    MinuitFactory factory (type, 100, 200, 0.05);
    auto minimizer = factory.create_minimizer();
    ASSERT_EQ(minimizer->MaxFunctionCalls(), 100);
    ASSERT_EQ(minimizer->MaxIterations(), 200);
    ASSERT_EQ(minimizer->Tolerance(), 0.05);
  }
}

TEST(TestMinuitFactory, TestSimpleMinimization) {
  // Test that minimizer finds right minimum for simple function
  MinuitFactory factory (ROOT::Minuit2::kMigrad, 10000, 10000, 0.001);
  auto minimizer = factory.create_minimizer();
  
  // Simple 1D function: (x-1)^2 -> Minimum at 1
  const ROOT::Math::Functor simple_function ([](const double *x) { return std::pow(x[0]-1.0, 2); }, 1);
  
  minimizer->SetFunction(simple_function);
  double variable[1]  = { -1.5}; // Initial value
  double step_size[1] = {0.001}; // Initial error esitmate
  minimizer->SetVariable(0,"x",variable[0], step_size[0]);
  minimizer->Minimize();
  const double * result = minimizer->X();
  ASSERT_EQ( fabs(result[0] - 1.0) < 0.000001 , true);
}