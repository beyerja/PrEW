#include <CppUtils/Root.h>
#include <CppUtils/Str.h>
#include <CppUtils/Vec.h>
#include <Data/PredDistr.h>
#include <Fit/FitBin.h>
#include <GlobalVar/Chiral.h>
#include <Input/InfoRKFile.h>
#include <Input/CSVMetadata.h>
#include <Input/Reading.h>

#include "spdlog/spdlog.h"
#include "TFile.h"
#include "TMatrixT.h"
#include "TTree.h"

namespace PrEW {
namespace Input {
  
//------------------------------------------------------------------------------

void Reading::read_csv_file(const InputInfo *input_info,
                            Data::PredDistrVec *pred_distrs,
                            Data::CoefDistrVec *coef_distrs) {
  /** Read input CSV file containing info for one chiral distribution.
   **/
  // Extract metadata and get CSV file reader
  CSVMetadata metadata;
  auto csv_reader = metadata.strip_metadata(input_info->m_file_path);

  // Get distribution info
  Data::DistrInfo info{};
  info.m_distr_name = metadata.get<std::string>("name");
  info.m_energy = metadata.get<int>("energy");
  info.m_pol_config = GlobalVar::Chiral::transform(
      metadata.get<int>("e- chirality"), metadata.get<int>("e+ chirality"));

  // Read the bins (= rows of the csv file)
  Data::PredDistr pred_distr{};
  pred_distr.m_info = info;
  for (auto &row : csv_reader) {
    // Find and interpret the bin centers (separated by ":")
    std::vector<double> bin_centers{};
    auto center_strs = CppUtils::Str::string_to_vec(
        row["Bin centers"].get<std::string>(), ":");
    for (const auto &center : center_strs) {
      bin_centers.push_back(CppUtils::Str::cast_string<double>(center));
    }

    // Bin centers and signal 
    pred_distr.m_bin_centers.push_back(bin_centers);
    pred_distr.m_sig_distr.push_back(row["Cross sections"].get<double>());
  }
  pred_distrs->push_back(pred_distr);
}

//------------------------------------------------------------------------------

void Reading::read_RK_file(
  const InputInfo *input_info,
  Data::PredDistrVec *pred_distrs,
  Data::CoefDistrVec *coef_distrs ) 
{
  /** Read input file that is in style of Robert Karls root files.
      Extracts predicted cross sections and coefficiencts for TGCs.
  **/
  
  // Get all the information needed to read the file
  const InfoRKFile* info = static_cast<const InfoRKFile*>(input_info);
  if ( nullptr == info ) {
    throw std::invalid_argument("Given InputInfo can not be cast to InfoRKFile!");
  }
  std::string file_path = info->m_file_path;
  int energy = info->m_energy;
  
  // Parameters to read out of tree
  std::string *process {};
  int n_bins {};
  TMatrixT < double > *bin_centers = 0;
  
  const unsigned int number_init = 10000;
  double diff_sigma_signal_LR[number_init] {};
  double diff_sigma_signal_RL[number_init] {};
  double diff_sigma_signal_RR[number_init] {};
  double diff_sigma_signal_LL[number_init] {};
  
  std::string * diff_TGC_coeff_label {};
  TMatrixT<double> *diff_TGC_coeff_LR = 0;
  TMatrixT<double> *diff_TGC_coeff_RL = 0;
  TMatrixT<double> *diff_TGC_coeff_RR = 0;
  TMatrixT<double> *diff_TGC_coeff_LL = 0;
  
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
  
  tree->SetBranchAddress("differential_PNPC_label", &diff_TGC_coeff_label );
  tree->SetBranchAddress("differential_PNPC_LR", &diff_TGC_coeff_LR );
  tree->SetBranchAddress("differential_PNPC_RL", &diff_TGC_coeff_RL );
  tree->SetBranchAddress("differential_PNPC_RR", &diff_TGC_coeff_RR );
  tree->SetBranchAddress("differential_PNPC_LL", &diff_TGC_coeff_LL );
  
  int n_processes = tree->GetEntries();
  for(int p=0; p<n_processes; p++){
    tree->GetEntry(p);
    
    // Info that identifies distribution
    Data::DistrInfo basic_info {};
    basic_info.m_energy = energy;
    basic_info.m_distr_name = *process;
    
    Data::DistrInfo info_LL = basic_info,
                    info_LR = basic_info,
                    info_RL = basic_info,
                    info_RR = basic_info;
                    
    info_LL.m_pol_config = GlobalVar::Chiral::eLpL;
    info_LR.m_pol_config = GlobalVar::Chiral::eLpR;
    info_RL.m_pol_config = GlobalVar::Chiral::eRpL;
    info_RR.m_pol_config = GlobalVar::Chiral::eRpR;

    Data::PredDistr pred_LL {}, pred_LR {}, pred_RL {}, pred_RR {};
    pred_LL.m_info = info_LL;
    pred_LR.m_info = info_LR;
    pred_RL.m_info = info_RL;
    pred_RR.m_info = info_RR;
    
    // Reading of actual distribution and coefficients
    CppUtils::Vec::Matrix2D<double> bin_center_mtx 
      = CppUtils::Root::matrix2D_from_TMatrixT( *bin_centers );
    pred_LL.m_bin_centers = bin_center_mtx;
    pred_LR.m_bin_centers = bin_center_mtx;
    pred_RL.m_bin_centers = bin_center_mtx;
    pred_RR.m_bin_centers = bin_center_mtx;
    
    std::vector<std::string> coef_labels = CppUtils::Str::string_to_vec( *diff_TGC_coeff_label, ";");
    size_t n_coefs = coef_labels.size();
    
    std::vector<std::vector<double>> coef_vals_LL (n_coefs), 
      coef_vals_LR (n_coefs), coef_vals_RL (n_coefs), coef_vals_RR (n_coefs);
    
    for (int bin=0; bin<n_bins; bin++) {
      pred_LL.m_sig_distr.push_back( diff_sigma_signal_LL[bin] );
      pred_LR.m_sig_distr.push_back( diff_sigma_signal_LR[bin] );
      pred_RL.m_sig_distr.push_back( diff_sigma_signal_RL[bin] );
      pred_RR.m_sig_distr.push_back( diff_sigma_signal_RR[bin] );
      
      for (size_t coef=0; coef<n_coefs; coef++) {
        coef_vals_LL[coef].push_back((*diff_TGC_coeff_LL)[bin][coef]);
        coef_vals_LR[coef].push_back((*diff_TGC_coeff_LR)[bin][coef]);
        coef_vals_RL[coef].push_back((*diff_TGC_coeff_RL)[bin][coef]);
        coef_vals_RR[coef].push_back((*diff_TGC_coeff_RR)[bin][coef]);
      }
    }
    
    Data::CoefDistrVec coefs_LL (n_coefs), coefs_LR (n_coefs), coefs_RL (n_coefs), coefs_RR (n_coefs);
    for (size_t c=0; c<n_coefs; c++) { 
      coefs_LL[c] = Data::CoefDistr(coef_labels[c],info_LL,coef_vals_LL[c]);
      coefs_LR[c] = Data::CoefDistr(coef_labels[c],info_LR,coef_vals_LR[c]);
      coefs_RL[c] = Data::CoefDistr(coef_labels[c],info_RL,coef_vals_RL[c]);
      coefs_RR[c] = Data::CoefDistr(coef_labels[c],info_RR,coef_vals_RR[c]);
    }
    
    // RK style files don't contain background distributions => 0-entry vectors
    pred_LL.m_bkg_distr = std::vector<double>(pred_LL.m_sig_distr.size());
    pred_LR.m_bkg_distr = std::vector<double>(pred_LR.m_sig_distr.size());
    pred_RL.m_bkg_distr = std::vector<double>(pred_RL.m_sig_distr.size());
    pred_RR.m_bkg_distr = std::vector<double>(pred_RR.m_sig_distr.size());
    
    // Saving distributions in given pointers
    pred_distrs->push_back(pred_LL);
    pred_distrs->push_back(pred_LR);
    pred_distrs->push_back(pred_RL);
    pred_distrs->push_back(pred_RR);
    
    for (auto & coef: coefs_LL) { coef_distrs->push_back(coef); }
    for (auto & coef: coefs_LR) { coef_distrs->push_back(coef); }
    for (auto & coef: coefs_RL) { coef_distrs->push_back(coef); }
    for (auto & coef: coefs_RR) { coef_distrs->push_back(coef); }
  }
  
  spdlog::debug("Number of distributions found: {}", pred_distrs->size());
  
  file->Close();
}
  
//------------------------------------------------------------------------------
  
}
}