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
    
    // Fit quality measures
    double m_chisq_fin {};
    double m_edm_fin {}; // Expected distance from minimum
    int m_cov_status {};
    
    bool operator==(const FitResult& result) const;
    bool operator!=(const FitResult& result) const;
  };
  
}
}

// Define operator for FitResult class
template<typename OStream>
OStream& operator<<(OStream& os, const PREW::Fit::FitResult& fr);

#include <Fit/FitResult.tpp>

#endif 