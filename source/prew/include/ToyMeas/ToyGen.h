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
    

    // TODO 
    // Similar to DataToFit
    // Takes same input expect: No measurement & no output pointer
    // => Creates its own internal FitContainer (with correctly connected FitBin predictions)
    //      -> NOPE, needs to use combination of DiffDistrVec and ParVec instead because connection to DistrInfo still needed
    // => Has function that creates vector DiffDistrVec in which predictions of FitBins are varied with Poissonian
    // => Has function that can manipulate one of the FitPar values (by name of FitPar)
    // TODO
    
    // TODO
    // The function to create a toy distribution must take energy as input argument!
    // TODO
    
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