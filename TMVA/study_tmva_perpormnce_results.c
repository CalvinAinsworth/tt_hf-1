#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>


///////////////////
//     MAIN      //
//////////////////
void study_tmva_perpormnce_results()
{
  // Open the tfile - mtva ouput
  TFile *tmva_file = new TFile("TMVA200.root");

  
  // Open the test tree
  TTree *test_tree = (TTree*)tmva_file->Get("dataset/TestTree");
  
  
  // Set branches
  float kbdt_val, weight, topHOF;
  kbdt_val = weight = topHOF = 0;  

  test_tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
  test_tree->SetBranchAddress("weight", &weight);
  test_tree->SetBranchAddress("KBDT", &kbdt_val);


  // Get number of entries
  Int_t nEntries = test_tree->GetEntries();


  // Set an array of KBDT cuts to test and an array to store efficiency
  vector<double> kbdt_cut = {}; // list of cuts to analyze
  
  vector<double> n_topHOF_and_kbdt = {}; // KBDT identified correctly - compared to topHOF
  vector<double> n_topHOF          = {}; // N jets not from top by topHOF info
  vector<double> n_kbdt            = {}; // N jets not from top by KBDT cut
  
  vector<double> eff    = {};
  vector<double> purity = {};


  // Set kbdt cuts in the whole range of kbdt
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get("dataset/Method_BDT/KBDT/MVA_KBDT_S");
  double x_min = h_NN_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_NN_classifier_output_S->GetXaxis()->GetBinUpEdge(h_NN_classifier_output_S->GetNbinsX());
  for (double i=x_min; i<=x_max; i += 0.05) {
    kbdt_cut.push_back(i); }
  
  // Loop over the entries
  for (int entry=0; entry<nEntries; entry++) {
    
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    test_tree->GetEntry(entry);
    
    for (int cut_i=0; cut_i<kbdt_cut.size(); cut_i++) {
      
      if (entry==0) {
	n_topHOF_and_kbdt.push_back(0);
	n_topHOF.push_back(0); 
	n_kbdt.push_back(0); }
      
      if (topHOF!=4) {
	n_topHOF[cut_i] += weight;
	if ( kbdt_val>kbdt_cut[cut_i] ) n_topHOF_and_kbdt[cut_i] += weight; }
	
      if (kbdt_val>kbdt_cut[cut_i]) n_kbdt[cut_i] += weight;
    
    } // [cut] - loop over given kbdt cut values under the study
    
  } // [entry] - loop over entries


  // Compute the discriminative power / efficiency
  for (int cut_i=0; cut_i<kbdt_cut.size(); cut_i++) {
    
    double eff_tmp = n_topHOF_and_kbdt[cut_i] / n_topHOF[cut_i];
    eff.push_back(eff_tmp);
    
    double tmp_purity = n_topHOF_and_kbdt[cut_i] / n_kbdt[cut_i];
    purity.push_back(tmp_purity);
    
    //cout << "KBDT cut = " << kbdt_cut[cut_i] << "  \tEff. = " << eff_tmp << "  \tPurity = " << tmp_purity << endl; 
  }


  // Close the input file
  tmva_file->Close();


  
  // Create TGraphs with eff|purity wrk kbdt cut and then plot them
  TGraph *gr_eff    = new TGraph(kbdt_cut.size(), &kbdt_cut[0], &eff[0]);
  TGraph *gr_purity = new TGraph(kbdt_cut.size(), &kbdt_cut[0], &purity[0]);

  // Crate a canvas
  TCanvas *c = new TCanvas("c", "c", 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();
  
  // Draw nicely
  gr_eff->SetLineColor(4);
  gr_eff->SetLineWidth(4);
  gr_eff->Draw("AC");
  gr_eff->SetTitle("");
  gr_eff->GetXaxis()->SetTitle("KBDT");
  gr_eff->GetYaxis()->SetTitle("Eff. | Purity");
  gr_purity->SetLineColor(417);
  gr_purity->SetLineWidth(4);
  gr_purity->Draw("same");

  // Create a legend
  TLegend *legend = new TLegend(0.7, 0.6, 0.9, 0.8);
  legend->AddEntry(gr_eff, "Efficiency");
  legend->AddEntry(gr_purity, "Purity");
  legend->Draw("same");

  // Save the plot
  c->Print("Plots/eff_purity_plot.png");
  
  

}
