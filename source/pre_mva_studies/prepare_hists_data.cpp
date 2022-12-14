#include "../common_functions.h"


// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
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
  

  // Create directory for results
  gSystem->Exec("mkdir results");
  

  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/user/c/cainswor/File/tt_c_dilpeton_samples/data/";
  std::vector<TString> dir_paths = get_list_of_files(path_to_ntuples);
  

  // Declare histograms
  #include "include/declare_hists_data_mc.h"



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
      bool is_data15 = false;
      bool is_data16 = false;
      bool is_data17 = false;
      bool is_data18 = false;
      bool is_mc16a = false;
      bool is_mc16d = false;
      bool is_mc16e = false;
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
            if (subpiece == "mc16a") is_mc16a = true;
            if (subpiece == "mc16d") is_mc16d = true;
            if (subpiece == "mc16e") is_mc16e = true;
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

      // Work with data only here
      if (is_data != true) continue;
      if (campaign=="mc16a" && (is_data17==true || is_data18==true)) continue;
      if (campaign=="mc16d" && is_data17!=true) continue;
      if (campaign=="mc16e" && is_data18!=true) continue;
      std::cout << "\nWorking with:\n" << ntuple_name << std::endl;
      

      // Open an ntuples
      TFile *ntuple = new TFile (ntuple_name);
      TTree *tree_nominal = (TTree*)ntuple->Get("nominal_Loose");
      
      
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
	bool lep_pt_cut = false;
	bool jets_n_cut = false;
	bool btags_n2_cut = false;
	bool btags_n3_cut = false;


	// Define cuts themselves
	if ((*el_pt).size()==1 && (*mu_pt).size()==1){
	  emu_cut = true;
	}else{
	  continue;
	}
	if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	if ( ((*el_pt)[0]*0.001>28 && (*mu_pt)[0]*0.001>28) || lep_pt_cut_suffix=="")  lep_pt_cut = true;

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
	// 3b (tags) incl, emu, OS
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
	    
	    // Discriminatory variables used in NN //
	    
	    float dR_jel = jets_lvec[jet_i].DeltaR(el_lvec);
	    float dR_jmu = jets_lvec[jet_i].DeltaR(mu_lvec);
	    float dR_jl = std::min(dR_jel, dR_jmu);
	    float dR_jj = 999999.;
	    float dR_jb = 999999.; 
	    float dR_jj_tmp = jets_lvec[jet_i].DeltaR(jets_lvec[jet_i]);
	    dR_jj = std::min(dR_jj, dR_jj_tmp);
	    float m_jel = ( jets_lvec[jet_i] + el_lvec).M();
	    float m_jmu = ( jets_lvec[jet_i] + mu_lvec).M();
	    float m_jl_min = std::min(m_jel, m_jmu);
	    float m_jl_max = std::max(m_jel, m_jmu);
	    float m_jl_closest;
	    if (dR_jel >= dR_jmu) { m_jl_closest = m_jel; }
	    else { m_jl_closest = m_jmu; }

	    h_dR_jel->Fill(dR_jel, weight);
	    h_dR_jmu->Fill(dR_jmu, weight);
	    h_dR_jl->Fill(dR_jl, weight);
	    h_dR_jj->Fill(dR_jj, weight);
	    h_m_jel->Fill(m_jel, weight);
	    h_m_jmu->Fill(m_jmu, weight);
	    h_m_jl_min->Fill(m_jl_min, weight);
	    h_m_jl_closest->Fill(m_jl_closest, weight);

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
      
    } // loop over line of the ntuples tist (txt)
  } // "if (ntuples_list_file.is_open())"


      
  // Save hists
  TString savename = "results/hists_data_" + lep_pt_cut_suffix + ".root";
  TFile *hists_file = new TFile(savename, "RECREATE");
  hists_file->cd();
  #include "include/write_hists_data_mc.h"
  hists_file->Close();

  return 0;
} // END OF MAIN
