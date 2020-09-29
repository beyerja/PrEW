#ifndef LIB_CSVMETADATA_TPP
#define LIB_CSVMETADATA_TPP 1

#include "CppUtils/Str.h"
#include "Input/CSVMetadata.h"

// Standard library
#include <typeinfo>

namespace PrEW {
namespace Input {

//------------------------------------------------------------------------------

template <class OutClass>
OutClass CSVMetadata::get(const std::string &name) const {
  /** Find the requested element and cast it to the requested output format.
   **/
  if (m_metadata.find(name) == m_metadata.end()) {
    throw std::invalid_argument("Metadata parameter wasn't found: " + name);
  }

  // Check that a valid type is requested
  std::string type = typeid(OutClass).name();
  if ((type != typeid(double(0.0)).name()) &&
      (type != typeid(float(0.0)).name()) &&
      (type != typeid(int(0.0)).name())) {
    throw std::invalid_argument("Asking for unknown output type: " + type);
  }

  auto element_str = m_metadata.at(name); // Return value in string form
  // Cast the string into the correct type of class
  return CppUtils::Str::cast_string<OutClass>(element_str);
}

//------------------------------------------------------------------------------

template <>
std::string inline CSVMetadata::get<std::string>(
    const std::string &name) const {
  /** Find the requested string element.
   **/
  if (m_metadata.find(name) == m_metadata.end()) {
    throw std::invalid_argument("Metadata parameter wasn't found: " + name);
  }

  return m_metadata.at(name); // Return value in string form
}

//------------------------------------------------------------------------------

} // namespace Input
} // namespace PrEW

#endif