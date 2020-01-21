#include <CppUtils/Sys.h>
#include <Input/DataReader.h>

#include <exception>

namespace PREW {
namespace Input {
  
//------------------------------------------------------------------------------
// Constructors

DataReader::DataReader(InputInfo *input_info, std::string input_style) :
  m_input_info(input_info), m_input_style(input_style) 
{
  if ( ! CppUtils::Sys::file_exists(input_info->m_file_path) ) {
    throw std::invalid_argument(("File not found: " + input_info->m_file_path).c_str());
  }
}

//------------------------------------------------------------------------------

void DataReader::read_file() {
  /** Read the file whose info is provided in input_info according to the 
      given style of input file.
  **/
  
  if ( m_input_style == "RK" ) {
    m_distributions = Reading::read_RK_file(m_input_info);
  } else {
    throw std::invalid_argument(("Invalid file style " + m_input_style).c_str());
  }
}


//------------------------------------------------------------------------------

Data::Distr1DVec DataReader::get_distributions() const {
  return m_distributions;
}

//------------------------------------------------------------------------------

}
}