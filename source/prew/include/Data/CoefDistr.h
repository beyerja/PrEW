#ifndef LIBRARY_COEFDISTR_H
#define LIBRARY_COEFDISTR_H 1

#include <Data/DistrInfo.h>

#include <string>
#include <vector>

namespace PrEW {
namespace Data {

class CoefDistr {
  /** Class holding a coefficient for parameterised prediction of a
      distribution for one polarisation/chirality.
      Coefficient can be local (different for different bins) or global
      (same for all bins);
  **/
  std::string m_coef_name{}; // Name that identifies this coefficient
  DistrInfo m_info{}; // Info which identifies the corresponding distribution

  // Coefficient(s)
  std::vector<double> m_coefficients{}; // Differential case
  double m_coefficient{};               // Global case
  
  bool m_is_global{};

public:
  // Constructors
  CoefDistr();
  CoefDistr(const std::string &coef_name, const DistrInfo &info,
            const std::vector<double> &coefficients);
  CoefDistr(const std::string &coef_name, const DistrInfo &info,
            double coefficient);

  // Manipulation funcitons
  void set_info(const DistrInfo & info);

  // Access functions
  const std::string &get_coef_name() const;
  const DistrInfo &get_info() const;
  double get_coef(int bin) const;
  
  // Operators
  bool operator==(const CoefDistr& other) const;
  bool operator!=(const CoefDistr& other) const;
};

using CoefDistrVec = std::vector<CoefDistr>;

} // namespace Data
} // namespace PrEW

#endif