#include <CppUtils/Num.h>
#include <Fcts/Systematics.h>

#include <gtest/gtest.h>

#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Fcts;

//------------------------------------------------------------------------------

TEST(TestSystematics, PolarisationFactor) {
  // Test the polarisation factor function.
  std::vector<double> c {
    +1, // e- chirality = R
    -1, // e+ chirality = L
    -1, // e- beam polarisation sign
    +1  // e- beam polarisation sign
  };
  std::vector<double> p_vals {  
    0.2, // e- beam polarisation amplitude
    0.6  // e+ beam polarisation amplitude
  };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  // (1+(+1)*(-0.2)) * (1+(-1)*(0.6)) / 4.0 = 0.08
  ASSERT_EQ( 
    Num::equal_to_eps( Systematics::polarisation_factor({},c,p_ptrs), 0.08, 1e-9), 
    true 
  ) << "Expected " << 0.08 
    << " got " << Systematics::polarisation_factor({},c,p_ptrs);
}

TEST(TestSystematics, LuminosityFraction) {
  // Test the luminosity fraction function.
  std::vector<double> c { 0.25 };
  std::vector<double> p_vals { 2000 };
  std::vector<double*> p_ptrs {};
  for (double & p: p_vals) { p_ptrs.push_back(&p); }
  
  ASSERT_EQ( 
    Num::equal_to_eps( Systematics::luminosity_fraction({},c,p_ptrs), 500.0, 1e-9), 
    true 
  ) << "Expected " << 500.0
    << " got " << Systematics::luminosity_fraction({},c,p_ptrs);
}

//------------------------------------------------------------------------------
