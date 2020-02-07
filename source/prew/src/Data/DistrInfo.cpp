#include <Data/DistrInfo.h>

namespace PREW {
namespace Data {

//------------------------------------------------------------------------------
// Operators

bool DistrInfo::operator==(const DistrInfo& other) const {
  return  (m_distr_name == other.m_distr_name) &&
          (m_pol_config == other.m_pol_config) &&
          (m_energy == other.m_energy);
}

//------------------------------------------------------------------------------

}
}