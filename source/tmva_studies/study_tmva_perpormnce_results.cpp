#include "../common_functions.h"
#include "../draw_functions.h"

// ###################
// ##     MAIN      ##
// ###################
int main(int argc, char *argv[])
{
  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");
  
  
  // Open the tfile - mtva ouput
  TFile *tmva_file = new TFile("results/TMVA_pl.root");

  
  // Open the test tree
  TTree *test_tree = (TTree*)tmva_file->Get("dataset/TestTree");
  
  
  // Set branches
  float classifier_val, weight, topHOF;
  classifier_val = weight = topHOF = 0;  

  test_tree->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &topHOF);
  test_tree->SetBranchAddress("weight", &weight);
  test_tree->SetBranchAddress(tmva_config_info[0], &classifier_val);


  // Get number of entries
  Int_t nEntries = test_tree->GetEntries();
  std::cout << "Total number of entries: " << nEntries << std::endl;
  

  // Set an array of Classifier cuts to test and an array to store efficiency
  std::vector<double> classifier_cut = {}; // list of cuts to analyze
  
  std::vector<double> n_topHOF_and_classifier = {}; // classifier identified correctly - compared to topHOF
  std::vector<double> n_topHOF                = {}; // N jets not from top by topHOF info
  std::vector<double> n_classifier            = {}; // N jets not from top by classifier cut
  
  std::vector<double> eff    = {};
  std::vector<double> purity = {};


  // Set Classifier cuts in the whole range of the classifier
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get("dataset/Method_" + tmva_config_info[0] + "/" + tmva_config_info[0] + "/MVA_" + tmva_config_info[0] + "_S");
  double x_min = h_NN_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_NN_classifier_output_S->GetXaxis()->GetBinUpEdge(h_NN_classifier_output_S->GetNbinsX());
  for (double i=x_min; i<=x_max; i += 0.05) {
    classifier_cut.push_back(i); }
  
  // Loop over the entries
  for (int entry=0; entry<nEntries; entry++) {
    
    if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
    test_tree->GetEntry(entry);
    
    for (int cut_i=0; cut_i<classifier_cut.size(); cut_i++) {
      
      if (entry==0) {
	n_topHOF_and_classifier.push_back(0);
	n_topHOF.push_back(0); 
	n_classifier.push_back(0); }
      
      if (topHOF!=4) {
	n_topHOF[cut_i] += weight;
	if ( classifier_val>classifier_cut[cut_i] ) n_topHOF_and_classifier[cut_i] += weight; }
	
      if (classifier_val>classifier_cut[cut_i]) n_classifier[cut_i] += weight;
    
    } // [cut] - loop over given classifier cut values under the study
    
  } // [entry] - loop over entries


  // Compute the discriminative power / efficiency
  for (int cut_i=0; cut_i<classifier_cut.size(); cut_i++) {
    
    double eff_tmp = n_topHOF_and_classifier[cut_i] / n_topHOF[cut_i];
    eff.push_back(eff_tmp);
    
    double tmp_purity = n_topHOF_and_classifier[cut_i] / n_classifier[cut_i];
    purity.push_back(tmp_purity);
    
    //std::cout << "CLassifier cut = " << classifier_cut[cut_i] << "  \tEff. = " << eff_tmp << "  \tPurity = " << tmp_purity << std::endl; 
  }


  // Close the input file
  tmva_file->Close();


  
  // Create TGraphs with eff|purity wrk kbdt cut and then plot them
  TGraph *gr_eff    = new TGraph(classifier_cut.size(), &classifier_cut[0], &eff[0]);
  TGraph *gr_purity = new TGraph(classifier_cut.size(), &classifier_cut[0], &purity[0]);
  TGraph *gr_eff_vs_purity = new TGraph(classifier_cut.size(), &purity[0], &eff[0]);

  std::vector<TGraph*> gr_eff_and_purity_vs_classifier = {gr_eff, gr_purity};
  int gr_eff_and_purity_vs_classifier_draw = draw_graphs(gr_eff_and_purity_vs_classifier, tmva_config_info[0], "Eff. | Purity", {"Efficiency", "Purity"}, "eff_and_purity_vs_classifier");

  std::vector<TGraph*> gr_eff_vs_purity_vec = {gr_eff_vs_purity};
  int gr_eff_vs_purity_draw = draw_graphs(gr_eff_vs_purity_vec, "Purity", "Efficiency", {"Eff. vs. Purity"}, "eff_vs_purity");
  
  return 0;
}
