#ifndef LIBRARY_INFORKFILE_H
#define LIBRARY_INFORKFILE_H 1

#include <Input/InputInfo.h>

namespace PREW {
namespace Input {
  
  struct InfoRKFile : public InputInfo {
    /** Input info needed to read a file created by Robert Karl.
    **/
    
    int energy {};
  };
  
}
}


#endif