#include "../common_functions.h"


// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Get config info about MVA setup
  bool mc16a_only_test = mc16a_only_choise();


  // Create directory for results
  gSystem->Exec("mkdir results");
  

  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/atlas/atlascerngroupdisk/phys-top/ttjets/v4/data/";
  std::vector<TString> dir_paths = get_list_of_files(path_to_ntuples);


  // Declare histograms
  #include "include/declare_hists_data_mc.h"


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
      #include "include/branches.h"
      std::cout << "\nSome branches don't exists for data - that's normal and won't affect performance" << std::endl;

      
      // Loop over entries
      Int_t nEntries = tree_nominal->GetEntries();
      std::cout << "\tEntries = " << nEntries << std::endl;
      for (int entry=0; entry<nEntries; entry++)  {

	// Show events counter
	if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
	tree_nominal->GetEntry(entry);

	
	// There are no weights in data
	// But common with prepare_hists_mc.cpp pieces of code use "hist->Fill(vel, weight);", thus:
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

	
	// ///
	// 2b (tags) incl, emu, OS
	// ///
	if (emu_cut*OS_cut*btags_n2_cut*jets_n_cut == true) {
	  
	  // ///
	  // jets_n - data/mc
	  h_jets_n->Fill((*jet_pt).size(), weight);
	  h_btags_n->Fill(btags_n, weight);


	  
	  // Loop over jets
	  for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {

	    
	    // ///
	    // jets parameters - data/mc
	    h_all_jets_pt->Fill((*jet_pt)[jet_i]*0.001, weight);
            h_all_jets_eta->Fill((*jet_eta)[jet_i], weight);
            h_all_jets_phi->Fill((*jet_phi)[jet_i], weight);
	    


	    // ///
	    // dR between btag and the 1st/2nd lepton
	    double dR0 = 0;
	    double dR1 = 0;
	    
	    if ((*mu_pt)[0]>(*el_pt)[0]) {
	      dR0 = mu_lvec.DeltaR(jets_lvec[jet_i]);
	      dR1 = el_lvec.DeltaR(jets_lvec[jet_i]); }
	    else {
	      dR0 = el_lvec.DeltaR(jets_lvec[jet_i]);
	      dR1 = mu_lvec.DeltaR(jets_lvec[jet_i]); }

	    if ((*jet_DL1r_77)[jet_i]==1) {
	      h_dR_btag_lep0->Fill(dR0, weight);
	      h_dR_btag_lep1->Fill(dR1, weight);
	      h_min_dR_btag_lep->Fill( std::min(dR0, dR1), weight ); }


	    
	    // ///
	    // M of btag and the closest lepton
	    double m_jet_lep = 0;
	    double dr_jet_el = jets_lvec[jet_i].DeltaR(el_lvec);
	    double dr_jet_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
	    if (dr_jet_el <= dr_jet_mu) { m_jet_lep = (jets_lvec[jet_i] + el_lvec).M(); }
	    else { m_jet_lep = (jets_lvec[jet_i] + mu_lvec).M(); }
	    if (m_jet_lep!=0 && (*jet_DL1r_77)[jet_i]==1) h_m_btag_lep_min_dR->Fill(m_jet_lep, weight);
	    

	    
	    // ///
	    // M of btag and el/mu 
	    if ( (*jet_DL1r_77)[jet_i]==1 ) {
	      h_m_btag_el->Fill( (jets_lvec[jet_i] + el_lvec).M() , weight);
	      h_m_btag_mu->Fill( (jets_lvec[jet_i] + mu_lvec).M() , weight); }

	    
	    
	    // ///
	    // M min and max of btag and lepton
	    double m_jet_lep_max = std::max((jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M());
	    double m_jet_lep_min = std::min((jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M());
	    if ( (*jet_DL1r_77)[jet_i]==1 ) {
	      h_m_btag_lep_max->Fill(m_jet_lep_max, weight);
	      h_m_btag_lep_min->Fill(m_jet_lep_min, weight); }
	    

	    
	    // ///
	    // dR between the jet [jet_i] and the closest btag
	    double min_dR_jet_bjet = 999999.;
	    for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
	      if (jet_i==jet_j) continue;
	      if ( (*jet_DL1r_77)[jet_j]==1) {
		double dR_jet_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		min_dR_jet_bjet = std::min(min_dR_jet_bjet, dR_jet_bjet);
	      } // if - DL1r tagged jet_j
	    } // [jet_j] - loop over jets 
	    h_min_dR_jet_btag->Fill(min_dR_jet_bjet, weight);



	    // ///
	    // M of btag and not btag
	    float m_jet = jets_lvec[jet_i].M();
	    if ( (*jet_DL1r_77)[jet_i]==1 ) { h_m_btag->Fill(m_jet, weight); }
	    else { h_m_notbtag->Fill(m_jet, weight); }



	    // ///
	    // M of {btag, btag}, {btag, not-btag}, {not-btag, not-btag} pairs
	    for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
	      if (jet_i==jet_j) continue;
	      float m_jet_jet = (jets_lvec[jet_i] + jets_lvec[jet_j]).M();
	      if ( (*jet_DL1r_77)[jet_i]==1 && (*jet_DL1r_77)[jet_j]==1 ) h_m_btag_btag->Fill(m_jet_jet, weight);
	      if ( (*jet_DL1r_77)[jet_i]!=1 ^  (*jet_DL1r_77)[jet_j]!=1 ) h_m_btag_notbtag->Fill(m_jet_jet, weight);
	      if ( (*jet_DL1r_77)[jet_i]!=1 && (*jet_DL1r_77)[jet_j]!=1 ) h_m_notbtag_notbtag->Fill(m_jet_jet, weight);
	    } // [jet_j] - loop over jets
	    

	  } // [jet_i] - loop over jets
	  
	} // [if] - 2b-emu-OS
	
      } // [entry] - loop over entries

      ntuple->Close();
      
    } // [job_number]
    
  } // [dir_counter]


  
  // Save hists
  TFile *hists_file = new TFile("results/hists_data_test.root", "RECREATE");
  hists_file->cd();
  #include "include/write_hists_data_mc.h"
  hists_file->Close();

  return 0;
} // END OF MAIN
