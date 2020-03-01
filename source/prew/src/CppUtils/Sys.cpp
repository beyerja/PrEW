#include <CppUtils/Sys.h>

#include <string>
#include <sys/stat.h>

namespace PREW {
namespace CppUtils {

//------------------------------------------------------------------------------

bool Sys::path_exists (const std::string& path) {
  /** Quick check that path is accessible (may be file or directory).
  **/
  struct stat buffer;   
  return (stat (path.c_str(), &buffer) == 0); 
}

//------------------------------------------------------------------------------
  
}
}