#ifndef LIBRARY_DATAREADER_H
#define LIBRARY_DATAREADER_H 1

#include <Data/DiffDistr.h>
#include <Data/PredDistr.h>
#include <Input/InputInfo.h>
#include <Input/Reading.h>


namespace PrEW {
namespace Input {
  
  class DataReader {
    /** General data reader class to get data from an input file.
        Three types of data can be of relevance for a fit and may be extracted
        depending on the file type:
        - Measured differential distributions (in number of events!)
          with uncorrelated uncertainties
        - Predicted chiral distributions
        - Coefficients of parameterisation of the prediction
    **/
    
    InputInfo *m_input_info {};
    Data::DiffDistrVec m_meas_distrs {};
    Data::PredDistrVec m_pred_distrs {};
    Data::CoefDistrVec m_coef_distrs {};
    
    public:
      // Constructors
      DataReader(InputInfo *input_info);
      
      void read_file();
      const Data::DiffDistrVec& get_meas_distrs() const;
      const Data::PredDistrVec& get_pred_distrs() const;
      const Data::CoefDistrVec& get_coef_distrs() const;
  };
  
}
}

#endif