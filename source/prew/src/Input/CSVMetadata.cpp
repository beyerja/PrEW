#include <CppUtils/Str.h>
#include <CppUtils/Sys.h>
#include <Input/CSVMetadata.h>

// Standard library
#include <fstream>

namespace PrEW {
namespace Input {

//------------------------------------------------------------------------------

csv::CSVReader CSVMetadata::strip_metadata(const std::string &file_path) {
  /** Read the given CSV file, strip the metadata from it and return the CSV
   *file without the metadata header.
   **/
  // Check that file exists
  if (!CppUtils::Sys::path_exists(file_path)) {
    throw std::invalid_argument("CSV file does not exist: " + file_path);
  }

  auto metadata_lines = this->get_metadata_lines(file_path);

  this->interpret(metadata_lines); // Find info from metadata lines

  // Return a CSVReader that knows where to start reading
  int header_size = metadata_lines.size() + 2; // Include header ID lines
  csv::CSVFormat format;
  format.header_row(header_size);
  return csv::CSVReader(file_path, format);
}

//------------------------------------------------------------------------------

std::vector<std::string> CSVMetadata::keys() const {
  /** Return the keys of all the available metadata from the header.
   **/
  std::vector<std::string> keys{};
  for (const auto &[key, _] : m_metadata) {
    keys.push_back(key);
  }
  return keys;
}

//------------------------------------------------------------------------------

void CSVMetadata::interpret(const std::vector<std::string> &metadata_lines) {
  /** Interpret the header lines.
   **/
  for (const auto &line : metadata_lines) {
    auto split_line = CppUtils::Str::string_to_vec(line, " ");

    // Only expect identifier and value, no other blank spaces!
    if (split_line.size() != 2) {
      throw std::invalid_argument(
          "CSV metadata header line not of size 2 but " +
          std::to_string(split_line.size()));
    }

    // Remove trailing ":" from ID
    auto ID_str = CppUtils::Str::string_to_vec(split_line[0], ":").at(0);
    auto value = split_line[1];

    if (CSVMetadata::metadata_IDs.find(ID_str) ==
        CSVMetadata::metadata_IDs.end()) {
      throw std::invalid_argument("Unknown CSV metadata ID: " + ID_str);
    }

    // Store the value
    m_metadata[CSVMetadata::metadata_IDs.at(ID_str)] = value;
  }
}

//------------------------------------------------------------------------------

std::vector<std::string>
CSVMetadata::get_metadata_lines(const std::string &file_path) const {
  /** Find the lines describing the metadata other an otherwise csv-compatible
   *file.
   **/
  std::string current_line{};
  std::vector<std::string> metadata_lines{};
  std::ifstream file(file_path.c_str());
  bool in_metadata = false;
  while (getline(file, current_line)) {
    if (current_line == CSVMetadata::end_ID) {
      break; // Found end, time to return
    } else if (in_metadata) {
      metadata_lines.push_back(current_line); // Current line is metadata, keep
    } else if (current_line == CSVMetadata::begin_ID) {
      in_metadata = true;
    } else {
      throw std::invalid_argument(
          "First line isn't CSV metadata header, this is "
          "currently not implemented: " +
          file_path);
    }
  }
  file.close();

  return metadata_lines;
}

//------------------------------------------------------------------------------

} // namespace Input
} // namespace PrEW