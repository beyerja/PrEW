#include <Connect/Linker.h>
#include <CppUtils/Num.h>
#include <CppUtils/Vec.h>
#include <Data/CoefDistr.h>
#include <Data/FctLink.h>
#include <Fit/FitPar.h>

#include <gtest/gtest.h>

#include <functional>
#include <string>

using namespace PrEW::Connect;
using namespace PrEW::CppUtils;
using namespace PrEW::Data;
using namespace PrEW::Fit;

//------------------------------------------------------------------------------
// Tests for Linker class that creates connected and easily callable functions

TEST(TestLinker, GaussianTest) {
  // Test that linker can give out correct functions that gives values for a 
  // gaussian at a given x value, gaussian values must change when gaussian 
  // parameters are changed.
  
  // --- Prediction data:
  // x values to look at (1D)
  Vec::Matrix2D<double> bin_centers {
    {{-1}},{{0}},{{1}},{{1.5}},{{2}}
  };
  // No coefficients for now
  CoefDistrVec coefs {};
  // Fit parameters (which will change during minimization in other stage)
  ParVec pars = {
    FitPar("A", 1, 0),    // Amplitude
    FitPar("mu", 0, 0),   // Mean
    FitPar("sigma", 1, 0) // Width
  };
  
  // --- Instructions on how to connect to prediction:
  FctLinkVec fct_links {
    {"Gaussian1D", // Only one function here: Gaussian
      {"A","mu","sigma"}, // Parameter map
      {} // Coefficient map
    }
  };
  
  // --- Get functions from linker which are linked to parameters
  Linker linker = Linker(fct_links, bin_centers, coefs);
  
  auto gaussian_bin0 = linker.get_bonded_fct_at_bin("Gaussian1D",0,&pars);
  auto gaussian_bin1 = linker.get_bonded_fct_at_bin("Gaussian1D",1,&pars);
  auto gaussian_bin2 = linker.get_bonded_fct_at_bin("Gaussian1D",2,&pars);
  auto gaussian_bin3 = linker.get_bonded_fct_at_bin("Gaussian1D",3,&pars);
  auto gaussian_bin4 = linker.get_bonded_fct_at_bin("Gaussian1D",4,&pars);
  
  // --- Check (changing) function output
  // Looked up exact expected values in WolframAlpha -> compare
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin0(), 0.2419707245, 1e-9), true)
    << "Got " << gaussian_bin0() << " expected " << 0.2419707245;
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin1(), 0.3989422804, 1e-9), true)
    << "Got " << gaussian_bin1() << " expected " << 0.3989422804;
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin2(), 0.2419707245, 1e-9), true)
    << "Got " << gaussian_bin2() << " expected " << 0.2419707245;
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin3(), 0.1295175957, 1e-9), true)
    << "Got " << gaussian_bin3() << " expected " << 0.1295175957;
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin4(), 0.0539909665, 1e-9), true)
    << "Got " << gaussian_bin4() << " expected " << 0.0539909665;
    
  // Changing parameters (same bin coordinates!)
  pars[0].m_val_mod = 2; // Amplitude
  pars[1].m_val_mod = 1.5; // Mean
  pars[2].m_val_mod = 0.5; // Width
  
  // Did function outputs change?
  // -> If so succesfully established link between parameters and functions
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin1(), 0.0177273936, 1e-9), true)
    << "Got " << gaussian_bin1() << " expected " << 0.0177273936;
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin2(), 0.9678828981, 1e-9), true)
    << "Got " << gaussian_bin2() << " expected " << 0.9678828981;
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin3(), 1.5957691216, 1e-9), true)
    << "Got " << gaussian_bin3() << " expected " << 1.5957691216;
}

TEST(TestLinker, GaussianManyValuesTest) {
  // Same as GaussianTest but only testing that it's not a problem to create
  // loads of functions
  
  // --- Prediction data:
  // x values to look at (1D)
  Vec::Matrix2D<double> bin_centers {};
  for (double center=-20; center<20; center+=0.001) {
    bin_centers.push_back({center});
  } // 40/0.01 = 40000 bins -> as many functions
  // No coefficients for now
  CoefDistrVec coefs {};
  // Fit parameters (which will change during minimization in other stage)
  ParVec pars = {
    FitPar("A", 1, 0),    // Amplitude
    FitPar("mu", 0, 0),   // Mean
    FitPar("sigma", 1, 0) // Width
  };
  
  // --- Instructions on how to connect to prediction:
  FctLinkVec fct_links {
    {"Gaussian1D", // Only one function here: Gaussian
      {"A","mu","sigma"}, // Parameter map
      {} // Coefficient map
    }
  };
  
  // --- Get functions from linker which are linked to parameters
  Linker linker = Linker(fct_links, bin_centers, coefs);
  
  // Get all bin functions
  std::vector<std::function<double()>> gaussian_at_bins {};
  for (int bin=0; bin<bin_centers.size(); bin++) {
    gaussian_at_bins.push_back(
      linker.get_bonded_fct_at_bin("Gaussian1D",bin,&pars)
    );
  }
  
  // --- Check output (only roughly)
  // Check that no zero value occured
  bool found_zero_val = false;
  for (const auto & gaussian_at_bin: gaussian_at_bins ) {
    if (gaussian_at_bin() == 0) {
      found_zero_val = true;
      break;
    }
  }
  ASSERT_EQ(found_zero_val, false);
  
  // Modify amplitude to zero and check again, now all should be zero
  pars[0].m_val_mod = 0;
  bool found_nonzero_val = false;
  for (const auto & gaussian_at_bin: gaussian_at_bins ) {
    if (gaussian_at_bin() != 0) {
      found_nonzero_val = true;
      break;
    }
  }
  ASSERT_EQ(found_nonzero_val, false);
}


TEST(TestLinker, MultiFunctionTest) {
  // Test that it works for having two different functions whose parameters 
  // are connected
  
  // --- Prediction data:
  // x values to look at (1D)
  Vec::Matrix2D<double> bin_centers { {1.0} }; // Just one bin at x=1
  // No coefficients for now
  CoefDistrVec coefs {};
  
  // Fit parameters (which will change during minimization in other stage)
  ParVec pars = {
    FitPar("P1", 1, 0),   // Parameter will be in both parameterisations!
    FitPar("mu", 1, 0),   // Gaussian Mean
    FitPar("sigma", 1, 0),// Gaussian Width
    FitPar("P2", 2, 0),   // Linear polynomial parameter
    FitPar("P3", 3, 0)    // Quadratic polynomial parameter
  };
  
  // --- Instructions on how to connect to prediction:
  FctLinkVec fct_links {
    {"Gaussian1D", {"P1","mu","sigma"}, {}},
    {"Quadratic1DPolynomial", {"P1","P2","P3"}, {}} // Notice P1 overlap!
  };
  
  // --- Get functions from linker which are linked to parameters
  Linker linker = Linker(fct_links, bin_centers, coefs);
  auto all_bin_fcts = linker.get_all_bonded_fcts_at_bin(0,&pars);
  ASSERT_EQ( all_bin_fcts.size(), 2 );
  
  auto gaussian_bin  = all_bin_fcts.at(0);
  auto quadratic_bin = all_bin_fcts.at(1);

  // --- Check for expected output
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin(), 0.3989422804, 1e-9), true)
    << "Got " << gaussian_bin() << " expected " << 0.3989422804;
  ASSERT_EQ(Num::equal_to_eps(quadratic_bin(), 6.0, 1e-9), true) // 1+2+3=6
    << "Got " << quadratic_bin() << " expected " << 6;
    
  // Change one(!) parameter -> Both outputs should change!
  pars[0].m_val_mod = 2; // -> Amplite/Offset doubled
  
  ASSERT_EQ(Num::equal_to_eps(gaussian_bin(), 2.0*0.3989422804, 1e-9), true)
    << "Got " << gaussian_bin() << " expected " << 2.0*0.3989422804;
  ASSERT_EQ(Num::equal_to_eps(quadratic_bin(), 7.0, 1e-9), true) // 2+2+3=6
    << "Got " << quadratic_bin() << " expected " << 2;
}
//------------------------------------------------------------------------------
