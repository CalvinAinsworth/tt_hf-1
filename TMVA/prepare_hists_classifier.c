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
#include <TAttLine.h>
#include <TLine.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


int const n_steps = 20; // number of steps in classifier cut on its whole range
vector<Int_t> colors = {632, 416+1, 600, 800-3, 432+2, 616+1, 400+1};



// ##############################
// ##   DRAW "Sig/Bkg" HISTS   ##
// ##############################
int draw_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, float classifier_cut, vector<TString> legend_entries_titles, bool norm_to_1, double y_min, double y_max);



// ###########################
// ## Draw a few histograms ##
// ###########################
int draw_n_hists(vector<TH1*> h_vec, vector<TString> h_title, TString x_axis_title, TString title, bool norm_to_0, Double_t y_min, Double_t y_max);



// ###################################
// ## Declare LaTeX func beforehand ##
// ###################################
int write_latex_header(TString tex_file_name);
int add_plots_to_tex_file(TString tex_file_name, TString gr0_name, TString gr1_name, bool conf_fig);
int write_latex_end_of_the_file(TString tex_file_name);



// ##################
// ##     MAIN     ##
// ##################
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
  tmva_file->Close();
  

  // Declare hists
  TH1 *h_dl1r_tag_weight_sig[n_steps];
  TH1 *h_dl1r_tag_weight_bkg[n_steps];
  TH1 *h_jet_pT_notfromtop_truth[n_steps];
  TH1 *h_jet_eta_notfromtop_truth[n_steps];
  TH1 *h_jet_phi_notfromtop_truth[n_steps];
  TH1 *h_jet_pT_notfromtop_classifier[n_steps];
  TH1 *h_jet_eta_notfromtop_classifier[n_steps];
  TH1 *h_jet_phi_notfromtop_classifier[n_steps];
  TH1 *h_n_add_jets_truth[n_steps];
  TH1 *h_n_add_jets_classifier[n_steps];
  TH1 *h_n_add_btags_classifier[n_steps];
  TH1 *h_n_add_btags_truth[n_steps];
  TH1 *h_n_add_bjets_classifier[n_steps];
  TH1 *h_n_add_bjets_truth[n_steps];
  for (int i=0; i<n_steps; i++) {  
    TString dl1r_sig_title = "DL1r_tag_sig" + to_string(i);
    h_dl1r_tag_weight_sig[i] = new TH1F(dl1r_sig_title, dl1r_sig_title, 100, -10, 20);
    TString dl1r_bkg_title = "DL1r_tag_bkg" + to_string(i);
    h_dl1r_tag_weight_bkg[i] = new TH1F(dl1r_bkg_title, dl1r_bkg_title, 100, -10, 20);
    TString jet_pT_truth_title = "pT_truth" + to_string(i);
    h_jet_pT_notfromtop_truth[i] = new TH1F(jet_pT_truth_title, jet_pT_truth_title, 20, 0, 1000 );
    TString jet_eta_truth_title = "eta_truth" + to_string(i);
    h_jet_eta_notfromtop_truth[i] = new TH1F(jet_eta_truth_title, jet_eta_truth_title, 40, -4.0, 4.0);
    TString jet_phi_truth_title = "phi_truth" + to_string(i);
    h_jet_phi_notfromtop_truth[i] = new TH1F(jet_phi_truth_title, jet_phi_truth_title, 40, -4.0, 4.0);
    TString jet_pT_classifier_title = "pT_classifier" + to_string(i);
    h_jet_pT_notfromtop_classifier[i] = new TH1F(jet_pT_classifier_title, jet_pT_classifier_title, 20, 0, 1000 );
    TString jet_eta_classifier_title = "eta_classifier" + to_string(i);
    h_jet_eta_notfromtop_classifier[i] = new TH1F(jet_eta_classifier_title, jet_eta_classifier_title, 40, -4.0, 4.0);
    TString jet_phi_classifier_title = "phi_classifier" + to_string(i);
    h_jet_phi_notfromtop_classifier[i] = new TH1F(jet_phi_classifier_title, jet_phi_classifier_title, 40, -4.0, 4.0); 
    TString n_add_jets_truth_title = "n_add_jets_truth" + to_string(i);
    h_n_add_jets_truth[i] = new TH1F(n_add_jets_truth_title, n_add_jets_truth_title, 7, 0, 7);
    TString n_add_jets_classifier_title = "n_add_jets_classifier" + to_string(i);
    h_n_add_jets_classifier[i] = new TH1F(n_add_jets_classifier_title, n_add_jets_classifier_title, 7, 0, 7);
    TString n_add_btags_classifier_title = "n_add_btags_classifier" + to_string(i);
    h_n_add_btags_classifier[i] = new TH1F(n_add_btags_classifier_title, n_add_btags_classifier_title, 6, 0, 6);
    TString n_add_btags_truth_title = "n_add_btags_truth" + to_string(i);
    h_n_add_btags_truth[i] = new TH1F(n_add_btags_truth_title, n_add_btags_truth_title, 6, 0, 6);
    TString n_add_bjets_classifier_title = "n_add_bjets_classifier" + to_string(i);
    h_n_add_bjets_classifier[i] = new TH1F(n_add_bjets_classifier_title, n_add_bjets_classifier_title, 6, 0 ,6);
    TString n_add_bjets_truth_title = "n_add_bjets_truth" + to_string(i);
    h_n_add_bjets_truth[i] = new TH1F(n_add_bjets_truth_title, n_add_bjets_truth_title, 6, 0, 6);
  }
  TH1 *h_lead_classifier_jet_tag[3];
  TH1 *h_lead_classifier_jet_pt[3];
  TH1 *h_lead_classifier_jet_truth_flav[3];
  for (int i=0; i<3; i++) {
    TString lead_classifier_jet_tag_title = "lead_classifier_jet_tag_" + to_string(i);
    h_lead_classifier_jet_tag[i] = new TH1F(lead_classifier_jet_tag_title, lead_classifier_jet_tag_title, 100, -10, 20);
    TString lead_classifier_jet_pt_title = "lead_classifier_jet_pt_" + to_string(i);
    h_lead_classifier_jet_pt[i] = new TH1F(lead_classifier_jet_pt_title, lead_classifier_jet_pt_title, 20, 0, 600);
    TString lead_classifier_jet_truth_flav_title = "h_lead_classifier_jet_truth_flav_" + to_string(i);
    h_lead_classifier_jet_truth_flav[i] = new TH1F(lead_classifier_jet_truth_flav_title, lead_classifier_jet_truth_flav_title, 6, 0, 6);
  }
  

  // Declare counters of events
  float n1[n_steps] = {0}; // events with two b-tags from top according to classifier
  float n2 = 0; // events with two b-tags from top on truth level 
  float n3 = 0; // total number of events


  // Open the file and get a tree
  TFile *file = new TFile("skimmed_test_ntuple.root");
  TTree *tree = (TTree*)file->Get("test");


  // Set all the needed branches
  vector<float> *jet_DL1r_tag_weight, *classifier, *jet_pt, *jet_eta, *jet_phi;
  vector<int> *topHOF, *jet_isbtagged_DL1r_77, *jet_truthflav;
  float weight;
  jet_DL1r_tag_weight = classifier = jet_pt = jet_eta = jet_phi = 0;
  topHOF = jet_isbtagged_DL1r_77 = jet_truthflav = 0;
  
  tree->SetBranchAddress("jet_DL1r", &jet_DL1r_tag_weight);
  tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77);
  tree->SetBranchAddress("jet_truthflav", &jet_truthflav);
  tree->SetBranchAddress(method_name, &classifier);
  tree->SetBranchAddress("tot_event_weight", &weight);
  tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
  tree->SetBranchAddress("jet_pt", &jet_pt);
  tree->SetBranchAddress("jet_eta", &jet_eta);
  tree->SetBranchAddress("jet_phi", &jet_phi);
  
  
  // Loop over events
  Int_t nEntries = tree->GetEntries();
  cout << "\tnumber of entries = " << nEntries << endl;
  
  for (int entry=0; entry<nEntries; entry++) {
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    tree->GetEntry(entry);


    // ///
    // Fill jet_* and n_add_* hsts in classifier_cut slices
    // ///
    
    // Loop over different cuts in classifier
    for (int cut_iter = 0; cut_iter < n_steps; cut_iter++) {
      
      // Counter for btags from top
      int n_jets_from_top_classifier = 0;
      int n_btags_from_top_classifier = 0;
      int n_bjets_from_top_classifier = 0;
      int n_jets_from_top_truth = 0;
      int n_btags_from_top_truth = 0;
      int n_bjets_from_top_truth = 0;
      
      // Loop over jets
      for (int jet_i = 0; jet_i<jet_DL1r_tag_weight->size(); jet_i++) {
	
	if ( (*classifier)[jet_i]<classifier_cuts[cut_iter] ) {
	  n_jets_from_top_classifier++;
	  if ( (*jet_isbtagged_DL1r_77)[jet_i]==1 ) n_btags_from_top_classifier++;
	  if ( (*jet_truthflav)[jet_i]==5 ) n_bjets_from_top_classifier++; }
	
	if ( (*topHOF)[jet_i]==4 ) {
	  n_jets_from_top_truth++;
	  if ( (*jet_isbtagged_DL1r_77)[jet_i] ) n_btags_from_top_truth++;
	  if ( (*jet_truthflav)[jet_i]==5 ) n_bjets_from_top_truth++; }
	
	// Fill DL1r tag weight hist
	if ( (*classifier)[jet_i] >= classifier_cuts[cut_iter] ) { h_dl1r_tag_weight_sig[cut_iter]->Fill( (*jet_DL1r_tag_weight)[jet_i], weight); }
	else { h_dl1r_tag_weight_bkg[cut_iter]->Fill( (*jet_DL1r_tag_weight)[jet_i], weight); }
	
	// Fill jet_* hists
	if ( (*topHOF)[jet_i] != 4 ) { 
	  h_jet_pT_notfromtop_truth[cut_iter]->Fill( (*jet_pt)[jet_i]/1000, weight); 
	  h_jet_eta_notfromtop_truth[cut_iter]->Fill( (*jet_eta)[jet_i], weight);
	  h_jet_phi_notfromtop_truth[cut_iter]->Fill( (*jet_phi)[jet_i], weight); }
	if ( (*classifier)[jet_i] >= classifier_cuts[cut_iter] ) {
	  h_jet_pT_notfromtop_classifier[cut_iter]->Fill( (*jet_pt)[jet_i]/1000, weight);
	  h_jet_eta_notfromtop_classifier[cut_iter]->Fill( (*jet_eta)[jet_i], weight);
	  h_jet_phi_notfromtop_classifier[cut_iter]->Fill( (*jet_phi)[jet_i], weight); }
	
      } // [jet_i] - loop over jets
      

      // Fill additional jets hists
      int n_add_jets_truth = jet_DL1r_tag_weight->size() - n_jets_from_top_truth;
      h_n_add_jets_truth[cut_iter]->Fill(n_add_jets_truth, weight);
      int n_add_jets_classifier = jet_DL1r_tag_weight->size() - n_jets_from_top_classifier;
      h_n_add_jets_classifier[cut_iter]->Fill(n_add_jets_classifier, weight);
      int n_add_btags_truth = jet_pt->size() - n_btags_from_top_truth;
      h_n_add_btags_truth[cut_iter]->Fill(n_add_btags_truth, weight);
      int n_add_btags_classifier = jet_pt->size() - n_btags_from_top_classifier;
      h_n_add_btags_classifier[cut_iter]->Fill(n_add_btags_classifier, weight);
      int n_add_bjets_truth = jet_pt->size() - n_bjets_from_top_truth;
      h_n_add_bjets_truth[cut_iter]->Fill(n_add_bjets_truth, weight);
      int n_add_bjets_classifier = jet_pt->size() - n_bjets_from_top_classifier;
      h_n_add_bjets_classifier[cut_iter]->Fill(n_add_bjets_classifier, weight);

        
      // Update counters of events
      if (n_jets_from_top_classifier==2) n1[cut_iter] += weight;
      if (cut_iter==0 && n_jets_from_top_truth==2) n2 += weight;
      
    } // [cut_iter] - iteration over cuts values
    
    
    // Update the total number of events.
    n3 += weight;



    // ///
    // Analyze the jet with the highest BDT value
    // ///

    // A trick to "arrange" jets wrt other than pT variable 
    // create a vector of pairs that will consist of {classifier_val, jet_#} pairs 
    vector<pair<float, int>> jet_classifier_jetnumber = {};
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      jet_classifier_jetnumber.push_back( make_pair( (*classifier)[jet_i], jet_i) );
    } // [jet_i] - loop over jets

    // Sort the vector of pair wrt the classifier value from the lowest to the highst
    sort(jet_classifier_jetnumber.begin(), jet_classifier_jetnumber.end());

    // Declare a vector of indeces from the original order for the jets "ordered" wrt the classifier value 
    vector<int> JN;
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      JN.push_back(jet_classifier_jetnumber[jet_i].second);
    } // [jet_i] - loop over jets
    
    int last_jet_index = (*jet_pt).size() - 1;
     
    for (int i=0; i<3; i++) {
      int idx = last_jet_index - i;
      h_lead_classifier_jet_tag[i]->Fill( (*jet_DL1r_tag_weight)[JN[idx]], weight );
      h_lead_classifier_jet_pt[i]->Fill( (*jet_pt)[JN[idx]]/1000, weight );
      h_lead_classifier_jet_truth_flav[i]->Fill( (*jet_truthflav)[JN[idx]], weight );
    }

    

  } // [entry] - loop over entries



  // Number of events
  int dl1r_tag_plots_tex_begin = write_latex_header("plots_dl1r_tag.tex");
  int jet_pt_plots_tex_begin = write_latex_header("plots_jet_pt.tex");
  int jet_eta_plots_begin = write_latex_header("plots_jet_eta.tex");
  int jet_phi_plots_begin = write_latex_header("plots_jet_phi.tex");
  int n_add_jets_plots_begin = write_latex_header("plots_n_add_jets.tex");
  int n_add_btags_plots_begin = write_latex_header("plots_n_add_btags.tex");
  int n_add_bjets_plots_begin = write_latex_header("plots_n_add_bjets.tex");
  int n_add_btags_vs_bjets_begin = write_latex_header("plots_n_add_btags_vs_bjets.tex");
  
  for (int cut_iter=0; cut_iter < n_steps; cut_iter++) {
    cout << "\n\nclassifier: " << n1[cut_iter]/n3 << "\ntopHOF: " << n2/n3 << endl << endl;
    
    // Draw plots:   
 
    TString dl1r_tag_weights_savename = "DL1r_tag_weight_tmva__" + to_string(cut_iter);
    int draw_dl1r_tag_weights = draw_hists(h_dl1r_tag_weight_sig[cut_iter], h_dl1r_tag_weight_bkg[cut_iter], "#bf{DL1r tag weight}", dl1r_tag_weights_savename, classifier_cuts[cut_iter], {"Jets not from top\n", "Jets from top"}, true, 0, 0.1);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "DL1r_tag_weight_tmva__" + to_string(cut_iter-1);
      TString gr1_name = "DL1r_tag_weight_tmva__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_dl1r_tag.tex", gr0_name, gr1_name, cont_float); }

    
    TString jet_pt_savename = "jets_notfromtop_pt__" + to_string(cut_iter);
    int draw_jet_pt = draw_hists(h_jet_pT_notfromtop_classifier[cut_iter], h_jet_pT_notfromtop_truth[cut_iter], "#bf{jet p_{T}, MeV}", jet_pt_savename, classifier_cuts[cut_iter], {method_name, "true"}, true, 0, 0.6);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "jets_notfromtop_pt__" + to_string(cut_iter-1);
      TString gr1_name = "jets_notfromtop_pt__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_jet_pt.tex", gr0_name, gr1_name, cont_float); }
    
    
    TString jet_eta_savename = "jets_notfromtop_eta__" + to_string(cut_iter);
    int draw_jet_eta = draw_hists(h_jet_eta_notfromtop_classifier[cut_iter], h_jet_eta_notfromtop_truth[cut_iter],  "#bf{jet #eta}", jet_eta_savename, classifier_cuts[cut_iter], {method_name, "true"}, true, 0, 0.07);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "jets_notfromtop_eta__" + to_string(cut_iter-1);
      TString gr1_name = "jets_notfromtop_eta__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_jet_eta.tex", gr0_name, gr1_name, cont_float); }

      
    TString jet_phi_savename = "jets_notfromtop_phi__" + to_string(cut_iter);
    int draw_jet_phi = draw_hists(h_jet_phi_notfromtop_classifier[cut_iter], h_jet_phi_notfromtop_truth[cut_iter], "#bf{jet #phi}", jet_phi_savename, classifier_cuts[cut_iter], {method_name, "true"}, true, 0, 0.06);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "jets_notfromtop_phi__" + to_string(cut_iter-1);
      TString gr1_name = "jets_notfromtop_phi__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_jet_phi.tex", gr0_name, gr1_name, cont_float); }
      
    
    TString n_add_jets_savename = "n_additional_jets__" + to_string(cut_iter);
    int draw_n_add_jets = draw_hists(h_n_add_jets_classifier[cut_iter], h_n_add_jets_truth[cut_iter], "#bf{N_{jets}^{additional}}", n_add_jets_savename, classifier_cuts[cut_iter], {method_name, "true"}, true, 0, 0.6);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_jets__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_jets__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_jets.tex", gr0_name, gr1_name, cont_float); }
    
  
    TString n_add_btags_savename = "n_additional_btags__" + to_string(cut_iter);
    int draw_n_add_btags = draw_hists(h_n_add_btags_classifier[cut_iter], h_n_add_btags_truth[cut_iter], "#bf{N_{btags}^{additional}}", n_add_btags_savename, classifier_cuts[cut_iter], {method_name, "truth"}, true, 0, 0.6);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_btags__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_btags__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_btags.tex", gr0_name, gr1_name, cont_float); }
      
    
    TString n_add_bjets_savename = "n_additional_bjets__" + to_string(cut_iter);
    int draw_n_add_bjets = draw_hists(h_n_add_bjets_classifier[cut_iter], h_n_add_bjets_truth[cut_iter], "#bf{N_{bjets}^{additional}}", n_add_bjets_savename, classifier_cuts[cut_iter], {method_name, "truth"}, true, 0, 0.6);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_bjets__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_bjets__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_bjets.tex", gr0_name, gr1_name, cont_float); }
    
  
    TString n_add_btags_vs_bjets_savename = "n_additional_btags_vs_bjets__" + to_string(cut_iter);
    int draw_n_add_btags_vs_bjets = draw_hists(h_n_add_btags_classifier[cut_iter], h_n_add_bjets_classifier[cut_iter], "#bf{N_{b(tags/jets)}^{additional}}", n_add_btags_vs_bjets_savename, classifier_cuts[cut_iter], {"add. btags", "add. bjets"}, true, 0, 0.6);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_btags_vs_bjets__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_btags_vs_bjets__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_btags_vs_bjets.tex", gr0_name, gr1_name, cont_float); }

  } // [cut_iter] - loop over cut slices


  int dl1r_tag_plots_tex_end = write_latex_end_of_the_file("plots_dl1r_tag.tex");
  int jet_pt_plots_tex_end = write_latex_end_of_the_file("plots_jet_pt.tex");
  int jet_eta_plots_end = write_latex_end_of_the_file("plots_jet_eta.tex");
  int jet_phi_plots_end = write_latex_end_of_the_file("plots_jet_phi.tex");
  int n_add_jets_plots_end = write_latex_end_of_the_file("plots_n_add_jets.tex");
  int n_add_btags_plots_end = write_latex_end_of_the_file("plots_n_add_btags.tex");
  int n_add_bjets_plots_end = write_latex_end_of_the_file("plots_n_add_bjets.tex");
  int n_add_btags_vs_bjets_end = write_latex_end_of_the_file("plots_n_add_btags_vs_bjets.tex");
  
  

  // Common title for the three leading classifier jets
  vector<TString> lead_classifier_jets_titles = {"lead jet " + method_name, "2nd-lead jet " + method_name, "3rd-lead jet " + method_name}; 
  
  // DL1r tag weigth of the three leading classifier jets
  vector<TH1*> collection_h_lead_classifier_jet_tag = {h_lead_classifier_jet_tag[0], h_lead_classifier_jet_tag[1], h_lead_classifier_jet_tag[2]};
  int draw_lead_classifier_jet_tag = draw_n_hists(collection_h_lead_classifier_jet_tag, lead_classifier_jets_titles, "#bf{DL1r tag weight}", "lead_classifier_jet_tag", true, 0, 0.1);

  // pT of the three leading classifier jets
  vector<TH1*> collection_h_lead_classifier_jet_pt = {h_lead_classifier_jet_pt[0], h_lead_classifier_jet_pt[1], h_lead_classifier_jet_pt[2]};
  int draw_lead_classifier_jet_pt = draw_n_hists(collection_h_lead_classifier_jet_pt, lead_classifier_jets_titles, "#bf{p_{T}^{jet}}", "lead_classifier_jet_pt", true, 0, 0.5);

  // truth flav of the three leading classifier jets
  vector<TH1*> collection_h_lead_classifier_jet_truth_flav = {h_lead_classifier_jet_truth_flav[0], h_lead_classifier_jet_truth_flav[1], h_lead_classifier_jet_truth_flav[2]};
  int draw_lead_classifier_jet_truth_flav = draw_n_hists(collection_h_lead_classifier_jet_truth_flav, lead_classifier_jets_titles, "#bf{truth flav}", "lead_classifier_jet_truth_flav", true, 0, 1.3);
  
  
  
  

  
  // Draw jet_* hists
  /*int draw_dl1r_tag_weights = draw_hists(h_dl1r_tag_weight_sig, h_dl1r_tag_weight_bkg, "#bf{DL1r tag weight}", "DL1r_tag_weight_tmva", {"Jets not from top", "Jets from top"});
  int draw_jet_pt = draw_hists(h_jet_pT_notfromtop_classifier, h_jet_pT_notfromtop_topHOF, "#bf{jet p_{T}, MeV}", "jets_notfromtop_pt", {method_name, "topHOF"}, false);
  int draw_jet_eta = draw_hists(h_jet_eta_notfromtop_classifier, h_jet_eta_notfromtop_topHOF, "#bf{jet #eta}", "jets_notfromtop_eta", {method_name, "topHOF"});
  int draw_jet_phi = draw_hists(h_jet_phi_notfromtop_classifier, h_jet_phi_notfromtop_topHOF, "#bf{jet #phi}", "jets_notfromtop_phi", {method_name, "topHOF"});
  */
} // END OF MAIN




// ##############################
// ##   DRAW "Sig/Bkg" HISTS   ##
// ##############################
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

  c->Print("Plots/cut_iter_slices/"+savename+".png");
  cout << "Plotted " << savename << endl << endl << endl;

  return 0;
}



// ###########################
// ## Draw a few histograms ##
// ###########################
int draw_n_hists(vector<TH1*> h_vec, vector<TString> h_title, TString x_axis_title, TString title, bool norm_to_1=false, Double_t y_min=0, Double_t y_max=10000)
{
  // Draws N histogras in one canvas (not stacked)
  cout << "Start drawing " << title << " !" << endl;
  if (h_vec.size()==0) { cout << "h_vec is emmpty, aborting!!!" << endl; return 0; }


  TCanvas *c = new TCanvas(h_title[0], h_title[0], 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();
  if (norm_to_1==false) gPad->SetLogy();
  double legend_height = 0.09*h_vec.size();
  double legend_y1 = 0.90 - legend_height;
  TLegend *legend = new TLegend(0.70, legend_y1, 0.90, 0.90);

  for (int i=0; i<h_vec.size(); i++){

    if (!h_vec[i]) cout << "Requested object TH_[" << i << "] wasn't found!" << endl;

    double h_int = h_vec[i]->Integral(0, h_vec[i]->GetNbinsX()+1);
    double sf = 1/h_int;
    cout << "Hist: " << h_title[i] << " , Integral: " << h_int << endl;

    h_vec[i]->SetMarkerStyle(20);
    h_vec[i]->SetMarkerSize(2);
    h_vec[i]->SetMarkerColor(colors[i]);
    h_vec[i]->SetLineColor(colors[i]);
    h_vec[i]->SetLineWidth(4);
    if (norm_to_1==true) h_vec[i]->Scale(sf);

    if (i==0) {
      h_vec[i]->Draw("hist");
      h_vec[i]->SetTitle(title);

      if (norm_to_1==true) {
        h_vec[i]->GetYaxis()->SetRangeUser(y_min, y_max);
        h_vec[i]->GetYaxis()->SetTitle("#bf{Events, norm to 1}"); }
      else {
        if (y_min==0) y_min = 1;
        h_vec[i]->GetYaxis()->SetRangeUser(y_min, y_max);
        h_vec[i]->GetYaxis()->SetTitle("#bf{Events}"); }

      h_vec[i]->GetXaxis()->SetTitle(x_axis_title); }

    else { h_vec[i]->Draw("hist same"); }
    legend->AddEntry(h_vec[i], h_title[i]); }
  legend->Draw("same");

  c->Print("Plots/" + title + ".png");
  cout << "Drawn " + title + " !\n\n" << endl;

  return 0;
}




// ######################################
// ## Define the write LaTeX functions ##
// ######################################
int write_latex_header(TString tex_file_name) 
{
  ofstream tex_file("Plots/cut_iter_slices/" + tex_file_name, ios::binary);

  tex_file << "\\documentclass[12pt]{extarticle}" << endl;
  tex_file << "\\usepackage{lineno}                 % line numbering" << endl;
  tex_file << "\\linenumbers                        % line numbering" << endl;
  tex_file << "\\usepackage[margin=2cm]{geometry}   % set left and right margins" << endl;
  tex_file << "\\usepackage[none]{hyphenat}         % do not break words on the end of a line" << endl;
  tex_file << "\\usepackage{blindtext}              % something for a table of contents" << endl;
  tex_file << "\\usepackage{color}                  % use blue color for hyperlinks, need to enable colors" << endl;
  tex_file << "\\usepackage[citecolor=blue]{hyperref}" << endl;
  tex_file << "\\hypersetup{" << endl;
  tex_file << "\tcolorlinks=true, % make the links colored" << endl;
  tex_file << "\tlinkcolor=blue, % color TOC links in blue" << endl;
  tex_file << "\turlcolor=red, % color URLs in red" << endl;
  tex_file << "\tlinktoc=all % 'all' will create links for everything in the TOC \n}" << endl;
  tex_file << "\\usepackage{booktabs}               % for tables midrule and toprule" << endl;
  tex_file << "\\usepackage{multirow}               % for multirow in tables" << endl;
  tex_file << "\\usepackage{float} % this and following needed for usage of [H] in tables declaration" << endl;
  tex_file << "\\restylefloat{table}" << endl;
  tex_file << "\\usepackage{graphicx}               % This and the next two for pictures" << endl;
  tex_file << "\\usepackage{caption}" << endl;
  tex_file << "\\usepackage{subcaption}" << endl;
  tex_file << "\\usepackage{subfiles}               % To add subfiles: one tex as a combination of a few texes" << endl;
  tex_file << "\\usepackage{amsmath}                % for matrices" << endl;
  tex_file << "\\usepackage{indentfirst} % indent the firs paragraph of a section" << endl;
  tex_file << "\\usepackage{datetime}               % to show time" << endl;
  tex_file << "\\usepackage{textcomp}               % degree sign" << endl;
  tex_file << "\n\\begin{document}\n\n" << endl;  
  
  tex_file.close();
  return 0;
}




// #################################
// ## Add plots to the LaTeX file ##
// #################################
int add_plots_to_tex_file(TString tex_file_name, TString gr0_name, TString gr1_name, bool cont_fig=false)
{
  ofstream tex_file("Plots/cut_iter_slices/" + tex_file_name, ios_base::app);

  TString continued_float = "";
  if (cont_fig==true) continued_float = "\\ContinuedFloat";
  
  tex_file << "\n\n" << endl;
  tex_file << "\\begin{figure}[H]" << continued_float << endl;
  tex_file << "\t\\centering" << endl;
  tex_file << "\t\\captionsetup{justification=centering}" << endl;
  tex_file << "\t\\begin{subfigure}[t]{0.45\\textwidth}" << endl;
  tex_file << "\t\t\\centering" << endl;
  tex_file << "\t\t\\includegraphics[width=\\textwidth]{" << gr0_name << "}" << endl; // add gr name
  tex_file << "\t\t\\caption{}" << endl;
  tex_file << "\t\\end{subfigure}" << endl;
  tex_file << "\t\\hspace{2mm}" << endl;
  tex_file << "\t\\begin{subfigure}[t]{0.45\\textwidth}" << endl;
  tex_file << "\t\t\\centering" << endl;
  tex_file << "\t\t\\includegraphics[width=\\textwidth]{" << gr1_name << "}" << endl; // add gr name
  tex_file << "\t\t\\caption{}" << endl;
  tex_file << "\t\\end{subfigure}" << endl;
  tex_file << "\\end{figure}" << endl;

  tex_file.close();
  return 0;
}




// #####################################################
// ## Write the \end statement in the end of teh file ##
// #####################################################
int write_latex_end_of_the_file(TString tex_file_name)
{
  ofstream tex_file("Plots/cut_iter_slices/" + tex_file_name, ios_base::app);
  tex_file << "\n\n\\end{document}" << endl;
  return 0;
}
