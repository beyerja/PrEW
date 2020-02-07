#ifndef LIB_DATACONNECTOR_H
#define LIB_DATACONNECTOR_H 1

#include <Data/CoefDistr.h>
#include <Data/DiffDistr.h>
#include <Data/PolLink.h>
#include <Data/PredDistr.h>
#include <Data/PredLink.h>
#include <Fit/FitContainer.h>

namespace PREW {
namespace Connect {
  
  class DataConnector {
    /** Class that stores all the information supplied by input files and
        instructions and uses it to form fitable bins from given distributions.
    **/
    
    // Store predictions, coefficients and linking instructions
    // => Everything that is needed to properly connect bins of a distribution
    Data::PredDistrVec m_pred_distrs {};
    Data::CoefDistrVec m_coef_distrs {};
    Data::PredLinkVec  m_pred_links {};
    Data::PolLinkVec   m_pol_links {};
    
    public:
      // Constructor
      DataConnector (
        Data::PredDistrVec pred_distrs = {},
        Data::CoefDistrVec coef_distrs = {},
        Data::PredLinkVec  pred_links  = {},
        Data::PolLinkVec   pol_links   = {}
      );
      
      // Core functionality: providing connected bins
      void fill_bins(
        const Data::DiffDistr & diff_distr,
        Fit::ParVec *pars,
        Fit::BinVec *bins
      ) const;
      
      void fill_fit_container(
        const Data::DiffDistrVec & diff_distrs,
        const Fit::ParVec        & pars,
        Fit::FitContainer *fit_container 
      ) const;
      
      // Read functions
      const Data::PredDistrVec& get_pred_distrs() const;
      const Data::CoefDistrVec& get_coef_distrs() const;
      const Data::PredLinkVec&  get_pred_links() const;
      const Data::PolLinkVec&   get_pol_links() const;
  };
  
}
}

#endif