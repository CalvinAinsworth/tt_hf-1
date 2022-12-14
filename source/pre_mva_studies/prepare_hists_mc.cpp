#include "../common_functions.h"


// // ///
// MAIN
// ///
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


  // Create directory for results
  gSystem->Exec("mkdir results");


  // Declare histograms
  #include "include/declare_hists_data_mc.h"
  #include "include/declare_hists_sig_bkg.h"

  
  // Declare TFile, TTree, TBranches and variables for MVA
  TFile *MVA_tfile;
  if (process == "tt") {
    TString mva_fname = "results/tt_c_MVA_input_reco_" + generator + lep_pt_cut_suffix + ".root";
    MVA_tfile = new TFile(mva_fname, "RECREATE"); }
  TTree *MVA_sig_tree = new TTree("Signal", "inputS");
  TTree *MVA_bkg_tree = new TTree("Background", "inputB");
  #include "include/set_mva_ntuple_branches.h"
 

  
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
	    std::cout<<subpiece<<std::endl;
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
      
      // We work with MC only
      if (is_data == true) continue;
      if (campaign=="mc16a" && is_mc16a!=true) continue;
      if (campaign=="mc16d" && is_mc16d!=true) continue;
      if (campaign=="mc16e" && is_mc16e!=true) continue;
      
      // Select only dids of our interest
      bool correct_did = false;
      //std::cout << job_DID << std::endl;
      #include "include/did_selection.h"
      if (correct_did==false) continue;
      std::cout << "\nWorking with:\n" << ntuple_name << std::endl; 


      // Open the ntuple
      TFile *ntuple = new TFile (ntuple_name);
      TTree *tree_nominal = (TTree*)ntuple->Get("nominal_Loose");

      // Set all the needed branches
      #include "include/branches.h"
      
      
      // Loop over entries
      Int_t nEntries = tree_nominal->GetEntries();
      std::cout << "\tEntries = " << nEntries << std::endl;
      for (int entry=0; entry<nEntries; entry++)  {
	
	// Show events counter
	if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
	tree_nominal->GetEntry(entry);

	
	// Compute weights
	double weight = 1;
        #include "include/compute_weight.h"
	weight = w_mc * w_pu * w_leptonSF * w_DL1r_77 * w_jvt * weight_lumi;

	  
	// Declare cuts names
	bool emu_cut = false;
	bool OS_cut = false;
	bool lep_pt_cut = false;
	bool jets_n_cut = false;
	bool btags_n2_cut = false;
	bool btags_n3_cut = false;
	bool topHFFF_cut = false;
	
	
	// Define cuts themselves
	if ((*el_pt).size()==1 && (*mu_pt).size()==1) { 
	  emu_cut = true;
	} else {
	  continue;
	}
	if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	if ( ((*el_pt)[0]*0.001>28 && (*mu_pt)[0]*0.001>28) || lep_pt_cut_suffix=="")  lep_pt_cut = true; // 28 GeV
	
	int jets_n = (*jet_pt).size();
	if (jets_n >=3) jets_n_cut = true;
	
	int btags_n = 0;
	for (int i=0; i<(*jet_pt).size(); i++) { if ((*jet_DL1r_77)[i]==1) btags_n++; }
	if (btags_n >=2) btags_n2_cut = true;
	if (btags_n >=3) btags_n3_cut = true;
	
	if (process=="tt") {
	  // ttbb
	  if (topHFFF==1 && (job_DID=="411076" || job_DID=="411085" || job_DID=="412069" || job_DID=="411332") ) topHFFF_cut = true;
	  // ttb
	  if (topHFFF==2 && (job_DID=="411077" || job_DID=="411086" || job_DID=="412070" || job_DID=="411333") ) topHFFF_cut = true;
	  // ttc
	  if (topHFFF==3 && (job_DID=="411078" || job_DID=="411087" || job_DID=="412071" || job_DID=="411334") ) topHFFF_cut = true;
	  // inclusive
	  if (topHFFF==0 && (job_DID == "410472" || job_DID=="410558" || job_DID=="410465" || job_DID=="411234") ) topHFFF_cut = true; //had 410470
	  // 3mtop
	  if (job_DID=="410482") topHFFF_cut = true;
	} else { topHFFF_cut = true; }
	
	
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
	
	// TLorentzVectors for systems with 3 and 4 jets
	TLorentzVector four_btags_lvecs; // excl
	TLorentzVector three_btags_lvecs; // incl
	four_btags_lvecs.SetPtEtaPhiE(0, 0, 0, 0);
	three_btags_lvecs.SetPtEtaPhiE(0, 0, 0, 0);
	int n_btags_tmp = 0;
	
	
	
	// ///
	// 3b (tag) incl, emu, OS //I removed lep_pt_cut
	// ///
	if (emu_cut*OS_cut*btags_n2_cut*topHFFF_cut*jets_n_cut*lep_pt_cut == true) {
	  
	  // ///
	  // jets_n - data/mc
	  h_jets_n->Fill((*jet_pt).size(), weight);
	  h_btags_n->Fill(btags_n, weight);
	  
	  
	  
	  // ///
	  // Variables for min dR (sub)leaning btag and leptons, and dR between btag and 1st/2nd leptons - sig/bkg and data/mc
	  double min_dR0_top = 999999.; // leading lepton
	  double min_dR1_top = 999999.; // subleading lepton
	  double min_dR0_not_top = 999999.; // leading lepton
	  double min_dR1_not_top = 999999.; // subleading lepton
	  
	  
	  
	  // Loop over jets
	  for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {
	    
	    
	    // ///
	    // jets parameters - data/mc
	    h_all_jets_pt->Fill((*jet_pt)[jet_i]*0.001, weight);
	    h_all_jets_eta->Fill((*jet_eta)[jet_i], weight);
	    h_all_jets_phi->Fill((*jet_phi)[jet_i], weight);
	    
	    
	    ///
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
	    // jets parameters - sig/bkg
	    if (process=="tt") { 
	      if ( (*topHadronOriginFlag)[jet_i]==4 ) {
		h_all_jets_from_top_pt->Fill((*jet_pt)[jet_i]*0.001, weight);
		h_all_jets_from_top_eta->Fill((*jet_eta)[jet_i], weight);
		h_all_jets_from_top_phi->Fill((*jet_phi)[jet_i], weight); }
	      else {
		h_all_jets_not_from_top_pt->Fill((*jet_pt)[jet_i]*0.001, weight);
		h_all_jets_not_from_top_eta->Fill((*jet_eta)[jet_i], weight);
		h_all_jets_not_from_top_phi->Fill((*jet_phi)[jet_i], weight); }
	    } // if "tt"
	    
	    
	    // ///
	    // min dR (sub)leaning btag and leptons, and dR between btag and 1st/2nd leptons - sig/bkg and data/mc
	    
	    if ((*jet_DL1r_77)[jet_i]==1) {
	      
	      double dR0 = 0;
	      double dR1 = 0;
	      
	      if ((*mu_pt)[0]>(*el_pt)[0]) {
		dR0 = mu_lvec.DeltaR(jets_lvec[jet_i]);
		dR1 = el_lvec.DeltaR(jets_lvec[jet_i]); }
	      else {
		dR0 = mu_lvec.DeltaR(jets_lvec[jet_i]);
		dR1 = mu_lvec.DeltaR(jets_lvec[jet_i]); }
	      
	      if (process=="tt") {
		if ((*topHadronOriginFlag)[jet_i]==4) {
		  min_dR0_top = std::min(min_dR0_top, dR0);
		  min_dR1_top = std::min(min_dR1_top, dR1);}
		else {
		  min_dR0_not_top = std::min(min_dR0_not_top, dR0);
		  min_dR1_not_top = std::min(min_dR1_not_top, dR1); }
	      } // if "tt" 
	    } // [if] DL1r tag

	    h_min_dR_lep0_btags_from_top->Fill(min_dR0_top, weight);
	    h_min_dR_lep0_btags_not_from_top->Fill(min_dR0_not_top, weight);
	    h_min_dR_lep1_btags_from_top->Fill(min_dR1_top, weight);
	    h_min_dR_lep1_btags_not_from_top->Fill(min_dR1_not_top, weight);
	    
	    
	  
	    // ///
	    // min dR between defferent objets for MVA - sig/bkg discrimination
	    
	    double min_dR_btag_from_top_to_btag = 999999.;
	    double min_dR_btag_not_from_top_to_btag = 999999.;
	    double min_dR_not_btag_to_btag = 999999.;
	    double min_dR_btag_from_top_to_jet = 999999.;
	    double min_dR_btag_not_from_top_to_jet = 999999.;
	    double min_dR_not_btag_to_jet = 999999.;
	    double min_dR_btag_from_top_to_lep = 999999.;
	    double min_dR_btag_not_from_top_to_lep = 999999.;
	    double min_dR_not_btag_to_lep = 999999.;
	    
	    if (process=="tt") {
	      
	      for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
		if (jet_i==jet_j) continue;
		
		// dR_min
		if ((*jet_DL1r_77)[jet_i]==1 && (*topHadronOriginFlag)[jet_i]==4 && (*jet_DL1r_77)[jet_j]==1) {
		  double dR_btag_from_top_to_btag = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  if (dR_btag_from_top_to_btag < min_dR_btag_from_top_to_btag) min_dR_btag_from_top_to_btag = dR_btag_from_top_to_btag; }
		
		if ((*jet_DL1r_77)[jet_i]==1 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_DL1r_77)[jet_j]==1) {
		  double dR_btag_not_from_top_to_btag = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  if (dR_btag_not_from_top_to_btag < min_dR_btag_not_from_top_to_btag) min_dR_btag_not_from_top_to_btag = dR_btag_not_from_top_to_btag; }
		
		if ((*jet_DL1r_77)[jet_i]!=1 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_DL1r_77)[jet_j]==1) {
		  double dR_not_btag_to_btag = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  if (dR_not_btag_to_btag < min_dR_not_btag_to_btag) min_dR_not_btag_to_btag = dR_not_btag_to_btag; }
		
		if ((*jet_DL1r_77)[jet_i]==1 && (*topHadronOriginFlag)[jet_i]==4 && (*jet_DL1r_77)[jet_j]!=1) {
		  double dR_btag_from_top_to_jet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  if (dR_btag_from_top_to_jet < min_dR_btag_from_top_to_jet) min_dR_btag_from_top_to_jet = dR_btag_from_top_to_jet; }
		
		if ((*jet_DL1r_77)[jet_i]==1 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_DL1r_77)[jet_j]!=1) {
		  double dR_btag_not_from_top_to_jet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  if (dR_btag_not_from_top_to_jet < min_dR_btag_not_from_top_to_jet) min_dR_btag_not_from_top_to_jet = dR_btag_not_from_top_to_jet; }
		
		if ((*jet_DL1r_77)[jet_i]!=1 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_DL1r_77)[jet_j]!=1) {
		  double dR_not_btag_to_jet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  if (dR_not_btag_to_jet < min_dR_not_btag_to_jet) min_dR_not_btag_to_jet = dR_not_btag_to_jet; }
	      } // loop over [jet_j]
	      
	      if ((*jet_DL1r_77)[jet_i]==1 && (*topHadronOriginFlag)[jet_i]==4) {
		double dR_btag_from_top_to_el = jets_lvec[jet_i].DeltaR(el_lvec);
		double dR_btag_from_top_to_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		double dR_btag_from_top_to_lep = std::min(dR_btag_from_top_to_el, dR_btag_from_top_to_mu);
		if (dR_btag_from_top_to_lep < min_dR_btag_from_top_to_lep) min_dR_btag_from_top_to_lep = dR_btag_from_top_to_lep; }
	      
	      if ((*jet_DL1r_77)[jet_i]==1 && (*topHadronOriginFlag)[jet_i]!=4) {
		double dR_btag_not_from_top_to_el = jets_lvec[jet_i].DeltaR(el_lvec);
		double dR_btag_not_from_top_to_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		double dR_btag_not_from_top_to_lep = std::min(dR_btag_not_from_top_to_el, dR_btag_not_from_top_to_mu);
		if (dR_btag_not_from_top_to_lep < min_dR_btag_not_from_top_to_lep) min_dR_btag_not_from_top_to_lep = dR_btag_not_from_top_to_lep; }
	      
	      if ((*jet_DL1r_77)[jet_i]!=1 && (*topHadronOriginFlag)[jet_i]!=4) {
		double dR_not_btag_to_el = jets_lvec[jet_i].DeltaR(el_lvec);
		double dR_not_btag_to_mu = jets_lvec[jet_i].DeltaR(mu_lvec);;
		double dR_not_btag_to_lep = std::min(dR_not_btag_to_el, dR_not_btag_to_mu);
		if (dR_not_btag_to_lep < min_dR_not_btag_to_lep) min_dR_not_btag_to_lep = dR_not_btag_to_lep; }
	      
	      
	      if (min_dR_btag_from_top_to_btag!=999999) h_min_dR_btag_from_top_to_btag->Fill(min_dR_btag_from_top_to_btag, weight);
	      if (min_dR_btag_not_from_top_to_btag!=999999) h_min_dR_btag_not_from_top_to_btag->Fill(min_dR_btag_not_from_top_to_btag, weight);
	      if (min_dR_not_btag_to_btag!=999999) h_min_dR_not_btag_to_btag->Fill(min_dR_not_btag_to_btag, weight);
	      if (min_dR_btag_from_top_to_jet!=999999) h_min_dR_btag_from_top_to_jet->Fill(min_dR_btag_from_top_to_jet, weight);
	      if (min_dR_btag_not_from_top_to_jet!=999999) h_min_dR_btag_not_from_top_to_jet->Fill(min_dR_btag_not_from_top_to_jet, weight);
	      if (min_dR_not_btag_to_jet!=999999) h_min_dR_not_btag_to_jet->Fill(min_dR_not_btag_to_jet, weight);
	      if (min_dR_btag_from_top_to_lep!=999999) h_min_dR_btag_from_top_to_lep->Fill(min_dR_btag_from_top_to_lep, weight);
	      if (min_dR_btag_not_from_top_to_lep!=999999) h_min_dR_btag_not_from_top_to_lep->Fill(min_dR_btag_not_from_top_to_lep, weight);
	      if (min_dR_not_btag_to_lep!=999999) h_min_dR_not_btag_to_lep->Fill(min_dR_not_btag_to_lep, weight);
	    } // if "tt"
	    
	    
	    
	    // ///
	    // M {lep, btag/jet} - sig/bkg discrimination
	    
	    double min_m_lep_btag_from_top = 999999;
	    double max_m_lep_btag_from_top = 0;
	    double min_m_lep_btag_not_from_top = 999999;
	    double max_m_lep_btag_not_from_top = 0;
	    double min_m_lep_other_jet = 999999;
	    double max_m_lep_other_jet = 0;
	    
	    if (process=="tt") {
	      
	      // btag and the closest lepton
	      if ((*jet_DL1r_77)[jet_i]==1) {
		if ((*topHadronOriginFlag)[jet_i]==4) {
		  double dr_j_el = jets_lvec[jet_i].DeltaR(el_lvec);
		  double dr_j_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		  double m_j_lep = 0;
		  if (dr_j_el <= dr_j_mu) { m_j_lep = (jets_lvec[jet_i] + el_lvec).M(); }
		  else { m_j_lep = (jets_lvec[jet_i] + mu_lvec).M(); }
		  if (m_j_lep != 0) h_m_lep_btag_from_top_min_dR->Fill(m_j_lep, weight); }
		else {
		  double dr_j_el = jets_lvec[jet_i].DeltaR(el_lvec);
		  double dr_j_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		  double m_j_lep = 0;
		  if (dr_j_el <= dr_j_mu) { m_j_lep = (jets_lvec[jet_i] + el_lvec).M(); }
		  else { m_j_lep = (jets_lvec[jet_i] + mu_lvec).M(); }
		  if (m_j_lep != 0) h_m_lep_btag_not_from_top_min_dR->Fill(m_j_lep, weight); } } // [else], [if jet_i tagged] closing
	      
	      // btag and leptons - min and max M
	      if ((*jet_DL1r_77)[jet_i]==1) {
		if ((*topHadronOriginFlag)[jet_i]==4) {
		  double min_m_lep_btag_from_top_tmp = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  double max_m_lep_btag_from_top_tmp = std::max( (jets_lvec[jet_i] + mu_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  min_m_lep_btag_from_top = std::min(min_m_lep_btag_from_top_tmp, min_m_lep_btag_from_top);
		  max_m_lep_btag_from_top = std::max(max_m_lep_btag_from_top_tmp, max_m_lep_btag_from_top_tmp); }
		else {
		  double min_m_lep_btag_not_from_top_tmp = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  double max_m_lep_btag_not_from_top_tmp = std::max( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  min_m_lep_btag_not_from_top = std::min(min_m_lep_btag_not_from_top_tmp, min_m_lep_btag_not_from_top);
		  max_m_lep_btag_not_from_top = std::max(max_m_lep_btag_not_from_top_tmp, max_m_lep_btag_not_from_top); } }
	      
	      // other than btags and leptons - min and max M
	      else {
		double min_m_lep_other_jet_tmp = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		double max_m_lep_other_jet_tmp = std::max( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		min_m_lep_other_jet = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		max_m_lep_other_jet = std::max( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() ); }
	      
	      // Fill the min/max M hists
	      if (min_m_lep_btag_from_top!=999999) h_min_m_lep_btag_from_top->Fill(min_m_lep_btag_from_top, weight);
	      if (max_m_lep_btag_from_top!=0) h_max_m_lep_btag_from_top->Fill(max_m_lep_btag_from_top, weight);
	      if (min_m_lep_btag_not_from_top!=999999) h_min_m_lep_btag_not_from_top->Fill(min_m_lep_btag_not_from_top, weight);
	      if (max_m_lep_btag_not_from_top!=0) h_max_m_lep_btag_not_from_top->Fill(max_m_lep_btag_not_from_top, weight);
	      if (min_m_lep_other_jet!=999999) h_min_m_lep_other_jet->Fill(min_m_lep_other_jet, weight);
	      if (max_m_lep_other_jet!=0) h_max_m_lep_other_jet->Fill(max_m_lep_other_jet, weight);
	    } // if "tt"
	    
	    
	    
	    // ///
	    // M of btags, other than btags and combinations: {btag, btag} {btag, other than} {other than, other than} - data/mc
	    
	    float m_jet = jets_lvec[jet_i].M();
	    if ( (*jet_DL1r_77)[jet_i]==1 ) { h_m_btag->Fill(m_jet, weight); }
	    else { h_m_notbtag->Fill(m_jet, weight); }
	    
	    for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
	      
	      if (jet_i==jet_j) continue;
	      
	      float m_jet_jet = (jets_lvec[jet_i] + jets_lvec[jet_j]).M();
	      if ( (*jet_DL1r_77)[jet_i]==1 && (*jet_DL1r_77)[jet_j]==1 ) h_m_btag_btag->Fill(m_jet_jet, weight);
	      if ( (*jet_DL1r_77)[jet_i]!=1 ^  (*jet_DL1r_77)[jet_j]!=1 ) h_m_btag_notbtag->Fill(m_jet_jet, weight);
	      if ( (*jet_DL1r_77)[jet_i]!=1 && (*jet_DL1r_77)[jet_j]!=1 ) h_m_notbtag_notbtag->Fill(m_jet_jet, weight);
	      
	    } // [jet_j] - loop over jets
	    
	    
	    
	    // ///
	    // M of btags and their combinations - sig/bkg
	    
	    float jet_m = jets_lvec[jet_i].M();
	    
	    float m_min_btag_top_btag_top = 999999.;
	    float m_max_btag_top_btag_top = 0.;
	    float m_min_btag_top_btag_other = 999999.;
	    float m_max_btag_top_btag_other = 0.;
	    float m_min_btag_other_btag_other = 999999.;
	    float m_max_btag_other_btag_other = 0.;
	    
	    float m_min_jet_jet = 999999.;
	    float m_max_jet_jet = 0.;
	    
	    if (process=="tt") {
	      
	      if ( (*jet_DL1r_77)[jet_i]!=1) {
		n_btags_tmp++ ;
		if ( (*topHadronOriginFlag)[jet_i]==4 ) { h_m_btag_top->Fill(m_jet, weight); }
		else { h_m_btag_other->Fill(m_jet, weight); } }
	      
	      for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
		
		if (jet_i==jet_j) continue;
		
		float m_jet_jet = (jets_lvec[jet_i] + jets_lvec[jet_j]).M();
		m_min_jet_jet = std::min(m_min_jet_jet, m_jet_jet);
		m_max_jet_jet = std::max(m_max_jet_jet, m_jet_jet);
		
		if ( (*jet_DL1r_77)[jet_j]!=1 ) continue;
		
		if ( (*topHadronOriginFlag)[jet_i]==4 && (*topHadronOriginFlag)[jet_j]==4) {
		  h_m_btag_top_btag_top->Fill(m_jet_jet, weight);
		  m_min_btag_top_btag_top = std::min(m_jet_jet, m_min_btag_top_btag_top);
		  m_max_btag_top_btag_top = std::max(m_jet_jet, m_max_btag_top_btag_top); }
		if ( (*topHadronOriginFlag)[jet_i]==4 ^  (*topHadronOriginFlag)[jet_j]==4) {
		  h_m_btag_top_btag_other->Fill(m_jet_jet, weight);
		  m_min_btag_top_btag_other = std::min(m_jet_jet, m_min_btag_top_btag_other);
		  m_max_btag_top_btag_other = std::max(m_jet_jet, m_max_btag_top_btag_other); }
		if ( (*topHadronOriginFlag)[jet_i]!=4 && (*topHadronOriginFlag)[jet_j]!=4) {
		  h_m_btag_other_btag_other->Fill(m_jet_jet, weight);
		  m_min_btag_other_btag_other = std::min(m_jet_jet, m_min_btag_other_btag_other);
		  m_max_btag_other_btag_other = std::max(m_jet_jet, m_max_btag_other_btag_other); }
		
	      } // [jet_j] - loop over jets
	      
	      three_btags_lvecs += jets_lvec[jet_i];
	      if (n_btags_tmp<=3) three_btags_lvecs += jets_lvec[jet_i];
	      
	      if (m_min_btag_top_btag_top!=999999.) h_m_min_btag_top_btag_top->Fill(m_min_btag_top_btag_top, weight);
	      if (m_max_btag_top_btag_top!=0)       h_m_max_btag_top_btag_top->Fill(m_max_btag_top_btag_top, weight);
	      if (m_min_btag_top_btag_other!=999999.) h_m_min_btag_top_btag_other->Fill(m_min_btag_top_btag_other, weight);
	      if (m_max_btag_top_btag_other!=0)       h_m_max_btag_top_btag_other->Fill(m_max_btag_top_btag_other, weight);
	      if (m_min_btag_other_btag_other!=999999.) h_m_min_btag_other_btag_other->Fill(m_min_btag_other_btag_other, weight);
	      if (m_max_btag_other_btag_other!=0)       h_m_max_btag_other_btag_other->Fill(m_max_btag_other_btag_other, weight);	    
	    } // if "tt"
	    
	    
	    
	    // ///
	    // Compute values for MVA ntuple and fill some hists for data/mc
	    // ///

	    // ///
	    // dR between jet and the 1st/2nd
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
	    // M of btag and el/mu: Sig/Bkg
	    if ( (*jet_DL1r_77)[jet_i]==1 ) {
	      if ( (*topHadronOriginFlag)[jet_i]==4 ) {
		h_m_btag_el_from_top->Fill( (jets_lvec[jet_i] + el_lvec).M() , weight);
		h_m_btag_mu_from_top->Fill( (jets_lvec[jet_i] + mu_lvec).M() , weight);
	      } else {
		h_m_btag_el_not_from_top->Fill( (jets_lvec[jet_i] + el_lvec).M() , weight);
		h_m_btag_mu_not_from_top->Fill( (jets_lvec[jet_i] + mu_lvec).M() , weight);
	      }
	    }
	    
	    
	    // ///
	    // M min and max of btag and lepton
	    double m_jet_lep_max = std::max((jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M());
	    double m_jet_lep_min = std::min((jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M());
	    if ( (*jet_DL1r_77)[jet_i]==1 ) {
	      h_m_btag_lep_max->Fill(m_jet_lep_max, weight);
	      h_m_btag_lep_min->Fill(m_jet_lep_min, weight); }
	    
	    
	    // ///
	    // dR between the jet [jet_i] and the closes btag
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
	    // Populate the MVA ntuple with values
	    if (process=="tt") { 
	      #include "include/populate_mva_ntuple.h"
	    }
	    
	  } // [jet_i] - loop over jets (populating MVA input ntuple)
	  
	  
	  
	  float m_three_tags = three_btags_lvecs.M();
	  float m_four_tags = four_btags_lvecs.M();
	  if (n_btags_tmp==3) h_m_three_tags->Fill(m_three_tags, weight);
	  if (n_btags_tmp>=4) h_m_four_tags->Fill(m_four_tags, weight);
	  
	} // 3b (tags), emu, OS cuts
	
	
      } // [entry] - loop over entries
      
      ntuple->Close();
      
    } // loop over ntuple names from the file
    
  } // if (ntuples_list_file.is_open) - checking the file
  

  
  // Save trees to the MVA ntuple
  if (process=="tt") {
    MVA_sig_tree->SetDirectory(MVA_tfile);
    MVA_bkg_tree->SetDirectory(MVA_tfile);
    MVA_tfile->cd();
    MVA_sig_tree->Write("Signal", TTree::kOverwrite);
    MVA_bkg_tree->Write("Background", TTree::kOverwrite);
    MVA_tfile->Close();
  }
  
  
  // Save hists
  TString savename = std::string("results/hists_") + process + std::string("_reco_" + generator + lep_pt_cut_suffix + ".root");
  TFile *hists_file = new TFile(savename, "RECREATE");
  hists_file->cd();
  #include "include/write_hists_data_mc.h"
  if (process=="tt") {
    #include "include/write_hists_sig_bkg.h"
  }
  hists_file->Close();
	  
  return 0;
} // END OF MAIN
