#ifndef LIB_FITRESULT_H
#define LIB_FITRESULT_H 1

#include <vector>
#include <string>

namespace PREW {
namespace Fit {
  
  struct FitResult {
    std::vector<std::string> m_par_names {};
    std::vector<double> m_pars_fin {};
    std::vector<double> m_uncs_fin {};
    std::vector<std::vector<double>> m_cov_matrix {};
    std::vector<std::vector<double>> m_cor_matrix {};
    
    // Fit setup information
    int m_n_bins {};
    int m_n_free_pars {};
    
    // Minimization process information
    int m_n_fct_calls {}; // Number of function calls by minimizer
    int m_n_iters {};     // Number of iterations in minimization stepping
    
    // Fit quality measures
    double m_chisq_fin {};
    double m_edm_fin {}; // Expected distance from minimum
    int m_min_status {}; // Can be 0-6 (0=success,1&2&5=cov-problems,3=edm-above-max,4=call-limit-reached,6=unexpected)
    int m_cov_status {};
    
    bool operator==(const FitResult& result) const;
    bool operator!=(const FitResult& result) const;
  };
  
  using ResultVec = std::vector<Fit::FitResult>;
}
}

// Define operator for FitResult class
template<typename OStream>
OStream& operator<<(OStream& os, const PREW::Fit::FitResult& fr);

#include <Fit/FitResult.tpp>

#endif 