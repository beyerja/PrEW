// Includes from PrEW
#include <Data/BinCoord.h>
#include <Data/DistrUtils.h>
#include <Fit/FitBin.h>

#include <cmath>
#include <limits>
#include <vector>

namespace PrEW {
namespace Data {

//------------------------------------------------------------------------------

Data::BinCoord DistrUtils::bin_middle(const CoordVec &coords) {
  /** Find center of the bins in all axes, edges will be set to outermost
    *values.
   **/
  int n_dims = coords.at(0).get_dim();

  // Find minimum and maximum value of each axis and min,max edges
  auto coord_max = coords[0].get_center();
  auto coord_min = coords[0].get_center();
  auto edge_max = coords[0].get_edge_up();
  auto edge_min = coords[0].get_edge_low();

  for (const auto &bin : coords) {
    auto center = bin.get_center();
    auto edge_up = bin.get_edge_up();
    auto edge_low = bin.get_edge_low();
    for (int d = 0; d < n_dims; d++) {
      if (coord_max[d] < center[d]) {
        coord_max[d] = center[d];
      }
      if (coord_min[d] > center[d]) {
        coord_min[d] = center[d];
      }

      if (edge_max[d] < edge_up[d]) {
        edge_max[d] = edge_up[d];
      }
      if (edge_min[d] > edge_low[d]) {
        edge_min[d] = edge_low[d];
      }
    }
  }

  // Find middle
  std::vector<double> bin_middle(n_dims, 0);
  for (int d = 0; d < n_dims; d++) {
    bin_middle[d] = 0.5 * (coord_min[d] + coord_max[d]);
  }

  return BinCoord(bin_middle, edge_min, edge_max);
}

//------------------------------------------------------------------------------

DiffDistr DistrUtils::combine_bins(const DiffDistr &distr) {
  /** Combine all the bins of a given distributions into a single bin.
      Bin center is set to the center of each axis.
      Bin values are added up.
      Bin value uncertainties are combined in root-mean-square.
  **/
  int n_bins = distr.m_coords.size();

  // Vectors to find bin middle
  auto coord = {bin_middle(distr.m_coords)};

  // Average distribution value
  double val_comb = 0;

  // Combine bin uncertainties
  double unc_comb = 0;
  double unc_comb_sqr = 0;

  for (int b = 0; b < n_bins; b++) {
    val_comb += distr.m_distribution[b].get_val_mst();
    unc_comb_sqr += std::pow(distr.m_distribution[b].get_val_unc(), 2);
  }

  // Find final uncertainty
  unc_comb = std::sqrt(unc_comb_sqr);

  return DiffDistr{distr.m_info, coord, {Fit::FitBin(val_comb, unc_comb)}};
}

//------------------------------------------------------------------------------

PredDistr DistrUtils::combine_bins(const PredDistr &distr) {
  /** Combine all the bins of a given predicted distribution into a single bin.
      Bin center is set to the center of each axis.
      Bin values are added up.
  **/
  int n_bins = distr.m_coords.size();

  // Vectors to find bin middle
  auto coord = {bin_middle(distr.m_coords)};

  // Average distribution value
  double val_sig_comb = 0, val_bkg_comb = 0;

  for (int b = 0; b < n_bins; b++) {
    val_sig_comb += distr.m_sig_distr[b];
    val_bkg_comb += distr.m_bkg_distr[b];
  }

  return PredDistr{distr.m_info, coord, {val_sig_comb}, {val_bkg_comb}};
}

//------------------------------------------------------------------------------

PredDistrVec DistrUtils::combine_bins(const PredDistrVec &distrs) {
  /** Combine the bins for each distribution in the vector.
   **/
  PredDistrVec new_distrs{};
  for (const auto &distr : distrs) {
    new_distrs.push_back(combine_bins(distr));
  }
  return new_distrs;
}

//------------------------------------------------------------------------------

} // Namespace Data
} // Namespace PrEW