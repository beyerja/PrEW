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

  // Markers used in column headers (defined in .cpp file)
  static const std::string BinCenterMarker;
  static const std::string BinLowMarker;
  static const std::string BinUpMarker;
  static const std::string CoefficientMarker;

  class CSVCoord {
    /** Small helper class that ensures the conventions for the coordinate data
     * naming is followed.
     **/
    std::string m_name{}; // coordinate name
  public:
    CSVCoord(std::string name) : m_name(name) {}
    const std::string &name() const { return m_name; }
    std::string bin_center() const { return BinCenterMarker + ":" + m_name; }
    std::string edge_low() const { return BinLowMarker + ":" + m_name; }
    std::string edge_up() const { return BinUpMarker + ":" + m_name; }
  };

public:
  // Constructors
  CSVInterpreter(const std::string &file_path);

  // Access functions
  const Data::DistrInfo &get_info() const;
  const Data::PredDistr &get_pred_distr() const;
  const Data::CoefDistrVec &get_coef_distrs() const;

protected:
  void read_metadata(const CSVMetadata &metadata);
  void read_csv(csv::CSVReader &csv_reader);

  std::vector<CSVCoord> find_coords(const std::vector<std::string> &col_names);
  std::vector<std::string>
  find_coefs(const std::vector<std::string> &col_names);

  bool col_fits_key(const std::string &col_name, const std::string &key);

  void check_coord(const CSVCoord &coord,
                   const std::vector<std::string> &col_names);
};

} // namespace Input
} // namespace PrEW

#endif