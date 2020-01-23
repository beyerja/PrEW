#ifndef LIBRARY_READING_H
#define LIBRARY_READING_H 1

#include <Data/PredDistr.h>
#include <Input/InputInfo.h>

namespace PREW {
namespace Input {
  
namespace Reading {
  /** Namespace for functions that can read different kinds of files.
  **/
  
  //----------------------------------------------------------------------------

  Data::PredDistrVec read_RK_file(InputInfo *input_info);

  //----------------------------------------------------------------------------
  
}
  
}  
}

#endif