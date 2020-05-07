#ifndef LIB_POLLINK_H
#define LIB_POLLINK_H 1

#include <string>
#include <vector>

namespace PrEW {
namespace Data {

  class PolLink {
    /** Class that stores information about how to couple beam polarisations
        for a given polarisation configuration at a given energy.
    **/
    
    int m_energy {}; // Depends on the energy
    std::string m_pol_config {}; // Name of polarisation configuration
    
    // Name of single beam polarisation variables
    std::string m_eM_pol {}; 
    std::string m_eP_pol {};  
    
    // Signs of sign beam polarisation variables ("+" or "-")
    std::string m_eM_sgn {};
    std::string m_eP_sgn {};
    
    public:
      // Constructors
      PolLink() = default;
      PolLink(
        int energy, 
        std::string pol_config, 
        std::string eM_pol,     std::string eP_pol, 
        std::string eM_sgn="+", std::string eP_sgn="+"
      );
      
      int get_energy() const;
      std::string get_pol_config() const;
      std::string get_eM_pol() const;
      std::string get_eP_pol() const;
      
      double get_eM_sgn_factor() const;
      double get_eP_sgn_factor() const;
  };

  using PolLinkVec = std::vector<PolLink>;
}
}

#endif