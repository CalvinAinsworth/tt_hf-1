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


int const n_steps = 20; // number of steps in classifier cut on its whole range


/////////////////////
//   DRAW HISTS    //
/////////////////////
int draw_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, float classifier_cut, vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1 = true, double y_min = 0, double y_max = 1)
{
  cout << "Drawing " << savename << endl;
  
  // Create a canvas
  TCanvas *c = new TCanvas(savename, savename, 1600, 1200);
  gStyle->SetOptStat(0);
  

  // Get integrals and normalize if needed
  double h_s_int = h_s->Integral(0, h_s->GetNbinsX()+1);
  double h_b_int = h_b->Integral(0, h_b->GetNbinsX()+1);
  cout << "n signal events: " << h_s_int << endl;
  cout << "n background events: " << h_b_int << endl;
  
  if (norm_to_1 == true) {
    h_s->Scale(1/h_s_int);
    h_b->Scale(1/h_b_int); }


  // Define two TPads for distributions and ratio
  TPad *tPad = new TPad("tPad", "tPad", 0, 0.3, 1, 1);
  TPad *bPad = new TPad("bPad", "bPad", 0, 0,   1, 0.3);
  tPad->Draw();
  bPad->Draw("same");


  // Top pad: hists
  tPad->cd();
  tPad->SetGrid();
  tPad->SetRightMargin(0.05);
  tPad->SetLeftMargin(0.07);
  tPad->SetBottomMargin(0.02);
  tPad->SetTopMargin(0.03);
  
  h_s->SetLineColor(2);
  h_s->SetLineWidth(4);
  h_s->Draw("hist");
  h_s->SetTitle("");
  if (norm_to_1 == true) { 
    h_s->GetYaxis()->SetRangeUser(y_min, y_max); 
    h_s->GetYaxis()->SetTitle("#bf{norm to 1.}"); }
  else {
    gPad->SetLogy();
    h_s->GetYaxis()->SetRangeUser(1, pow(10,6) );
    h_s->GetYaxis()->SetTitle("#bf{Events}"); }
  h_s->GetYaxis()->SetTitleOffset(0.9);
  h_s->GetXaxis()->SetTitle("");
  h_s->GetXaxis()->SetLabelSize(0);

  h_b->SetLineColor(4);
  h_b->SetLineWidth(4);
  h_b->Draw("hist same");

  TLegend *legend = new TLegend(0.55, 0.8, 0.95, 0.95);
  int n_cut_digits = 0;
  if (classifier_cut >= 0) { n_cut_digits = 4; }
  else { n_cut_digits = 5; }
  TString classifier_legend_entry = legend_entries_titles[0] + " (cut = " + to_string(classifier_cut).substr(0,n_cut_digits) + ")";
  legend->AddEntry(h_s, classifier_legend_entry);
  legend->AddEntry(h_b, legend_entries_titles[1]);
  legend->SetTextSize(0.04);
  legend->Draw("same");


  // Bottom pad: ratio
  bPad->cd();
  bPad->SetGrid();
  bPad->SetRightMargin(0.05);
  bPad->SetLeftMargin(0.07);
  bPad->SetBottomMargin(0.3);
  bPad->SetTopMargin(0.02);
  
  TH1 *h_ratio = (TH1*)h_s->Clone();
  h_ratio->Divide(h_b);

  h_ratio->SetMarkerStyle(20);
  h_ratio->SetMarkerColor(1);
  h_ratio->SetMarkerSize(1.5);
  h_ratio->SetLineWidth(0);
  h_ratio->SetTitle("");
 
  h_ratio->GetYaxis()->SetTitle("Ratio");
  h_ratio->GetYaxis()->SetLabelSize(0.06);
  h_ratio->GetYaxis()->SetTitleOffset(0.5);
  h_ratio->GetYaxis()->SetTitleSize(0.07);
  h_ratio->GetYaxis()->CenterTitle();
  h_ratio->GetYaxis()->SetRangeUser(0,4);
  h_ratio->GetYaxis()->SetNdivisions(8);
  
  h_ratio->GetXaxis()->SetTitle(x_axis_title);
  h_ratio->GetXaxis()->SetTitleOffset(1.0);
  h_ratio->GetXaxis()->SetTitleSize(0.12);
  h_ratio->GetXaxis()->SetLabelSize(0.10);
  
  h_ratio->Draw("E1");

  // Draw a line along R=1
  TLine *line = new TLine(h_ratio->GetXaxis()->GetXmin(), 1, h_ratio->GetXaxis()->GetXmax(), 1);
  line->SetLineColor(9);
  line->SetLineWidth(3);
  line->SetLineStyle(7);
  line->Draw("same");

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
  ifstream config_file("tmva_config.txt", ios::binary);
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

  
  // Set Classifier cuts in the whole range of the classifier
  TFile *tmva_file = new TFile("TMVA.root");
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get("dataset/Method_" + method_name + "/" + method_name + "/MVA_" + method_name + "_S");
  double x_min = h_NN_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_NN_classifier_output_S->GetXaxis()->GetBinUpEdge(h_NN_classifier_output_S->GetNbinsX());
  double step = (x_max - x_min)/n_steps;
  vector<float> classifier_cuts = {};
  for (double i=x_min; i<=x_max; i += step) classifier_cuts.push_back(i);


  // Declare hists
  TH1 *h_dl1r_tag_weight_sig[n_steps];
  TH1 *h_dl1r_tag_weight_bkg[n_steps];
  TH1 *h_jet_pT_notfromtop_topHOF[n_steps];
  TH1 *h_jet_eta_notfromtop_topHOF[n_steps];
  TH1 *h_jet_phi_notfromtop_topHOF[n_steps];
  TH1 *h_jet_pT_notfromtop_classifier[n_steps];
  TH1 *h_jet_eta_notfromtop_classifier[n_steps];
  TH1 *h_jet_phi_notfromtop_classifier[n_steps];
  TH1 *h_n_add_jets_topHOF[n_steps];
  TH1 *h_n_add_jets_classifier[n_steps];
  for (int i=0; i<n_steps; i++) {  
    TString dl1r_sig_title = "DL1r_tag_sig" + to_string(i);
    h_dl1r_tag_weight_sig[i] = new TH1F(dl1r_sig_title, dl1r_sig_title, 100, -10, 20);
    TString dl1r_bkg_title = "DL1r_tag_bkg" + to_string(i);
    h_dl1r_tag_weight_bkg[i] = new TH1F(dl1r_bkg_title, dl1r_bkg_title, 100, -10, 20);
    TString jet_pT_topHOF_title = "pT_topHOF" + to_string(i);
    h_jet_pT_notfromtop_topHOF[i] = new TH1F(jet_pT_topHOF_title, jet_pT_topHOF_title, 20, 0, 1000 );
    TString jet_eta_topHOF_title = "eta_topHOF" + to_string(i);
    h_jet_eta_notfromtop_topHOF[i] = new TH1F(jet_eta_topHOF_title, jet_eta_topHOF_title, 40, -4.0, 4.0);
    TString jet_phi_topHOF_title = "phi_topHOF" + to_string(i);
    h_jet_phi_notfromtop_topHOF[i] = new TH1F(jet_phi_topHOF_title, jet_phi_topHOF_title, 40, -4.0, 4.0);
    TString jet_pT_classifier_title = "pT_classifier" + to_string(i);
    h_jet_pT_notfromtop_classifier[i] = new TH1F(jet_pT_classifier_title, jet_pT_classifier_title, 20, 0, 1000 );
    TString jet_eta_classifier_title = "eta_classifier" + to_string(i);
    h_jet_eta_notfromtop_classifier[i] = new TH1F(jet_eta_classifier_title, jet_eta_classifier_title, 40, -4.0, 4.0);
    TString jet_phi_classifier_title = "phi_classifier" + to_string(i);
    h_jet_phi_notfromtop_classifier[i] = new TH1F(jet_phi_classifier_title, jet_phi_classifier_title, 40, -4.0, 4.0); 
    TString n_add_jets_topHOF_title = "n_add_jets_topHOF" + to_string(i);
    h_n_add_jets_topHOF[i] = new TH1F(n_add_jets_topHOF_title, n_add_jets_topHOF_title, 7, 0, 7);
    TString n_add_jets_classifier_title = "n_add_jets_classifier" + to_string(i);
    h_n_add_jets_classifier[i] = new TH1F(n_add_jets_classifier_title, n_add_jets_classifier_title, 7, 0, 7);
  }
  

  // Declare counters of events
  float n1[n_steps] = {0}; // events with two b-tags from top according to classifier
  float n2 = 0; // events with two b-tags from top according to topHOF 
  float n3 = 0; // total number of events

  // Open the file and get a tree
  TFile *file = new TFile("skimmed_test_ntuple.root");
  TTree *tree = (TTree*)file->Get("test");


  // Set all the needed branches
  vector<float> *jet_DL1r_tag_weight, *classifier, *topHOF, *jet_isbtagged_DL1r_77, *jet_pt, *jet_eta, *jet_phi;
  float weight;
  jet_DL1r_tag_weight = jet_isbtagged_DL1r_77 = classifier = topHOF = jet_pt = jet_eta = jet_phi = 0;
  
  tree->SetBranchAddress("jet_DL1r", &jet_DL1r_tag_weight);
  tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77);
  tree->SetBranchAddress(method_name, &classifier);
  tree->SetBranchAddress("tot_event_weight", &weight);
  tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
  tree->SetBranchAddress("jet_pt", &jet_pt);
  tree->SetBranchAddress("jet_eta", &jet_eta);
  tree->SetBranchAddress("jet_phi", &jet_phi);
  
  
  // Loop over entries
  Int_t nEntries = tree->GetEntries();
  cout << "\tnumber of entries = " << nEntries << endl;
  
  for (int entry=0; entry<nEntries; entry++) {
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    tree->GetEntry(entry);

    // Loop over different cuts in classifier
    for (int cut_iter = 0; cut_iter < classifier_cuts.size(); cut_iter++) {
      
      // Counter for btags from top
      int n_jets_from_top_classifier = 0;
      int n_jets_from_top_topHOF = 0;
      
      // Loop over jets
      for (int jet_i = 0; jet_i<jet_DL1r_tag_weight->size(); jet_i++) {
        
	if ( (*classifier)[jet_i]<classifier_cuts[cut_iter] /*&& (*jet_isbtagged_DL1r_77)[jet_i]==1*/ ) n_jets_from_top_classifier++;
	if ( (*topHOF)[jet_i]==4 /*&& (*jet_isbtagged_DL1r_77)[jet_i]==1*/ ) n_jets_from_top_topHOF++;
	
	// Fill DL1r tag weight hist
	if ( (*classifier)[jet_i] >= classifier_cuts[cut_iter] ) { h_dl1r_tag_weight_sig[cut_iter]->Fill( (*jet_DL1r_tag_weight)[jet_i], weight); }
	else { h_dl1r_tag_weight_bkg[cut_iter]->Fill( (*jet_DL1r_tag_weight)[jet_i], weight); }
	
	// Fill jet_* hists
	if ( (*topHOF)[jet_i] != 4 ) { 
	  h_jet_pT_notfromtop_topHOF[cut_iter]->Fill( (*jet_pt)[jet_i]/1000, weight); 
	  h_jet_eta_notfromtop_topHOF[cut_iter]->Fill( (*jet_eta)[jet_i], weight);
	  h_jet_phi_notfromtop_topHOF[cut_iter]->Fill( (*jet_phi)[jet_i], weight); }
	if ( (*classifier)[jet_i] >= classifier_cuts[cut_iter] ) {
	  h_jet_pT_notfromtop_classifier[cut_iter]->Fill( (*jet_pt)[jet_i]/1000, weight);
	  h_jet_eta_notfromtop_classifier[cut_iter]->Fill( (*jet_eta)[jet_i], weight);
	  h_jet_phi_notfromtop_classifier[cut_iter]->Fill( (*jet_phi)[jet_i], weight); }
	
      } // [jet_i] - loop over jets
      
      // Fill additional jets hists
      int n_add_jets_topHOF = jet_DL1r_tag_weight->size() - n_jets_from_top_topHOF;
      h_n_add_jets_topHOF[cut_iter]->Fill(n_add_jets_topHOF, weight);
      int n_add_jets_classifier = jet_DL1r_tag_weight->size() - n_jets_from_top_classifier;
      h_n_add_jets_classifier[cut_iter]->Fill(n_add_jets_classifier, weight);

      // Update counters of events
      if (n_jets_from_top_classifier==2) n1[cut_iter] += weight;
      if (cut_iter==0 && n_jets_from_top_topHOF==2) n2 += weight;
      
    } // [cut_iter] - iteration over cuts values

    
    // Update the total number of events.
    n3 += weight;
    

  } // [entry] - loop over entries


  // Number of events
  for (int cut_iter=0; cut_iter < n_steps; cut_iter++) {
    cout << "\n\nclassifier: " << n1[cut_iter]/n3 << "\ntopHOF: " << n2/n3 << endl << endl;
  
    
    // Draw plots:    
    TString dl1r_tag_weights_savename = "DL1r_tag_weight_tmva__" + to_string(cut_iter);
    int draw_dl1r_tag_weights = draw_hists(h_dl1r_tag_weight_sig[cut_iter], h_dl1r_tag_weight_bkg[cut_iter], "#bf{DL1r tag weight}", dl1r_tag_weights_savename, classifier_cuts[cut_iter], {"Jets not from top\n", "Jets from top"}, true, 0, 0.1);

    TString jet_pt_savename = "jets_notfromtop_pt__" + to_string(cut_iter);
    int draw_jet_pt = draw_hists(h_jet_pT_notfromtop_classifier[cut_iter], h_jet_pT_notfromtop_topHOF[cut_iter], "#bf{jet p_{T}, MeV}", jet_pt_savename, classifier_cuts[cut_iter], {method_name, "topHOF"}, true, 0, 0.6);

    TString jet_eta_savename = "jets_notfromtop_eta__" + to_string(cut_iter);
    int draw_jet_eta = draw_hists(h_jet_eta_notfromtop_classifier[cut_iter], h_jet_eta_notfromtop_topHOF[cut_iter],  "#bf{jet #eta}", jet_eta_savename, classifier_cuts[cut_iter], {method_name, "topHOF"}, true, 0, 0.07);

    TString jet_phi_savename = "jets_notfromtop_phi__" + to_string(cut_iter);
    int draw_jet_phi = draw_hists(h_jet_phi_notfromtop_classifier[cut_iter], h_jet_phi_notfromtop_topHOF[cut_iter], "#bf{jet #phi}", jet_phi_savename, classifier_cuts[cut_iter], {method_name, "topHOF"}, true, 0, 0.06);

    TString n_add_jets_savename = "n_additional_jets__" + to_string(cut_iter);
    int draw_n_add_jets = draw_hists(h_n_add_jets_classifier[cut_iter], h_n_add_jets_topHOF[cut_iter], "#bf{N_{jets}^{additional}}", n_add_jets_savename, classifier_cuts[cut_iter], {method_name, "topHOF"}, true, 0, 0.6);
  }

  
  // Draw jet_* hists
  /*int draw_dl1r_tag_weights = draw_hists(h_dl1r_tag_weight_sig, h_dl1r_tag_weight_bkg, "#bf{DL1r tag weight}", "DL1r_tag_weight_tmva", {"Jets not from top", "Jets from top"});
  int draw_jet_pt = draw_hists(h_jet_pT_notfromtop_classifier, h_jet_pT_notfromtop_topHOF, "#bf{jet p_{T}, MeV}", "jets_notfromtop_pt", {method_name, "topHOF"}, false);
  int draw_jet_eta = draw_hists(h_jet_eta_notfromtop_classifier, h_jet_eta_notfromtop_topHOF, "#bf{jet #eta}", "jets_notfromtop_eta", {method_name, "topHOF"});
  int draw_jet_phi = draw_hists(h_jet_phi_notfromtop_classifier, h_jet_phi_notfromtop_topHOF, "#bf{jet #phi}", "jets_notfromtop_phi", {method_name, "topHOF"});
  */
}
