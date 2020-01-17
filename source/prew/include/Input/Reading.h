#ifndef LIBRARY_READING_H
#define LIBRARY_READING_H 1

#include <Data/Distr1D.h>
#include <Input/InputInfo.h>

namespace PREW {
namespace Input {
  
namespace Reading {
  /** Namespace for functions that can read different kinds of files.
  **/
  
  //----------------------------------------------------------------------------

  Data::Distr1DVec read_RK_file(InputInfo *input_info);

  //----------------------------------------------------------------------------
  
}
  
}  
}

#endif