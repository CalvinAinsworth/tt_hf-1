#include <TH2.h>
#include <TTree.h>
#include <TFile.h>
#include <TSystemFile.h>
#include <TSystemDirectory.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPad.h>
#include <TMath.h>
#include <TLorentzVector.h>

#include <iostream>
#include <sstream>
#include <vector>



/////////////////////
//   DRAW HISTS    //
/////////////////////
int draw_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1 = true)
{
  cout << "Drawing " << savename << endl;

  TCanvas *c = new TCanvas(savename, savename, 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();

  if (norm_to_1 == true) {
    double h_s_int = h_s->Integral(0, h_s->GetNbinsX());
    h_s->Scale(1/h_s_int);
    double h_b_int = h_b->Integral(0, h_b->GetNbinsX());
    h_b->Scale(1/h_b_int); }

  h_s->SetLineColor(2);
  h_s->SetLineWidth(4);
  h_s->Draw("hist");
  h_s->SetTitle("");
  if (norm_to_1 == true) { h_s->GetYaxis()->SetRangeUser(0, 0.1); }
  else { h_s->GetYaxis()->SetRangeUser(0, 1.2); }
  if (norm_to_1 == true) h_s->GetYaxis()->SetTitle("#bf{norm to 1.}");
  h_s->GetXaxis()->SetTitle(x_axis_title);

  h_b->SetLineColor(4);
  h_b->SetLineWidth(4);
  h_b->Draw("hist same");

  TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
  legend->AddEntry(h_s, legend_entries_titles[0]);
  legend->AddEntry(h_b, legend_entries_titles[1]);
  legend->SetTextSize(0.04);
  legend->Draw("same");

  c->Print("Plots/"+savename+".png");
  cout << "Plotted " << savename << endl << endl << endl;

  return 0;
}



//////////////////
//     MAIN     //
/////////////////
void prepare_hists_classifier()
{
  // Get name of the TMVA method
  TString method_name = "";
  ifstream config_file("tmva_config.txt", ifstream::binary);
  if (config_file.is_open()) {
    string str1;
    string delim = ": ";
    while(getline(config_file, str1)) {
      string par_name = str1.substr(0, str1.find(delim));
      string par_val  = str1.substr(str1.find(delim)+2, str1.size()); // +2 due to delim length
      if (par_name == "TMVA Method")  method_name += par_val;
    }
  }
  config_file.close();


  // Declare hists
  TH1 *h_dl1r_tag_weight_sig = new TH1F("DL1r_tag_sig", "DL1r_tag_sig", 100, -10, 20);
  TH1 *h_dl1r_tag_weight_bkg = new TH1F("DL1r_tag_bkg", "DL1r_tag_bkg", 100, -10, 20);
    

  // Open the file and get a tree
  TFile *file = new TFile("skimmed_test_ntuple.root");
  TTree *tree = (TTree*)file->Get("test");


  // Set all the needed branches
  vector<float> *jet_DL1r_tag_weight, *classifier, *topHOF, *jet_DL1r_tag_status;
  float weight;
  jet_DL1r_tag_weight = jet_DL1r_tag_status = classifier = topHOF = 0;
  
  tree->SetBranchAddress("jet_DL1r", &jet_DL1r_tag_weight);
  tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_tag_status);
  tree->SetBranchAddress(method_name, &classifier);
  tree->SetBranchAddress("tot_event_weight", &weight);
  tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
  
  
  // Loop over entries
  Int_t nEntries = tree->GetEntries();
  cout << "\tnumber of entries = " << nEntries << endl;
  
  for (int entry=0; entry<nEntries; entry++) {
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    tree->GetEntry(entry);
    
    //cout << "\nEntry: " << entry << endl;
    
    // Loop over jets
    for (int jet_i = 0; jet_i<jet_DL1r_tag_weight->size(); jet_i++) {
        
      // Fill DL1r tag weight hist
      if ( (*classifier)[jet_i] >= 0.1 ) { h_dl1r_tag_weight_sig->Fill( (*jet_DL1r_tag_weight)[jet_i], weight); }
      else { h_dl1r_tag_weight_bkg->Fill( (*jet_DL1r_tag_weight)[jet_i], weight); }
      
      
    } // [jet_i] - loop over jets
  } // [entry] - loop over entries

  
  // Draw the DL1r tag distribution
  int draw_dl1r_tag_weights = draw_hists(h_dl1r_tag_weight_sig, h_dl1r_tag_weight_bkg, "#bf{DL1r tag weight}", "DL1r_tag_weight_tmva", {"Jets not from top", "Jets from top"});
  
}
