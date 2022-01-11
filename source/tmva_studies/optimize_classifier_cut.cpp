#include "../common_functions.h"
#include "../draw_functions.h"

// #################
// ##    MAIN     ##
// #################
int main(int argc, char *argv[])
{
  // Check for the generator choise
  std::string generator = ttbar_generator();
  if (generator=="quit") return 0;
  if (generator=="" && std::string(argv[1])=="tt") {
    generator="nominal";
    std::cout << "No generator was selected for ttbar, assuming nominal" << std::endl; }


  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");

  
  // Open the TMVA output file
  TString in_fname = "results/TMVA_pl_" + generator+ ".root";
  TFile *tmva_file = new TFile(in_fname);


  // Open classifier hists
  TString var_hists_address = "dataset/Method_" + tmva_config_info[0] + "/" + tmva_config_info[0] + "/";
  TH1 *h_classifier_output_S = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_S");
  TH1 *h_classifier_output_B = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_B");
 
  
  // Prepare hists
  int n_bins =  h_classifier_output_S->GetNbinsX();
  double sig_total = h_classifier_output_S->Integral(0, h_classifier_output_S->GetNbinsX()+1);
  double bkg_total = h_classifier_output_B->Integral(0, h_classifier_output_B->GetNbinsX()+1);
 

  // Create a hist for ratio
  double x_min = h_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_classifier_output_S->GetXaxis()->GetBinUpEdge(h_classifier_output_S->GetNbinsX());
  std::cout << "x_min = " << x_min << ";\tx_max = " << x_max << std::endl;
  TH1 *h_classifiers_ratio = new TH1F("ratio", "ratio", n_bins, x_min, x_max);
  

  // Find the most optimal MVA value
  double classifiers_ratio_best = -99.; 
  double optimal_MVA_score_cut = -99.;  
  
  for (int bin=0; bin<n_bins; bin++) {
    double sig_with_cut = h_classifier_output_S->Integral(bin, n_bins);
    double bkg_with_cut = h_classifier_output_B->Integral(bin, n_bins);
    if (bkg_with_cut==0) continue;
    
    double classifiers_ratio_tmp = sig_with_cut/sqrt(bkg_with_cut);
    h_classifiers_ratio->SetBinContent(bin, classifiers_ratio_tmp);
    std::cout << "Bin: " << bin << ";    Ratio: " << classifiers_ratio_tmp << ";   classifier val: " << h_classifier_output_S->GetBinCenter(bin) << std::endl;
    if (classifiers_ratio_tmp > classifiers_ratio_best) {
      classifiers_ratio_best = classifiers_ratio_tmp;
      optimal_MVA_score_cut = h_classifier_output_S->GetBinCenter(bin); }
    
  } // [bin] loop over bins 

  std::cout << "\n\nThe optimal classifier value is " << optimal_MVA_score_cut << std::endl;

  
  int classifier_draw = draw_sig_bkgd_plot(h_classifier_output_S, h_classifier_output_B, h_classifiers_ratio, tmva_config_info[0], "optimize_" + tmva_config_info[0] + "_cut", {"Sig", "Bkgd"}, true);

  return 0;
}
