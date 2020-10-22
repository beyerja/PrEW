#include <Fcts/Systematics.h>

// Standard library
#include <cmath>

namespace PrEW {
namespace Fcts {

//------------------------------------------------------------------------------

double Systematics::polarisation_factor ( 
  const Data::BinCoord &/*x*/,
  const std::vector<double> &c,
  const std::vector<double*> &p
) {
  /** Calculate polarisation factor for given chirality (handled by 
      coeffficients) and polarisations (amplitude handled by parameters, 
      sign handled by coefficients).
      Coefficients: c[0] - electron chirality (-1 = L , +1 = R)
                    c[1] - positron chirality (-1 = L , +1 = R)
                    c[2] - electron beam polarisation sign
                    c[3] - positron beam polarisation sign
      Parameters: p[0] - electron beam polarisation amplitude
                  p[1] - positron beam polarisation amplitude
  **/
  
  return 0.25 * 
        ( 1 + c[0] * c[2] * (*(p[0])) ) * 
        ( 1 + c[1] * c[3] * (*(p[1])) );
}

//------------------------------------------------------------------------------

double Systematics::luminosity_fraction ( 
  const Data::BinCoord &/*x*/,
  const std::vector<double> &c,
  const std::vector<double*> &p
) {
  /** Calculate the fraction of the luminosity.
      Coefficients: c[0] - luminosity fraction
      Parameters: p[0] - total luminosity
  **/
  
  return c[0] * (*(p[0]));
}

//------------------------------------------------------------------------------

double Systematics::acceptance_box (
  const Data::BinCoord &x,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** Calculate factor from detector acceptance for box-like detector acceptance
      assumining that the distribution is constant within the bin.
      Coordinates: x[c[0]] - coordinate of the acceptance range
      Coefficients: c[0] - index of the relevant coordinate (must be int!)
                    c[1] - bin width
      Parameters: p[0] - center of box
                  p[1] - box width
  **/
  double box_center = *(p[0]);
  double box_width = *(p[1]);
  double edge_up = box_center + box_width/2.0;
  double edge_low = box_center - box_width/2.0;
  
  double coord = x.get_center()[int(c[0])];
  double bin_width = c[1];
  double bin_max = coord + bin_width/2.0;
  double bin_min = coord - bin_width/2.0;
  
  double factor = 0.0; // Default assumes x outside acceptance
  
  if ( ( edge_low > bin_min ) && ( edge_low < bin_max ) ) {
    // Lower edge within bin
    factor = ( bin_max - edge_low ) / bin_width;
  } else if ( ( edge_low <= bin_min ) && ( edge_up >= bin_max ) ) {
    // Bin in acceptance and edges not in bin
    factor = 1.0;
  } else if ( ( edge_up > bin_min) && ( edge_up < bin_max) ) {
    // Upper edge within bin
    factor = ( edge_up - bin_min ) / bin_width;
  }
  
  return factor;
}

//------------------------------------------------------------------------------

double Systematics::acceptance_box_polynomial (
  const Data::BinCoord &x,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** Calculate factor from detector acceptance for box-like detector 
      acceptance in a given coordinate.
      Small deviations of the edges on both sides of the box lead to deviations
      which are described by a second order polynomial. The coefficients for 
      that polynomial should be calculated from Monte Carlo events.
      Additional restrictions are put that bins completely outside (inside) of 
      acceptance are exactly 0 (1) and the factor only be between 0 and 1.
      Coordinates: x[c[0]] - coordinate of the acceptance range
      Coefficients: c[0] - index of the relevant coordinate (must be int!)
                    c[1] - the initial cut value (symmetric on + and - side)
                    c[2] - constant polynomial term
                    c[3] - linear polyn. term in center deviation
                    c[4] - linear polyn. term in width deviation
                    c[5] - quadratic polyn. term in center deviation
                    c[6] - quadratic polyn. term in width deviation
                    c[7] - mixed polynomial term in width and center deviations
      Parameters: p[0] - deviation in the box center
                  p[1] - deviation in the box width
  **/
  int coord_index = int(c[0]);
  double edge_low = x.get_edge_low()[coord_index];
  double edge_up = x.get_edge_up()[coord_index];
  
  double initial_cut = c[1];
  double dc = (*(p[0]));
  double dw = (*(p[1]));
  double cut_low = - initial_cut + dc - dw/2.0;
  double cut_up = initial_cut + dc + dw/2.0;
  
  double factor = 0;
  
  if ( edge_up < cut_low ) {
    // Bin completely outside of acceptance on negative side
    factor = 0.0;
  } else if ( edge_low > cut_up ) {
    // Bin completely outside of acceptance on positive side
    factor = 0.0;
  } else if ( (edge_low > cut_low) && (edge_up < cut_up) ) {
    // Bin completely inside acceptance
    factor = 1.0;
  } else {
    // Bin is partially in acceptance, apply polynomial
    factor = c[2] 
             + c[3] * dc + c[4] * dw
             + c[5] * std::pow(dc,2) + c[6] * std::pow(dw,2)
             + c[7] * dc * dw;
  }
  
  // Enforce that the factor can only be between 0 and 1
  if (factor > 1) {
    factor = 1.0;
  } else if (factor < 0) {
    factor = 0.0;
  }
  
  return factor;
}

//------------------------------------------------------------------------------

} // Namespace Fcts
} // Namespace PrEW