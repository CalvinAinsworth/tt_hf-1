#include "../common_functions.h"

int main(int argc, char *argv[])
{
  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");


  // Setup MVA reader
  #include "include/setup_tmva_reader.h"


  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/atlas/atlascerngroupdisk/phys-top/ttjets/v4/data/";
  std::vector<TString> dir_paths = get_list_of_files(path_to_ntuples);


  // Loop over directories with ntuples collections
  for (int dir_counter=0; dir_counter<dir_paths.size(); dir_counter++) {
    
    // Select only directiories from the list of files and directories
    TSystemFile dir(dir_paths[dir_counter], dir_paths[dir_counter]);
    if (!(dir.IsDirectory())) continue;
    std::cout << "\n\n\n" << dir_paths[dir_counter] << std::endl;


    // Analyze directiry's name: data/mc, which campaign?
    std::vector<TString> dir_path_components = split(dir_paths[dir_counter], '/');
    int last_element_index = dir_path_components.size();
    std::vector<TString> dir_name_components = split(dir_path_components[last_element_index-1], '.');
    bool is_data = false;
    bool is_2015 = false;
    bool is_2016 = false;
    bool is_2017 = false;
    bool is_2018 = false;
    bool is_mc16a = false;
    bool is_mc16d = false;
    bool is_mc16e = false;
    TString data_year_dir = "";
    for (int i=0; i<dir_name_components.size(); i++) {
      if (dir_name_components[i] == "periodAllYear") is_data = true;
      if (dir_name_components[i] == "grp15_v01_p4030") { is_2015 = true; data_year_dir = "grp15"; }
      if (dir_name_components[i] == "grp16_v01_p4030") { is_2016 = true; data_year_dir = "grp16"; }
      if (dir_name_components[i] == "grp17_v01_p4030") { is_2017 = true; data_year_dir = "grp17"; }
      if (dir_name_components[i] == "grp18_v01_p4030") { is_2018 = true; data_year_dir = "grp18"; } }


    // We work with data
    if (is_data != true) continue;
    std::cout << dir_path_components[last_element_index-1] << std::endl;
    if (data_year_dir!="grp17") continue; // TEST
    
    
    // Create a directory for the output file
    TString dir1 = std::string("/eos/user/e/eantipov/Files/tthf_analysis/data");
    gSystem->Exec(std::string("mkdir ") + dir1);
    TString dir2 = dir1 + std::string("/") + data_year_dir;
    gSystem->Exec(std::string("mkdir ") + dir2);
    std::cout << "Created " << dir2 << std::endl;


    // Make a list of paths to jobs/DIDs outputs (pieces of a full ntuple)
    std::vector<TString> paths_to_jobs = get_list_of_files(dir_paths[dir_counter]);
    

    // Loop over files
    for (int job_number=0; job_number<paths_to_jobs.size(); job_number++) {
      
      std::vector<TString> paths_to_jobs_components = split(paths_to_jobs[job_number], '/');
      std::vector<TString> ntuple_name_components = split(paths_to_jobs_components[paths_to_jobs_components.size()-1], '.'); // TEST
      bool tmp_bool = false;
      if (ntuple_name_components[3]=="_000175") tmp_bool = true; // TEST
      if (tmp_bool == false) continue; // TEST
      
      // Declare the output file and set all branches
      TString savefile_name = dir2 + std::string("/") + std::to_string(job_number) + std::string(".root");
      TFile *out_ntuple = new TFile(savefile_name, "RECREATE");
      TTree *out_tree = new TTree("nominal", "nominal");
      #include "include/savefile_data_declare_branches.h"


      // Open the input ntuple
      std::cout << paths_to_jobs[job_number] << "\n" << std::endl;
      TFile *ntuple = new TFile(paths_to_jobs[job_number]);
      TTree *tree_sumWeights = (TTree*)ntuple->Get("sumWeights");
      TTree *tree_nominal = (TTree*)ntuple->Get("nominal");
      bool tree_pl_exists = false;

      
      // Set all the needed branches
      #include "include/branches_data.h"
      
      
      // ///
      // Loop over entries
      
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
	// TETS: get all events
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
	    
	  } // [jet_i] - loop over jets

	  // var_out = var_in for those that we don't need to change
	  #include "include/var_out_data.h"

	  out_tree->Fill();
	  
	} // 2b emu OS

      } // [entry] - loop over entries

      out_ntuple->cd();

      TTree *out_sumWeights_tree = (TTree*)tree_sumWeights->CloneTree(0);
      out_sumWeights_tree->CopyEntries(tree_sumWeights);
      out_sumWeights_tree->SetDirectory(out_ntuple);
      out_sumWeights_tree->Write("sumWeights", TTree::kOverwrite);

      out_tree->SetDirectory(out_ntuple);
      out_tree->Write("nominal", TTree::kOverwrite);

      out_ntuple->Close();

      ntuple->Close();
      
    } // [job_number] - loop over ntuples

  } // [dir_couter] - lopp over years

  return 0;
}
