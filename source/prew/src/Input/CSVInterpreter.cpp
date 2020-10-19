#include <CppUtils/Str.h>
#include <CppUtils/Vec.h>
#include <GlobalVar/Chiral.h>
#include <Input/CSVInterpreter.h>

// Standard library
#include <fstream>

namespace PrEW {
namespace Input {

//------------------------------------------------------------------------------
// Marker definition

const std::string CSVInterpreter::BinCenterMarker = "BinCenters";
const std::string CSVInterpreter::BinLowMarker = "BinUp";
const std::string CSVInterpreter::BinUpMarker = "BinLow";
const std::string CSVInterpreter::CoefficientMarker = "Coef";

//------------------------------------------------------------------------------

CSVInterpreter::CSVInterpreter(const std::string &file_path) {
  /** Constructor interprets the given CSV file (metadata and distribution).
   **/

  CSVMetadata metadata{};
  auto csv_reader = metadata.strip_metadata(file_path);

  this->read_metadata(metadata);
  this->read_csv(csv_reader);
}

//------------------------------------------------------------------------------

const Data::DistrInfo &CSVInterpreter::get_info() const { return m_info; }

const Data::PredDistr &CSVInterpreter::get_pred_distr() const {
  return m_pred_distr;
}

const Data::CoefDistrVec &CSVInterpreter::get_coef_distrs() const {
  return m_coef_distrs;
}

//------------------------------------------------------------------------------

void CSVInterpreter::read_metadata(const CSVMetadata &metadata) {
  /** Use the given metadata to read the distribution info.
   **/
  m_info.m_distr_name = metadata.get<std::string>("name");
  m_info.m_energy = metadata.get<int>("energy");
  m_info.m_pol_config = GlobalVar::Chiral::transform(
      metadata.get<int>("e- chirality"), metadata.get<int>("e+ chirality"));
}

//------------------------------------------------------------------------------

void CSVInterpreter::read_csv(csv::CSVReader &csv_reader) {
  /** Read the differential distribution and coefficients from the given
   *CSVReader.
   **/

  auto col_names = csv_reader.get_col_names();
  auto csv_coords = this->find_coords(col_names);
  auto coef_cols = this->find_coefs(col_names);

  Data::CoordVec coords{};
  std::vector<double> bin_values{};
  std::map<std::string, std::vector<double>> coef_values{};

  // Initialize coefficients vectors
  for (const auto &coef_col : coef_cols) {
    coef_values[coef_col] = {};
  }

  // Collect all the values for the predicted distributions and coefficients
  for (auto &row : csv_reader) {
    // Collect bin centers for this bin
    std::vector<double> bin_centers{}, edges_low{}, edges_up{};
    for (const auto &csv_coord : csv_coords) {
      bin_centers.push_back(row[csv_coord.bin_center()].get<double>());
      edges_low.push_back(row[csv_coord.edge_low()].get<double>());
      edges_up.push_back(row[csv_coord.edge_up()].get<double>());
    }
    coords.push_back(Data::BinCoord(bin_centers,edges_low,edges_up));

    // Collect bin values
    bin_values.push_back(row["Cross sections"].get<double>());

    // Find coefficient vectors
    for (const auto &coef_col : coef_cols) {
      coef_values[coef_col].push_back(row[coef_col].get<double>());
    }
  }

  // Construct the predicted distribution (no backgrounds contained in CSV file)
  m_pred_distr = Data::PredDistr{m_info, coords, bin_values,
                                 std::vector<double>(bin_values.size(), 0.0)};

  // Construct coefficient distributions
  for (const auto &coef_col : coef_cols) {
    auto name = CppUtils::Str::string_to_vec(coef_col, ":").at(1);
    m_coef_distrs.push_back(
        Data::CoefDistr(name, m_info, coef_values.at(coef_col)));
  }
}

//------------------------------------------------------------------------------

std::vector<CSVInterpreter::CSVCoord>
CSVInterpreter::find_coords(const std::vector<std::string> &col_names) {
  /** Find the bin coordinates contained in the column headers.
   **/
  // Find all coordinates using the bin center marker
  auto condition = [this](const std::string &col_name) {
    return this->col_fits_key(col_name, BinCenterMarker);
  };
  auto center_col_names =
      CppUtils::Vec::subvec_by_condition(col_names, condition);

  // Strip the center marker of the name and create coord object with pure name
  std::vector<CSVInterpreter::CSVCoord> coords{};
  for (const auto &center_col_name : center_col_names) {
    auto pure_name = CppUtils::Str::string_to_vec(center_col_name, ":").at(1);
    CSVInterpreter::CSVCoord new_coord(pure_name);
    this->check_coord(new_coord, col_names); // Check if all columns are there
    coords.push_back(new_coord);
  }

  return coords;
}

//------------------------------------------------------------------------------

std::vector<std::string>
CSVInterpreter::find_coefs(const std::vector<std::string> &col_names) {
  /** Find the names of coefficients contained in the column headers.
   **/
  auto condition = [this](const std::string &col_name) {
    return this->col_fits_key(col_name, CoefficientMarker);
  };
  return CppUtils::Vec::subvec_by_condition(col_names, condition);
}

//------------------------------------------------------------------------------

bool CSVInterpreter::col_fits_key(const std::string &col_name,
                                  const std::string &key) {
  /** Check if the given column header name fits the given key like "key:...".
   **/
  bool fits = false;
  auto split_str = CppUtils::Str::string_to_vec(col_name, ":");

  if ((split_str.size() == 2) && (split_str.at(0) == key)) {
    fits = true;
  }

  return fits;
}

//------------------------------------------------------------------------------

void CSVInterpreter::check_coord(const CSVCoord &coord,
                                 const std::vector<std::string> &col_names) {
  /** Check that the given columns contain all needed columns to describe the
   *given coordinate.
   **/
  bool has_center_col = false;
  bool has_low_col = false;
  bool has_up_col = false;
  for (const auto &col_name : col_names) {
    if (col_name == coord.bin_center()) {
      has_center_col = true;
    } else if (col_name == coord.edge_up()) {
      has_up_col = true;
    } else if (col_name == coord.edge_low()) {
      has_low_col = true;
    }
  }

  if (!has_center_col) {
    throw std::invalid_argument("Missing bin center column for coordinate " +
                                coord.name());
  }
  if (!has_low_col) {
    throw std::invalid_argument("Missing lower edge column for coordinate " +
                                coord.name());
  }
  if (!has_up_col) {
    throw std::invalid_argument("Missing upper edge column for coordinate " +
                                coord.name());
  }
}

//------------------------------------------------------------------------------

} // namespace Input
} // namespace PrEW