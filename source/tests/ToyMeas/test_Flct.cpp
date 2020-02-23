#include <CppUtils/Num.h>
#include <Fit/FitBin.h>
#include <ToyMeas/Flct.h>

#include <gtest/gtest.h>

#include <cmath>

using namespace PREW::CppUtils;
using namespace PREW::Fit;
using namespace PREW::ToyMeas;

//------------------------------------------------------------------------------
// Test random fluctuation of FitBin values

TEST(TestFlct, BinGetsFluctuated) {
  FitBin bin(20.5,0.5);
  Flct::poisson_fluctuate(bin);
  
  ASSERT_EQ( Num::equal_to_eps(fmod(bin.get_val_mst(), 1.0), 0.0), true )
    << "Poisson fluctuation gave non-integer: " << bin.get_val_mst();
  ASSERT_EQ( (bin.get_val_unc() != 0.5), true );
}

//------------------------------------------------------------------------------