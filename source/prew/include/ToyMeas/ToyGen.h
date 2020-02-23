#ifndef LIB_TOYGENERATOR_H
#define LIB_TOYGENERATOR_H 1

#include <Connect/DataConnector.h>
#include <Data/DiffDistr.h>
#include <Fit/FitPar.h>

#include <functional>

namespace PREW { 
namespace ToyMeas {

  class ToyGen {
    /** Class that can create a toy measurements from generator level 
        distributions by fluctuating the expected number of events within 
        their poissonian uncertainty.
    **/
    
    // Provided as input
    Connect::DataConnector  m_connector {};
    Fit::ParVec             m_pars {};
    
    // Created by ToyGen: diff. distrs. from predictions
    Data::DiffDistrVec      m_diff_distrs {};
    
    public:
      ToyGen(
        const Connect::DataConnector & connector,
        const Fit::ParVec & pars
      );
      
      Data::DiffDistrVec get_expected_distrs ( int energy ) const;
      Data::DiffDistrVec get_fluctuated_distrs ( int energy ) const;
  };
}
}

#endif