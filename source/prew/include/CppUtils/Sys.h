#ifndef LIB_CPPHELPSYS_H
#define LIB_CPPHELPSYS_H 1

#include <string>

namespace PREW {
namespace CppUtils {

namespace Sys {
  
  bool path_exists (const std::string& file_path);
  bool file_writable(const std::string& file_path);

}

}
}

#endif