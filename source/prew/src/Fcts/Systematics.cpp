#include <Fcts/Systematics.h>

namespace PrEW {
namespace Fcts {

//------------------------------------------------------------------------------

double Systematics::polarisation_factor ( 
  const std::vector<double> &/*x*/,
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
  const std::vector<double> &/*x*/,
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
  const std::vector<double>   &x,
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
  
  double bin_center = x[int(c[0])];
  double bin_width = c[1];
  double bin_max = bin_center + bin_width/2.0;
  double bin_min = bin_center - bin_width/2.0;
  
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

} // Namespace Fcts
} // Namespace PrEW