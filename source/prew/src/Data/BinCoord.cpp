#include <Data/BinCoord.h>

#include <stdexcept>

namespace PrEW {
namespace Data {

//------------------------------------------------------------------------------
// Constructors

BinCoord::BinCoord(std::vector<double> center, std::vector<double> edge_low,
                   std::vector<double> edge_up)
    : m_center(center), m_edge_low(edge_low), m_edge_up(edge_up) {
  /** Constructor checks that the coordinate is self-consistent.
   **/
  if ((m_center.size() != m_edge_low.size()) ||
      (m_center.size() != m_edge_up.size())) {
    throw std::invalid_argument("Size of coordinate vectors is inconsistent.");
  }
}

//------------------------------------------------------------------------------
// Access functions

int BinCoord::get_dim() const { return m_center.size(); }
const std::vector<double> &BinCoord::get_center() const { return m_center; }
const std::vector<double> &BinCoord::get_edge_low() const { return m_edge_low; }
const std::vector<double> &BinCoord::get_edge_up() const { return m_edge_up; }

//------------------------------------------------------------------------------
// Operators

bool BinCoord::operator==(const BinCoord &other) const {
  return (m_center == other.get_center()) &&
         (m_edge_low == other.get_edge_low()) &&
         (m_edge_up == other.get_edge_up());
}

bool BinCoord::operator!=(const BinCoord &other) const {
  return !(*this == other);
}

//------------------------------------------------------------------------------

} // namespace Data
} // namespace PrEW