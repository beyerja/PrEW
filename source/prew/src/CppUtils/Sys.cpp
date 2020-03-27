#include <CppUtils/Str.h>
#include <CppUtils/Sys.h>

#include <fstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

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

bool Sys::file_writable(const std::string& file_path) {
  /** Check if framework is in principle able to write to given file path.
      If file exists user must be able to write to it.
      If file does not exist user must be able to write to directory.
  **/
  bool writable {};
  if (file_path == "") { writable = false; }
  else if (Sys::path_exists(file_path)) {
    // File exists -> Check if it can be opened in append mode
    try { 
      std::ofstream file;
      file.open( file_path.c_str(), std::ios::app ); // Open in append mode
      file << ""; // Try appending empty string
      file.close(); // Close file
      writable = true;
    } catch (const std::exception& /*e*/) { writable = false; }
  } else {
    // File doesn't exist -> Check if user has read & write access to directory
    std::string dir_path = Str::get_dir_path_str(file_path);

    // If file in current dir (either "." or no dir path) make dir path usable
    if ( (dir_path == ".") || (dir_path == "") ) { dir_path = "./."; }
    
    // Check access writes to directory
    if ( access(Str::get_dir_path_str(dir_path).c_str(), R_OK | W_OK) == 0 ) {
     writable = true;  
   } else {
     writable = false;  
   }
  }
  
  return writable;
}

//------------------------------------------------------------------------------
  
}
}