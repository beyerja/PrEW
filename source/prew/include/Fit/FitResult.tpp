#ifndef LIB_FITRESULT_TPP
#define LIB_FITRESULT_TPP 1

#include <Fit/FitResult.h>

#include "spdlog/fmt/ostr.h" // To allow output with spdlog

//------------------------------------------------------------------------------

template<typename OStream>
OStream& operator<<(OStream& os, const PREW::Fit::FitResult& fr) {
  /** Define what happens if a stream operator is called on the FitResult class.
      (e.g. like `std::cout << fit_result;`)
  **/
  std::string output {};
  output.append("Parameters:\n");
  for (size_t par=0;par<fr.m_par_names.size();par++) {
    output.append(
      "p"  + std::to_string(par) 
      + " : " + fr.m_par_names.at(par)
      + " = " + std::to_string(fr.m_pars_fin.at(par)) 
      + " +- " + std::to_string(fr.m_uncs_fin.at(par))
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
      output.append(std::to_string(fr.m_cov_matrix[par1][par2]) + "  ");
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
      output.append(std::to_string(fr.m_cor_matrix[par1][par2]) + "  ");
    }
    output.append("\n");
  }
  output.append("\n");

  output.append("Fit quality measures:\n");
  output.append("Chi^2 = " + std::to_string(fr.m_chisq_fin) + "\n");
  output.append("EDM   = " + std::to_string(fr.m_edm_fin) + "\n");
  output.append("Cov. matrix status : " + std::to_string(fr.m_cov_status));
  
  // Feed the output string into the given stream
  os << output;
  return os;
}

//------------------------------------------------------------------------------


#endif 