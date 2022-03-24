// USAGE:
// $ prepare_jets_bkg "process_name" DID1 DID2 ...
// The first command line argument must be the name of the process (Wt, Zt ...) and it is used in the output ntuple name
// Next agruments are DIDs for this process. Do not mix DIDs from different physical processes.
// Test command: ./prepare_jets_bkg Wt 410648 410649


#include "../common_functions.h"

int main(int argc, char *argv[])
{
  // Get MC config info
  std::map<TString, TString> mc_config_info = get_mc_config_info(std::string(argv[1]));
  if (mc_config_info.size()==0) return 0;
  std::map<TString, TString>::iterator it;
  for (it=mc_config_info.begin(); it!=mc_config_info.end(); it++) {
    std::cout << it->first << " :\t" << it->second << std::endl;
  }
  TString process = mc_config_info["process"];
  TString generator = mc_config_info["generator"];
  TString lep_pt_cut_suffix = mc_config_info["lep_pt_cut_suffix"];
  TString campaign = mc_config_info["campaign"];


  // Check for provided DIDs
  if (argc <= 2) {
    std::cout << "\nNo DID provided! \nUse the command line argv to add DID,\nex: ./prepare_jets_bkg config.txt 410472 \nAborting! \n" << std::endl;
    return 0;
  }


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
    std::ifstream ntuples_list_file("ntuples_list_systs_test.txt");
    if (ntuples_list_file.is_open()) {
      while (getline(ntuples_list_file, in_line)) {
	TString ntuple_name = in_line;

	// Split the ntuple name (that also contains ful path) into components to analyze them
	std::vector<TString> ntuple_name_components = split(ntuple_name, '/');

	// Analyze the ntuple name: data or mc? which campaign?
	bool is_data = false;
	bool is_data15 = false;
	bool is_data16 = false;
	bool is_data17 = false;
	bool is_data18 = false;
	bool is_mc16a = false;
	bool is_mc16d = false;
	bool is_mc16e = false;
	TString mc16_str = "";
	bool is_fullsim = false;
	std::string job_DID = "";
	for (int i=0; i<ntuple_name_components.size(); i++) {
	  std::vector<TString> pieces = split(ntuple_name_components[i], '.');
	  for (int j=0; j<pieces.size(); j++) {
	    std::string piece = std::string(pieces[j]);
	    std::vector<TString> subpieces = split(piece, '_');
	    for (int k=0; k<subpieces.size(); k++) {
	      std::string subpiece = std::string(subpieces[k]);

	      if (subpiece == "data") is_data = true;
	      if (subpiece == "grp15") is_data15 = true;
	      if (subpiece == "grp16") is_data16 = true;
	      if (subpiece == "grp17") is_data17 = true;
	      if (subpiece == "grp18") is_data18 = true;
	      if (subpiece == "mc16a") { is_mc16a = true; mc16_str = "mc16a"; }
	      if (subpiece == "mc16d") { is_mc16d = true; mc16_str = "mc16d"; }
	      if (subpiece == "mc16e") { is_mc16e = true; mc16_str = "mc16e"; }
	      if (subpiece == "s3126") is_fullsim = true;

	      // DID is a 6-digits code
	      try {
		int tmp_int_did = stoi(subpiece);
		if (tmp_int_did>=300000 && tmp_int_did<=999999) job_DID = subpiece;
	      } catch(const std::exception& e) {
		// Do nothing
	      }
	    
	    } // [k] - split wrt "_"
	  } // [j] - split wrt "."
	} // [i] - split wrt "/"
	
	// Skip DIDs that are not of outr interest
	if (std::string(argv[did_n])!=job_DID) continue;

	// We work with MC only
	if (is_data == true) continue;
	if (campaign=="mc16a" && is_mc16a!=true) continue;
	if (campaign=="mc16d" && is_mc16d!=true) continue;
	if (campaign=="mc16e" && is_mc16e!=true) continue;
	

	// Select only dids of our interest
	bool correct_did = false;
        #include "../pre_mva_studies/include/did_selection.h"
	if (correct_did==false) continue;
	std::cout << "\nWorking with:\n" << ntuple_name << std::endl;

      
	// Create directory for the output file      
	TString dir1 = std::string("/eos/user/e/eantipov/Files/tthf_analysis/") + mc16_str;
	if (did_n==2) { 
	  gSystem->Exec(std::string("mkdir ") + dir1);
	  std::cout << "Created " << dir1 << std::endl; }
	TString dir2 = dir1 + std::string("/") + process;
	gSystem->Exec(std::string("mkdir ") + dir2);
	std::cout << "Created " << dir2 << std::endl;
	TString dir3 = dir2 + std::string("/") + argv[did_n];
	gSystem->Exec(std::string("mkdir ") + dir3);
	std::cout << "Created " << dir3 << std::endl;
      

	// If just one ntuple needs to be created
	bool one_ntuple_bool = false;
	//if (ntuple_number==22) one_ntuple_bool = true;
	//if (one_ntuple_bool==false) continue;
	  
	
	// Declare the output file and set all branches
	//TString savefile_name = dir3 + std::string("/") + std::to_string(ntuple_number) + std::string(".root");
	TString savefile_name = std::to_string(ntuple_number) + ".root";
	TFile *out_ntuple = new TFile(savefile_name, "RECREATE");
	
	  
	// Open the ntuple
	TFile *ntuple = new TFile(ntuple_name);


	// Count number of trees
	int n_trees_to_work_with = 0;
	TIter next1(ntuple->GetListOfKeys());
        TKey *key1;
        while (key1 = (TKey*)next1()) {

	  TString obj_name = key1->GetName();
	  TString obj_class_name = key1->GetClassName();
          if (obj_class_name!="TTree") continue;

	  bool is_syst_tree = false;
	  std::vector<TString> obj_name_parts = split(obj_name, '_');
	  if (obj_name_parts[obj_name_parts.size()-1]=="1down" || obj_name_parts[obj_name_parts.size()-1]=="1up" || obj_name_parts[obj_name_parts.size()-1]=="ResoPara" || obj_name_parts[obj_name_parts.size()-1]=="ResoPrep" || obj_name_parts[obj_name_parts.size()-1]=="ScaleDown" || obj_name_parts[obj_name_parts.size()-1]=="ScaleUp") is_syst_tree = true;

	  if (is_syst_tree!=true && obj_name!="nominal" && obj_name!="particleLevel") continue;
	  n_trees_to_work_with++;
	} // [key] - loop over trees
	

	// Run actual preparation of output trees
	int tree_number = 0;
	TIter next(ntuple->GetListOfKeys());
	TKey *key;
	while (key = (TKey*)next()) {
	  
	  TString obj_name = key->GetName();
	  TString obj_class_name = key->GetClassName();
	  
	  if (obj_class_name!="TTree") continue;

	  bool is_syst_tree = false;
	  std::vector<TString> obj_name_parts = split(obj_name, '_');
	  if (obj_name_parts[obj_name_parts.size()-1]=="1down" || obj_name_parts[obj_name_parts.size()-1]=="1up" || obj_name_parts[obj_name_parts.size()-1]=="ResoPara" || obj_name_parts[obj_name_parts.size()-1]=="ResoPrep" || obj_name_parts[obj_name_parts.size()-1]=="ScaleDown" || obj_name_parts[obj_name_parts.size()-1]=="ScaleUp") is_syst_tree = true;


	  // Declare the input and output trees
	  TTree *tree =  (TTree*)ntuple->Get(obj_name);
	  TTree *out_tree;

	  
	  // Skip all trees where BDT is not needed, but save them
	  if (is_syst_tree!=true && obj_name!="nominal" && obj_name!="particleLevel") {
	    
	    out_tree = (TTree*)tree->CloneTree(0);
	    out_tree->CopyEntries(tree);
	    out_tree->SetDirectory(out_ntuple);
	    out_tree->Write(obj_name, TTree::kOverwrite);
	    
	    continue;
	  }

	  
	  // Print what we're working with
	  tree_number++;
	  std::cout << "\n\n\t" << "[" << tree_number << "/" << n_trees_to_work_with << "] Branch: " << obj_name << std::endl;

	  
	  // Declare output tree and branches
	  out_ntuple->cd();
	  out_tree = new TTree(obj_name, obj_name);
	  #include "include/savefile_declare_branches.h"

	  // Set all the needed branches in the input file
	  ntuple->cd();
          #include "include/branches.h"

	  // Ignore the "ReadStreamerInfo, class:string, illegal uid=-2" error

	    
	  // ///
	  // Loop over entries
	  
	  Int_t nEntries = tree->GetEntries();
	  std::cout << "\tEntries = " << nEntries << std::endl;
	  for (int entry=0; entry<nEntries; entry++) {
	    
	    if (((entry+1)*1000/nEntries)%10==0) { std::cout << "\t" << int((entry+1)*100/nEntries) << "%\r"; std::cout.flush(); }
	    tree->GetEntry(entry);
	    
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
	    #include "../pre_mva_studies/include/compute_weight.h"
	    if (obj_name!="particleLevel") {
	      weight = w_mc * w_pu * w_leptonSF * w_DL1r_77 * w_jvt * weight_lumi;
	    } else {
	      weight = w_mc * weight_lumi;
	    }
	    
	    
	    // Decalre cuts names and set to false as the defualt
	    bool emu_cut = false;
	    bool OS_cut = false;
	    bool lep_pt_cut = false;
	    bool jets_n_cut = false;
	    bool btags_n2_cut = false;
	    bool btags_n3_cut = false;
	    bool btags_n3plus_cut = false;
	    
	    // Declare cuts themselves
	    if ((*el_pt).size()==1 && (*mu_pt).size()==1) emu_cut = true;
	    if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	    if ( ((*el_pt)[0]*0.001>28 && (*mu_pt)[0]*0.001>28) || lep_pt_cut_suffix=="")  lep_pt_cut = true;
	    
	    int jets_n = (*jet_pt).size();
	    if (jets_n >=3) jets_n_cut = true;
	    
	    int btags_n = 0;
	    for (int i=0; i<(*jet_pt).size(); i++) { 
	      if (obj_name!="particleLevel") {
		if ((*jet_DL1r_77)[i]==1) btags_n++;
	      } else {
		if ( (*jet_nGhosts_bHadron)[i]>=1) btags_n++;
	      } 
	    }
	    if (btags_n >= 2) btags_n2_cut = true;
	    if (btags_n == 3) btags_n3_cut = true;
	    if (btags_n >= 3) btags_n3plus_cut = true;
	    
	    
	    // TLorentzVector for leptons and jets
	    TLorentzVector el_lvec;
	    TLorentzVector mu_lvec;
	    std::vector<TLorentzVector> jets_lvec;
	    if (obj_name!="particleLevel") {
	      el_lvec.SetPtEtaPhiE((*el_pt)[0]*0.001, (*el_cl_eta)[0], (*el_phi)[0], (*el_e)[0]*0.001);
	    } else {
	      el_lvec.SetPtEtaPhiE((*el_pt)[0]*0.001, (*el_eta)[0], (*el_phi)[0], (*el_e)[0]*0.001);
	    }
	    mu_lvec.SetPtEtaPhiE((*mu_pt)[0]*0.001, (*mu_eta)[0], (*mu_phi)[0], (*mu_e)[0]*0.001);
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	      TLorentzVector lvec;
	      lvec.SetPtEtaPhiE((*jet_pt)[jet_i]*0.001, (*jet_eta)[jet_i], (*jet_phi)[jet_i], (*jet_e)[jet_i]*0.001);
	      jets_lvec.push_back(lvec); }
	    
	    
	    // 3b inclusive channel, emu-OS
	    if (emu_cut*OS_cut*lep_pt_cut*btags_n3plus_cut*jets_n_cut == true) {
	      
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
		
	      } // [jet_i] - loop over jets 

	      // var_out = var_in for those that we don't need to change
	      #include "include/var_out.h"

	      out_tree->Fill();
	      
	    } //3b incl, emu-OS
	    
	  } // [entry] - loop over entries 
	 

	  // Save the output tree
	  out_ntuple->cd();
	  out_tree->SetDirectory(out_ntuple);
	  out_tree->Write(obj_name, TTree::kOverwrite);
 
	} // while key 
	    
	out_ntuple->Close();
       
	ntuple->Close();
	
      } // loop over ntuple names from the file
	  
    } // if (ntuples_list_file.is_open) - checking the file
    
  } // [did_n] did number (1,2,3..) from the command line arguments

  std::cout << "\n\nGreat success!" << std::endl;
  return 0;
}
