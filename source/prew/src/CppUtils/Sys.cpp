#include <CppUtils/Sys.h>

#include <string>
#include <sys/stat.h>

namespace PREW {
namespace CppUtils {

//------------------------------------------------------------------------------


bool Sys::file_exists (const std::string& file_path) {
  /** Quick check that file given by file path exists.
  **/
  struct stat buffer;   
  return (stat (file_path.c_str(), &buffer) == 0); 
}

//------------------------------------------------------------------------------
  
}
}