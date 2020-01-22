#include <CppUtils/Vec.h>
#include <CppUtils/Root.h>
#include <Data/Distr1D.h>
#include <Fit/FitBin.h>
#include <Input/Reading.h>
#include <Input/InfoRKFile.h>

#include "spdlog/spdlog.h"
#include "TFile.h"
#include "TTree.h"

namespace PREW {
namespace Input {
  
//------------------------------------------------------------------------------

Data::Distr1DVec Reading::read_RK_file(InputInfo *input_info) {
  /** Read input file that is in style of Robert Karls root files.
  **/
  
  Data::Distr1DVec distributions {};

  // Get all the information needed to read the file
  InfoRKFile* info = static_cast<InfoRKFile*>(input_info);
  if ( nullptr == info ) {
    throw std::invalid_argument("Given InputInfo can not be cast to InfoRKFile!");
  }
  std::string file_path = info->m_file_path;
  int energy = info->m_energy;
  
  // Parameters to read out of tree
  std::string *process {};
  int n_bins {};
  TMatrixT < double > * bin_centers = 0;
  
  const unsigned int number_init = 10000;
  double diff_sigma_signal_LR[number_init] {};
  double diff_sigma_signal_RL[number_init] {};
  double diff_sigma_signal_RR[number_init] {};
  double diff_sigma_signal_LL[number_init] {};
  
  // TODO Add when know what to do with TGC parameters
  // string * diff_TGC_coeff_label = new string();
  // TMatrixT < double > * diff_TGC_coeff_LR = 0;
  // TMatrixT < double > * diff_TGC_coeff_RL = 0;
  // TMatrixT < double > * diff_TGC_coeff_RR = 0;
  // TMatrixT < double > * diff_TGC_coeff_LL = 0;
  
  // Open tree
  spdlog::debug("Opening file: {}", file_path);
  TFile *file = new TFile( file_path.c_str(), "Read" );	
  std::string tree_name = "MinimizationProcesses" + std::to_string(energy) + "GeV";
  TTree *tree = static_cast<TTree*>( file->Get( tree_name.c_str() ) );
  if( tree == nullptr ){
    throw std::invalid_argument( ("No tree for energy " + std::to_string(energy) 
                                  + " in file " + file_path + " !").c_str());
  }
  
  // Couple parameters to tree addresses
  tree->SetBranchAddress("describtion", &process );
  tree->SetBranchAddress("angular_number", &n_bins );
  tree->SetBranchAddress("angular_center", &bin_centers );
  
  tree->SetBranchAddress("differential_sigma_LR", diff_sigma_signal_LR );
  tree->SetBranchAddress("differential_sigma_RL", diff_sigma_signal_RL );
  tree->SetBranchAddress("differential_sigma_RR", diff_sigma_signal_RR );
  tree->SetBranchAddress("differential_sigma_LL", diff_sigma_signal_LL );
  
  // TODO Add when know what to do with TGC parameters
  // tree->SetBranchAddress("differential_PNPC_label", &diff_TGC_coeff_label );
  // tree->SetBranchAddress("differential_PNPC_LR", &diff_TGC_coeff_LR );
  // tree->SetBranchAddress("differential_PNPC_RL", &diff_TGC_coeff_RL );
  // tree->SetBranchAddress("differential_PNPC_RR", &diff_TGC_coeff_RR );
  // tree->SetBranchAddress("differential_PNPC_LL", &diff_TGC_coeff_LL );
  
  int n_processes = tree->GetEntries();
  for(int p=0; p<n_processes; p++){
    tree->GetEntry(p);
    
    Data::Distr1D distr_LL {};
    Data::Distr1D distr_LR {};
    Data::Distr1D distr_RL {};
    Data::Distr1D distr_RR {};
    
    distr_LL.m_info.m_pol_config = "LL";
    distr_LR.m_info.m_pol_config = "LR";
    distr_RL.m_info.m_pol_config = "RL";
    distr_RR.m_info.m_pol_config = "RR";
    
    distr_LL.m_info.m_energy = energy;
    distr_LR.m_info.m_energy = energy;
    distr_RL.m_info.m_energy = energy;
    distr_RR.m_info.m_energy = energy;
    
    distr_LL.m_info.m_process_name = *process;
    distr_LR.m_info.m_process_name = *process;
    distr_RL.m_info.m_process_name = *process;
    distr_RR.m_info.m_process_name = *process;
    
    CppUtils::Vec::Matrix2D<double> bin_center_mtx 
      = CppUtils::Root::matrix2D_from_TMatrixT( *bin_centers );
    distr_LL.m_bin_centers = bin_center_mtx;
    distr_LR.m_bin_centers = bin_center_mtx;
    distr_RL.m_bin_centers = bin_center_mtx;
    distr_RR.m_bin_centers = bin_center_mtx;
    
    for (int bin=0; bin<n_bins; bin++) {
      distr_LL.m_distribution.push_back( Fit::FitBin(diff_sigma_signal_LL[bin],0) );
      distr_LR.m_distribution.push_back( Fit::FitBin(diff_sigma_signal_LR[bin],0) );
      distr_RL.m_distribution.push_back( Fit::FitBin(diff_sigma_signal_RL[bin],0) );
      distr_RR.m_distribution.push_back( Fit::FitBin(diff_sigma_signal_RR[bin],0) );
    }
    
    // TODO Add when know what to do with TGC parameters
    //     // Works for both angular_number = 1 and > 1
    //     current_process.chiral_signal_scaling = ChiralStructure(1,1,1,1).create_duplicates(angular_number);
    // 
    //     // Parameters other than polarizations can only be fitted if differential is used => Always read differential version
    //     current_process.coefficient_labels  = StrHelp::string_to_vec( *differential_PNPC_label, ";");
    //     current_process.coefficients        = ChiralStructure::create_nxm_structures(*differential_PNPC_LL,*differential_PNPC_LR,*differential_PNPC_RR,*differential_PNPC_RL);				
    // 
    //     current_energy_processes.push_back(current_process);
    //     // TODO TODO TODO LUMINOSITIES!!! => HOW IS THIS HANDLED??? WHY IS IT READ FROM TREE???
    // 
    
    distributions.push_back(distr_LL);
    distributions.push_back(distr_LR);
    distributions.push_back(distr_RL);
    distributions.push_back(distr_RR);
  }
  
  spdlog::debug("Number of distributions found: {}", distributions.size());
  
  file->Close();
  
  return distributions;
}
  
//------------------------------------------------------------------------------
  
}
}