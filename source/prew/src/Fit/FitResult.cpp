#include <Fit/FitResult.h>
#include <CppUtils/Num.h>

namespace PREW {
namespace Fit {

//------------------------------------------------------------------------------
// Constructors

bool FitResult::operator==(const FitResult& result) const {
  bool are_equal = true &&
    ( this->m_par_names == result.m_par_names ) &&
    ( this->m_pars_fin == result.m_pars_fin ) &&
    ( this->m_uncs_fin == result.m_uncs_fin ) &&
    CppUtils::Num::equal_to_eps( this->m_chisq_fin, result.m_chisq_fin )  &&
    CppUtils::Num::equal_to_eps( this->m_edm_fin, result.m_edm_fin )  &&
    ( this->m_cov_status == result.m_cov_status ) &&
    ( this->m_cov_matrix.size() == result.m_cov_matrix.size() ) &&
    ( this->m_cor_matrix.size() == result.m_cor_matrix.size() );
    
  for (unsigned int i=0; i<this->m_cov_matrix.size(); i++) {
    are_equal = are_equal && 
      (this->m_cov_matrix[i] == result.m_cov_matrix[i]);
  }
  
  for (unsigned int i=0; i<this->m_cor_matrix.size(); i++) {
    are_equal = are_equal && 
      (this->m_cor_matrix[i] == result.m_cor_matrix[i]);
  }
  
  return are_equal;
}

//------------------------------------------------------------------------------

}
}