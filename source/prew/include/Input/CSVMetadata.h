#ifndef LIB_CSVMETADATA_H
#define LIB_CSVMETADATA_H 1

// Standard library
#include <map>
#include <string>

#include "csv.hpp"

namespace PrEW {
namespace Input {

class CSVMetadata {
  /** Class to strip and interpret the custom metadata from an (otherwise) CSV
   *file.
   **/

  // Identifiers for metadata header
  const std::string begin_ID{"#BEGIN-METADATA"};
  const std::string end_ID{"#END-METADATA"};
  const std::map<std::string, std::string> metadata_IDs{
      {"Name", "name"},
      {"Energy", "energy"},
      {"e-Chirality", "e- chirality"},
      {"e+Chirality", "e+ chirality"}};
  const std::string coef_ID{"Coef"};

  // Map holding the metadata strings by its identifiers (to be interpreted)
  std::map<std::string, std::string> m_metadata{};

public:
  // Constructors
  CSVMetadata(){};

  // Core functionality
  csv::CSVReader strip_metadata(const std::string &file_path);

  // Access functions
  template <class OutClass> OutClass get(const std::string &name) const;
  std::vector<std::string> keys() const;

protected:
  bool ID_is_coef(const std::string & ID_str);
  void interpret(const std::vector<std::string> &metadata_lines);
  std::vector<std::string>
  get_metadata_lines(const std::string &file_path) const;
};

} // namespace Input
} // namespace PrEW

#include "Input/CSVMetadata.tpp"

#endif