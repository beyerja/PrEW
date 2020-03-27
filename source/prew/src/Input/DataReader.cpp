#include <CppUtils/Sys.h>
#include <Input/DataReader.h>

#include <exception>
#include "spdlog/spdlog.h"

namespace PREW {
namespace Input {
  
//------------------------------------------------------------------------------
// Constructors

DataReader::DataReader(InputInfo *input_info) :
  m_input_info(input_info)
{
  if ( ! CppUtils::Sys::path_exists(input_info->m_file_path) ) {
    throw std::invalid_argument(("File not found: " + input_info->m_file_path).c_str());
  }
}

//------------------------------------------------------------------------------

void DataReader::read_file() {
  /** Read the file whose info is provided in input_info according to the 
      given style of input file.
      Could read measurement, prediction and/or coefficients depending on file
      type.
  **/
  
  if ( m_input_info->m_input_style == "RK" ) {
    spdlog::debug("Reading RK style file, these only contain predictions and coefficients, no measurement.");
    Reading::read_RK_file(m_input_info, &m_pred_distrs, &m_coef_distrs);
  } else {
    throw std::invalid_argument(
      ("Invalid file style " + m_input_info->m_input_style).c_str() );
  }
}


//------------------------------------------------------------------------------

const Data::DiffDistrVec& DataReader::get_meas_distrs() const {
 return m_meas_distrs; 
}

const Data::PredDistrVec& DataReader::get_pred_distrs() const {
 return m_pred_distrs; 
}

const Data::CoefDistrVec& DataReader::get_coef_distrs() const {
 return m_coef_distrs; 
}

//------------------------------------------------------------------------------

}
}