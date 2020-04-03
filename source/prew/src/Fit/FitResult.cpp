#include <Fit/FitResult.h>
#include <CppUtils/Num.h>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Operators

bool FitResult::operator==(const FitResult& result) const {
  bool are_equal = true &&
    ( this->m_par_names == result.m_par_names ) &&
    ( this->m_pars_fin == result.m_pars_fin ) &&
    ( this->m_uncs_fin == result.m_uncs_fin ) &&
    ( this->m_n_bins == result.m_n_bins ) &&
    ( this->m_n_free_pars == result.m_n_free_pars ) &&
    ( this->m_n_fct_calls == result.m_n_fct_calls ) &&
    ( this->m_n_iters == result.m_n_iters ) &&
    CppUtils::Num::equal_to_eps( this->m_chisq_fin, result.m_chisq_fin )  &&
    CppUtils::Num::equal_to_eps( this->m_edm_fin, result.m_edm_fin )  &&
    ( this->m_min_status == result.m_min_status ) &&
    ( this->m_cov_status == result.m_cov_status ) &&
    ( this->m_cov_matrix.size() == result.m_cov_matrix.size() ) &&
    ( this->m_cor_matrix.size() == result.m_cor_matrix.size() );
  
  if (are_equal) {  
    // Check if cov. and cor. matrix are fully equal
    for (unsigned int i=0; i<this->m_cov_matrix.size(); i++) {
      are_equal = are_equal && 
        (this->m_cov_matrix[i] == result.m_cov_matrix[i]);
    }
    
    for (unsigned int i=0; i<this->m_cor_matrix.size(); i++) {
      are_equal = are_equal && 
        (this->m_cor_matrix[i] == result.m_cor_matrix[i]);
    }
  }
  
  return are_equal;
}

bool FitResult::operator!=(const FitResult& result) const {
  return ! (*this == result);
}

//------------------------------------------------------------------------------

}
}