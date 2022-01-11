// USAGE:
// $ prepare_jets_bkg "process_name" DID1 DID2 ...
// The first command line argument must be the name of the process (Wt, Zt ...) and it is used in the output ntuple name
// Next agruments are DIDs for this process. Do not mix DIDs from different physical processes.
// Test command: ./prepare_jets_bkg Wt 410648 410649


#include "../common_functions.h"

int main(int argc, char *argv[])
{
  // Check for provided DIDs
  if (argc == 1) {
    std::cout << "\nNo DID provided! \nUse the command line argv to add DID, ex: ./prepare_jets_bkg 410472 \nAborting! \n" << std::endl;
    return 0;
  }

  // Check for the generator and mc16a choice
  std::string generator = ttbar_generator();
  if (generator=="quit") return 0;
  if (generator=="" && std::string(argv[1])=="tt") {
    generator="nominal";
    std::cout << "No generator was selected for ttbar, assuming nominal" << std::endl; 
  }
  bool mc16a_only_test = mc16a_only_choise();


  // Check for mc16 campaign choice
  std::string mc16_choice = "";
  std::cout << "\nSelect mc16 campaing. Options:" << std::endl;;
  std::cout << "-- mc16a" << "\n-- mc16d" << "\n-- mc16e" << "\n-- FullRun2" << std::endl;
  std::cout << "Type in your choice: ";
  std::cin >> mc16_choice;
  while (mc16_choice!="mc16a" && mc16_choice!="mc16d" && mc16_choice!="mc16e" && mc16_choice!="FullRun2") {
    std::cout << "Wrong mc16 campaign choice!\n Select from the options above or type \"exit\" to terminate." << std::endl;
    std::cout << "Type your choice: ";
    std::cin >> mc16_choice;
    if (mc16_choice=="exit") return 0;
    std::cout << std::endl;
  }
  std::cout << std::endl;


  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");
  

  // Seup MVA reader
  #include "include/setup_tmva_reader.h"
  
  
  // Loop over provided DIDs:
  for (int did_n=2; did_n<argc; did_n++) {

    // Declare a counter of ntuples
    int ntuple_number = 0;


    // Run a loop over ntuples names from a txt file
    std::string in_line = "";
    std::ifstream ntuples_list_file("ntuples_list.txt");
    if (ntuples_list_file.is_open()) {
      while (getline(ntuples_list_file, in_line)) {
	TString ntuple_name = in_line;

	// Split the ntuple name (that also contains ful path) into components to analyze them
	std::vector<TString> ntuple_name_components = split(ntuple_name, '/');

	// Analyze the ntuple name: data or mc? which campaign?
	bool is_data = false;
	bool is_mc16a = false;
	bool is_mc16d = false;
	bool is_mc16e = false;
	TString mc16_dir = "";
	bool is_fullsim = false;
	std::string job_DID = "";
	for (int i=0; i<ntuple_name_components.size(); i++) {
	  std::vector<TString> pieces = split(ntuple_name_components[i], '.');
	  for (int j=0; j<pieces.size(); j++) {
	    std::string piece = std::string(pieces[j]);
	    if (piece == "data") is_data = true;
	    if (piece == "mc16a") { is_mc16a = true; mc16_dir = "mc16a"; }
	    if (piece == "mc16d") { is_mc16d = true; mc16_dir = "mc16d"; }
	    if (piece == "mc16e") { is_mc16e = true; mc16_dir = "mc16e"; }
	    if (piece == "s3126") is_fullsim = true;
	    if (i==ntuple_name_components.size()-2 && j==2) job_DID = piece;
	  }
	}
	
	// We work with MC only
	if (is_data == true) continue;
	if (mc16a_only_test==true && is_mc16a!=true) continue;

	// Select only dids of our interest
	bool correct_did = false;
        #include "../pre_mva_studies/include/did_selection.h"
	if (correct_did==false) continue;
	std::cout << "\nWorking with:\n" << ntuple_name << std::endl;

      
	// Create directory for the output file      
	TString dir1 = std::string("/eos/user/e/eantipov/Files/tthf_analysis/") + mc16_dir;
	if (did_n==2) { 
	  gSystem->Exec(std::string("mkdir ") + dir1);
	  std::cout << "Created " << dir1 << std::endl; }
	TString dir2 = dir1 + std::string("/") + argv[1];
	gSystem->Exec(std::string("mkdir ") + dir2);
	std::cout << "Created " << dir2 << std::endl;
	TString dir3 = dir2 + std::string("/") + argv[did_n];
	gSystem->Exec(std::string("mkdir ") + dir3);
	std::cout << "Created " << dir3 << std::endl;
      

	// If just one ntuple needs to be created
	bool tmp_bool = false;
	//if (ntuple_number==22) tmp_bool = true;
	//if (tmp_bool==false) continue;
	  
	
	// Declare the output file and set all branches
	TString savefile_name = dir3 + std::string("/") + std::to_string(ntuple_number) + std::string(".root");
	TFile *out_ntuple = new TFile(savefile_name, "RECREATE");
	TTree *out_tree_pl = new TTree("particleLevel", "particleLevel");
	TTree *out_tree = new TTree("nominal", "nominal");
        #include "include/savefile_declare_branches.h"

	  
	// Open the ntuple
	TFile *ntuple = new TFile(ntuple_name);
	
	// Set trees. Particle level exists not for all the ntuples.
	TTree *tree_pl;
	bool tree_pl_exists = false;
	TIter next(ntuple->GetListOfKeys());
	TKey *key;
	while (key = (TKey*)next()) {
	  TString obj_name = key->GetName();
	  TString obj_class_name = key->GetClassName();
	  if (obj_name=="particleLevel" && obj_class_name=="TTree") {
	    tree_pl = (TTree*)ntuple->Get(obj_name);
	    tree_pl_exists = true; }
	} // while key
	
	TTree *tree_sumWeights = (TTree*)ntuple->Get("sumWeights");
	TTree *tree_truth = (TTree*)ntuple->Get("truth");
	TTree *tree_nominal = (TTree*)ntuple->Get("nominal");
	
	
	// Set all the needed branches
        #include "include/branches.h"
	  
	  
	// Ignore the "ReadStreamerInfo, class:string, illegal uid=-2" error



	// ///
	// Loop over entries - nominal tree
	
	Int_t nEntries = tree_nominal->GetEntries();
	std::cout << "\tEntries = " << nEntries << std::endl;
	for (int entry=0; entry<nEntries; entry++) {
	
	  if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
	  tree_nominal->GetEntry(entry);
	    
	    
	  // Zero vectors for output file 
	  dR_jet_lep0_out->clear();
	  dR_jet_lep1_out->clear();
	  min_dR_jet_lep_out->clear();
	  m_jet_el_out->clear();
	  m_jet_mu_out->clear();
	  m_jet_lep_max_out->clear();
	  jet_m_out->clear();
	  m_min_jet_jet_out->clear();
	  m_max_jet_jet_out->clear();
	  min_dR_jet_bjet_out->clear();
	  MVA_score->clear();
	  
	  
	  // Compute weights
	  double weight = 1;
	  #include "include/compute_weight.h"
	  weight = w_mc * w_pu * w_leptonSF * w_DL1r_77 * w_jvt * weight_lumi;
	  
	  
	  // Decalre cuts names and set to false as the defualt
	  bool emu_cut = false;
	  bool OS_cut = false;
	  bool jets_n_cut = false;
	  bool btags_n2_cut = false;
	  bool btags_n3_cut = false;
	  bool btags_n3plus_cut = false;
	  
	  // Declare cuts themselves
	  if ((*el_pt).size()==1 && (*mu_pt).size()==1) emu_cut = true;
	  if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	  
	  int jets_n = (*jet_pt).size();
	  if (jets_n >=3) jets_n_cut = true;
	  
	  int btags_n = 0;
	  for (int i=0; i<(*jet_pt).size(); i++) { if ((*jet_DL1r_77)[i]==1) btags_n++; }
	  if (btags_n >= 2) btags_n2_cut = true;
	  if (btags_n == 3) btags_n3_cut = true;
	  if (btags_n >= 3) btags_n3plus_cut = true;
	  
	  
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
	  
	  
	  
	  // 2b (tags) inclusive, emu, OS
	  //if (emu_cut*OS_cut*btags_n2_cut*jets_n_cut == true) {
	  // TEST: get all events
	  if (btags_n2_cut == true) {
	    
	    // Loop over jets
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      
	      // Set localVar = treeVar for further mva score estimation
	      #include "include/get_mva_score.h"
	      for (int i=0; i<(*MVA_score).size(); i++) {
		if (mvaValue==(*MVA_score)[i]) {
		  if (min_dR_jet_lep >= (*min_dR_jet_lep_out)[i]) { mvaValue = mvaValue + 0.000001; }
		  else { mvaValue = mvaValue - 0.000001; }
		}
	      }
	      MVA_score->push_back(mvaValue);
	      
	      dR_jet_lep0_out->push_back(dR_jet_lep0);
	      dR_jet_lep1_out->push_back(dR_jet_lep1);
	      min_dR_jet_lep_out->push_back(min_dR_jet_lep);
	      m_jet_el_out->push_back(m_jet_el);
	      m_jet_mu_out->push_back(m_jet_mu);
	      m_jet_lep_max_out->push_back(m_jet_lep_max);
	      jet_m_out->push_back(jet_m);
	      m_min_jet_jet_out->push_back(m_min_jet_jet);
	      m_max_jet_jet_out->push_back(m_max_jet_jet);
	      min_dR_jet_bjet_out->push_back(min_dR_jet_bjet);
	      
	    }  // [jet_i] - loop over jets
	    
	    // var_out = var_in for those that we don't need to change
	    #include "include/var_out.h"

	      out_tree->Fill();
	      
	  } // [if] - 2b dilep OS cuts
	  
	} // [entry] - loop over entries, nominal tree
	


	  
	// ///
	// Loop over entries - Partilce level tree
	
	if (tree_pl_exists==true) {
	  
	  Int_t nEntries_pl = tree_pl->GetEntries();
	  std::cout << "\tEntris PL = " << nEntries_pl << std::endl;
	  for (int entry=0; entry<nEntries_pl; entry++) {
	    
	    if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
	    tree_pl->GetEntry(entry);
	    
	    
	    // Zero vector for output file (particle level tree)
	    dR_jet_lep0_pl_out->clear();
	    dR_jet_lep1_pl_out->clear();
	    min_dR_jet_lep_pl_out->clear();
	    m_jet_el_pl_out->clear();
	    m_jet_mu_pl_out->clear();
	    m_jet_lep_max_pl_out->clear();
	    jet_m_pl_out->clear();
	    m_min_jet_jet_pl_out->clear();
	    m_max_jet_jet_pl_out->clear();
	    min_dR_jet_bjet_pl_out->clear();
	    MVA_score_pl->clear();
	    
	    
	    // Declare cuts names nad set to false as the default
	    bool emu_cut = false;
	    bool OS_cut = false;
	    bool jets_n_cut = false;
	    bool btags_n2_cut = false;
	    bool btags_n3plus_cut = false;
	    
	    // Declare cuts themselves
	    if ((*el_pt_pl).size()==1 && (*mu_pt_pl).size()==1) emu_cut = true;
	    if ((*el_charge_pl)[0]!=(*mu_charge_pl)[0]) OS_cut = true;
	    
	    int jets_n = (*jet_pt_pl).size();
	    if (jets_n >= 3) jets_n_cut = true;
	    
	    int btags_n = 0;
	    for (int i=0; i<(*jet_pt_pl).size(); i++) { if ( (*jet_nGhosts_bHadron)[i]>=1 ) btags_n++; }
	    if (btags_n >= 2) btags_n2_cut = true;
	    if (btags_n >= 3) btags_n3plus_cut = true;
	    
	    
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
	    
	    
	    
	    // 2b (truth) inclusive, emu, OS
	    //if (emu_cut*OS_cut*jets_n_cut*btags_n2_cut == true) {
	    // TEST: get all events
	    if (btags_n2_cut == true) {
	      
	      // Loop over jets
	      for (int jet_i=0; jet_i<(*jet_pt_pl).size(); jet_i++) {
		
		// Set localVar - treeVar for furthe mva score estimation
		#include "include/get_mva_score_pl.h"
		for (int i=0; i<(*MVA_score_pl).size(); i++) {
		  if (mvaValue==(*MVA_score_pl)[i]) {
		    if (min_dR_jet_lep >= (*min_dR_jet_lep_pl_out)[i]) { mvaValue = mvaValue + 0.000001; }
		    else { mvaValue = mvaValue - 0.000001; }
		  }
		}
		MVA_score_pl->push_back(mvaValue);
		
		dR_jet_lep0_pl_out->push_back(dR_jet_lep0);
		dR_jet_lep1_pl_out->push_back(dR_jet_lep1);
		min_dR_jet_lep_pl_out->push_back(min_dR_jet_lep);
		m_jet_el_pl_out->push_back(m_jet_el);
		m_jet_mu_pl_out->push_back(m_jet_mu);
		m_jet_lep_max_pl_out->push_back(m_jet_lep_max);
		jet_m_pl_out->push_back(jet_m);
		m_min_jet_jet_pl_out->push_back(m_min_jet_jet);
		m_max_jet_jet_pl_out->push_back(m_max_jet_jet);
		min_dR_jet_bjet_pl_out->push_back(min_dR_jet_bjet);
		
	      } // [jet_i] - loop over jets
	      
	      // var_out = var_in for those that we don't need to change
	      #include "include/var_out_pl.h"

	      out_tree_pl->Fill();
		
	    } // if - 2b (truth) inclusive, emu, OS
	    
	  } // [entry] - loop over entries, particle level tree
	  
	} // if PL tree exists
	
	
        // ///
	// Save all the trees to the new file now
	
	out_ntuple->cd();
	  
	TTree *out_sumWeights_tree = (TTree*)tree_sumWeights->CloneTree(0);
	out_sumWeights_tree->CopyEntries(tree_sumWeights);
	out_sumWeights_tree->SetDirectory(out_ntuple);
	out_sumWeights_tree->Write("sumWeights", TTree::kOverwrite); 
	
	TTree *out_truth_tree = (TTree*)tree_truth->CloneTree(0);
	out_truth_tree->CopyEntries(tree_truth);
	out_truth_tree->SetDirectory(out_ntuple);
	out_truth_tree->Write("truth", TTree::kOverwrite); 
	  
	out_tree->SetDirectory(out_ntuple);
	out_tree->Write("nominal", TTree::kOverwrite);
	
	if (tree_pl_exists==true) {
	  out_tree_pl->SetDirectory(out_ntuple);
	  out_tree_pl->Write("particleLevel", TTree::kOverwrite);
	}
	
	out_ntuple->Close();
	
	ntuple->Close();
	
      } // loop over ntuple names from the file
	  
    } // if (ntuples_list_file.is_open) - checking the file
    
  } // [did_n] did number (1,2,3..) from the command line arguments

  std::cout << "\n\nGreat success!" << std::endl;
  return 0;
}
