#ifndef LIB_BINCOORD_H
#define LIB_BINCOORD_H 1

#include <vector>

namespace PrEW {
namespace Data {

class BinCoord {
  /** Class describing the coordinates of a single bin.
   **/

  std::vector<double> m_center{};
  std::vector<double> m_edge_low{};
  std::vector<double> m_edge_up{};

public:
  // Constructors
  BinCoord(std::vector<double> center = {}, std::vector<double> edge_low = {},
           std::vector<double> edge_up = {});

  // Access functions
  int get_dim() const;
  const std::vector<double> &get_center() const;
  const std::vector<double> &get_edge_low() const;
  const std::vector<double> &get_edge_up() const;

  // Operators
  bool operator==(const BinCoord &other) const;
  bool operator!=(const BinCoord &other) const;
};

using CoordVec = std::vector<BinCoord>;

} // namespace Data
} // namespace PrEW

#endif