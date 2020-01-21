#ifndef LIBRARY_DATAREADER_H
#define LIBRARY_DATAREADER_H 1

#include <Data/Distr1D.h>
#include <Input/InputInfo.h>
#include <Input/Reading.h>


namespace PREW {
namespace Input {
  
  class DataReader {
    /** General data reader class to get data from an input file.
        -> Only does general checks, e.g. if file exists.
    **/
    
    InputInfo *m_input_info {};
    std::string m_input_style {};
    Data::Distr1DVec m_distributions {};
    
    public:
      // Constructors
      DataReader(InputInfo *input_info, std::string input_style);
      
      void read_file();
      Data::Distr1DVec get_distributions() const;
  };
  
}
}

#endif