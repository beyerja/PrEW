#include <Fcts/Physics.h>

#include <math.h>

namespace PrEW {
namespace Fcts {

//------------------------------------------------------------------------------
/** Chiral cross section scaling factors accounting for an the chiral asymmetry
    in the case that only two chiral cross sections are allowed.
    The two factors are chosen such that the sum of the chiral cross sections
    is not modified, while the asymmetry is modified like:
      A = (xs0 - xs1)/(xs0 + xs1) -> A' = A + DeltaA
**/

double Physics::asymm_2chixs_a0 (
  const Data::BinCoord   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs0.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
      Parameters: p[0] - Change in asymmetry DeltaA
  **/
  return 1 + 0.5 * ( 1 + c[1]/c[0]) * (*(p[0]));
}
                        
double Physics::asymm_2chixs_a1 (
  const Data::BinCoord &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs1.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
      Parameters: p[0] - Change in asymmetry DeltaA
  **/
  return 1 - 0.5 * ( 1 + c[0]/c[1]) * (*(p[0]));
}
                        
//------------------------------------------------------------------------------
/** Chiral cross section scaling factors accounting for the chiral asymmetries
    in the case that three chiral cross sections are allowed.
    The asymmtries are choosen such that they reduce to the 2-cross section 
    case in case the third cross section goes to 0.
      A_I   = ((xs0+xs2) - xs1) / (xs0+xs1+xs2)
      A_II  = (xs0 - (xs1+xs2)) / (xs0+xs1+xs2)
    The factors represent a shift in the asymmetry.
      A_x -> A'_x = A_x + DeltaA_x
**/

double Physics::asymm_3chixs_a0 (
  const Data::BinCoord &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs0.
      Coefficients: c[0] - inital (SM) value for chiral cross section xs0
                    c[1] - inital (SM) value for chiral cross section xs1
                    c[2] - inital (SM) value for chiral cross section xs2
      Parameters: p[0] - Change in asymmetry I DeltaA_I  
                  p[1] - Change in asymmetry II DeltaA_II
  **/
  return 1.0 + (c[0]+c[1]+c[2])/c[0] * (*(p[1]));
}

double Physics::asymm_3chixs_a1 (
  const Data::BinCoord &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs1.
      Coefficients: c[0] - inital (SM) value for chiral cross section xs0
                    c[1] - inital (SM) value for chiral cross section xs1
                    c[2] - inital (SM) value for chiral cross section xs2
      Parameters: p[0] - Change in asymmetry I DeltaA_I  
                  p[1] - Change in asymmetry II DeltaA_II
  **/
  return 1.0 - (c[0]+c[1]+c[2])/c[1] * (*(p[0]));
}

double Physics::asymm_3chixs_a2 (
  const Data::BinCoord &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs2.
      Coefficients: c[0] - inital (SM) value for chiral cross section xs0
                    c[1] - inital (SM) value for chiral cross section xs1
                    c[2] - inital (SM) value for chiral cross section xs2
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
  **/
  return 1.0 + (c[0]+c[1]+c[2])/c[2] * ( (*(p[0])) - (*(p[1])) );
}

//------------------------------------------------------------------------------
/** Final state asymmetry factors in di-fermion production.
    Underlying assumption:
      The distribution has the shape predicted by the Standard Model at tree 
      level. Only the chiral coefficients (c_L/R^f/e) are allowed to vary.
    Final state asymmetry is defined as:
      Af = [(c_L^f)^2 - (c_R^f)^2] / [(c_L^f)^2 - (c_R^f)^2]
    A change in this asymmetry changes the shape of the distribution but keeps
    it's integral constant.
    The shape-change depends on the chirality of the incoming particles and the
    polar angle.
**/

double Physics::asymm_Af_2f_LR (
  const Data::BinCoord &x,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Polar angle-dependent scaling of the LR distribution due to a change in
      the final state asymmetry.
      Coefficients: c[0] - total LR cross section @ SM (or: initial prediction)
                    c[1] - differential LR cross section in this bin @ SM
                    c[2] - index of cos(theta) coordinate in coordinate vector
      Coordinates: x[c[2]] - cosine of the polar angle of the fermion 
      Parameters: p[0] - change in asymmetry DeltaA_f
  **/
  return 1.0 + 0.75 * c[0] / c[1] * x.get_center()[int(c[2])] * (*(p[0]));
}

double Physics::asymm_Af_2f_RL (
  const Data::BinCoord &x,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Polar angle-dependent scaling of the RL distribution due to a change in
      the final state asymmetry.
      Coefficients: c[0] - total RL cross section @ SM (or: initial prediction)
                    c[1] - differential RL cross section in this bin @ SM
                    c[2] - index of cos(theta) coordinate in coordinate vector
      Coordinates: x[c[2]] - cosine of the polar angle of the fermion 
      Parameters: p[0] - change in asymmetry DeltaA_f
  **/
  return 1.0 - 0.75 * c[0] / c[1] * x.get_center()[int(c[2])] * (*(p[0]));
}

//------------------------------------------------------------------------------

/** Generalised differential parametrisation in di-fermion production.
    Derived from the helicity amplitude approach, with additional correction 
    term to take higher order effects into account.
    Integrates over the bin to be in agreement with the datapoint.
    
    Coordinates: x[c[1]] - cosine of polar angle in ffbar system
    Coefficients:
      c[0] - cross section in bin (completely replaced)
      c[1] - integrated LR cross section
      c[2] - integrated RL cross section
      c[3] - index of cos(theta) coordinate in coordinate vector
    Parameters:
      p[0] - xs0
      p[1] - Ae
      p[2] - Af
      p[3] - epsilon_f
      p[4] - k0
      p[5] - Delta k
**/

double Physics::general_2f_param_LR(const Data::BinCoord &x,
                                    const std::vector<double> &c,
                                    const std::vector<double *> &p) {
  /** LR factor of generalised difermion parametrisation (see above).
   **/
  double x_up = x.get_edge_up()[int(c[3])];
  double x_low = x.get_edge_low()[int(c[3])];
  double integral_const = x_up - x_low;
  double integral_lin = 0.5 * (std::pow(x_up, 2) - std::pow(x_low, 2));
  double integral_quad = 1.0 / 3.0 * (std::pow(x_up, 3) - std::pow(x_low, 3));
  
  double xs_fraction = c[0] / (c[1] + c[2]);
  
  double kL = (*(p[4]) + *(p[5])) / 2.0;

  double factor = 3.0 / 8.0 * (*(p[0])) / xs_fraction * 
                  (1.0 + (*(p[1]))) / 2.0 * ((1.0 + kL) * integral_const +
                   ((*(p[3])) + 2.0 * (*(p[2]))) * integral_lin +
                   (1.0 - 3.0 * kL) * integral_quad);
  if (factor < 0.0) {
    factor = 0.0;
  }
  return factor;
}

double Physics::general_2f_param_RL(const Data::BinCoord &x,
                                    const std::vector<double> &c,
                                    const std::vector<double *> &p) {
  /** RL factor of generalised difermion parametrisation (see above).
   **/                                      
  double x_up = x.get_edge_up()[int(c[3])];
  double x_low = x.get_edge_low()[int(c[3])];
  double integral_const = x_up - x_low;
  double integral_lin = 0.5 * (std::pow(x_up, 2) - std::pow(x_low, 2));
  double integral_quad = 1.0 / 3.0 * (std::pow(x_up, 3) - std::pow(x_low, 3));
  
  double xs_fraction = c[0] / (c[1] + c[2]);
  
  double kR = (*(p[4]) - *(p[5])) / 2.0;

  double factor = 3.0 / 8.0 * (*(p[0])) / xs_fraction * 
                  (1.0 - (*(p[1]))) / 2.0 * ((1.0 + kR) * integral_const +
                   ((*(p[3])) - 2.0 * (*(p[2]))) * integral_lin +
                   (1.0 - 3.0 * kR) * integral_quad);
  if (factor < 0.0) {
    factor = 0.0;
  }
  return factor;
}

//------------------------------------------------------------------------------

/** Differential parametrisation in unpolarised di-fermion production.
    Originates from general formula above.
    It assumes that polarised quantities (e.g. Ae*epsilon_f in constant term) 
    can be neglected and that Delta-k * Ae can be neglected as well (<=1e-4).
    It uses the transformation: A_FB = 3/8 * (ef + 2 Ae Af)
    Integrates over the bin to be in agreement with the datapoint.
    
    Coordinates: x[c[1]] - cosine of polar angle in ffbar system
    Coefficients:
      c[0] - cross section in bin (completely replaced)
      c[1] - integrated LR cross section
      c[2] - integrated RL cross section
      c[3] - index of cos(theta) coordinate in coordinate vector
    Parameters:
      p[0] - xs0
      p[1] - A_FB
      p[4] - k0
**/

double Physics::unpol_2f_param(const Data::BinCoord &x,
                               const std::vector<double> &c,
                               const std::vector<double *> &p) {
  /** Factor of unpolarised difermion parametrisation (see above).
      Factor function is same for LR and RL (coefficient values differ).
   **/
  double x_up = x.get_edge_up()[int(c[3])];
  double x_low = x.get_edge_low()[int(c[3])];
  double integral_const = x_up - x_low;
  double integral_lin = 0.5 * (std::pow(x_up, 2) - std::pow(x_low, 2));
  double integral_quad = 1.0 / 3.0 * (std::pow(x_up, 3) - std::pow(x_low, 3));
  
  double xs_fraction = c[0] / (c[1] + c[2]);
  
  double factor = 3.0 / 8.0 * (*(p[0])) / xs_fraction * 0.5 *
                  ((1.0 + (*(p[2]))/2.0) * integral_const +
                   8.0 / 3.0 * (*(p[1])) * integral_lin +
                   (1.0 - 3.0 * (*(p[2]))/2.0) * integral_quad);
  if (factor < 0.0) {
    factor = 0.0;
  }
  return factor;
}

//------------------------------------------------------------------------------

} // Namespace Fcts
} // Namespace PrEW