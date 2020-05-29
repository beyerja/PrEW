// Includes from PrEW
#include <Data/DistrUtils.h>
#include <Fit/FitBin.h>

#include <cmath>
#include <limits>
#include <vector>

namespace PrEW {
namespace Data {

//------------------------------------------------------------------------------

std::vector<double>
DistrUtils::bin_middle(const CppUtils::Vec::Matrix2D<double> &bin_centers) {
  /** Find center of the bins in all axes.
   **/
  int n_dims = bin_centers.at(0).size();

  // Find minimum and maximum value of each axis
  std::vector<double> bin_center_max = bin_centers[0];
  std::vector<double> bin_center_min = bin_centers[0];
  std::vector<double> bin_middle(n_dims, 0);

  for (const auto & bin: bin_centers) {
    for (int d = 0; d < n_dims; d++) {
      if (bin_center_max[d] < bin[d]) { bin_center_max[d] = bin[d]; }
      if (bin_center_min[d] > bin[d]) { bin_center_min[d] = bin[d]; }
    }
  }

  // Find middle
  for (int d = 0; d < n_dims; d++) {
    bin_middle[d] = 0.5 * (bin_center_min[d] + bin_center_max[d]);
  }
  
  return bin_middle;
}

//------------------------------------------------------------------------------

DiffDistr DistrUtils::combine_bins(const DiffDistr &distr) {
  /** Combine all the bins of a given distributions into a single bin.
      Bin center is set to the center of each axis.
      Bin values are added up.
      Bin value uncertainties are combined in root-mean-square.
  **/
  int n_bins = distr.m_bin_centers.size();

  // Vectors to find bin middle
  CppUtils::Vec::Matrix2D<double> bin_center = 
    { bin_middle(distr.m_bin_centers) };

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

  return DiffDistr{distr.m_info, bin_center, {Fit::FitBin(val_comb, unc_comb)}};
}

//------------------------------------------------------------------------------

PredDistr DistrUtils::combine_bins(const PredDistr &distr) {
  /** Combine all the bins of a given predicted distribution into a single bin.
      Bin center is set to the center of each axis.
      Bin values are added up.
  **/
  int n_bins = distr.m_bin_centers.size();

  // Vectors to find bin middle
  CppUtils::Vec::Matrix2D<double> bin_center = 
    { bin_middle(distr.m_bin_centers) };

  // Average distribution value
  double val_sig_comb = 0, val_bkg_comb = 0;

  for (int b = 0; b < n_bins; b++) {
    val_sig_comb += distr.m_sig_distr[b];
    val_bkg_comb += distr.m_bkg_distr[b];
  }

  return PredDistr{distr.m_info, bin_center, {val_sig_comb}, {val_bkg_comb}};
}

//------------------------------------------------------------------------------

} // Namespace Data
} // Namespace PrEW