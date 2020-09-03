#ifndef LIB_PRINTER_H
#define LIB_PRINTER_H 1

#include <Connect/DataConnector.h>
#include <Fit/FitResult.h>

#include <string>

namespace PrEW {
namespace Output {
  
  class Printer {
    /** Class to print the information and results from fits to a file.
    **/
    
    // Input arguments
    std::string m_file_path {}; // Path of the output file
    
    // Internal strings to collect complete output before writing it
    std::string m_info_str {};  // String for input information
    std::string m_res_str {};   // String for fit results 
    
    std::string m_compl_setups {}; // String to collect completed setup outputs
    
    // Tracking number of fit results
    int m_n_fits {};
    
    // Internal functions defining actual output
    void add_meta_info(int energy, const Connect::DataConnector & connector);
    void add_par_info(const Fit::FitResult & result);
    void add_fit_res(const Fit::FitResult & result);
    std::string assemble_setup_output() const;
    
    // Internal functions to handle output
    bool has_setup_to_save() const;
    void save_setup_output();
    void reset_setup();

    public:
      // Constructors
      Printer( std::string file_path );

      // Writing functions
      void new_setup( int energy, const Connect::DataConnector & connector );
      void add_fit( const Fit::FitResult & result );
      void add_fits( const Fit::ResultVec & results );
      void write( const std::string & mode="overwrite" ) const;

      // Functions to check current status
      std::string get_current_output() const;
  };
  
}
}

#endif