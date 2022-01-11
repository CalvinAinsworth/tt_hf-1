#include "../common_functions.h"

// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Check for the generator and mc16a_only choises
  std::string generator = ttbar_generator();
  if (generator=="quit") return 0;
  if (generator=="" && std::string(argv[1])=="tt") {
    generator="nominal";
    std::cout << "No generator was selected for ttbar, assuming nominal" << std::endl; }
  bool mc16a_only_test = mc16a_only_choise();


  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");
  
  
  // Declare hists discriminative variables and bdt, 0 - all, 1- ft, 2- nft 
  TH1 *h_dR_jet_lep0[3];
  TH1 *h_dR_jet_lep1[3];
  TH1 *h_min_dR_jet_lep[3];
  TH1 *h_m_jet_el[3];
  TH1 *h_m_jet_mu[3];
  TH1 *h_min_dR_jet_bjet[3];
  TH1 *h_jet_eta[3];
  TH1 *h_mva_score[3];
  for (int i=0; i<3; i++) {
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


  // btags pTs
  Double_t pt_bins_edges[7] = {50,70,100,150,200,300,400};
  // TH1F *hist = new TH1F("", "", int bins_n, float bins_edges[bins_n+1]);
  TH1F *h_bTop_1_bdt = new TH1F("h_bTop_1_bdt", "h_bTop_1_bdt", 100, 0, 1000);
  TH1F *h_bTop_2_bdt = new TH1F("h_bTop_2_bdt", "h_bTop_2_bdt", 100, 0, 1000);
  TH1F *h_bAdd_1_bdt = new TH1F("h_bAdd_1_bdt", "h_bAdd_1_bdt", 100, 0, 1000);
  TH1F *h_bAdd_2_bdt = new TH1F("h_bAdd_2_bdt", "h_bAdd_2_bdt", 100, 0, 1000);


  // Seup MVA reader
  #include "../post_mva/include/setup_tmva_reader.h"
  

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
    for (int i=0; i<dir_name_components.size(); i++) {
      if (dir_name_components[i] == "periodAllYear") is_data = true;
      if (dir_name_components[i] == "grp15_v01_p4030") is_2015 = true;
      if (dir_name_components[i] == "grp16_v01_p4030") is_2016 = true;
      if (dir_name_components[i] == "grp17_v01_p4030") is_2017 = true;
      if (dir_name_components[i] == "grp18_v01_p4030") is_2018 = true;
      if (dir_name_components[i] == "mc16a") is_mc16a = true;
      if (dir_name_components[i] == "mc16d") is_mc16d = true;
      if (dir_name_components[i] == "mc16e") is_mc16e = true; }


    // We work with data
    if (is_data != true) continue;
    if (mc16a_only_test==true && (is_2017==true || is_2018==true)) continue;
    std::cout << dir_path_components[last_element_index-1] << std::endl;

    
    // Make a list of paths to jobs/DIDs outputs (pieces of a full ntuple) 
    std::vector<TString> paths_to_jobs = get_list_of_files(dir_paths[dir_counter]);

    
    // Loop over files 
    for (int job_number=0; job_number<paths_to_jobs.size(); job_number++) {

      TFile *ntuple = new TFile (paths_to_jobs[job_number]);
      TTree *tree_nominal = (TTree*)ntuple->Get("nominal");

      std::cout << paths_to_jobs[job_number] << "\n" << std::endl;


      // Set all needed branches
      #include "../pre_mva_studies/include/branches.h"
      std::cout << "\nSome branches don't exists for data - that's normal and won't affect performance" << std::cout;

      
      // Loop over entries
      Int_t nEntries = tree_nominal->GetEntries();
      std::cout << "\tEntries = " << nEntries << std::endl;
      for (int entry=0; entry<nEntries; entry++)  {

	// Show events counter
	if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
        tree_nominal->GetEntry(entry);


        // There are no weights in data
	double weight = 1;

	
	// Declare cuts names
	bool emu_cut = false;
        bool OS_cut = false;
        bool jets_n_cut = false;
        bool btags_n2_cut = false;
        bool btags_n3_cut = false;


	// Define cuts themselves
	if ((*el_pt).size()==1 && (*mu_pt).size()==1) emu_cut = true;
        if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;

        int jets_n = (*jet_pt).size();
        if (jets_n >=3) jets_n_cut = true;

        int btags_n = 0;
        for (int i=0; i<(*jet_pt).size(); i++) { if ((*jet_DL1r_77)[i]==1) btags_n++; }
        if (btags_n >=2) btags_n2_cut = true;
        if (btags_n >=3) btags_n3_cut = true;
	

	// TLorentzVectors for leptons and jets
	TLorentzVector el_lvec;
        TLorentzVector mu_lvec;
	std::vector<TLorentzVector> jets_lvec;
        el_lvec.SetPtEtaPhiE((*el_pt)[0]*0.001, (*el_eta)[0], (*el_phi)[0], (*el_e)[0]*0.001);
        mu_lvec.SetPtEtaPhiE((*mu_pt)[0]*0.001, (*mu_eta)[0], (*mu_phi)[0], (*mu_e)[0]*0.001);
        for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
          TLorentzVector lvec;
          lvec.SetPtEtaPhiE((*jet_pt)[jet_i]*0.001, (*jet_eta)[jet_i], (*jet_phi)[jet_i], (*jet_e)[jet_i]*0.001);
	  jets_lvec.push_back(lvec); }


	// 3b incl channel
	if (emu_cut*OS_cut*jets_n_cut*btags_n3_cut) {

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

	    // Fill discr vars hists
	    h_dR_jet_lep0[0]->Fill(dR_jet_lep0, weight);
	    h_dR_jet_lep1[0]->Fill(dR_jet_lep1, weight);
	    h_min_dR_jet_lep[0]->Fill(min_dR_jet_lep, weight);
	    h_m_jet_el[0]->Fill(m_jet_el, weight);
	    h_m_jet_mu[0]->Fill(m_jet_mu, weight);
	    h_min_dR_jet_bjet[0]->Fill(min_dR_jet_bjet, weight);
	    h_jet_eta[0]->Fill((*jet_eta)[jet_i], weight);
	    h_mva_score[0]->Fill(MVA_score[jet_i], weight);
	    if (jet_i==b_bdt_indeces[0] || jet_i==b_bdt_indeces[1]) {
	      h_dR_jet_lep0[1]->Fill(dR_jet_lep0, weight);
	      h_dR_jet_lep1[1]->Fill(dR_jet_lep1, weight);
	      h_min_dR_jet_lep[1]->Fill(min_dR_jet_lep, weight);
	      h_m_jet_el[1]->Fill(m_jet_el, weight);
	      h_m_jet_mu[1]->Fill(m_jet_mu, weight);
	      h_min_dR_jet_bjet[1]->Fill(min_dR_jet_bjet, weight);
	      h_jet_eta[1]->Fill((*jet_eta)[jet_i], weight);
	      h_mva_score[1]->Fill(MVA_score[jet_i], weight);
	    } else {
	      h_dR_jet_lep0[2]->Fill(dR_jet_lep0, weight);
	      h_dR_jet_lep1[2]->Fill(dR_jet_lep1, weight);
	      h_min_dR_jet_lep[2]->Fill(min_dR_jet_lep, weight);
	      h_m_jet_el[2]->Fill(m_jet_el, weight);
	      h_m_jet_mu[2]->Fill(m_jet_mu, weight);
	      h_min_dR_jet_bjet[2]->Fill(min_dR_jet_bjet, weight);
	      h_jet_eta[2]->Fill((*jet_eta)[jet_i], weight);
	      h_mva_score[2]->Fill(MVA_score[jet_i], weight);
	    }

	    // Fill pT hists
	    if (jet_i==b_bdt_indeces[0]) h_bTop_1_bdt->Fill((*jet_pt)[jet_i]*0.001, weight);
	    if (jet_i==b_bdt_indeces[1]) h_bTop_2_bdt->Fill((*jet_pt)[jet_i]*0.001, weight);
	    if (jet_i==b_bdt_indeces[2]) h_bAdd_1_bdt->Fill((*jet_pt)[jet_i]*0.001, weight);
	    if (btags_n>3 && jet_i==b_bdt_indeces[3]) h_bAdd_2_bdt->Fill((*jet_pt)[jet_i]*0.001, weight);

	  } // [jet_i] - loop over jets
	  
	} // 3b emu os	

      } // [entry] - loop over entries
      
      ntuple->Close();
      
    } // [job_number] - loop over files

  } // [dir_counter] - loop over directories with ntuples collections

  // Save hists to a file
  TString histfile_savename = std::string("results/hists_bdt_eff_study.root");
  TFile *histfile = new TFile(histfile_savename, "UPDATE");
  histfile->cd();
  std::vector<TString> disrc_vars_str = {"_data_all", "_data_ft", "_data_nft"};
  for (int i=0; i<3; i++) {
    h_dR_jet_lep0[i]->Write("3b_incl_dR_jet_lep0" + disrc_vars_str[i], TObject::kOverwrite);
    h_dR_jet_lep1[i]->Write("3b_incl_dR_jet_lep1" + disrc_vars_str[i], TObject::kOverwrite);
    h_min_dR_jet_lep[i]->Write("3b_incl_min_dR_jet_lep" + disrc_vars_str[i], TObject::kOverwrite);
    h_m_jet_el[i]->Write("3b_incl_m_jet_el" + disrc_vars_str[i], TObject::kOverwrite);
    h_m_jet_mu[i]->Write("3b_incl_m_jet_mu" + disrc_vars_str[i], TObject::kOverwrite);
    h_min_dR_jet_bjet[i]->Write("3b_incl_min_dR_jet_bjet" + disrc_vars_str[i], TObject::kOverwrite);
    h_jet_eta[i]->Write("3b_incl_jet_eta" + disrc_vars_str[i], TObject::kOverwrite);
    h_mva_score[i]->Write("3b_incl_mva_score" + disrc_vars_str[i], TObject::kOverwrite);
  }

  h_bTop_1_bdt->Write("bTop_1_data", TObject::kOverwrite);
  h_bTop_2_bdt->Write("bTop_2_data", TObject::kOverwrite);
  h_bAdd_1_bdt->Write("bAdd_1_data", TObject::kOverwrite);
  h_bAdd_2_bdt->Write("bAdd_2_data", TObject::kOverwrite);


  histfile->Close();

  return 0;
} // END of MAIN
