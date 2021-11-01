#include "../common_functions.h"
#include "../draw_functions.h"

// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Declare histograms
  TH1 *h_jets_trom_top_reco = new TH1F("jets_from_top_reco", "jets_from_top_reco", 3, 0, 3);
  TH1 *h_jets_trom_top_pl = new TH1F("jets_from_top_pl", "jets_from_top_pl", 3, 0, 3);
  std::vector<TString> x_axis_labels = {"0", "1", "2"};
  for (int i=0; i<x_axis_labels.size(); i++) {
    h_jets_trom_top_reco->GetXaxis()->SetBinLabel(i+1, x_axis_labels[i]);
    h_jets_trom_top_pl->GetXaxis()->SetBinLabel(i+1, x_axis_labels[i]);
  }
  h_jets_trom_top_reco->GetXaxis()->CenterTitle();
  h_jets_trom_top_pl->GetXaxis()->CenterTitle();

  
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

    
    // Only nominal ntuples
    dir_paths[dir_counter] += "nominal/";


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
      bool correct_did = false;
      if (job_DID=="410472" || job_DID=="411076" || job_DID=="411077" || job_DID=="411078") correct_did = true;
      if (correct_did==false) continue;
      else { std::cout << "\n\nDID: " << job_DID << std::endl; }


      // Make a list of paths to ntuples for the given DID
      std::vector<TString> paths_to_ntuples = get_list_of_files(paths_to_jobs[job_number]);


      // Loop over samll ntuples of one DID
      for (int ntuple_number=0; ntuple_number<paths_to_ntuples.size(); ntuple_number++) {

	// Open the ntuple
	std::cout << paths_to_ntuples[ntuple_number] << std::endl;
        TFile *ntuple = new TFile (paths_to_ntuples[ntuple_number]);
        TTree *tree_nominal = (TTree*)ntuple->Get("nominal");
	TTree *tree_pl = (TTree*)ntuple->Get("particleLevel");


	// Set branches

	// Nominal
	std::vector<char> *jet_DL1r_77;
	std::vector<Float_t> *jet_DL1r;
	std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag;
	std::vector<Int_t> *jet_truthflav;
	std::vector<Float_t> *el_charge;
	std::vector<Float_t> *mu_charge;
	jet_DL1r_77 = 0;
	jet_DL1r = el_charge = mu_charge = 0;
	jet_GBHInit_topHadronOriginFlag = jet_truthflav = 0;
	Int_t topHFFF, runNumber;
	Float_t w_mc, w_pu, w_leptonSF, w_DL1r_77, w_jvt;

	tree_nominal->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_77);
	tree_nominal->SetBranchAddress("jet_DL1r", &jet_DL1r);
	tree_nominal->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag);
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
	std::vector<Float_t> *el_charge_pl;
	std::vector<Float_t> *mu_charge_pl;
	jet_truthPartonLabel_pl = jet_GBHInit_topHadronOriginFlag_pl = jet_nGhosts_bHadron = 0;
	el_charge_pl = mu_charge_pl = 0;
	Float_t w_mc_pl;
	Float_t w_pu_pl;
	Int_t topHFFF_pl;

	tree_pl->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel_pl);
	tree_pl->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron);
	tree_pl->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag_pl);
	tree_pl->SetBranchAddress("el_charge", &el_charge_pl);
	tree_pl->SetBranchAddress("mu_charge", &mu_charge_pl);
	tree_pl->SetBranchAddress("weight_mc", &w_mc_pl);
	tree_pl->SetBranchAddress("weight_pileup", &w_pu_pl);
	tree_pl->SetBranchAddress("topHeavyFlavorFilterFlag", &topHFFF_pl);


	
	// ///
	// Loop over nominal tree entries
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

	  int jets_n  = (*MVA_score).size();
	  if (jets_n >= 3) jets_n_cut = true;

	  int btags_n = 0;
	  for (int i=0; i<(*MVA_score).size(); i++) { if ((*jet_DL1r_77)[i]==1) btags_n++; }
	  if (btags_n >=2) btags_n2_cut = true;
	  if (btags_n >=3) btags_n3_cut = true;

	  if ( (topHFFF==1 && job_DID=="411076") || (topHFFF==2 && job_DID=="411077") || (topHFFF==3 && job_DID=="411078") || (topHFFF==0 && job_DID=="410472") ) topHFFF_cut = true;

	  
	  // 3b, emu-OS
	  if (emu_cut*OS_cut*jets_n_cut*btags_n3_cut*topHFFF_cut==true) {

	    int n_btags_from_top = 0; // could be 2, 1, or 0
	    int n_jets_from_top = 0; // should be 2, will skip events with !=2

	    for (int jet_i=0; jet_i<(*jet_DL1r_77).size(); jet_i++) {
	      if ((*jet_DL1r_77)[jet_i]==1 && (*jet_GBHInit_topHadronOriginFlag)[jet_i]==4) n_btags_from_top++;
	      if ((*jet_GBHInit_topHadronOriginFlag)[jet_i]==4) n_jets_from_top++;
	    }

	    if (n_jets_from_top!=2) continue;
	    h_jets_trom_top_reco->Fill(n_btags_from_top+0.5, weight);
	    
	  } // 3b, emu-OS
	  
	} // [entry] - loop over nominal tree entries


	
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
	  double weight = 1;//w_mc_pl * w_pu_pl;

	  
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

	  int jets_n = (*MVA_score_pl).size();
	  if (jets_n >=3) jets_n_cut = true;

	  int bjets_n = 0;
	  for (int i=0; i<(*MVA_score_pl).size(); i++) { if ((*jet_nGhosts_bHadron)[i]>=1) bjets_n++; }
	  if (bjets_n >=2) bjets_n2_cut = true;
	  if (bjets_n >=3) bjets_n3_cut = true;

	  if ( (topHFFF_pl==1 && job_DID=="411076") || (topHFFF_pl==2 && job_DID=="411077") || (topHFFF_pl==3 && job_DID=="411078") || (topHFFF_pl==0 && job_DID=="410472") ) topHFFF_cut = true;

	  
	  // 3b, emu-OS
	  if (emu_cut*OS_cut*jets_n_cut*bjets_n3_cut*topHFFF_cut==true) {

	    int n_bjets_from_top = 0; // could be 2, 1, or 0
	    int n_jets_from_top = 0; // should be 2, will skip events with !=2

	    for (int jet_i=0; jet_i<(*jet_nGhosts_bHadron).size(); jet_i++) {
	      if ((*jet_nGhosts_bHadron)[jet_i]>=1 && (*jet_GBHInit_topHadronOriginFlag_pl)[jet_i]==4) n_bjets_from_top++;
	      if ((*jet_GBHInit_topHadronOriginFlag_pl)[jet_i]==4) n_jets_from_top++;
	    }

	    if (n_jets_from_top!=2) continue;
	    h_jets_trom_top_pl->Fill(n_bjets_from_top+0.5, weight);
	    
	  } // 3b, emu-OS

	} // [entry] - loop over PL tree entries
	
	ntuple->Close();
	
      } // [ntuple_number] - loop over ntuples themselves
      
    } // [job_number] - loop over content of {mc16X, data} directories

  } // [dir_counter] - loop over {mc16X, data} directories


  // Draw the stats hists
  std::vector<TH1*> h_vec = {h_jets_trom_top_pl, h_jets_trom_top_reco};
  std::vector<TString> h_title = {"particle level", "reco level"};
  int draw_stats = draw_n_hists(h_vec, h_title, "number of bjets/btags from top (truth info)", "mc/number_of_b_from_top_study", true, 0, 1);

  return 0;
}
