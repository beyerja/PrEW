#include <CppUtils/Str.h>
#include <CppUtils/Sys.h>
#include <Output/Printer.h>

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace PREW {
namespace Output {
  
//------------------------------------------------------------------------------
// Constructors

Printer::Printer( std::string file_path ) : m_file_path(file_path) {
  /** Constructor assesses if file is accessible.
  **/
  if (! CppUtils::Sys::file_writable(file_path)) {
    throw std::invalid_argument("No write access to " + m_file_path);
  }
}

//------------------------------------------------------------------------------
// Writing functions

void Printer::new_setup(
  int energy,
  const Connect::DataConnector & connector
) {
  /** Start a new setup defined by the function links in the data connector and 
      the energy at which the fit is performed.
      Output of any previous setup(s) is saved.
  **/
  this->save_setup_output();
  this->reset_setup();
  this->add_meta_info(energy,connector);
}

void Printer::add_fit( const Fit::FitResult & result ) {
  /** Add the content of a FitResult object to the current setup output.
  **/
  if (m_n_fits == 0) { this->add_par_info(result); } // Collect general par info
  this->add_fit_res(result);
  m_n_fits++;
}

void Printer::add_fits( const Fit::ResultVec & results ) {
  /** Add the content of all given FitResult objects to the current setup 
      output.
  **/
  for ( const auto & result: results ) {
    this->add_fit(result);
  }
}

void Printer::write(const std::string & mode) const {
  /** Write the current output of all setups into the file at the location
      defined in the constructor.
      The mode determines how file is accessed. Options are:
        "overwrite" -> Overwrites previous file content
        "append"    -> Appends at end of file to previous content
  **/
  
  // Determine file opening/writing mode
  std::ios::openmode open_mode;
  if ( mode == "overwrite" )    { open_mode = std::ios::trunc; }
  else if ( mode == "append ")  { open_mode = std::ios::app; }
  else { throw std::invalid_argument("Unknown opening mode: " + mode); }
  
  // Access file and write
  std::ofstream outfile;
  outfile.open( m_file_path.c_str(), open_mode ); // Open file
  outfile << this->get_current_output(); // Write content
  outfile.close(); // Close file
}

//------------------------------------------------------------------------------
// Functions to check current status

std::string Printer::get_current_output() const {
  /** Return the current output of all added setups as it would be written into
      the output file.
  **/
  return m_compl_setups + this->assemble_setup_output();
}

//------------------------------------------------------------------------------
// Internal functions defining actual output

void Printer::add_meta_info(
  int energy, 
  const Connect::DataConnector & connector
) {
  /** Add information about:
        - energy
        - polarisation configurations
        - applied functions and their parameter names
      as they were used in the given setup (defined by energy and connector).
  **/
  // Read energy
  m_info_str += "Energy: " + std::to_string(energy) + "\n";
  
  // Read polarisation configurations
  m_info_str += "Polarisation configurations: ";
  m_info_str += "  [Name] ePol-Name pPol-Name ePol-Sign pPol-Sign\n";
  for ( const auto & pol_link : connector.get_pol_links() ) {
    if (pol_link.get_energy() == energy) {
      m_info_str += "  [" + pol_link.get_pol_config() + "] " +
                    pol_link.get_eM_pol() + " " +
                    pol_link.get_eP_pol() + " " +
                    std::to_string(pol_link.get_eM_sgn_factor()) + " " +
                    std::to_string(pol_link.get_eP_sgn_factor()) + "\n";
    }
  }
  
  // Read the function links applied to the distributions
  m_info_str += "Applied functions:\n";
  m_info_str += "  [Distr-Name] [Pol-Config]\n";
  m_info_str += "    Sig/Bkg Fct-Name {Par1,Par2,...} {C1,C2,...}\n";
  for ( const auto & pred_link : connector.get_pred_links() ) {
    if (pred_link.m_info.m_energy == energy) {
      
      // Read function links for signal predictions
      m_info_str += "  [" + pred_link.m_info.m_distr_name + "] " +
                    "[" + pred_link.m_info.m_pol_config + "]\n";
                    
      for ( const auto & fct_link: pred_link.m_fcts_links_sig ) {
        m_info_str += "    Sig " +
                      fct_link.m_fct_name + " {";
        for (size_t p=0; p<fct_link.m_pars.size(); p++) {
          if (p>0) { m_info_str += ","; }
          m_info_str += fct_link.m_pars[p];
        }
        m_info_str += "} {";
        for (size_t c=0; c<fct_link.m_coefs.size(); c++) {
          if (c>0) { m_info_str += ","; }
          m_info_str += fct_link.m_coefs[c];
        }
        m_info_str += "}\n";
      }
      
      // Read function links for background predictions              
      for ( const auto & fct_link: pred_link.m_fcts_links_bkg ) {
        m_info_str += "    Bkg " +
                      fct_link.m_fct_name + " {";
        for (size_t p=0; p<fct_link.m_pars.size(); p++) {
          if (p>0) { m_info_str += ","; }
          m_info_str += fct_link.m_pars[p];
        }
        m_info_str += "} {";
        for (size_t c=0; c<fct_link.m_coefs.size(); c++) {
          if (c>0) { m_info_str += ","; }
          m_info_str += fct_link.m_coefs[c];
        }
        m_info_str += "}\n";
      }
    }
  }
}

void Printer::add_par_info( const Fit::FitResult & result ) {
  /** Add information about the parameter names in this setup.
  **/
  m_res_str += "Parameters:";
  for ( const auto & par_name: result.m_par_names ) {
    m_res_str += " " + par_name;
  }
  m_res_str += "\n";
}

void Printer::add_fit_res( const Fit::FitResult & result ) {
  /** Add information about results of one performed fit. Contains
        - Final parameter values and uncertainties
        - Final covariance and correlation matrices
        - Chi-Squared at the minimum
        - Expected distance from minimum
        - Status of the covariance matrix caluclation (see Minuit2)
  **/
  m_res_str += "[F" + std::to_string(m_n_fits) + "]\n"; // fit result identifier
  
  // Parameter values and uncertainties
  std::string vals_fin = "  Fin:";
  std::string uncs_fin = "  Unc:";
  for (size_t par=0;par<result.m_par_names.size();par++) {
    vals_fin += " " + CppUtils::Str::sci_string(result.m_pars_fin.at(par));
    uncs_fin += " " + CppUtils::Str::sci_string(result.m_uncs_fin.at(par));
  }
  m_res_str += vals_fin + "\n";
  m_res_str += uncs_fin + "\n";
  
  // Covariance matrix
  m_res_str += "Cov:\n";
  for (size_t par1=0;par1<result.m_cov_matrix.size();par1++) {
    m_res_str += " ";
    for (size_t par2=0;par2<result.m_cov_matrix[par1].size();par2++) {
      m_res_str += 
        " " + CppUtils::Str::sci_string(result.m_cov_matrix[par1][par2]);
    }
    m_res_str += "\n";
  }
  
  // Correlation matrix
  m_res_str += "Cor:\n";
  for (size_t par1=0;par1<result.m_cor_matrix.size();par1++) {
    m_res_str += " ";
    for (size_t par2=0;par2<result.m_cor_matrix[par1].size();par2++) {
      m_res_str += " " + CppUtils::Str::sci_string(result.m_cor_matrix[par1][par2]);
    }
    m_res_str += "\n";
  }
  
  //Fit setup info
  m_res_str += "NBins: " + std::to_string(result.m_n_bins) + "\n";
  m_res_str += "NFreePars: " + std::to_string(result.m_n_free_pars) + "\n";
  
  //Minimization information
  m_res_str += "NFctCalls: " + std::to_string(result.m_n_fct_calls) + "\n";
  m_res_str += "NIterations: " + std::to_string(result.m_n_iters) + "\n";
  
  // Fit quality measures
  m_res_str += "Chi-Sq: " + CppUtils::Str::sci_string(result.m_chisq_fin) + "\n";
  m_res_str += "EDM: " + CppUtils::Str::sci_string(result.m_edm_fin) + "\n";
  m_res_str += "Min.-Status: " + std::to_string(result.m_min_status) + "\n";
  m_res_str += "Cov.-Status: " + std::to_string(result.m_cov_status) + "\n";
  
  // Identifier that end of fit result is reached
  m_res_str += "[END F" + std::to_string(m_n_fits) + "]\n";
}

std::string Printer::assemble_setup_output() const {
  /** Assemble the complete output for the current setup (if one exists).
      Includes fit meta information and information about all added fit results.
  **/
  std::string output = "";
  if (this->has_setup_to_save()) {
    output += std::string("") +
      "<=========================== BEGIN ==========================>\n" +
      "<SETUP>\n" + m_info_str + "<END SETUP>\n\n" +
      "<FITS>\n" + m_res_str + "<END FITS>\n" +
      "<============================ END ===========================>\n";
  }
  return output;
}

//------------------------------------------------------------------------------
// Internal functions to handle output

bool Printer::has_setup_to_save() const {
  /** Check if there currently is a setup to save.
  **/
  return (m_info_str != "") || (m_res_str != "");
}

void Printer::save_setup_output() {
  /** Internally save the complete output of the current setup into the string
      that hold the total output of all setups.
  **/
  m_compl_setups += this->assemble_setup_output();
}

void Printer::reset_setup() {
  /** Reset the strings and member variables describing the current setup.
      Does not touch the total setup collection string.
  **/
  m_info_str = "";
  m_res_str = "";
  m_n_fits = 0;
}

//------------------------------------------------------------------------------

}
}