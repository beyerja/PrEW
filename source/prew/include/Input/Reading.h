#ifndef LIBRARY_READING_H
#define LIBRARY_READING_H 1

#include <Data/CoefDistr.h>
#include <Data/PredDistr.h>
#include <Input/InputInfo.h>

namespace PrEW {
namespace Input {
  
namespace Reading {
  /** Namespace for functions that can read different kinds of files.
  **/
  
  //----------------------------------------------------------------------------

  void read_RK_file(const InputInfo *input_info, 
                    Data::PredDistrVec *pred_distrs, 
                    Data::CoefDistrVec *coef_distrs);

  //----------------------------------------------------------------------------
  
}
  
}  
}

#endif