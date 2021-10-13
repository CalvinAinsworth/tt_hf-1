#include "../common_functions.h"
#include "../draw_functions.h"

// ///
// MAIN
// ///
int main()
{  
  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");


  // Declare a hist to keep stats on b-jets MVA identification
  TH1* h_jets_origin_stat_pl = new TH1F("jets_origin_stats_pl", "jets_origin_stats_pl", 6, 0, 6);
  TH1* h_jets_origin_stat_reco = new TH1F("jets_origin_stats_reco", "jets_origin_stats_reco", 6, 0, 6);
  std::vector<TString> x_axis_labels = {"1,2", "1,3", "2,3", "X,4", "<2 bFT", ">2 bFT"};
  for (int i=0; i<x_axis_labels.size(); i++) {
    h_jets_origin_stat_pl->GetXaxis()->SetBinLabel(i+1, x_axis_labels[i]);
    h_jets_origin_stat_reco->GetXaxis()->SetBinLabel(i+1, x_axis_labels[i]);
  }
  h_jets_origin_stat_pl->GetXaxis()->CenterTitle();
  h_jets_origin_stat_reco->GetXaxis()->CenterTitle();


  // Declare pT if the jets from top and not from top
  Int_t n_bins_pt = 6;
  Double_t edges_bTopJetPt_1[7] = {50,70,100,150,200,300,400};
  Double_t edges_bTopJetPt_2[7] = {50,70,100,150,200,300,400};
  Double_t edges_AddBJetPt_1[7] = {50,70,100,150,200,300,400};
  Double_t edges_AddBJetPt_2[7] = {50,70,100,150,200,300,400};
  //Double_t edges_AddBJetPt_2[n_bins_pt+1] = {30,50,70,100,150,200,300};
  TH1 *h_bTopJetPt_1_pl = new TH1F("bTopJetPt_1_pl", "bTopJetPt_1_pl", n_bins_pt, edges_bTopJetPt_1);
  TH1 *h_bTopJetPt_2_pl = new TH1F("bTopJetPt_2_pl", "bTopJetPt_2_pl", n_bins_pt, edges_bTopJetPt_2); 
  TH1 *h_AddTopJetPt_1_pl = new TH1F("AddTopJetPt_1_pl", "AddTopJetPt_1_pl", n_bins_pt, edges_AddBJetPt_1);
  TH1 *h_AddTopJetPt_2_pl = new TH1F("AddTopJetPt_2_pl", "AddTopJetPt_2_pl", n_bins_pt, edges_AddBJetPt_2);


  // Declare n_bjets hists
  TH1 *h_btags_n_reco = new TH1F("btags_n_reco", "btags_n_reco", 3, 3, 6);
  TH1 *h_bjets_n_pl = new TH1F("bjets_n_pl", "bjets_n_pl", 3, 3, 6);
  for (int i=0; i<h_btags_n_reco->GetNbinsX(); i++) {
    TString label_str = std::to_string(i+3);
    h_btags_n_reco->GetXaxis()->SetBinLabel(i+1, label_str);
    h_bjets_n_pl->GetXaxis()->SetBinLabel(i+1, label_str);
  }


  // Declare njets from top hists
  TH1 *h_jets_n_ft_reco = new TH1F("jets_n_ft_reco", "jets_n_ft_reco", 5, 0, 5);
  TH1 *h_jets_n_ft_pl = new TH1F("jets_n_ft_pl", "jets_n_ft_pl", 5, 0, 5);
  TH1 *h_btags_n_ft_reco = new TH1F("btags_n_ft_reco", "btags_n_ft_reco", 5, 0, 5);
  TH1 *h_bjets_n_ft_pl = new TH1F("bjets_n_ft_pl", "bjets_n_ft_pl", 5, 0, 5);
  for (int i=0; i<h_jets_n_ft_reco->GetNbinsX(); i++) {
    TString label_str = std::to_string(i);
    h_jets_n_ft_reco->GetXaxis()->SetBinLabel(i+1, label_str);
    h_jets_n_ft_pl->GetXaxis()->SetBinLabel(i+1, label_str);
    h_btags_n_ft_reco->GetXaxis()->SetBinLabel(i+1, label_str);
    h_bjets_n_ft_pl->GetXaxis()->SetBinLabel(i+1, label_str);
  }


  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/user/e/eantipov/Files/tthf_analysis/";
  std::vector<TString> dir_paths = get_list_of_files(path_to_ntuples);

  // Loop over directories with ntuples collections
  for (int dir_counter=0; dir_counter<dir_paths.size(); dir_counter++) {
    
    // Select only directiories from the list of files and directories
    TSystemFile dir(dir_paths[dir_counter], dir_paths[dir_counter]);
    if (!(dir.IsDirectory())) continue;
    std::cout << "\n\n\n" << dir_paths[dir_counter] << std::endl;

    
    // Analyze directiry's name: data/mc, which campaign? 
    std::vector<TString> dir_path_components = split(dir_paths[dir_counter], '/');
    int last_element_index = dir_path_components.size()-1;
    bool is_data = false;
    bool is_mc16a = false;
    bool is_mc16d = false;
    bool is_mc16e = false;
    if (dir_path_components[last_element_index] == "data") is_data = true;
    if (dir_path_components[last_element_index] == "mc16a") is_mc16a = true;
    if (dir_path_components[last_element_index] == "mc16d") is_mc16d = true;
    if (dir_path_components[last_element_index] == "mc16e") is_mc16e = true;

    
    // Interested in MC only
    if (is_data == true) continue;


    // Make a list of processes in the directory (with ful paths)
    std::vector<TString> paths_to_processes = get_list_of_files(dir_paths[dir_counter]);
    
    
    // Loop over processes directories
    for (int job_number=0; job_number<paths_to_processes.size(); job_number++) {
      
      // Get info about the process
      std::vector<TString> path_to_processes_components = split(paths_to_processes[job_number], '/');
      TString process_name = path_to_processes_components[path_to_processes_components.size() - 1];
      
      // Analyzie only tt
      if (process_name!="tt") continue;

      // Make a list of paths to dids
      std::vector<TString> paths_to_dids = get_list_of_files(paths_to_processes[job_number]);
      
      
      // Loop over DIDs
      for (int did_number=0; did_number<paths_to_dids.size(); did_number++) {
	
	// Get list of ntuples with full paths
	std::vector<TString> paths_to_ntuples = get_list_of_files(paths_to_dids[did_number]);
	std::vector<TString> paths_to_ntuples_components = split(paths_to_ntuples[did_number], '/');
	TString job_DID = paths_to_ntuples_components[paths_to_ntuples_components.size() - 2];

	
	// Loop over ntuples
	for (int ntuple_number=0; ntuple_number<paths_to_ntuples.size(); ntuple_number++) {

	  std::cout << paths_to_ntuples[ntuple_number] << std::endl;
	  
	  // Open the ntuple
	  TFile *ntuple = new TFile(paths_to_ntuples[ntuple_number]);
	  TTree *tree_pl = (TTree*)ntuple->Get("particleLevel");
	  TTree *tree_nominal = (TTree*)ntuple->Get("nominal");
	  
	  // Set needed branches
	  
	  // Nominal
	  std::vector<char> *jet_DL1r_77;
	  std::vector<Float_t> *jet_DL1r;
	  std::vector<Float_t> *MVA_score;
	  std::vector<Float_t> *jet_pt;
	  std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag;
	  std::vector<Int_t> *jet_truthflav;
	  std::vector<Float_t> *el_charge;
	  std::vector<Float_t> *mu_charge;
	  jet_DL1r_77 = 0;
	  jet_pt = MVA_score = jet_DL1r = el_charge = mu_charge = 0;
	  jet_GBHInit_topHadronOriginFlag = jet_truthflav = 0;
	  Int_t topHFFF, runNumber;
	  Float_t w_mc, w_pu, w_leptonSF, w_DL1r_77, w_jvt;
	  
	  tree_nominal->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_77);
	  tree_nominal->SetBranchAddress("jet_DL1r", &jet_DL1r);
	  tree_nominal->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag);
	  tree_nominal->SetBranchAddress(tmva_config_info[0], &MVA_score);
	  tree_nominal->SetBranchAddress("jet_pt", &jet_pt);
	  tree_nominal->SetBranchAddress("el_charge", &el_charge);
	  tree_nominal->SetBranchAddress("mu_charge", &mu_charge);
	  tree_nominal->SetBranchAddress("topHeavyFlavorFilterFlag", &topHFFF);
	  tree_nominal->SetBranchAddress("runNumber", &runNumber);
	  tree_nominal->SetBranchAddress("weight_mc", &w_mc);
          tree_nominal->SetBranchAddress("weight_pileup", &w_pu);
          tree_nominal->SetBranchAddress("weight_leptonSF", &w_leptonSF);
          tree_nominal->SetBranchAddress("weight_bTagSF_DL1r_77", &w_DL1r_77);
          tree_nominal->SetBranchAddress("weight_jvt", &w_jvt);

	  // PL
	  std::vector<Int_t> *jet_truthPartonLabel_pl;
	  std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag_pl;
	  std::vector<Int_t> *jet_nGhosts_bHadron;
	  std::vector<Float_t> *jet_pt_pl;
	  std::vector<Float_t> *MVA_score_pl;
	  std::vector<Float_t> *el_charge_pl;
	  std::vector<Float_t> *mu_charge_pl;
	  jet_nGhosts_bHadron = jet_truthPartonLabel_pl = jet_GBHInit_topHadronOriginFlag_pl = 0;
	  jet_pt_pl = MVA_score_pl = el_charge_pl = mu_charge_pl = 0;
	  Float_t w_mc_pl;
	  Float_t w_pu_pl;
	  Int_t topHFFF_pl;

	  tree_pl->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel_pl);
	  tree_pl->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag_pl);
	  tree_pl->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron);
	  tree_pl->SetBranchAddress(tmva_config_info[0], &MVA_score_pl);
	  tree_pl->SetBranchAddress("jet_pt", &jet_pt_pl);
	  tree_pl->SetBranchAddress("el_charge", &el_charge_pl);
	  tree_pl->SetBranchAddress("mu_charge", &mu_charge_pl);
	  tree_pl->SetBranchAddress("weight_mc", &w_mc_pl);
	  tree_pl->SetBranchAddress("weight_pileup", &w_pu_pl);
	  tree_pl->SetBranchAddress("topHeavyFlavorFilterFlag", &topHFFF_pl);



	  // ///
	  // Loop over Nominal tree events
	  Int_t nEntries = tree_nominal->GetEntries();
	  std::cout << "\tnEntries = " << nEntries << std::endl;
	  for (int entry=0; entry<nEntries; entry++) {
	    
	    // Show events counter
	    if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
	    tree_nominal->GetEntry(entry);


	    // Compute weight
	    double weight_lumi = 1;
	    double sumWeights = 1;
	    double campaign_lumi = 1;
	    double campaign_xsection_x_genFiltEff = 1;
	    double kFactor = 1;
	    double total_lumi = 3.21956 + 32.9881 + 44.3074 + 58.4501;
	    
	    if (job_DID=="411076") {
	      campaign_xsection_x_genFiltEff = 0.6740;
	      kFactor = 1.1397; }
	    if (job_DID=="411077") {
	      campaign_xsection_x_genFiltEff = 3.582;
	      kFactor = 1.1398; }
	    if (job_DID=="411078") {
	      campaign_xsection_x_genFiltEff = 3.034;
	      kFactor = 1.1397; }
	    if (job_DID=="410472") {
	      campaign_xsection_x_genFiltEff = 76.95;
	      kFactor = 1.13975636159; }
	    
	    if (runNumber==284500) {
	      campaign_lumi = 3.21956 + 32.9881;
	      if (job_DID=="411076") sumWeights = 3.33006*pow(10, 9);
	      if (job_DID=="411077") sumWeights = 3.61088*pow(10, 9);
	      if (job_DID=="411078") sumWeights = 3.61598*pow(10, 9);
	      if (job_DID=="410472") sumWeights = 5.82869*pow(10, 10);
	    }

	    if (runNumber==300000) {
	      campaign_lumi = 44.3074;
	      if (job_DID=="411076") sumWeights = 4.21891*pow(10, 9);
	      if (job_DID=="411077") sumWeights = 4.49595*pow(10, 9);
	      if (job_DID=="411078") sumWeights = 4.49400*pow(10, 9);
	      if (job_DID=="410472") sumWeights = 7.26510*pow(10, 10);
	    }
	    
	    if (runNumber==310000) {
	      campaign_lumi = 58.4501;
	      if (job_DID=="411076") sumWeights = 5.47811*pow(10,9);
	      if (job_DID=="411077") sumWeights = 5.94763*pow(10,9);
	      if (job_DID=="411078") sumWeights = 5.94190*pow(10,9);
	      if (job_DID=="410472") sumWeights = 1.01641*pow(10,11);
	    }

	    weight_lumi = campaign_lumi * pow(10,3) * campaign_xsection_x_genFiltEff * kFactor / sumWeights;
	    double weight = w_mc * w_pu * w_leptonSF * w_DL1r_77 * w_jvt * weight_lumi;


	    // Declare cuts names
	    bool emu_cut = false;
            bool OS_cut = false;
            bool jets_n_cut = false;
            bool btags_n2_cut = false;
            bool btags_n3_cut = false;
            bool topHFFF_cut = false;

	    
	    // Define cuts themselves
	    if ((*el_charge).size()==1 && (*mu_charge).size()==1) emu_cut = true;
            if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	    
	    int jets_n  = (*jet_pt).size();
	    if (jets_n >= 3) jets_n_cut = true;
	    
	    int btags_n = 0;
	    for (int i=0; i<(*jet_pt).size(); i++) { if ((*jet_DL1r_77)[i]==1) btags_n++; }
	    if (btags_n >=2) btags_n2_cut = true;
	    if (btags_n >=3) btags_n3_cut = true;
	    
	    if ( (topHFFF==1 && job_DID=="411076") || (topHFFF==2 && job_DID=="411077") || (topHFFF==3 && job_DID=="411078") || (topHFFF==0 && job_DID=="410472") ) topHFFF_cut = true;


	    // 3b, emu-OS
	    if (emu_cut*OS_cut*jets_n_cut*btags_n3_cut*topHFFF_cut==true) {

	      // Fill non-top-origin related hists first
	      h_btags_n_reco->Fill(btags_n, weight);


	      // Check if there is not b from top
	      bool not_b_from_top = false;
	      int n_jets_from_top = 0;
	      int n_btags_from_top = 0;
	      for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
		if ((*jet_GBHInit_topHadronOriginFlag)[jet_i]==4) {
		  n_jets_from_top++;
		  if ((*jet_DL1r_77)[jet_i]!=1) not_b_from_top = true;
		  if ((*jet_DL1r_77)[jet_i]==1) n_btags_from_top++;
		} // if jet from top top
	      } // [jet_i] - loop over jets
	      if (n_btags_from_top<2) h_jets_origin_stat_reco->Fill(4.5, weight);
	      if (n_btags_from_top>2) h_jets_origin_stat_reco->Fill(5.5, weight);
	      h_jets_n_ft_reco->Fill(n_jets_from_top, weight);
	      h_btags_n_ft_reco->Fill(n_btags_from_top, weight);
	      
	      
	      
	      // A trick to "arrange" bjets wrt other than pT variable
	      // 1. Create a vector of {MVA_score, pT_ordered_index} pairs
	      std::vector<std::pair<float, int>> btag_MVAscore_pTidx = {};
	      for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
		if ((*jet_DL1r_77)[jet_i]==1) { 
		  btag_MVAscore_pTidx.push_back( std::make_pair( (*MVA_score)[jet_i], jet_i) );
		}
	      } // [jet_i] - loop over jets
	      
	      // 2. Sort the vector wrt the MVA classifier scores (lowest --> highest) 
	      sort(btag_MVAscore_pTidx.begin(), btag_MVAscore_pTidx.end());

	      // 3. Delcare a vector if pT ordered indeces for MVA sclassifier scores ordered jets
	      // i.e. {0, 1, 2, 3} will turn into {2, 1, 3, 0} if MVA classifier scores are in jet_2 < jet_1 < jet_3 < jet_0 order
	      std::vector<int> JN;
              for (int i=0; i<btag_MVAscore_pTidx.size(); i++) {
                JN.push_back(btag_MVAscore_pTidx[i].second);
              }


	      // Fill the stats hist  h_jets_origin_stat
	      if (n_btags_from_top==2) {
		if ((*jet_GBHInit_topHadronOriginFlag)[JN[0]]==4 && (*jet_GBHInit_topHadronOriginFlag)[JN[1]]==4 ) { h_jets_origin_stat_reco->Fill(0.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[JN[0]]==4 && (*jet_GBHInit_topHadronOriginFlag)[JN[2]]==4 ) { h_jets_origin_stat_reco->Fill(1.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[JN[1]]==4 && (*jet_GBHInit_topHadronOriginFlag)[JN[2]]==4 ) { h_jets_origin_stat_reco->Fill(2.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[JN[3]]==4 && ((*jet_GBHInit_topHadronOriginFlag)[JN[0]]==4 || (*jet_GBHInit_topHadronOriginFlag)[JN[1]]==4 || (*jet_GBHInit_topHadronOriginFlag)[JN[2]]==4)) { h_jets_origin_stat_reco->Fill(3.5, weight); }
	      }

	    } // 3b, emu-OS
	    
	  } // [entrly] - loop over nominal tree entries



	  // ///
	  // Loop over PL tree entries
	  Int_t nEntries_pl = tree_pl->GetEntries();
	  std::cout << "\tEntries PL = " << nEntries_pl << std::endl;
	  for (int entry=0; entry<nEntries_pl; entry++) {

	    // Show events counter
	    if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
	    tree_pl->GetEntry(entry);

	    
	    // Compute weight
	    double weight_lumi = 1;
            double sumWeights = 1;
            double campaign_lumi = 1;
            double campaign_xsection_x_genFiltEff = 1;
            double kFactor = 1;
            double total_lumi = 3.21956 + 32.9881 + 44.3074 + 58.4501;

            if (job_DID=="411076") {
              campaign_xsection_x_genFiltEff = 0.6740;
              kFactor = 1.1397; }
            if (job_DID=="411077") {
              campaign_xsection_x_genFiltEff = 3.582;
              kFactor = 1.1398; }
            if (job_DID=="411078") {
              campaign_xsection_x_genFiltEff = 3.034;
              kFactor = 1.1397; }
            if (job_DID=="410472") {
              campaign_xsection_x_genFiltEff = 76.95;
              kFactor = 1.13975636159; }

            if (runNumber==284500) {
              campaign_lumi = 3.21956 + 32.9881;
              if (job_DID=="411076") sumWeights = 3.33006*pow(10, 9);
              if (job_DID=="411077") sumWeights = 3.61088*pow(10, 9);
              if (job_DID=="411078") sumWeights = 3.61598*pow(10, 9);
              if (job_DID=="410472") sumWeights = 5.82869*pow(10, 10);
            }

            if (runNumber==300000) {
              campaign_lumi = 44.3074;
              if (job_DID=="411076") sumWeights = 4.21891*pow(10, 9);
              if (job_DID=="411077") sumWeights = 4.49595*pow(10, 9);
              if (job_DID=="411078") sumWeights = 4.49400*pow(10, 9);
              if (job_DID=="410472") sumWeights = 7.26510*pow(10, 10);
            }

            if (runNumber==310000) {
              campaign_lumi = 58.4501;
              if (job_DID=="411076") sumWeights = 5.47811*pow(10,9);
              if (job_DID=="411077") sumWeights = 5.94763*pow(10,9);
              if (job_DID=="411078") sumWeights = 5.94190*pow(10,9);
              if (job_DID=="410472") sumWeights = 1.01641*pow(10,11);
            }

            weight_lumi = campaign_lumi * pow(10,3) * campaign_xsection_x_genFiltEff * kFactor / sumWeights;
            double weight = w_mc_pl * weight_lumi; // removed weight_pileup

	    
	    // Declare cuts names
	    bool emu_cut = false;
	    bool OS_cut = false;
	    bool jets_n_cut = false;
	    bool bjets_n2_cut = false;
	    bool bjets_n3_cut = false;
	    bool topHFFF_cut = false;
	    
	    
	    // Define cuts themselves
	    if ((*el_charge_pl).size()==1 && (*mu_charge_pl).size()==1) emu_cut = true;
	    if ((*el_charge_pl)[0]!=(*mu_charge_pl)[0]) OS_cut = true;

	    int jets_n = (*jet_pt_pl).size();
	    if (jets_n >=3) jets_n_cut = true;

	    int bjets_n = 0;
	    for (int i=0; i<(*jet_pt_pl).size(); i++) { if ((*jet_nGhosts_bHadron)[i]>=1) bjets_n++; }
	    if (bjets_n >=2) bjets_n2_cut = true;
	    if (bjets_n >=3) bjets_n3_cut = true;
	    
	    if ( (topHFFF_pl==1 && job_DID=="411076") || (topHFFF_pl==2 && job_DID=="411077") || (topHFFF_pl==3 && job_DID=="411078") || (topHFFF_pl==0 && job_DID=="410472") ) topHFFF_cut = true;

	    
	    // 3b, emu-OS
	    if (emu_cut*OS_cut*jets_n_cut*bjets_n3_cut*topHFFF_cut==true) {

	      // Fill non-top-origin related hists firts
	      h_bjets_n_pl->Fill(bjets_n, weight);
	      
	      
	      // Check if there is not b from top
	      bool not_b_from_top = false;
	      int n_jets_from_top = 0;
	      int n_bjets_from_top = 0;
	      for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
		if ((*jet_GBHInit_topHadronOriginFlag_pl)[jet_i]==4) {
		  n_jets_from_top++;
		  if ((*jet_nGhosts_bHadron)[jet_i]==0) not_b_from_top = true;
		  if ((*jet_nGhosts_bHadron)[jet_i]>=1) n_bjets_from_top++;
		} // if jet from top
	      } // [jet_i] -loop over jets
	      if (n_jets_from_top<2) h_jets_origin_stat_pl->Fill(4.5, weight);
	      if (n_jets_from_top>2) h_jets_origin_stat_pl->Fill(5.5, weight);
	      h_jets_n_ft_pl->Fill(n_jets_from_top, weight);
	      h_bjets_n_ft_pl->Fill(n_bjets_from_top, weight);

	      
	      // A trick to "arrange" bjets wrt other than pT variable
	      // 1. Create a vector of {MVA_score, pT_ordered_index} pairs
	      std::vector<std::pair<float, int>> bjet_MVAscore_pTidx = {};
	      for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
		if ((*jet_nGhosts_bHadron)[jet_i]>=1) bjet_MVAscore_pTidx.push_back( std::make_pair( (*MVA_score_pl)[jet_i], jet_i) );
	      } // [jet_i] - loop over jets
	      
	      // 2. Sort the vector wrt the MVA classifier scores (lowest --> highest)
	      sort(bjet_MVAscore_pTidx.begin(), bjet_MVAscore_pTidx.end());
	      
	      // 3. Delcare a vector if pT ordered indeces for MVA sclassifier scores ordered jets
	      // i.e. {0, 1, 2, 3} will turn into {2, 1, 3, 0} if MVA classifier scores are in jet_2 < jet_1 < jet_3 < jet_0 order
	      std::vector<int> JN;
	      for (int i=0; i<bjet_MVAscore_pTidx.size(); i++) {
		JN.push_back(bjet_MVAscore_pTidx[i].second);
	      }

	      
	      // Fill the stats hist
	      if (n_bjets_from_top==2) {
		if ((*jet_GBHInit_topHadronOriginFlag_pl)[JN[0]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[JN[1]]==4) { h_jets_origin_stat_pl->Fill(0.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[JN[0]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[JN[2]]==4) { h_jets_origin_stat_pl->Fill(1.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[JN[1]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[JN[2]]==4) { h_jets_origin_stat_pl->Fill(2.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[JN[3]]==4 && ((*jet_GBHInit_topHadronOriginFlag_pl)[JN[0]]==4 || (*jet_GBHInit_topHadronOriginFlag_pl)[JN[1]]==4 || (*jet_GBHInit_topHadronOriginFlag_pl)[JN[2]]==4)) { h_jets_origin_stat_pl->Fill(3.5, weight); }
	      }

	      
	      // Fill pT hists
	      h_bTopJetPt_1_pl->Fill((*jet_pt_pl)[JN[0]]/1000, weight);
	      h_bTopJetPt_2_pl->Fill((*jet_pt_pl)[JN[1]]/1000, weight);
	      h_AddTopJetPt_1_pl->Fill((*jet_pt_pl)[JN[2]]/1000, weight);
	      h_AddTopJetPt_2_pl->Fill((*jet_pt_pl)[JN[3]]/1000, weight);
	      
	      
	    } // if - 2b, emu-OS 
	    
	  } // [entry] - loop over entries
	  
	  ntuple->Close();
	  
	} // [ntuple_number] - lopp over ntuples
	
      } // [process_number] - loop over DIDs of one process of one campaign
      
    } // [job_number] - loop over processes directories
    
  } // [dir_counter] - data, mc16a, mc16d, mc16e
  

  // Drat number of b
  std::vector<TH1*> h_b_n_vec = {h_bjets_n_pl, h_btags_n_reco};
  std::vector<TString> h_b_n_title = {"bjets, PL", "btags DL1r 77, Reco"};
  int draw_h_b_n = draw_n_hists(h_b_n_vec, h_b_n_title, "#bf{N_{bjets or btags}}", "bjets_btags_n", false, 0, 100000);
  
  // Draw number of jets from top
  std::vector<TH1*> h_jets_n_ft_vec = {h_jets_n_ft_pl, h_jets_n_ft_reco};
  std::vector<TString> h_jets_n_ft_title = {"particle level", "reco level"};
  int draw_jets_n_ft = draw_n_hists(h_jets_n_ft_vec, h_jets_n_ft_title, "number fo jets from top", "jets_n_from_top", false, 1, 100000);

  // Draw number of b from top
  std::vector<TH1*> h_b_n_ft_vec = {h_bjets_n_ft_pl, h_btags_n_ft_reco};
  std::vector<TString> h_b_n_ft_title = {"bjets, PL", "btags DL1r 77, Reco"};
  int draw_b_n_ft = draw_n_hists(h_b_n_ft_vec, h_b_n_ft_title, "number of b from top", "bjets_btags_n_from_top", false, 1, 100000);
  
  // Draw stats hist
  std::vector<TH1*> h_stats_vec = {h_jets_origin_stat_pl, h_jets_origin_stat_reco};
  std::vector<TString> h_stats_title = {"particle level", "reco level"};
  int draw_stats = draw_n_hists(h_stats_vec, h_stats_title, "bjets/btags from top (truth info) in "+tmva_config_info[0]+" order", "bjets_from_tops_study_"+tmva_config_info[0], true, 0, 1);

  // Draw pT of jets from tops, PL
  std::vector<TH1*> h_bTopJetPt_pl_vec = {h_bTopJetPt_1_pl, h_bTopJetPt_2_pl};
  std::vector<TString> h_bTopJetPt_pl_titles = {"Leading bjet from top", "Sub-leading"};
  int draw_bTopJetPt_pl = draw_n_hists(h_bTopJetPt_pl_vec, h_bTopJetPt_pl_titles, "#bf{p_{T} [GeV]}", "jets_from_top_"+tmva_config_info[0]+"_PL_pt", false, 0, 100000);

  // Draw pT of the additional jets, PL
  std::vector<TH1*> h_AddTopJetPt_pl_vec = {h_AddTopJetPt_1_pl, h_AddTopJetPt_2_pl};
  std::vector<TString> h_AddTopJetPt_pl_titles  = {"Leading additional bjet", "Sub-leading"};
  int draw_AddTopJetPt_pl = draw_n_hists(h_AddTopJetPt_pl_vec, h_AddTopJetPt_pl_titles, "#bf{p_{T} [GeV]}", "add_jets_"+tmva_config_info[0]+"_PL_pt", false, 0, 100000);

  return 0;
}
