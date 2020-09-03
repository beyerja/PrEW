#include <CppUtils/Num.h>
#include <Fit/FitPar.h>
#include <ToyMeas/ParFlct.h>

#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

#include <cmath>
#include <numeric>
#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Fit;
using namespace PrEW::ToyMeas;

//------------------------------------------------------------------------------

TEST(TestParFlct, SingleConstrFlct) {
  FitPar fp ("",0,0);
  fp.set_constrgauss(1.0,0.5);
  ParFlct::fluctuate_constr(fp);
  // Check that constraint value changed, but not its uncertainty
  ASSERT_EQ(Num::equal_to_eps(fp.get_constr_val(), 1.0), false);
  ASSERT_EQ(Num::equal_to_eps(fp.get_constr_unc(), 0.5), true);
}

//------------------------------------------------------------------------------

TEST(TestParFlct, ManyParConstrFlct) {
  // Test if fluctuation of constraints of many parameters works.
  // All parameters will have same mean & width of constraint.
  // => After fluctuation should reflect gaussian
  double mean = 1.0;
  double width = 0.5;
  int n_pars = 100000;
  ParVec pars (n_pars, FitPar("",mean,0));
  
  for ( auto & par: pars ) {
    par.set_constrgauss(mean, width);
  }
  
  // Fluctuate constrain values
  ParFlct::fluctuate_constrs(pars);
  
  std::vector<double> result_vals (n_pars);
  std::vector<double> result_chisqs (n_pars);
  for ( int p=0; p<n_pars; p++ ) { 
    result_vals[p] = pars[p].get_constr_val();
    result_chisqs[p] = pars[p].calc_constr_chisq();
  }
  
  double result_mean = 
    std::accumulate(result_vals.begin(), result_vals.end(), 0.0)/double(n_pars);
  double result_sq_sum = 
    std::inner_product(result_vals.begin(),result_vals.end(),result_vals.begin(),0.0);
  double result_width = 
    std::sqrt(result_sq_sum / double(n_pars) - result_mean * result_mean);

  double sum_chisq = 
    std::accumulate(result_chisqs.begin(), result_chisqs.end(), 0.0);

  ASSERT_EQ(Num::equal_to_eps(result_mean, mean, 0.1*width), true)
    << "Expected mean: " << mean << " , got: " << result_mean;
  ASSERT_EQ(Num::equal_to_eps(result_width, width, 5e-2*width), true)
    << "Expected width: " << width << " , got: " << result_width;
  ASSERT_EQ(Num::equal_to_eps(sum_chisq, double(n_pars), 5*std::sqrt(n_pars)), true)
    << "Expected chisq sum: " << double(n_pars) << " , got: " << sum_chisq;
}

//------------------------------------------------------------------------------