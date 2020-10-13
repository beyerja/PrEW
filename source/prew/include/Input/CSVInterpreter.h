#ifndef LIB_CSVINTERPRETER_H
#define LIB_CSVINTERPRETER_H 1

#include <Data/CoefDistr.h>
#include <Data/DistrInfo.h>
#include <Data/PredDistr.h>
#include <Input/CSVMetadata.h>

// Standard library
#include <string>
#include <vector>

#include "csv.hpp"

namespace PrEW {
namespace Input {

class CSVInterpreter {
  /** Class that interprets a CSV file containing all the information for one
   *distribution and the metadata of the distribution.
   **/

  Data::DistrInfo m_info{};
  Data::PredDistr m_pred_distr{};
  Data::CoefDistrVec m_coef_distrs{};
  
  // Markers used in column headers
  const std::string BinCenterMarker {"BinCenters"};
  const std::string CoefficientMarker {"Coef"};

public:
  // Constructors
  CSVInterpreter(const std::string &file_path);
  
  // Access functions
  const Data::DistrInfo & get_info() const;
  const Data::PredDistr & get_pred_distr() const;
  const Data::CoefDistrVec & get_coef_distrs() const;

protected:
  void read_metadata(const CSVMetadata &metadata);
  void read_csv(csv::CSVReader &csv_reader);

  std::vector<std::string>
  find_bin_centers(const std::vector<std::string> &col_names);
  std::vector<std::string>
  find_coefs(const std::vector<std::string> &col_names);

  bool col_fits_key(const std::string &col_name, const std::string &key);
};

} // namespace Input
} // namespace PrEW

#endif