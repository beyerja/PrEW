#ifndef LIBRARY_INPUTINFO_H
#define LIBRARY_INPUTINFO_H 1

namespace PrEW {
namespace Input {
  
  struct InputInfo {
    /** Abstract struct, concrete implementations have to hold all necessary
        info needed for the corresponding Reading function to read a file.
        Can only be used to read one specific file, for multiple files need
        multiple InputInfo.
    **/
    
    std::string m_file_path {};   // Input file path, must be in all input infos
    std::string m_input_style {}; // Type of input file
  };
  
}
}


#endif