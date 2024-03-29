#include <TSystem.h>
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
#include <TGraphAsymmErrors.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


int const n_steps = 20; // number of steps in classifier cut on its whole range
vector<Int_t> colors = {632, 416+1, 600, 800-3, 432+2, 616+1, 400+1};



// ##############################
// ##   DRAW "Sig/Bkg" HISTS   ##
// ##############################
int draw_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, float classifier_cut, vector<TString> legend_entries_titles, bool norm_to_1, double y_min, double y_max, double ratio_y_min, double ratio_y_max);



// ###########################
// ## Draw a few histograms ##
// ###########################
int draw_n_hists(vector<TH1*> h_vec, vector<TString> h_title, TString x_axis_title, TString title, bool norm_to_1, Double_t y_min, Double_t y_max);



// ##########################
// ##     Draw TGraphs     ##
// ##########################
int draw_graphs(vector<TGraphAsymmErrors*> gr, TString x_axis_title, TString y_axis_title, vector<TString> legend_entries, TString savename);



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


  // Create directory for plots
  gSystem->Exec("mkdir Plots");
  gSystem->Exec("mkdir Plots/cut_iter_slices");

  
  // Set Classifier cuts in the whole range of the classifier
  TFile *tmva_file = new TFile("results/004/TMVA.root");
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get("dataset/Method_" + method_name + "/" + method_name + "/MVA_" + method_name + "_S");
  double x_min = h_NN_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_NN_classifier_output_S->GetXaxis()->GetBinUpEdge(h_NN_classifier_output_S->GetNbinsX());
  double step = (x_max - x_min)/n_steps;
  double n_bins_classifier = h_NN_classifier_output_S->GetNbinsX();
  vector<float> classifier_cuts = {};
  for (double i=x_min; i<=x_max; i += step) classifier_cuts.push_back(i);
  tmva_file->Close();
 
  

  // Declare hists for number of additional X
  TH1 *h_n_add_jets_truth[n_steps];
  TH1 *h_n_add_jets_classifier[n_steps];
  TH1 *h_n_add_btags_classifier[n_steps];
  TH1 *h_n_add_btags_truth[n_steps];
  TH1 *h_n_add_bjets_classifier[n_steps];
  TH1 *h_n_add_bjets_truth[n_steps];
  for (int i=0; i<n_steps; i++) {
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
    TString add_truth_bjet_pt_title = "add_truth_bjet_pt" + to_string(i);
  }

  // Hist for the additional bjets and btags
  TH1 *h_add_truth_bjet_pt = new TH1F("add_truth_bjet_pt", "add_truth_bjet_pt", 20, 0, 1000);
  TH1 *h_add_truth_btag_pt = new TH1F("add_truth_btag_pt", "add_truth_btag_pt", 20, 0, 1000);
  TH1 *h_add_classifier_bjet_pt = new TH1F("add_classifier_bjet_pt", "add_classifier_bjet_pt", 20, 0, 1000);
  TH1 *h_add_classifier_btag_pt = new TH1F("add_classifier_btag_pt", "add_classifier_btag_pt", 20, 0, 1000);
  TH1 *h_add_bjet_missmatch = new TH1F("add_bjet_missmatch", "add_bjet_missmatch", 2, 0, 2);
  TH1 *h_add_btag_missmatch = new TH1F("add_btag_missmatch", "add_btag_missmatch", 2, 0, 2);

  // Missidentified vs. correctly identified additional btags
  TH1 *h_mi_add_btag_pt = new TH1F("mi_btag_pt", "mi_btag_pt", 20, 0, 1000);
  TH1 *h_ci_add_btag_pt = new TH1F("ci_btag_pt", "ci_btag_pt", 20, 0, 1000);
  TH1 *h_mi_add_btag_classifier = new TH1F("mi_add_btag_classifier", "mi_add_btag_classifier", n_bins_classifier, x_min, x_max);
  TH1 *h_ci_add_btag_classifier = new TH1F("ci_add_btag_classifier", "ci_add_btag_classifier", n_bins_classifier, x_min, x_max);
  TH1 *h_mi_add_btag_DL1r_idx = new TH1F("mi_add_btag_DL1r_idx", "mi_add_btag_DL1r_idx", 4, 0, 4);
  TH1 *h_ci_add_btag_DL1r_idx = new TH1F("ci_add_btag_DL1r_idx", "ci_add_btag_DL1r_idx", 4, 0, 4);
  TH1 *h_mi_add_btag_classifier_idx = new TH1F("mi_add_btag_classifier_idx", "mi_add_btag_classifier_idx", 3, 0, 3);
  TH1 *h_ci_add_btag_classifier_idx = new TH1F("ci_add_btag_classifier_idx", "ci_add_btag_classifier_idx", 3, 0 ,3);
  TH1 *h_mi_add_btag_classifier_sliced[n_steps];
  TH1 *h_ci_add_btag_classifier_sliced[n_steps];
  for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
    TString mi_title = "mi" + to_string(cut_iter);
    TString ci_title = "ci" + to_string(cut_iter);
    h_mi_add_btag_classifier_sliced[cut_iter] = new TH1F(mi_title, mi_title, n_bins_classifier, x_min, x_max);
    h_ci_add_btag_classifier_sliced[cut_iter] = new TH1F(ci_title, ci_title, n_bins_classifier, x_min, x_max);
  }

  // Counters of event for the 3rd BDT btag studies
  float n_ci_3rd[n_steps] = {0};
  float n_mi_3rd[n_steps] = {0};
  float n_all_3rd[n_steps] = {0};
  float n_tot_3rd = 0;
  float prob_3rd[n_steps] = {0};
  float prob_3rd_err[n_steps] = {0};
  float prob_3rd_err_h[n_steps] = {0};
  float prob_3rd_err_l[n_steps]= {0};
  float eff_3rd[n_steps] = {0};
  float eff_3rd_err[n_steps] = {0};
  float eff_3rd_err_h[n_steps] = {0};
  float eff_3rd_err_l[n_steps] = {0};
  float classifier_cuts_err[n_steps] = {0};

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

    
    // Get order of jets wrt DL1r value from H to L
    vector<pair<float, int>> jet_DL1r_jetnumber = {};
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      jet_DL1r_jetnumber.push_back( make_pair( (*jet_DL1r_tag_weight)[jet_i], jet_i) );
    } // [jet_i] - loop over jet

    // Sort the vector of pairs wrt DL1r tag weight from the highest to the lowest
    sort(jet_DL1r_jetnumber.rbegin(), jet_DL1r_jetnumber.rend());

    // Declare a vector of indeces from the original order for the jets "ordered" wrt the highest DL1r value
    vector<int> JN_DL1r;
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      JN_DL1r.push_back(jet_DL1r_jetnumber[jet_i].second);
    } // [jet_i] - loop over jets


    // Get order of jets wrt classifier value from L to H 
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

   

    // ///
    // Fill jet_* and n_add_* hsts in classifier_cut slices
    // ///
    
    // Loop over different cuts in classifier
    for (int cut_iter = 0; cut_iter < n_steps; cut_iter++) {
      
      // Counter for btags and bjets from top
      int n_jets_from_top_classifier = 0;
      int n_btags_from_top_classifier = 0;
      int n_bjets_from_top_classifier = 0;
      int n_jets_from_top_truth = 0;
      int n_btags_from_top_truth = 0;
      int n_bjets_from_top_truth = 0;
      int n_btags = 0;
      int n_bjets = 0;     
 
      // Loop over jets
      for (int jet_i = 0; jet_i<jet_DL1r_tag_weight->size(); jet_i++) {
	
        // Additional btags and bjets wrt classifier
	if ( (*classifier)[JN[jet_i]]<classifier_cuts[cut_iter] && n_btags_from_top_classifier<2) {
	  n_jets_from_top_classifier++;
	  if ( (*jet_isbtagged_DL1r_77)[JN[jet_i]]==1 ) n_btags_from_top_classifier++;
	  if ( (*jet_truthflav)[JN[jet_i]]==5 ) n_bjets_from_top_classifier++; }
	
        // Additional btags and bjets on the truth level
	if ( (*topHOF)[JN[jet_i]]==4 ) {
	  n_jets_from_top_truth++;
	  if ( (*jet_isbtagged_DL1r_77)[JN[jet_i]]==1 ) n_btags_from_top_truth++;
	  if ( (*jet_truthflav)[JN[jet_i]]==5 ) n_bjets_from_top_truth++; }

        // Total number of btags and bjets and index of the additional btag and bjet
        if ( (*jet_isbtagged_DL1r_77)[JN[jet_i]]==1 ) n_btags++;
        if ( (*jet_truthflav)[JN[jet_i]]==5 ) n_bjets++;

      } // [jet_i] - loop over jets
      

      // Fill additional jets hists
      int n_add_jets_truth = jet_pt->size() - n_btags_from_top_truth;
      h_n_add_jets_truth[cut_iter]->Fill(n_add_jets_truth, weight);
      int n_add_jets_classifier = jet_pt->size() - n_btags_from_top_classifier;
      h_n_add_jets_classifier[cut_iter]->Fill(n_add_jets_classifier, weight);
      int n_add_btags_truth = n_btags - n_btags_from_top_truth;
      h_n_add_btags_truth[cut_iter]->Fill(n_add_btags_truth, weight);
      int n_add_btags_classifier = n_btags - n_btags_from_top_classifier;
      h_n_add_btags_classifier[cut_iter]->Fill(n_add_btags_classifier, weight);
      int n_add_bjets_truth = n_bjets - n_bjets_from_top_truth;
      h_n_add_bjets_truth[cut_iter]->Fill(n_add_bjets_truth, weight);
      int n_add_bjets_classifier = n_bjets - n_bjets_from_top_classifier;
      h_n_add_bjets_classifier[cut_iter]->Fill(n_add_bjets_classifier, weight);

        
      // Update counters of events
      if (n_jets_from_top_classifier==2) n1[cut_iter] += weight;
      if (cut_iter==0 && n_jets_from_top_truth==2) n2 += weight;
      


      // ///
      // Analyze btag with the highest BDT value
      // ///

      
      // Vector of indexes for btags only
      vector<int> classifier_btags_idxes;
      for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	if ( (*jet_isbtagged_DL1r_77)[JN[jet_i]]==1 ) {
	  classifier_btags_idxes.push_back(JN[jet_i]); } } // [jet_i] - loop over jets
      
      // N correctly identified and miss-identified additional btags in classifier cut slices
      if (classifier_btags_idxes.size()==3) {
	
	if ( (*classifier)[classifier_btags_idxes[2]] >= classifier_cuts[cut_iter]) {
	  n_all_3rd[cut_iter] += weight;
	  if ( (*topHOF)[classifier_btags_idxes[2]]!=4 ) { 
	    h_ci_add_btag_classifier_sliced[cut_iter]->Fill( (*classifier)[classifier_btags_idxes[2]], weight); 
	    n_ci_3rd[cut_iter] += weight; }
	  else { 
	    h_mi_add_btag_classifier_sliced[cut_iter]->Fill( (*classifier)[classifier_btags_idxes[2]], weight); 
	    n_mi_3rd[cut_iter] += weight; }
	} // 3rd classifier btag with classifier >= cut
	  
	// total number of events with 3b excl.
	if (cut_iter==0) n_tot_3rd += weight;
      } // 3 btags only
      

      // Hypothesis: 3rd BDT == the lowest DL1r not from top
      // test it
      if (cut_iter==0) {

        int add_DL1r_bjet_idx = 0; // in all jets array
        int add_DL1r_btag_idx = 0; 
	int add_DL1r_bjet_idx2 = 0; // in bjets array
	int add_DL1r_btag_idx2 = 0;
        bool add_DL1r_bjet_found = false;
	bool add_DL1r_btag_found = false;
        int add_classifier_bjet_idx = 0; // in all jets array
	int add_classifier_btag_idx = 0;
	int add_classifier_bjet_idx2 = 0; // in bjets array
	int add_classifier_btag_idx2 = 0; 
	bool add_classifier_bjet_found = false;
	bool add_classifier_btag_found = false;
        int n_bjets_DL1r_order = 0;
        int n_btags_DL1r_order = 0;
	int n_bjets_classifier_order = 0;
	int n_btags_classifier_order = 0;

	
	// Lopp over jets
        for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {

	  // Search for the highest DL1r bjets not from top
          if ( (*jet_truthflav)[JN_DL1r[jet_i]]==5 ) {
            n_bjets_DL1r_order++;
            if ( (*topHOF)[JN_DL1r[jet_i]]!=4 && add_DL1r_bjet_found==false) {
              add_DL1r_bjet_idx = JN_DL1r[jet_i]; 
              add_DL1r_bjet_idx2 = n_bjets_DL1r_order-1;
	      add_DL1r_bjet_found = true; }
          } // truthflav==5          

	  // Search for the highest DL1r btag not from top
          if ( (*jet_isbtagged_DL1r_77)[JN_DL1r[jet_i]]==1 ) {
            n_btags_DL1r_order++;
            if ( (*topHOF)[JN_DL1r[jet_i]]!=4 && add_DL1r_btag_found==false) {
              add_DL1r_btag_idx = JN_DL1r[jet_i]; 
	      add_DL1r_btag_idx2 = n_btags_DL1r_order-1;
              add_DL1r_btag_found = true; }
          } // btagged
          
	  // Search for the highest BDT bjet and for the truth level not from top BDT ordered index
          if ( (*jet_truthflav)[JN[jet_i]]==5 ) {
	    add_classifier_bjet_idx = JN[jet_i];
	    n_bjets_classifier_order++; 
	    add_classifier_bjet_idx2 = n_bjets_classifier_order-1;
	  } // truthflav == 5
	    
	  // Search for the highest BDT btag and for the truth level not from top BDT ordered index
          if ( (*jet_isbtagged_DL1r_77)[JN[jet_i]]==1 ) {
	    add_classifier_btag_idx = JN[jet_i];
	    n_btags_classifier_order++;
	    add_classifier_btag_idx2 = n_btags_classifier_order-1;
	  } // btagged
        
	} // [jet_i] - loop over jets

	
        if (n_bjets_DL1r_order==3) {
          h_add_truth_bjet_pt->Fill( (*jet_pt)[add_DL1r_bjet_idx]/1000, weight);
          h_add_truth_btag_pt->Fill( (*jet_pt)[add_DL1r_btag_idx]/1000, weight);
          if (add_DL1r_bjet_idx!=add_classifier_bjet_idx) { h_add_bjet_missmatch->Fill(0.5, weight); }
          else { h_add_bjet_missmatch->Fill(1.5, weight); }
        } // 3 bjets

        if (n_btags_DL1r_order==3) {
	  int classifier_idx = -1;
	  auto it = find(classifier_btags_idxes.begin(), classifier_btags_idxes.end(), add_DL1r_btag_idx);
	  if (it != classifier_btags_idxes.end() ) classifier_idx = it - classifier_btags_idxes.begin();
	  
          h_add_classifier_bjet_pt->Fill( (*jet_pt)[add_classifier_bjet_idx]/1000, weight);
          h_add_classifier_btag_pt->Fill( (*jet_pt)[add_classifier_btag_idx]/1000, weight);
          if (add_DL1r_btag_idx!=add_classifier_btag_idx) {
	    h_add_btag_missmatch->Fill(0.5, weight); 
	    h_mi_add_btag_pt->Fill( (*jet_pt)[add_classifier_bjet_idx]/1000, weight); 
	    h_mi_add_btag_classifier->Fill( (*classifier)[add_classifier_bjet_idx], weight); 
	    h_mi_add_btag_DL1r_idx->Fill(add_DL1r_btag_idx2, weight); 
	    h_mi_add_btag_classifier_idx->Fill(classifier_idx, weight); }
	  else { 
	    h_add_btag_missmatch->Fill(1.5, weight); 
	    h_ci_add_btag_pt->Fill( (*jet_pt)[add_classifier_btag_idx]/1000, weight); 
	    h_ci_add_btag_classifier->Fill( (*classifier)[add_classifier_bjet_idx], weight); 
	    h_ci_add_btag_DL1r_idx->Fill(add_DL1r_btag_idx2, weight); 
	    h_ci_add_btag_classifier_idx->Fill(classifier_idx, weight); }
        } // 3 btags
     
 
      } // if cut_iter=0

    } // [cut_iter] - iteration over classifier slices
        
    
    // Update the total number of events.
    n3 += weight;

  } // [entry] - loop over entries



  // Efficiency of the cut for 3rd BDT btags
  // Probability to identify the 3rd BDT as the additional btag
  for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
    
    // Eff 3rd
    eff_3rd[cut_iter] = n_all_3rd[cut_iter] / n_tot_3rd;
    eff_3rd_err[cut_iter] = sqrt( n_all_3rd[cut_iter]*(n_tot_3rd-n_all_3rd[cut_iter]) / pow(n_tot_3rd,3) );
    if ( (eff_3rd[cut_iter]+eff_3rd_err[cut_iter]) >= 1 ) { eff_3rd_err_h[cut_iter] = 1-eff_3rd[cut_iter]; } 
    else { eff_3rd_err_h[cut_iter] = eff_3rd_err[cut_iter]; }
    if ( (eff_3rd[cut_iter]-eff_3rd_err[cut_iter]) <= 0 ) { eff_3rd_err_l[cut_iter] = eff_3rd[cut_iter]; }
    else { eff_3rd_err_l[cut_iter] = eff_3rd_err[cut_iter]; }

    // P 3rd
    prob_3rd[cut_iter] = n_ci_3rd[cut_iter] / n_all_3rd[cut_iter]; 
    prob_3rd_err[cut_iter] = sqrt( n_ci_3rd[cut_iter]*(n_all_3rd[cut_iter]-n_ci_3rd[cut_iter]) / pow(n_all_3rd[cut_iter],3) );
    if ( (prob_3rd[cut_iter]+prob_3rd_err[cut_iter]) >= 1 ) { prob_3rd_err_h[cut_iter] = 1 - prob_3rd[cut_iter]; }
    else { prob_3rd_err_h[cut_iter] = prob_3rd_err[cut_iter]; }
    if ( (prob_3rd[cut_iter]-prob_3rd_err[cut_iter]) <= 0 ) { prob_3rd_err_l[cut_iter] = prob_3rd[cut_iter]; }
    else { prob_3rd_err_l[cut_iter] = prob_3rd_err[cut_iter]; }
   

    cout << "\n\n\n" << method_name << " cut = " << classifier_cuts[cut_iter] << endl;
    
    cout << "\nn_ci_3rd[" << cut_iter << "] = " << n_ci_3rd[cut_iter] << endl;
    cout << "n_all_3rd[" << cut_iter << "] = " << n_all_3rd[cut_iter] << endl;
    cout << "prob_3rd[" << cut_iter << "] = " << prob_3rd[cut_iter]-prob_3rd_err_l[cut_iter] << " - " << prob_3rd[cut_iter]+prob_3rd_err_h[cut_iter] << endl;
    cout << "eff_3rd[" << cut_iter << "] = " << eff_3rd[cut_iter]-eff_3rd_err_l[cut_iter] << " - " << eff_3rd[cut_iter]+eff_3rd_err_h[cut_iter] << endl;
    
  } // [cut_iter] - loop over BDT slices

  
  TGraphAsymmErrors *gr_prob_3rd = new TGraphAsymmErrors(n_steps, &classifier_cuts[0], prob_3rd, classifier_cuts_err, classifier_cuts_err, prob_3rd_err_l, prob_3rd_err_h);
  TGraphAsymmErrors *gr_eff_3rd  = new TGraphAsymmErrors(n_steps, &classifier_cuts[0], eff_3rd,  classifier_cuts_err, classifier_cuts_err, eff_3rd_err_l,  eff_3rd_err_h);
  vector<TGraphAsymmErrors*> vec_gr_3rd = {gr_prob_3rd, gr_eff_3rd};
  vector<TString> vec_gr_3rd_titles = {"P to identify the 3rd " + method_name + " correctly", "Eff of the " + method_name + " cut"};
  int draw_3rd = draw_graphs(vec_gr_3rd, method_name, "P | Eff.", vec_gr_3rd_titles, method_name + "_3rd_P_and_Eff");
  


  // LaTeX headers
  int n_add_jets_plots_begin = write_latex_header("plots_n_add_jets.tex");
  int n_add_btags_plots_begin = write_latex_header("plots_n_add_btags.tex");
  int n_add_bjets_plots_begin = write_latex_header("plots_n_add_bjets.tex");
  int n_add_btags_vs_bjets_begin = write_latex_header("plots_n_add_btags_vs_bjets.tex");
  int prob_3rd_classifier_btag_begin = write_latex_header("plots_prob_3rd_"+method_name+"_btag.tex");  

  for (int cut_iter=0; cut_iter < n_steps; cut_iter++) {
    cout << "\n\nclassifier: " << n1[cut_iter]/n3 << "\ntopHOF: " << n2/n3 << endl << endl;
    
    // Draw plots:   
    
    TString n_add_jets_savename = "n_additional_jets__" + to_string(cut_iter);
    int draw_n_add_jets = draw_hists(h_n_add_jets_classifier[cut_iter], h_n_add_jets_truth[cut_iter], "#bf{N_{jets}^{additional}}", n_add_jets_savename, classifier_cuts[cut_iter], {method_name, "true"}, true, 0, 0.6, 0.5, 1.5);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_jets__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_jets__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_jets.tex", gr0_name, gr1_name, cont_float); }
    
  
    TString n_add_btags_savename = "n_additional_btags__" + to_string(cut_iter);
    int draw_n_add_btags = draw_hists(h_n_add_btags_classifier[cut_iter], h_n_add_btags_truth[cut_iter], "#bf{N_{btags}^{additional}}", n_add_btags_savename, classifier_cuts[cut_iter], {method_name, "truth"}, true, 0, 1.0, 0.5, 1.5);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_btags__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_btags__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_btags.tex", gr0_name, gr1_name, cont_float); }
      
    
    TString n_add_bjets_savename = "n_additional_bjets__" + to_string(cut_iter);
    int draw_n_add_bjets = draw_hists(h_n_add_bjets_classifier[cut_iter], h_n_add_bjets_truth[cut_iter], "#bf{N_{bjets}^{additional}}", n_add_bjets_savename, classifier_cuts[cut_iter], {method_name, "truth"}, true, 0, 1.0, 0.5, 1.5);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_bjets__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_bjets__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_bjets.tex", gr0_name, gr1_name, cont_float); }
    
  
    TString n_add_btags_vs_bjets_savename = "n_additional_btags_vs_bjets__" + to_string(cut_iter);
    int draw_n_add_btags_vs_bjets = draw_hists(h_n_add_btags_classifier[cut_iter], h_n_add_bjets_classifier[cut_iter], "#bf{N_{b(tags/jets)}^{additional}}", n_add_btags_vs_bjets_savename, classifier_cuts[cut_iter], {"add. btags", "add. bjets"}, true, 0, 0.6, 0, 4);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name = "n_additional_btags_vs_bjets__" + to_string(cut_iter-1);
      TString gr1_name = "n_additional_btags_vs_bjets__" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_n_add_btags_vs_bjets.tex", gr0_name, gr1_name, cont_float); }


    TString prob_3rd_classifier_btag_savename = "prob_3rd_"+method_name+"_btag" + to_string(cut_iter);
    int draw_prob_3rd_classifier_btag = draw_hists(h_ci_add_btag_classifier_sliced[cut_iter], h_mi_add_btag_classifier_sliced[cut_iter], "#bf{"+method_name+"}", prob_3rd_classifier_btag_savename, classifier_cuts[cut_iter], {"Correctly identified 3rd add. btag", "Missidentified"}, true, 0, 0.2, 0, 10);
    if (cut_iter%2==1) {
      bool cont_float = true;
      if (cut_iter==1) cont_float = false;
      TString gr0_name =  "prob_3rd_"+method_name+"_btag" + to_string(cut_iter-1);
      TString gr1_name =  "prob_3rd_"+method_name+"_btag" + to_string(cut_iter);
      int add_plots = add_plots_to_tex_file("plots_prob_3rd_"+method_name+"_btag.tex", gr0_name, gr1_name, cont_float); }
    
  } // [cut_iter] - loop over cut slices


  // LaTeX endings
  int n_add_jets_plots_end = write_latex_end_of_the_file("plots_n_add_jets.tex");
  int n_add_btags_plots_end = write_latex_end_of_the_file("plots_n_add_btags.tex");
  int n_add_bjets_plots_end = write_latex_end_of_the_file("plots_n_add_bjets.tex");
  int n_add_btags_vs_bjets_end = write_latex_end_of_the_file("plots_n_add_btags_vs_bjets.tex");
  int prob_3rd_classifier_btag_end = write_latex_end_of_the_file("plots_prob_3rd_"+method_name+"_btag.tex");
  


  // Draw dditional bjets and btags pt
  int draw_h_add_bjet_pt = draw_hists(h_add_classifier_bjet_pt, h_add_truth_bjet_pt, "#bf{p_{T}^{add. bjet}}", "additional_bjet_pt", -999, {"Highest " + method_name + " b", "highest DL1r b not from top"}, false, 1, pow(10,5), 0.5, 1.5);
  int draw_h_add_btag_pt = draw_hists(h_add_classifier_btag_pt, h_add_truth_btag_pt, "#bf{p_{T}^{add. btag}}", "additional_btag_pt", -999, {"Highest " + method_name + " b", "highest DL1r b not from top"}, false, 1, pow(10,5), 0.5, 1.5);

  // Draw add bjet/tag missmatch plots
  vector<TH1*> h_add_bjet_missmatch_v = {h_add_bjet_missmatch};
  int draw_h_add_bjet_missmatch = draw_n_hists(h_add_bjet_missmatch_v, {"add. bjets"}, "(miss)match", "N_(in)correctly_identified_additional_bjets", true, 0, 1);
  vector<TH1*> h_add_btag_missmatch_v = {h_add_btag_missmatch};
  int draw_h_add_btag_missmatch = draw_n_hists(h_add_btag_missmatch_v, {"add. btags"}, "(miss)match", "N_(in)correctly_identified_additional_btags", true, 0, 1);

  // Draw miss/correctly-identified btags plots
  int draw_ci_mi_add_btag_pt = draw_hists(h_ci_add_btag_pt, h_mi_add_btag_pt, "#bf{p_{T}^{add. btag}}", "ci_mi_add_btag_pt", -999, {"Correctly identified additional btag", "Missidentified"}, true, 0, 0.6, 0, 5);
  int draw_ci_truth_add_btag_pt = draw_hists(h_ci_add_btag_pt, h_add_truth_bjet_pt, "#bf{p_{T}^{add. btag}}", "ci_truth_add_btag_pt", -999, {"Correctly identified additional btag", "Truth level"}, true, 0, 1, 0.5, 1.5);
  int draw_mi_truth_add_btag_pt = draw_hists(h_mi_add_btag_pt, h_add_truth_btag_pt, "#bf{p_{T}^{add. btag}}", "mi_truth_add_btag_pt", -999, {"Missidentified additional btag", "Truth level"}, true, 0, 1, 0, 5);
  int draw_ci_mi_add_btag_classifier = draw_hists(h_ci_add_btag_classifier, h_mi_add_btag_classifier, "#bf{" + method_name + "}", "ci_mi_add_btag_classifier", -999, {"Correctly identified additional btag", "Missidentified"}, true, 0, 0.2, 0, 6);
  int draw_ci_mi_add_btag_DL1r_idx = draw_hists(h_ci_add_btag_DL1r_idx, h_mi_add_btag_DL1r_idx, "#bf{index in DL1r order (H->L)}", "ci_mi_add_btag_DL1r_idx", -999, {"Correctly identified additional btag", "Missidentified"}, true, 0, 1, 0.5, 1.5);
  int draw_ci_mi_add_btag_classifier_idx = draw_hists(h_ci_add_btag_classifier_idx, h_mi_add_btag_classifier_idx, "#bf{index in " + method_name + " order (L->H)}", "ci_mi_add_btag_classifier_idx", -999, {"Correctly identified additional btag", "Missidentified"}, true, 0, 1.3, 0, 10);
  
} // END OF MAIN




// ##############################
// ##   DRAW "Sig/Bkg" HISTS   ##
// ##############################
int draw_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, float classifier_cut, vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1 = true, double y_min = 0, double y_max = 1, double ratio_y_min = 0, double ratio_y_max = 4)
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
  else { 
    gPad->SetLogy(); 
    if (y_min==0) y_min = 1; 
    if (y_max<=y_min) y_max = pow(10,5); }


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
  h_s->GetYaxis()->SetRangeUser(y_min, y_max);
  if (norm_to_1 == true) {
    h_s->GetYaxis()->SetTitle("#bf{norm to 1.}"); }
  else {
    gPad->SetLogy();
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
  TString classifier_legend_entry = legend_entries_titles[0];
  if (classifier_cut!=-999) classifier_legend_entry += " (cut = " + to_string(classifier_cut).substr(0,n_cut_digits) + ")";
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
  h_ratio->SetLineWidth(3);
  h_ratio->SetLineColor(1);
  h_ratio->SetTitle("");

  h_ratio->GetYaxis()->SetTitle("Ratio");
  h_ratio->GetYaxis()->SetLabelSize(0.06);
  h_ratio->GetYaxis()->SetTitleOffset(0.5);
  h_ratio->GetYaxis()->SetTitleSize(0.07);
  h_ratio->GetYaxis()->CenterTitle();
  h_ratio->GetYaxis()->SetRangeUser(0,4);
  h_ratio->GetYaxis()->SetNdivisions(8);
  h_ratio->GetYaxis()->SetRangeUser(ratio_y_min, ratio_y_max);

  h_ratio->GetXaxis()->SetTitle(x_axis_title);
  h_ratio->GetXaxis()->SetTitleOffset(1.0);
  h_ratio->GetXaxis()->SetTitleSize(0.12);
  h_ratio->GetXaxis()->SetLabelSize(0.10);

  h_ratio->Draw("E1P");

  // Draw a line along R=1
  TLine *line = new TLine(h_ratio->GetXaxis()->GetXmin(), 1, h_ratio->GetXaxis()->GetXmax(), 1);
  line->SetLineColor(9);
  line->SetLineWidth(3);
  line->SetLineStyle(7);
  line->Draw("same");

  if (classifier_cut==-999) { c->Print("Plots/"+savename+".png"); }
  else { c->Print("Plots/cut_iter_slices/"+savename+".png"); }
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



// ##########################
// ##     Draw TGraphs     ##
// ##########################
int draw_graphs(vector<TGraphAsymmErrors*> gr, TString x_axis_title, TString y_axis_title, vector<TString> legend_entries, TString savename)
{
  // Create a canvas
  TCanvas *c = new TCanvas("c", "c", 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();

  // Create a legend 
  TLegend *legend = new TLegend(0.60, 0.40, 0.85, 0.60);

  // Draw everything nicely 
  for (int i=0; i<gr.size(); i++) {
    gr[i]->SetLineColor(colors[i]);
    gr[i]->SetFillColor(colors[i]);
    gr[i]->SetFillStyle(3005);
    gr[i]->SetLineWidth(4);

    if (i==0) {
      gr[i]->Draw("A3C");
      gr[i]->SetTitle("");
      gr[i]->GetXaxis()->SetTitle(x_axis_title);
      gr[i]->GetYaxis()->SetTitle(y_axis_title);
      gr[i]->GetYaxis()->SetRangeUser(0, 1.1); }
    else {
      gr[i]->Draw("same"); }

    legend->AddEntry(gr[i], legend_entries[i]);

  } // [i] - loop over graphs

  legend->Draw("save");

  // Save the plot
  c->Print("Plots/" + savename + ".png");

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
