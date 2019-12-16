#ifndef LIB_XMLREADER_H
#define LIB_XMLREADER_H 1

#include <tinyxml2.h>

#include <string>

namespace PREW {
namespace Input {
  
  class XMLReader {
    /** Class to interpret an xml steering file that contains info on what
        should be fitted and in which context.
        => This class hardcodes the structure of the steering files
    **/
    
    std::string m_steering_path {};
    tinyxml2::XMLDocument m_doc;
    
    void read_steering();
    
    public:
      // Constructors
      XMLReader(std::string steering_path, std::string process_dir);
  };
}
}

#endif