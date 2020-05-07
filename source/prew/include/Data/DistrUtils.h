#ifndef LIB_DISTRUTILS_H
#define LIB_DISTRUTILS_H 1

#include <string>
#include <vector>

namespace PrEW {
namespace Data {

namespace DistrUtils {
  /** Functions that can help with the various distribution types in the Data
      namespace and that are not specific to one distribution type.
  **/
  
  template<class T>
  std::vector<T> subvec_energy_and_name(const std::vector<T>& vec, 
                                        int energy, 
                                        std::string distr_name);
  
  template<class T>
  std::vector<T> subvec_energy(const std::vector<T>& vec, int energy);
  template<class T>
  std::vector<T> subvec_pol(const std::vector<T>& vec, 
                            const std::string& pol_config);
  
  template<class T>
  T element_pol(const std::vector<T>& vec, const std::string& pol_config);
}
  
}
}

#include <Data/DistrUtils.tpp>

#endif 