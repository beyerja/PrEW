#ifndef LIB_FITRESULT_TPP
#define LIB_FITRESULT_TPP 1

#include <CppUtils/Str.h>
#include <Fit/FitResult.h>

#include "spdlog/fmt/ostr.h" // To allow output with spdlog

//------------------------------------------------------------------------------

template<typename OStream>
OStream& operator<<(OStream& os, const PrEW::Fit::FitResult& fr) {
  /** Define what happens if a stream operator is called on the FitResult class.
      (e.g. like `std::cout << fit_result;`)
  **/
  using namespace PrEW; // CppUtils::Str
  
  std::string output {};
  
  output.append("Fit setup info:\n");
  output.append("#bins: " + std::to_string(fr.m_n_bins) + "\n");
  output.append("#free pars: " + std::to_string(fr.m_n_free_pars) + "\n");
  output.append("\n");
  
  output.append("Parameters:\n");
  for (size_t par=0;par<fr.m_par_names.size();par++) {
    output.append(
      "p"  + std::to_string(par) 
      + " : " + fr.m_par_names.at(par)
      + " = " + CppUtils::Str::sci_string(fr.m_pars_fin.at(par)) 
      + " +- " + CppUtils::Str::sci_string(fr.m_uncs_fin.at(par))
      + "\n"
    ) ;  
  }
  output.append("\n");
  output.append("Covariance matrix:\n");
  if (fr.m_cov_matrix.size() != 0) {
    output.append("      ");
    for (size_t par=0;par<fr.m_cov_matrix[0].size();par++) {
      output.append("p" + std::to_string(par) + "    ");
    }
    output.append("\n");
  }
  for (size_t par1=0;par1<fr.m_cov_matrix.size();par1++) {
    output.append("p" + std::to_string(par1) + "    ");
    for (size_t par2=0;par2<fr.m_cov_matrix[par1].size();par2++) {
      // Covariance matrix needs scientific string notation due to small values
      output.append(
        CppUtils::Str::sci_string(fr.m_cov_matrix[par1][par2]) + "  "
      );
    }
    output.append("\n");
  }
  output.append("\n");
  output.append("Correlation matrix:\n");
  if (fr.m_cor_matrix.size() != 0) {
    output.append("      ");
    for (size_t par=0;par<fr.m_cor_matrix[0].size();par++) {
      output.append("p" + std::to_string(par) + "    ");
    }
    output.append("\n");
  }
  for (size_t par1=0;par1<fr.m_cor_matrix.size();par1++) {
    output.append("p" + std::to_string(par1) + "    ");
    for (size_t par2=0;par2<fr.m_cor_matrix[par1].size();par2++) {
      output.append(
        CppUtils::Str::sci_string(fr.m_cor_matrix[par1][par2]) + "  "
      );
    }
    output.append("\n");
  }
  output.append("\n");

  output.append("Minimization information:\n");
  output.append("#Fct.-calls = " + std::to_string(fr.m_n_fct_calls) + "\n");
  output.append("#Iterations = " + std::to_string(fr.m_n_iters) + "\n");
  
  output.append("Fit quality measures:\n");
  output.append("Chi^2 = " + CppUtils::Str::sci_string(fr.m_chisq_fin) + "\n");
  output.append("EDM   = " + CppUtils::Str::sci_string(fr.m_edm_fin) + "\n");
  output.append("Minimizer status : " + std::to_string(fr.m_min_status) + "\n");
  output.append("Cov. matrix status : " + std::to_string(fr.m_cov_status));
  
  // Feed the output string into the given stream
  os << output;
  return os;
}

//------------------------------------------------------------------------------


#endif 