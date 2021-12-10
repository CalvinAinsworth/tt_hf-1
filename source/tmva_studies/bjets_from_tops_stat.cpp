#include "../common_functions.h"
#include "../draw_functions.h"

//////////////
//   MAIN   //
//////////////
int main(int argc, char *argv[])
{
  // Check for the generator and mc16a_only choises
  std::string generator = ttbar_generator();
  if (generator=="" && std::string(argv[1])=="tt") {
    generator="nominal";
    std::cout << "No generator was selected for ttbar, assuming nominal" << std::endl; }
  bool mc16a_only_test = mc16a_only_choise();
  

  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");
  

  // Counters of the total number of events on the Reco and PL
  double reco_integral_3b = 0;
  double reco_integral_4b = 0;
  double pl_integral_3b = 0;
  double pl_integral_4b = 0;

  
  // Declare a hist to keep stats on b-jets MVA identification
  
  // 3j3b excl
  TH1* h_jets_origin_stat_pl_3b = new TH1F("jets_origin_stats_pl_3b", "jets_origin_stats_pl_3b", 4, 0, 4);
  TH1* h_jets_origin_stat_reco_3b = new TH1F("jets_origin_stats_reco_3b", "jets_origin_stats_reco_3b", 4, 0, 4);
  std::vector<TString> x_axis_labels_jos_3b = {"1,2", "1,3", "2,3", "<2 bFT"};
  for (int i=0; i<x_axis_labels_jos_3b.size(); i++) {
    h_jets_origin_stat_pl_3b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_jos_3b[i]);
    h_jets_origin_stat_reco_3b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_jos_3b[i]);
  }
  h_jets_origin_stat_pl_3b->GetXaxis()->CenterTitle();
  h_jets_origin_stat_reco_3b->GetXaxis()->CenterTitle();
  
  TH1* h_origin_tag_eff_pl_3b = new TH1F("h_origin_tag_eff_pl_3b", "h_origin_tag_eff_pl_3b", 3, 0, 3);
  TH1* h_origin_tag_eff_reco_3b = new TH1F("h_origin_tag_eff_reco_3b", "h_origin_tag_eff_reco_3b", 3, 0, 3);
  std::vector<TString> x_axis_labels_otf_3b = {"Lead-p_{T} b FT", "Sub-Lead-p_{T} b FT", "Add. b"};
  for (int i=0; i<x_axis_labels_otf_3b.size(); i++) {
    h_origin_tag_eff_pl_3b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_otf_3b[i]);
    h_origin_tag_eff_reco_3b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_otf_3b[i]);
  }
  h_origin_tag_eff_pl_3b->GetXaxis()->CenterTitle();
  h_origin_tag_eff_reco_3b->GetXaxis()->CenterTitle();
  
  // 4j4b incl
  TH1* h_jets_origin_stat_pl_4b = new TH1F("jets_origin_stats_pl_4b", "jets_origin_stats_pl_4b", 5, 0, 5);
  TH1* h_jets_origin_stat_reco_4b = new TH1F("jets_origin_stats_reco_4b", "jets_origin_stats_reco_4b", 5, 0, 5);
  std::vector<TString> x_axis_labels_jos_4b = {"1,2", "1,3", "2,3", "any,4", "<2 bFT"};
  for (int i=0; i<x_axis_labels_jos_4b.size(); i++) {
    h_jets_origin_stat_pl_4b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_jos_4b[i]);
    h_jets_origin_stat_reco_4b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_jos_4b[i]);
  }
  h_jets_origin_stat_pl_4b->GetXaxis()->CenterTitle();
  h_jets_origin_stat_reco_4b->GetXaxis()->CenterTitle();
  
  TH1* h_origin_tag_eff_pl_4b =new TH1F("h_origin_tag_eff_pl_4b", "h_origin_tag_eff_pl_4b", 4, 0, 4);
  TH1* h_origin_tag_eff_reco_4b = new TH1F("h_origin_tag_eff_reco_4b", "h_origin_tag_eff_reco_4b", 4, 0, 4);
  std::vector<TString> x_axis_labels_otf_4b = {"Lead-p_{T} b FT", "Sub-Lead-p_{T} b FT", "Lead-p_{T} add. b", "Sub-lead-p_{T} add. b"};
  for (int i=0; i<x_axis_labels_otf_4b.size(); i++) {
    h_origin_tag_eff_pl_4b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_otf_4b[i]);
    h_origin_tag_eff_reco_4b->GetXaxis()->SetBinLabel(i+1, x_axis_labels_otf_4b[i]);
  }
  h_origin_tag_eff_pl_4b->GetXaxis()->CenterTitle();
  h_origin_tag_eff_reco_4b->GetXaxis()->CenterTitle();
  
  
  // Declare pT if the jets from top and not from top
  //Double_t pt_bins_edges[7] = {50,70,100,150,200,300,400};
  TH1F *h_bTop_1_bdt_reco = new TH1F("h_bTop_1_bdt_reco", "h_bTop_1_bdt_reco", 100, 0, 1000);
  TH1F *h_bTop_2_bdt_reco = new TH1F("h_bTop_2_bdt_reco", "h_bTop_2_bdt_reco", 100, 0, 1000);
  TH1F *h_bAdd_1_bdt_reco = new TH1F("h_bAdd_1_bdt_reco", "h_bAdd_1_bdt_reco", 100, 0, 1000);
  TH1F *h_bAdd_2_bdt_reco = new TH1F("h_bAdd_2_bdt_reco", "h_bAdd_2_bdt_reco", 100, 0, 1000);
  TH1F *h_bTop_1_bdt_pl = new TH1F("h_bTop_1_bdt_pl", "h_bTop_1_bdt_pl", 100, 0, 1000);
  TH1F *h_bTop_2_bdt_pl = new TH1F("h_bTop_2_bdt_pl", "h_bTop_2_bdt_pl", 100, 0, 1000);
  TH1F *h_bAdd_1_bdt_pl = new TH1F("h_bAdd_1_bdt_pl", "h_bAdd_1_bdt_pl", 100, 0, 1000);
  TH1F *h_bAdd_2_bdt_pl = new TH1F("h_bAdd_2_bdt_pl", "h_bAdd_2_bdt_pl", 100, 0, 1000);

  
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
  
  
  // Declare hists for discriminative variables
  // 0 - all reco, 1 - bdt from top reco, 2 - bdt not from top reco, 3 - true from top, 4 - true not from top, 5-9 same PL
  TH1 *h_dR_jet_lep0[10];
  TH1 *h_dR_jet_lep1[10];
  TH1 *h_min_dR_jet_lep[10];
  TH1 *h_m_jet_el[10];
  TH1 *h_m_jet_mu[10];
  TH1 *h_min_dR_jet_bjet[10];
  TH1 *h_jet_eta[10];
  TH1 *h_mva_score[10];
  for (int i=0; i<10; i++) {
    TString tmp = std::to_string(i);
    h_dR_jet_lep0[i] = new TH1F("h_dR_jet_lep0_"+tmp, "h_dR_jet_lep0_"+tmp, 13, 0, 6.5);
    h_dR_jet_lep1[i] = new TH1F("h_dR_jet_lep1_"+tmp, "h_dR_jet_lep1_"+tmp, 13, 0, 6.5);
    h_min_dR_jet_lep[i] = new TH1F("h_min_dR_jet_lep_"+tmp, "h_min_dR_jet_lep_"+tmp, 13, 0, 6.5);
    h_m_jet_el[i] = new TH1F("h_m_jet_el_"+tmp, "h_m_jet_el_"+tmp, 60, 0, 600);
    h_m_jet_mu[i] = new TH1F("h_m_jet_mu_"+tmp, "h_m_jet_mu_"+tmp, 60, 0, 600);
    h_min_dR_jet_bjet[i] = new TH1F("h_min_dR_jet_bjet_"+tmp, "h_min_dR_jet_bjet_"+tmp, 13, 0, 6.5);
    h_jet_eta[i] = new TH1F("h_jet_eta_"+tmp, "h_jet_eta_"+tmp, 30, -3, 3);
    h_mva_score[i] = new TH1F("h_mva_score"+tmp, "h_mva_score"+tmp, 130, -0.5, 0.7);
  }
  
  
  // Seup MVA reader
  #include "../post_mva/include/setup_tmva_reader.h"
  
  
  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/atlas/atlascerngroupdisk/phys-top/ttjets/v4/";
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
    
    
    // We work with mc only
    if (is_data == true) continue;
    if (mc16a_only_test==true && is_mc16a!=true) continue;
    
    
    // Only nominal ntuples
    if (generator=="nominal") { dir_paths[dir_counter] += "nominal/"; }
    else {  dir_paths[dir_counter] += "newSamples/"; }
    
    
    // Make a list of paths to jobs/DIDs outputs (pieces of a full ntuple)
    std::vector<TString> paths_to_jobs = get_list_of_files(dir_paths[dir_counter]);
    
    
    // Loop over directories (one directory = one mc16 campaign or data_year for one DID)
    for (int job_number=0; job_number<paths_to_jobs.size(); job_number++) {
      
      // Get info about the job/DID from its name
      std::vector<TString> path_to_jobs_components = split(paths_to_jobs[job_number], '/');
      TString job_name = path_to_jobs_components[path_to_jobs_components.size() - 1];
      std::vector<TString> job_name_components = split(job_name, '.');
      std::string job_DID = std::string(job_name_components[2]);
      std::vector<TString> campaign_info = split(job_name_components[5], '_');
      
      
      // Select only jobs/physics_processes of our interest
      // (1) regular, not alternative samples
      // (2) ttbar only
      if (generator=="nominal" && campaign_info[1]!="s3126") continue; // (1)
      
      bool correct_did = false;
      if ( (generator=="nominal" && (job_DID=="410472" || job_DID=="411076" || job_DID=="411077" || job_DID=="411078")) || (generator!="nominal" && (job_DID=="411234" || job_DID=="411332" || job_DID=="411333" || job_DID=="411334")) ) correct_did=true;
      if (correct_did==false) continue; // (2)
      else { std::cout << "\n\nDID: " << job_DID << std::endl; }
      
      
      // Run over the inclusive sample only - uncommebt the "if" statement line
      bool only_410472 = false;
      // Use only inclusive sample. Overlap removal won't be applied.
      //if (job_DID=="410472") { only_410472=true; } else { continue; }
      
      
      // Make a list of paths to ntuples for the given DID
      std::vector<TString> paths_to_ntuples = get_list_of_files(paths_to_jobs[job_number]);

    
      // Loop over samll ntuples of one DID
      for (int ntuple_number=0; ntuple_number<paths_to_ntuples.size(); ntuple_number++) {
	
	// Open the ntuple
	std::cout << paths_to_ntuples[ntuple_number] << std::endl;
        TFile *ntuple = new TFile (paths_to_ntuples[ntuple_number]);
        TTree *tree_pl = (TTree*)ntuple->Get("particleLevel");
	TTree *tree_nominal = (TTree*)ntuple->Get("nominal");

	
	// Set needed branches

	// Nominal tree
	std::vector<char> *jet_DL1r_77;
	std::vector<Float_t> *jet_DL1r;
	std::vector<Float_t> *jet_pt;
	std::vector<Float_t> *jet_eta;
	std::vector<Float_t> *jet_phi;
	std::vector<Float_t> *jet_e;
	std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag;
	std::vector<Int_t> *jet_truthflav;
	std::vector<Float_t> *el_pt;
	std::vector<Float_t> *el_eta;
	std::vector<Float_t> *el_phi;
	std::vector<Float_t> *el_e;
	std::vector<Float_t> *el_charge;
	std::vector<Float_t> *mu_pt;
	std::vector<Float_t> *mu_eta;
	std::vector<Float_t> *mu_phi;
	std::vector<Float_t> *mu_e;
	std::vector<Float_t> *mu_charge;
	jet_DL1r_77 = 0;
	jet_pt = jet_eta = jet_phi = jet_e = jet_DL1r = el_pt = el_eta = el_phi = el_e = el_charge = mu_pt = mu_eta = mu_phi = mu_e = mu_charge = 0;
	jet_GBHInit_topHadronOriginFlag = jet_truthflav = 0;
	Int_t topHFFF;
	UInt_t runNumber;
	Float_t w_mc, w_pu, w_leptonSF, w_DL1r_77, w_jvt;
      
	tree_nominal->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_77);
	tree_nominal->SetBranchAddress("jet_DL1r", &jet_DL1r);
	tree_nominal->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag);
	tree_nominal->SetBranchAddress("jet_pt", &jet_pt);
	tree_nominal->SetBranchAddress("jet_eta", &jet_eta);
	tree_nominal->SetBranchAddress("jet_phi", &jet_phi);
	tree_nominal->SetBranchAddress("jet_e", &jet_e);
	tree_nominal->SetBranchAddress("el_pt", &el_pt);
	tree_nominal->SetBranchAddress("el_eta", &el_eta);
	tree_nominal->SetBranchAddress("el_phi", &el_phi);
	tree_nominal->SetBranchAddress("el_e", &el_e);
	tree_nominal->SetBranchAddress("el_charge", &el_charge);
	tree_nominal->SetBranchAddress("mu_pt", &mu_pt);
	tree_nominal->SetBranchAddress("mu_eta", &mu_eta);
	tree_nominal->SetBranchAddress("mu_phi", &mu_phi);
	tree_nominal->SetBranchAddress("mu_e", &mu_e);
	tree_nominal->SetBranchAddress("mu_charge", &mu_charge);
	tree_nominal->SetBranchAddress("topHeavyFlavorFilterFlag", &topHFFF);
	tree_nominal->SetBranchAddress("runNumber", &runNumber);
	tree_nominal->SetBranchAddress("weight_mc", &w_mc);
	tree_nominal->SetBranchAddress("weight_pileup", &w_pu);
	tree_nominal->SetBranchAddress("weight_leptonSF", &w_leptonSF);
	tree_nominal->SetBranchAddress("weight_bTagSF_DL1r_77", &w_DL1r_77);
	tree_nominal->SetBranchAddress("weight_jvt", &w_jvt);

	// Particle level
	std::vector<Int_t> *jet_truthPartonLabel_pl;
	std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag_pl;
	std::vector<Int_t> *jet_nGhosts_bHadron;
	std::vector<Float_t> *jet_pt_pl;
	std::vector<Float_t> *jet_eta_pl;
	std::vector<Float_t> *jet_phi_pl;
	std::vector<Float_t> *jet_e_pl;
	std::vector<Float_t> *el_pt_pl;
	std::vector<Float_t> *el_eta_pl;
	std::vector<Float_t> *el_phi_pl;
	std::vector<Float_t> *el_e_pl;
	std::vector<Float_t> *el_charge_pl;
	std::vector<Float_t> *mu_pt_pl;
	std::vector<Float_t> *mu_eta_pl;
	std::vector<Float_t> *mu_phi_pl;
	std::vector<Float_t> *mu_e_pl;
	std::vector<Float_t> *mu_charge_pl;
	jet_nGhosts_bHadron = jet_truthPartonLabel_pl = jet_GBHInit_topHadronOriginFlag_pl = 0;
	jet_pt_pl = jet_eta_pl = jet_phi_pl = jet_e_pl = el_pt_pl = el_eta_pl = el_phi_pl = el_e_pl = el_charge_pl = mu_pt_pl = mu_eta_pl = mu_phi_pl = mu_e_pl = mu_charge_pl = 0;
	Float_t w_mc_pl;
	Float_t w_pu_pl;
	Int_t topHFFF_pl;

	tree_pl->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel_pl);
	tree_pl->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag_pl);
	tree_pl->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron);
	tree_pl->SetBranchAddress("jet_pt", &jet_pt_pl);
	tree_pl->SetBranchAddress("jet_eta", &jet_eta_pl);
	tree_pl->SetBranchAddress("jet_phi", &jet_phi_pl);
	tree_pl->SetBranchAddress("jet_e", &jet_e_pl);
	tree_pl->SetBranchAddress("el_pt", &el_pt_pl);
	tree_pl->SetBranchAddress("el_eta", &el_eta_pl);
	tree_pl->SetBranchAddress("el_phi", &el_phi_pl);
	tree_pl->SetBranchAddress("el_e", &el_e_pl);
	tree_pl->SetBranchAddress("el_charge", &el_charge_pl);
	tree_pl->SetBranchAddress("mu_pt", &mu_pt_pl);
	tree_pl->SetBranchAddress("mu_eta", &mu_eta_pl);
	tree_pl->SetBranchAddress("mu_phi", &mu_phi_pl);
	tree_pl->SetBranchAddress("mu_e", &mu_e_pl);
	tree_pl->SetBranchAddress("mu_charge", &mu_charge_pl);
	tree_pl->SetBranchAddress("weight_mc", &w_mc_pl);
	tree_pl->SetBranchAddress("weight_pileup", &w_pu_pl);
	tree_pl->SetBranchAddress("topHeavyFlavorFilterFlag", &topHFFF_pl);
	
      
	
	// ///
	// Loop over Nominal tree entries
	Int_t nEntries = tree_nominal->GetEntries();
	std::cout << "\tnEntries Nominal = " << nEntries << std::endl;
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
	  if (job_DID=="411234") {
	    campaign_xsection_x_genFiltEff = 77.01622;
	    kFactor = 1.1391;}
	  if (job_DID=="411332") {
	    campaign_xsection_x_genFiltEff = 0.713;
	    kFactor = 1.1392; }
	  if (job_DID=="411333") {
	    campaign_xsection_x_genFiltEff = 4.84;
	    kFactor = 1.1392; }
	  if (job_DID=="411334") {
	    campaign_xsection_x_genFiltEff = 3.19;
	    kFactor = 1.1392; }

	  if (runNumber==284500) {
	    campaign_lumi = 3.21956 + 32.9881;
	    if (job_DID=="411076") sumWeights = 3.33006*pow(10, 9);
	    if (job_DID=="411077") sumWeights = 3.61088*pow(10, 9);
	    if (job_DID=="411078") sumWeights = 3.61598*pow(10, 9);
	    if (job_DID=="410472") sumWeights = 5.82869*pow(10, 10);
	    if (job_DID=="411234") sumWeights = 1.46019*pow(10,10);
	    if (job_DID=="411332") sumWeights = 7.20547*pow(10,9);
	    if (job_DID=="411333") sumWeights = 6.9191*pow(10,9);
	    if (job_DID=="411334") sumWeights = 7.27183*pow(10,9);
	  }

	  if (runNumber==300000) {
	    campaign_lumi = 44.3074;
	    if (job_DID=="411076") sumWeights = 4.21891*pow(10, 9);
	    if (job_DID=="411077") sumWeights = 4.49595*pow(10, 9);
	    if (job_DID=="411078") sumWeights = 4.49400*pow(10, 9);
	    if (job_DID=="410472") sumWeights = 7.26510*pow(10, 10);
	    if (job_DID=="411234") sumWeights = 1.81515*pow(10,10);
	    if (job_DID=="411332") sumWeights = 9.0294*pow(10,9);
	    if (job_DID=="411333") sumWeights = 8.82188*pow(10,9);
	    if (job_DID=="411334") sumWeights = 9.00115*pow(10,9);
	  }

	  if (runNumber==310000) {
	    campaign_lumi = 58.4501;
	    if (job_DID=="411076") sumWeights = 5.47811*pow(10,9);
	    if (job_DID=="411077") sumWeights = 5.94763*pow(10,9);
	    if (job_DID=="411078") sumWeights = 5.94190*pow(10,9);
	    if (job_DID=="410472") sumWeights = 1.01641*pow(10,11);
	    if (job_DID=="411234") sumWeights = 2.44336*pow(10,10);
	    if (job_DID=="411332") sumWeights = 1.20387*pow(10,10);
	    if (job_DID=="411333") sumWeights = 1.2078*pow(10,10);
	    if (job_DID=="411334") sumWeights = 1.20945*pow(10,10);
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

	  if ( (topHFFF==1 && (job_DID=="411076" || job_DID=="411332")) || (topHFFF==2 && (job_DID=="411077" || job_DID=="411333")) || (topHFFF==3 && (job_DID=="411078" || job_DID=="411334")) || (topHFFF==0 && (job_DID=="410472" || job_DID=="411234")) ) topHFFF_cut = true;
	
	  
	  // TLorentzVector for leptons and jets
	  TLorentzVector el_lvec;
	  TLorentzVector mu_lvec;
	  std::vector<TLorentzVector> jets_lvec;
	  el_lvec.SetPtEtaPhiE((*el_pt)[0]*0.001, (*el_eta)[0], (*el_phi)[0], (*el_e)[0]*0.001);
	  mu_lvec.SetPtEtaPhiE((*mu_pt)[0]*0.001, (*mu_eta)[0], (*mu_phi)[0], (*mu_e)[0]*0.001);
	  for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	    TLorentzVector lvec;
	    lvec.SetPtEtaPhiE((*jet_pt)[jet_i]*0.001, (*jet_eta)[jet_i], (*jet_phi)[jet_i], (*jet_e)[jet_i]*0.001);
	    jets_lvec.push_back(lvec); }

	
	  // 3b, emu-OS
	  if (emu_cut*OS_cut*jets_n_cut*btags_n3_cut*topHFFF_cut==true) {

	    // Increment yields
	    if (btags_n==3) reco_integral_3b += weight;
	    if (btags_n>=4) reco_integral_4b += weight;


	    // Create a vector of MVA scores sorted wrt jet_pT
	    std::vector<Float_t> MVA_score = {};
	    std::vector<Float_t> tie_breaker = {};
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      #include "../post_mva/include/get_mva_score.h"
	      for (int i=0; i<MVA_score.size(); i++) {
		if (mvaValue==MVA_score[i]) {
		  if (min_dR_jet_lep >= tie_breaker[i]) { mvaValue = mvaValue + 0.000001; }
		  else { mvaValue = mvaValue - 0.000001;}
		}
	      }
	      MVA_score.push_back(mvaValue);
	      tie_breaker.push_back(min_dR_jet_lep);
	    }

	  
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
	    if (n_btags_from_top<2) {
	      if (btags_n==3) h_jets_origin_stat_reco_3b->Fill(3.5, weight);
	      if (btags_n>=4) h_jets_origin_stat_reco_4b->Fill(4.5, weight); }
	    h_jets_n_ft_reco->Fill(n_jets_from_top, weight);
	    h_btags_n_ft_reco->Fill(n_btags_from_top, weight);
	    

	    // Create a map with <MVA_score, btag_index> pairs
	    std::map<float, int> btag_MVAscore_pTidx_map;
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      if ((*jet_DL1r_77)[jet_i]==1) btag_MVAscore_pTidx_map[ MVA_score[jet_i] ] = jet_i;
	    }
	  
	    // Create a vector of indeces that will be {bTop_Pt_1, bTop_Pt2, AddBPt_1,...}
	    std::vector<int> b_bdt_indeces = {};
	    
	    // Loop over the map for the BDT origin info
	    std::map<float, int>::iterator it;
	    for (it = btag_MVAscore_pTidx_map.begin(); it != btag_MVAscore_pTidx_map.end(); it++) {
	      b_bdt_indeces.push_back(it->second);
	    }
	    
	    // Sort 1. btags from top in pT order and 2. the additional btags in pT order
	    std::sort(b_bdt_indeces.begin(), b_bdt_indeces.begin()+2); // 1.
	    std::sort(b_bdt_indeces.begin()+2, b_bdt_indeces.end()); // 2.

	
	    // Now b indeces for the truth level origin info
	    std::vector<int> b_truth_indeces = {};
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      if ( (*jet_GBHInit_topHadronOriginFlag)[jet_i]==4 && (*jet_DL1r_77)[jet_i]==1) b_truth_indeces.push_back(jet_i);
	    }
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      if ( (*jet_GBHInit_topHadronOriginFlag)[jet_i]!=4 && (*jet_DL1r_77)[jet_i]==1) b_truth_indeces.push_back(jet_i);
	    }


	    
	    // Fill the stats hist (12, 13, 23, X4) h_jets_origin_stats
	    if (n_btags_from_top==2) {
	      
	      if (btags_n==3) {
		if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[1]]==4 ) { h_jets_origin_stat_reco_3b->Fill(0.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[2]]==4 ) { h_jets_origin_stat_reco_3b->Fill(1.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[1]]==4 && (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[2]]==4 ) { h_jets_origin_stat_reco_3b->Fill(2.5, weight); }
	      }
	      
	      if (btags_n>=4) {
		if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[1]]==4 ) { h_jets_origin_stat_reco_4b->Fill(0.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[2]]==4 ) { h_jets_origin_stat_reco_4b->Fill(1.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[1]]==4 && (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[2]]==4 ) { h_jets_origin_stat_reco_4b->Fill(2.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[3]]==4 && ((*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[0]]==4 || (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[1]]==4 || (*jet_GBHInit_topHadronOriginFlag)[b_bdt_indeces[2]]==4)) { h_jets_origin_stat_reco_4b->Fill(3.5, weight); }
	      }
	    } // if nbtags from top == 2
	    
	    
      
	    // Jets origin identification efficiency with pT order (leading pT FT - correct, sub-leading pT FT - correct, leadinf pT add - correct ... )
	    for (int i=0; i<std::min(btags_n, 4); i++) { 
	      if (b_bdt_indeces[i]==b_truth_indeces[i]) {
		if (btags_n==3 && i!=3) h_origin_tag_eff_reco_3b->Fill(i+0.5, weight);
		if (btags_n>=4) h_origin_tag_eff_reco_4b->Fill(i+0.5, weight);
	      }
	    }

	    
	    
	    // Make disriminative variables plots
	    
	    // Declate variables
	    float dR_jet_lep0 = -1;
	    float dR_jet_lep1 = -1;
	    float min_dR_jet_lep = -1;
	    float m_jet_el = -1;
	    float m_jet_mu = -1;
	    float min_dR_jet_bjet = 999999.;
	    
	    // Compute the variables
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      
	      if ((*jet_DL1r_77)[jet_i]!=1) continue;
	      
	      float dR_jet_el = jets_lvec[jet_i].DeltaR(el_lvec);
	      float dR_jet_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
	      
	      if ((*el_pt)[0] >= (*mu_pt)[0]) { 
		dR_jet_lep0 = dR_jet_el; 
		dR_jet_lep1 = dR_jet_mu;
	      } else { 
		dR_jet_lep0 = dR_jet_mu;
		dR_jet_lep1 = dR_jet_el;
	      }
	      
	      min_dR_jet_lep = std::min(dR_jet_el, dR_jet_mu);
	      
	      m_jet_el = (jets_lvec[jet_i] + el_lvec).M();
	      m_jet_mu = (jets_lvec[jet_i] + mu_lvec).M();
	      
	      for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
		if ((*jet_DL1r_77)[jet_j]!=1) continue;
		if (jet_i==jet_j) continue;
		float dR_jet_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		if (dR_jet_bjet<min_dR_jet_bjet) min_dR_jet_bjet = dR_jet_bjet;
	      }
	      
	      // Fill hists
	      // All reco
	      h_dR_jet_lep0[0]->Fill(dR_jet_lep0, weight);
	      h_dR_jet_lep1[0]->Fill(dR_jet_lep1, weight);
	      h_min_dR_jet_lep[0]->Fill(min_dR_jet_lep, weight);
	      h_m_jet_el[0]->Fill(m_jet_el, weight);
	      h_m_jet_mu[0]->Fill(m_jet_mu, weight);
	      h_min_dR_jet_bjet[0]->Fill(min_dR_jet_bjet, weight);
	      h_jet_eta[0]->Fill((*jet_eta)[jet_i], weight);
	      h_mva_score[0]->Fill(MVA_score[jet_i], weight);
	      if (jet_i==b_bdt_indeces[0] || jet_i==b_bdt_indeces[1]) {
		// bdt from top
		h_dR_jet_lep0[1]->Fill(dR_jet_lep0, weight);
		h_dR_jet_lep1[1]->Fill(dR_jet_lep1, weight);
		h_min_dR_jet_lep[1]->Fill(min_dR_jet_lep, weight);
		h_m_jet_el[1]->Fill(m_jet_el, weight);
		h_m_jet_mu[1]->Fill(m_jet_mu, weight);
		h_min_dR_jet_bjet[1]->Fill(min_dR_jet_bjet, weight);
		h_jet_eta[1]->Fill((*jet_eta)[jet_i], weight);
		h_mva_score[1]->Fill(MVA_score[jet_i], weight);
	      } else {
		// bdt not from top
		h_dR_jet_lep0[2]->Fill(dR_jet_lep0, weight);
		h_dR_jet_lep1[2]->Fill(dR_jet_lep1, weight);
		h_min_dR_jet_lep[2]->Fill(min_dR_jet_lep, weight);
		h_m_jet_el[2]->Fill(m_jet_el, weight);
		h_m_jet_mu[2]->Fill(m_jet_mu, weight);
		h_min_dR_jet_bjet[2]->Fill(min_dR_jet_bjet, weight);
		h_jet_eta[2]->Fill((*jet_eta)[jet_i], weight);
		h_mva_score[2]->Fill(MVA_score[jet_i], weight);
	      }
	      if (jet_i==b_truth_indeces[0] || jet_i==b_truth_indeces[1]) {
		// truth from top
		h_dR_jet_lep0[3]->Fill(dR_jet_lep0, weight);
                h_dR_jet_lep1[3]->Fill(dR_jet_lep1, weight);
                h_min_dR_jet_lep[3]->Fill(min_dR_jet_lep, weight);
                h_m_jet_el[3]->Fill(m_jet_el, weight);
                h_m_jet_mu[3]->Fill(m_jet_mu, weight);
                h_min_dR_jet_bjet[3]->Fill(min_dR_jet_bjet, weight);
                h_jet_eta[3]->Fill((*jet_eta)[jet_i], weight);
		h_mva_score[3]->Fill(MVA_score[jet_i], weight);
	      } else {
		// truth not from top
		h_dR_jet_lep0[4]->Fill(dR_jet_lep0, weight);
                h_dR_jet_lep1[4]->Fill(dR_jet_lep1, weight);
                h_min_dR_jet_lep[4]->Fill(min_dR_jet_lep, weight);
                h_m_jet_el[4]->Fill(m_jet_el, weight);
                h_m_jet_mu[4]->Fill(m_jet_mu, weight);
                h_min_dR_jet_bjet[4]->Fill(min_dR_jet_bjet, weight);
                h_jet_eta[4]->Fill((*jet_eta)[jet_i], weight);
		h_mva_score[4]->Fill(MVA_score[jet_i], weight);
	      }

	      // Fill pT hists
	      if (jet_i==b_bdt_indeces[0]) h_bTop_1_bdt_reco->Fill((*jet_pt)[jet_i]*0.001, weight);
	      if (jet_i==b_bdt_indeces[1]) h_bTop_2_bdt_reco->Fill((*jet_pt)[jet_i]*0.001, weight);
	      if (jet_i==b_bdt_indeces[2]) h_bAdd_1_bdt_reco->Fill((*jet_pt)[jet_i]*0.001, weight);
	      if (btags_n>3 && jet_i==b_bdt_indeces[3]) h_bAdd_2_bdt_reco->Fill((*jet_pt)[jet_i]*0.001, weight);
	      
	    } // [jet_i] - loop over jets
	    

	    
	  } // 3b, emu-OS -- Nominal
	  
	} // [entry] - loop over Nominal tree entries
	
	

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
	  if (job_DID=="411234") {
            campaign_xsection_x_genFiltEff = 77.01622;
            kFactor = 1.1391;}
          if (job_DID=="411332") {
            campaign_xsection_x_genFiltEff = 0.713;
            kFactor = 1.1392; }
          if (job_DID=="411333") {
            campaign_xsection_x_genFiltEff = 4.84;
            kFactor = 1.1392; }
          if (job_DID=="411334") {
            campaign_xsection_x_genFiltEff = 3.19;
            kFactor = 1.1392; }

	  if (runNumber==284500) {
	    campaign_lumi = 3.21956 + 32.9881;
	    if (job_DID=="411076") sumWeights = 3.33006*pow(10, 9);
	    if (job_DID=="411077") sumWeights = 3.61088*pow(10, 9);
	    if (job_DID=="411078") sumWeights = 3.61598*pow(10, 9);
	    if (job_DID=="410472") sumWeights = 5.82869*pow(10, 10);
	    if (job_DID=="411234") sumWeights = 1.46019*pow(10,10);
            if (job_DID=="411332") sumWeights = 7.20547*pow(10,9);
            if (job_DID=="411333") sumWeights = 6.9191*pow(10,9);
            if (job_DID=="411334") sumWeights = 7.27183*pow(10,9);
	  }

	  if (runNumber==300000) {
	    campaign_lumi = 44.3074;
	    if (job_DID=="411076") sumWeights = 4.21891*pow(10, 9);
	    if (job_DID=="411077") sumWeights = 4.49595*pow(10, 9);
	    if (job_DID=="411078") sumWeights = 4.49400*pow(10, 9);
	    if (job_DID=="410472") sumWeights = 7.26510*pow(10, 10);
	    if (job_DID=="411234") sumWeights = 1.81515*pow(10,10);
            if (job_DID=="411332") sumWeights = 9.0294*pow(10,9);
            if (job_DID=="411333") sumWeights = 8.82188*pow(10,9);
            if (job_DID=="411334") sumWeights = 9.00115*pow(10,9);
	  }

	  if (runNumber==310000) {
	    campaign_lumi = 58.4501;
	    if (job_DID=="411076") sumWeights = 5.47811*pow(10,9);
	    if (job_DID=="411077") sumWeights = 5.94763*pow(10,9);
	    if (job_DID=="411078") sumWeights = 5.94190*pow(10,9);
	    if (job_DID=="410472") sumWeights = 1.01641*pow(10,11);
	    if (job_DID=="411234") sumWeights = 2.44336*pow(10,10);
            if (job_DID=="411332") sumWeights = 1.20387*pow(10,10);
            if (job_DID=="411333") sumWeights = 1.2078*pow(10,10);
            if (job_DID=="411334") sumWeights = 1.20945*pow(10,10);
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

	  if ( (topHFFF==1 && (job_DID=="411076" || job_DID=="411332")) || (topHFFF==2 && (job_DID=="411077" || job_DID=="411333")) || (topHFFF==3 && (job_DID=="411078" || job_DID=="411334")) || (topHFFF==0 && (job_DID=="410472" || job_DID=="411234")) ) topHFFF_cut = true;
	  

	  // TLorentzVector for leptons and jets
	  TLorentzVector el_lvec;
          TLorentzVector mu_lvec;
	  std::vector<TLorentzVector> jets_lvec;
          el_lvec.SetPtEtaPhiE((*el_pt_pl)[0]*0.001, (*el_eta_pl)[0], (*el_phi_pl)[0], (*el_e_pl)[0]*0.001);
          mu_lvec.SetPtEtaPhiE((*mu_pt_pl)[0]*0.001, (*mu_eta_pl)[0], (*mu_phi_pl)[0], (*mu_e_pl)[0]*0.001);
          for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
            TLorentzVector lvec;
            lvec.SetPtEtaPhiE((*jet_pt_pl)[jet_i]*0.001, (*jet_eta_pl)[jet_i], (*jet_phi_pl)[jet_i], (*jet_e_pl)[jet_i]*0.001);
            jets_lvec.push_back(lvec); }
	  
	  
	  // 3b, emu-OS PL
	  if (emu_cut*OS_cut*jets_n_cut*bjets_n3_cut*topHFFF_cut==true) {

	    // Increment yields
	    if (bjets_n==3) pl_integral_3b += weight;
	    if (bjets_n>=4) pl_integral_4b += weight;
	    
	    
	    // Create a vector of MVA scores sorted wrt jet_pT
	    std::vector<Float_t> MVA_score_pl = {};
	    std::vector<Float_t> tie_breaker = {};
	    for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
	      #include "../post_mva/include/get_mva_score_pl.h"
	      for (int i=0; i<MVA_score_pl.size(); i++) {
                if (mvaValue==MVA_score_pl[i]) {
                  if (min_dR_jet_lep >= tie_breaker[i]) { mvaValue = mvaValue + 0.000001; }
                  else { mvaValue = mvaValue - 0.000001;}
                }
              }
	      MVA_score_pl.push_back(mvaValue);
	      tie_breaker.push_back(min_dR_jet_lep);
	    }
	    
	    
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
	    if (n_jets_from_top<2) {
	      if (bjets_n==3) h_jets_origin_stat_pl_3b->Fill(3.5, weight);
	      if (bjets_n>=4) h_jets_origin_stat_pl_4b->Fill(4.5, weight); }
	    h_jets_n_ft_pl->Fill(n_jets_from_top, weight);
	    h_bjets_n_ft_pl->Fill(n_bjets_from_top, weight);
	    

	    // Create a map with <MVA_score, btag_index> pairs
	    std::map<float, int> bjet_MVAscore_pTidx_map = {};
	    for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
	      if ((*jet_nGhosts_bHadron)[jet_i]>=1) bjet_MVAscore_pTidx_map[ MVA_score_pl[jet_i] ] = jet_i;
	    }
	    
	    // Create a vector of indeces that will be {bTop_Pt_1, bTop_Pt2, AddBPt_1,...}
	    std::vector<int> b_bdt_indeces = {};

	    // Loop over the map for the BDT origin info
	    std::map<float, int>::iterator it;
	    for (it = bjet_MVAscore_pTidx_map.begin(); it != bjet_MVAscore_pTidx_map.end(); it++) {
	      b_bdt_indeces.push_back(it->second);
	    }

	    // Sort 1. btags from top in pT order and 2. the additional btags in pT order
	    std::sort(b_bdt_indeces.begin(), b_bdt_indeces.begin()+2); // 1.
	    std::sort(b_bdt_indeces.begin()+2, b_bdt_indeces.end()); // 2.
	    
	    
	    // Now b indeces for the truth level origin info
	    std::vector<int> b_truth_indeces = {};
	    for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
              if ( (*jet_GBHInit_topHadronOriginFlag_pl)[jet_i]==4 && (*jet_nGhosts_bHadron)[jet_i]>=1) b_truth_indeces.push_back(jet_i);
	    }
            for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
              if ( (*jet_GBHInit_topHadronOriginFlag_pl)[jet_i]!=4 && (*jet_nGhosts_bHadron)[jet_i]>=1) b_truth_indeces.push_back(jet_i);
            }

	    
	    // Fill the stats hist
	    if (n_bjets_from_top==2) {

	      if (bjets_n==3) {
		if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[1]]==4) { h_jets_origin_stat_pl_3b->Fill(0.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[2]]==4) { h_jets_origin_stat_pl_3b->Fill(1.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[1]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[2]]==4) { h_jets_origin_stat_pl_3b->Fill(2.5, weight); }
	      }

	      if (bjets_n>=4) {
		if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[1]]==4) { h_jets_origin_stat_pl_4b->Fill(0.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[0]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[2]]==4) { h_jets_origin_stat_pl_4b->Fill(1.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[1]]==4 && (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[2]]==4) { h_jets_origin_stat_pl_4b->Fill(2.5, weight); }
		else if ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[3]]==4 && ((*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[0]]==4 || (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[1]]==4 || (*jet_GBHInit_topHadronOriginFlag_pl)[b_bdt_indeces[2]]==4)) { h_jets_origin_stat_pl_4b->Fill(3.5, weight); }
	      }
	    }
 

	    // Jets origin identification efficiency with pT order (leading pT FT - correct, sub-leading pT FT - correct, leadinf pT add - correct ... ) 
	    for (int i=0; i<std::min(bjets_n, 4); i++) {
              if (b_bdt_indeces[i]==b_truth_indeces[i]) {
                if (bjets_n==3 && i!=3) h_origin_tag_eff_pl_3b->Fill(i+0.5, weight);
                if (bjets_n>=4) h_origin_tag_eff_pl_4b->Fill(i+0.5, weight);
              }
            }

	    
	    // Make disriminative variables plots
	    
	    // Declate variables
	    float dR_jet_lep0 = -1;
	    float dR_jet_lep1 = -1;
	    float min_dR_jet_lep = -1;
	    float m_jet_el = -1;
	    float m_jet_mu = -1;
	    float min_dR_jet_bjet = 999999.;
	    
	    // Compute the variables
	    for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
	      
	      if ((*jet_nGhosts_bHadron)[jet_i]<1) continue;

	      float dR_jet_el = jets_lvec[jet_i].DeltaR(el_lvec);
	      float dR_jet_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
	      
	      if ((*el_pt_pl)[0] >= (*mu_pt_pl)[0]) {
		dR_jet_lep0 = dR_jet_el;
		dR_jet_lep1 = dR_jet_mu;
	      } else {
		dR_jet_lep0 = dR_jet_mu;
		dR_jet_lep1 = dR_jet_el;
	      }
	      
	      min_dR_jet_lep = std::min(dR_jet_el, dR_jet_mu);

	      m_jet_el = (jets_lvec[jet_i] + el_lvec).M();
	      m_jet_mu = (jets_lvec[jet_i] + mu_lvec).M();
	      
	      for (int jet_j=0; jet_j<(*jet_pt_pl).size(); jet_j++) {
		if ((*jet_nGhosts_bHadron)[jet_j]<1) continue;
		if (jet_i==jet_j) continue;
		float dR_jet_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		if (dR_jet_bjet<min_dR_jet_bjet) min_dR_jet_bjet = dR_jet_bjet;
	      }
	      
	      // Fill hists
	      // PL all
	      h_dR_jet_lep0[5]->Fill(dR_jet_lep0, weight);
	      h_dR_jet_lep1[5]->Fill(dR_jet_lep1, weight);
	      h_min_dR_jet_lep[5]->Fill(min_dR_jet_lep, weight);
	      h_m_jet_el[5]->Fill(m_jet_el, weight);
	      h_m_jet_mu[5]->Fill(m_jet_mu, weight);
	      h_min_dR_jet_bjet[5]->Fill(min_dR_jet_bjet, weight);
	      h_jet_eta[5]->Fill((*jet_eta_pl)[jet_i], weight);
	      h_mva_score[5]->Fill(MVA_score_pl[jet_i], weight);
	      if (jet_i==b_bdt_indeces[0] || jet_i==b_bdt_indeces[1]) {
		// PL bdt from top
		h_dR_jet_lep0[6]->Fill(dR_jet_lep0, weight);
		h_dR_jet_lep1[6]->Fill(dR_jet_lep1, weight);
		h_min_dR_jet_lep[6]->Fill(min_dR_jet_lep, weight);
		h_m_jet_el[6]->Fill(m_jet_el, weight);
		h_m_jet_mu[6]->Fill(m_jet_mu, weight);
		h_min_dR_jet_bjet[6]->Fill(min_dR_jet_bjet, weight);
		h_jet_eta[6]->Fill((*jet_eta_pl)[jet_i], weight);
		h_mva_score[6]->Fill(MVA_score_pl[jet_i], weight);
	      } else {
		// PL bdt not from top
		h_dR_jet_lep0[7]->Fill(dR_jet_lep0, weight);
		h_dR_jet_lep1[7]->Fill(dR_jet_lep1, weight);
		h_min_dR_jet_lep[7]->Fill(min_dR_jet_lep, weight);
		h_m_jet_el[7]->Fill(m_jet_el, weight);
		h_m_jet_mu[7]->Fill(m_jet_mu, weight);
		h_min_dR_jet_bjet[7]->Fill(min_dR_jet_bjet, weight);
		h_jet_eta[7]->Fill((*jet_eta_pl)[jet_i], weight);
		h_mva_score[7]->Fill(MVA_score_pl[jet_i], weight);
	      }
	      if (jet_i==b_truth_indeces[0] || jet_i==b_truth_indeces[1]) {
		// PL truth from top
		h_dR_jet_lep0[8]->Fill(dR_jet_lep0, weight);
                h_dR_jet_lep1[8]->Fill(dR_jet_lep1, weight);
                h_min_dR_jet_lep[8]->Fill(min_dR_jet_lep, weight);
                h_m_jet_el[8]->Fill(m_jet_el, weight);
                h_m_jet_mu[8]->Fill(m_jet_mu, weight);
                h_min_dR_jet_bjet[8]->Fill(min_dR_jet_bjet, weight);
                h_jet_eta[8]->Fill((*jet_eta_pl)[jet_i], weight);
		h_mva_score[8]->Fill(MVA_score_pl[jet_i], weight);
	      } else {
		// PL truth not from top
                h_dR_jet_lep0[9]->Fill(dR_jet_lep0, weight);
                h_dR_jet_lep1[9]->Fill(dR_jet_lep1, weight);
                h_min_dR_jet_lep[9]->Fill(min_dR_jet_lep, weight);
                h_m_jet_el[9]->Fill(m_jet_el, weight);
                h_m_jet_mu[9]->Fill(m_jet_mu, weight);
                h_min_dR_jet_bjet[9]->Fill(min_dR_jet_bjet, weight);
                h_jet_eta[9]->Fill((*jet_eta_pl)[jet_i], weight);
		h_mva_score[9]->Fill(MVA_score_pl[jet_i], weight);
	      }

	      // Fill pT hists
	      if (jet_i==b_bdt_indeces[0]) h_bTop_1_bdt_pl->Fill((*jet_pt_pl)[jet_i]*0.001, weight);
	      if (jet_i==b_bdt_indeces[1]) h_bTop_2_bdt_pl->Fill((*jet_pt_pl)[jet_i]*0.001, weight);
	      if (jet_i==b_bdt_indeces[2]) h_bAdd_1_bdt_pl->Fill((*jet_pt_pl)[jet_i]*0.001, weight);
	      if (bjets_n>3 && jet_i==b_bdt_indeces[3]) h_bAdd_2_bdt_pl->Fill((*jet_pt_pl)[jet_i]*0.001, weight);
	      
	    } // [jet_i] - loop over jets
	    
	    
	  } // 3b, emu-OS -- PL

	} // [entry] - loop over PL entries
	    
	ntuple->Close();
	
      } // [ntuple_number] - loop over over samll ntuples of one DID
       
    } // [job_number] - loop over directories (one directory = one mc16 campaign or data_year for one DID)
    
  } // [dir_counter] - loop over directories with ntuples collections



  // Save hists to a file
  TString histfile_savename = std::string("results/hists_bdt_eff_study.root");
  TFile *histfile = new TFile(histfile_savename, "UPDATE");
  histfile->cd();
  std::vector<TString> disrc_vars_str = {"_reco_all", "_reco_bdt_ft", "_reco_bdt_nft", "_reco_truth_ft", "_reco_truth_nft", "_pl_all", "_pl_bdt_ft", "_pl_bdt_nft", "_pl_truth_ft", "_pl_truth_nft"};
  for (int i=0; i<10; i++) {
    h_dR_jet_lep0[i]->Write("3b_incl_dR_jet_lep0" + disrc_vars_str[i], TObject::kOverwrite);
    h_dR_jet_lep1[i]->Write("3b_incl_dR_jet_lep1" + disrc_vars_str[i], TObject::kOverwrite);
    h_min_dR_jet_lep[i]->Write("3b_incl_min_dR_jet_lep" + disrc_vars_str[i], TObject::kOverwrite);
    h_m_jet_el[i]->Write("3b_incl_m_jet_el" + disrc_vars_str[i], TObject::kOverwrite);
    h_m_jet_mu[i]->Write("3b_incl_m_jet_mu" + disrc_vars_str[i], TObject::kOverwrite);
    h_min_dR_jet_bjet[i]->Write("3b_incl_min_dR_jet_bjet" + disrc_vars_str[i], TObject::kOverwrite);
    h_jet_eta[i]->Write("3b_incl_jet_eta" + disrc_vars_str[i], TObject::kOverwrite);
    h_mva_score[i]->Write("3b_incl_mva_score" + disrc_vars_str[i], TObject::kOverwrite);
  }


  h_bTop_1_bdt_reco->Write("bTop_1_reco", TObject::kOverwrite);
  h_bTop_2_bdt_reco->Write("bTop_2_reco", TObject::kOverwrite);
  h_bAdd_1_bdt_reco->Write("bAdd_1_reco", TObject::kOverwrite);
  h_bAdd_2_bdt_reco->Write("bAdd_2_reco", TObject::kOverwrite);
  h_bTop_1_bdt_pl->Write("bTop_1_pl", TObject::kOverwrite);
  h_bTop_2_bdt_pl->Write("bTop_2_pl", TObject::kOverwrite);
  h_bAdd_1_bdt_pl->Write("bAdd_1_pl", TObject::kOverwrite);
  h_bAdd_2_bdt_pl->Write("bAdd_2_pl", TObject::kOverwrite);



  // Drat number of b 
  std::vector<TH1*> h_b_n_vec = {h_bjets_n_pl, h_btags_n_reco};
  std::vector<TString> h_b_n_title = {"bjets, PL", "btags DL1r 77, Reco"};
  int draw_h_b_n = draw_n_hists(h_b_n_vec, h_b_n_title, "#bf{N_{bjets or btags}}", "mc/bjets_btags_n", false, 1, 100000);


  // Draw number of jets from top
  std::vector<TH1*> h_jets_n_ft_vec = {h_jets_n_ft_pl, h_jets_n_ft_reco};
  std::vector<TString> h_jets_n_ft_title = {"particle level", "reco level"};
  int draw_jets_n_ft = draw_n_hists(h_jets_n_ft_vec, h_jets_n_ft_title, "number fo jets from top", "mc/jets_n_from_top", false, 1, 100000);


  // Draw number of b from top
  std::vector<TH1*> h_b_n_ft_vec = {h_bjets_n_ft_pl, h_btags_n_ft_reco};
  std::vector<TString> h_b_n_ft_title = {"bjets, PL", "btags DL1r 77, Reco"};
  int draw_b_n_ft = draw_n_hists(h_b_n_ft_vec, h_b_n_ft_title, "number of b from top", "mc/bjets_btags_n_from_top", false, 1, 100000);


  // Draw stats hist
  std::vector<TH1*> h_stats_3b_vec = {h_jets_origin_stat_pl_3b, h_jets_origin_stat_reco_3b};
  std::vector<TString> h_stats_title = {"particle level", "reco level"};
  int draw_stats_3b = draw_n_hists(h_stats_3b_vec, h_stats_title, "bjets/btags pairs from top (truth info) in "+tmva_config_info[0]+" order", "mc/bjets_pairs_from_tops_study_"+tmva_config_info[0]+"_3b", true, 0, 1);

  std::vector<TH1*> h_stats_4b_vec = {h_jets_origin_stat_pl_4b, h_jets_origin_stat_reco_4b};
  int draw_stats_4b = draw_n_hists(h_stats_4b_vec, h_stats_title, "bjets/btags pairs from top (truth info) in "+tmva_config_info[0]+" order", "mc/bjets_pairs_from_tops_study_"+tmva_config_info[0]+"_4b", true, 0, 1);


  // Draw jets origin identification efficiency with pT taken into account
  h_origin_tag_eff_pl_3b->Scale(1/pl_integral_3b);
  h_origin_tag_eff_reco_3b->Scale(1/reco_integral_3b);
  std::vector<TH1*> h_origin_tag_eff_3b = {h_origin_tag_eff_pl_3b, h_origin_tag_eff_reco_3b};
  int draw_origin_tag_eff_3b = draw_n_hists(h_origin_tag_eff_3b, h_stats_title, "bjets/btags origin identification eff. in pT order", "mc/bjets_from_tops_study_"+tmva_config_info[0]+"_3b", false, 0, 1);

  h_origin_tag_eff_pl_4b->Scale(1/pl_integral_4b);
  h_origin_tag_eff_reco_4b->Scale(1/reco_integral_4b);
  std::vector<TH1*> h_origin_tag_eff_4b= {h_origin_tag_eff_pl_4b, h_origin_tag_eff_reco_4b};
  int draw_origin_tag_eff_4b = draw_n_hists(h_origin_tag_eff_4b, h_stats_title, "bjets/btags origin identification eff. in pT order", "mc/bjets_from_tops_study_"+tmva_config_info[0]+"_4b", false, 0, 1);


  return 0;
}
