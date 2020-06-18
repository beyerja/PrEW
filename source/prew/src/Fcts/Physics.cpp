#include <Fcts/Physics.h>

#include <math.h>

namespace PrEW {
namespace Fcts {

//------------------------------------------------------------------------------

double Physics::polarisation_factor ( 
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

double Physics::luminosity_fraction ( 
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
/** Chiral cross section scaling factors accounting for an the chiral asymmetry
    in the case that only two chiral cross sections are allowed.
    The two factors are chosen such that the sum of the chiral cross sections
    is not modified, while the asymmetry is modified like:
      A = (xs0 - xs1)/(xs0 + xs1) -> A' = A + DeltaA
**/

double Physics::asymm_2chixs_a0 (
  const std::vector<double>   &/*x*/,
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
  const std::vector<double>   &/*x*/,
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
    The four factors are chosen such that the sum of the chiral cross sections
    is not modified, while the asymmetries are modified by an addative constant.
      A_I   = sqrt(2) * (xs0 - 1/2 * (xs1 + xs2)) / (xs0 + xs1 + xs2)
      A_II  = sqrt(3/2) * (xs1 - xs2) / (xs0 + xs1 + xs2)
      A_x -> A'_x = A_x + DeltaA_x
**/

double Physics::asymm_3chixs_a0 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs0.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
  **/
  return 1 + std::sqrt(2.0)/3.0 * (c[0] + c[1] + c[2]) / c[0] * (*(p[0]));
}

double Physics::asymm_3chixs_a1 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs1.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
  **/
  return 1 - 1.0 / (std::sqrt(2.0) * 3.0) 
            * ( (c[0] + c[1] + c[2]) 
                * ( (*(p[0])) - std::sqrt(3.0) * (*(p[1])) ) 
              ) / c[1];
}

double Physics::asymm_3chixs_a2 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs2.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
  **/
  return 1 - 1.0 / (std::sqrt(2.0) * 3.0) 
            * ( (c[0] + c[1] + c[2]) 
                * ( (*(p[0])) + std::sqrt(3.0) * (*(p[1])) ) 
              ) / c[2];
}

//------------------------------------------------------------------------------
/** Chiral cross section scaling factors accounting for the chiral asymmetries
    in the case that all four chiral cross sections are allowed.
    The four factors are chosen such that the sum of the chiral cross sections
    is not modified, while the asymmetries are modified by an addative constant.
      A_I   = (xs0 - xs1 + xs2 - xs3) / (xs0 + xs1 + xs2 + xs3)
      A_II  = (xs0 + xs1 - xs2 - xs3) / (xs0 + xs1 + xs2 + xs3)
      A_III = (xs0 - xs1 - xs2 + xs3) / (xs0 + xs1 + xs2 + xs3)
      A_x -> A'_x = A_x + DeltaA_x
**/

double Physics::asymm_4chixs_a0 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs0.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
                    c[3] - chiral cross section xs3 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
                  p[2] - Change in asymmetry III DeltaA_III
  **/
  return 1 + 0.25 * (c[0] + c[1] + c[2] + c[3]) / c[0] 
              * ( (*(p[0])) + (*(p[1])) + (*(p[2])) );
}
                        
double Physics::asymm_4chixs_a1 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs1.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
                    c[3] - chiral cross section xs3 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
                  p[2] - Change in asymmetry III DeltaA_III
  **/
  return 1 - 0.25 * (c[0] + c[1] + c[2] + c[3]) / c[1] 
              * ( (*(p[0])) - (*(p[1])) + (*(p[2])) );
}

double Physics::asymm_4chixs_a2 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs2.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
                    c[3] - chiral cross section xs3 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
                  p[2] - Change in asymmetry III DeltaA_III
  **/
  return 1 + 0.25 * (c[0] + c[1] + c[2] + c[3]) / c[2] 
              * ( (*(p[0])) - (*(p[1])) - (*(p[2])) );
}
                        
double Physics::asymm_4chixs_a3 (
  const std::vector<double>   &/*x*/,
  const std::vector<double>   &c,
  const std::vector<double*>  &p
) {
  /** (See general description above.)
      Factor for chiral cross section xs3.
      Coefficients: c[0] - chiral cross section xs0 (sum over all bins)
                    c[1] - chiral cross section xs1 (sum over all bins)
                    c[2] - chiral cross section xs2 (sum over all bins)
                    c[3] - chiral cross section xs3 (sum over all bins)
      Parameters: p[0] - Change in asymmetry I DeltaA_I
                  p[1] - Change in asymmetry II DeltaA_II
                  p[2] - Change in asymmetry III DeltaA_III
  **/
  return 1 - 0.25 * (c[0] + c[1] + c[2] + c[3]) / c[3] 
              * ( (*(p[0])) + (*(p[1])) - (*(p[2])) );
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
  const std::vector<double>   &x,
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
  return 1.0 + 0.75 * c[0] / c[1] * x[int(c[2])] * (*(p[0]));
}

double Physics::asymm_Af_2f_RL (
  const std::vector<double>   &x,
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
  return 1.0 - 0.75 * c[0] / c[1] * x[int(c[2])] * (*(p[0]));
}

//------------------------------------------------------------------------------

} // Namespace Fcts
} // Namespace PrEW