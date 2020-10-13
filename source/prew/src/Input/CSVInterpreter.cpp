#include <CppUtils/Str.h>
#include <CppUtils/Vec.h>
#include <GlobalVar/Chiral.h>
#include <Input/CSVInterpreter.h>

// Standard library
#include <fstream>

namespace PrEW {
namespace Input {

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
  auto bin_center_cols = this->find_bin_centers(col_names);
  auto coef_cols = this->find_coefs(col_names);

  //
  CppUtils::Vec::Matrix2D<double> bin_centers{};
  std::vector<double> bin_values{};
  std::map<std::string, std::vector<double>> coef_values{};

  // Initialize coefficients vectors
  for (const auto &coef_col : coef_cols) {
    coef_values[coef_col] = {};
  }

  // Collect all the values for the predicted distributions and coefficients
  for (auto &row : csv_reader) {
    // Collect bin centers for this bin
    std::vector<double> bin_center{};
    for (const auto &bin_center_col : bin_center_cols) {
      bin_center.push_back(row[bin_center_col].get<double>());
    }
    bin_centers.push_back(bin_center);

    // Collect bin values
    bin_values.push_back(row["Cross sections"].get<double>());

    // Find coefficient vectors
    for (const auto &coef_col : coef_cols) {
      coef_values[coef_col].push_back(row[coef_col].get<double>());
    }
  }

  // Construct the predicted distribution (no backgrounds contained in CSV file)
  m_pred_distr = Data::PredDistr{m_info, bin_centers, bin_values,
                                 std::vector<double>(bin_values.size(), 0.0)};

  // Construct coefficient distributions
  for (const auto &coef_col : coef_cols) {
    auto name = CppUtils::Str::string_to_vec(coef_col, ":").at(1);
    m_coef_distrs.push_back(
        Data::CoefDistr(name, m_info, coef_values.at(coef_col)));
  }
}

//------------------------------------------------------------------------------

std::vector<std::string>
CSVInterpreter::find_bin_centers(const std::vector<std::string> &col_names) {
  /** Find the names of bin centers contained in the column headers.
   **/
  auto condition = [this](const std::string &col_name) {
    return this->col_fits_key(col_name, BinCenterMarker);
  };
  return CppUtils::Vec::subvec_by_condition(col_names, condition);
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

} // namespace Input
} // namespace PrEW