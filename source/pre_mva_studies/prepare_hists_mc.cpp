#include "../common_functions.h"


// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Check for the request: sig (tt) or bkg (others)?
  if (argc == 1) {
    std::cout << "\nError: I don't know what to do =(\n" << std::endl;
    std::cout << "Aks for a process:" << std::endl;
    std::cout << "./run/prepare_hists_mc PROCESS\n" << std::endl;
    std::cout << "PROCESS = tt  or  singletop  or  ttV  or  ttH  or  diboson  or  z_jets  or  other\n" << std::endl;
    return 0;
  }
  if ( argc>1 && (std::string(argv[1])=="tt" || std::string(argv[1])=="singletop" || std::string(argv[1])=="ttV" || std::string(argv[1])=="ttH" || std::string(argv[1])=="diboson" || std::string(argv[1])=="z_jets" || std::string(argv[1])=="other") ) {
    std::cout << "Working with " << std::string(argv[1]) << "\n\n" <<std::endl; 
  } else {
    std::cout << "Error: Unknown argument " << std::string(argv[1]) << std::endl;
    std::cout << "Try \"tt\" or \"singletop\" or \"ttV\" or \"ttH\" or \"diboson\" or \"z_jets\" or \"other\" (without quotes)\n" << std::endl;
    return 0;
  }
  
  
  // Create directory for results
  gSystem->Exec("mkdir results");


  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/atlas/atlascerngroupdisk/phys-top/ttjets/v4/";
  std::vector<TString> dir_paths = get_list_of_files(path_to_ntuples);


  // Declare histograms
  #include "declare_hists_data_mc.h"
  #include "declare_hists_sig_bkg.h"

  
  // Declare TFile, TTree, TBranches and variables for MVA
  //TString tmva_file_name = std::string("results/") + std::string(argv[1]) + std::string("_MVA_input.root");
  //TFile *MVA_tfile =  new TFile(tmva_file_name, "RECREATE");
  TFile *MVA_tfile;
  if (std::string(argv[1]) == "tt") MVA_tfile = new TFile("results/tt_hv_MVA_input.root", "RECREATE");
  TTree *MVA_sig_tree = new TTree("Signal", "inputS");
  TTree *MVA_bkg_tree = new TTree("Background", "inputB");
  #include "set_mva_ntuple_branches.h"


  
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

    
    // Only nominal trees
    dir_paths[dir_counter] += "nominal/";


    // Testing option: run over mc16a only
    //if (is_mc16a != true) continue;


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
      // (2) the process of our interest
      if (campaign_info[1]!="s3126") continue; // (1)
      
      bool correct_did = false;
      if (std::string(argv[1])=="tt" && (job_DID=="410472" || job_DID=="411076" || job_DID=="411077" || job_DID=="411078") ) correct_did = true;
      if (std::string(argv[1])=="singletop" && (job_DID=="410648" || job_DID=="410649" || job_DID=="410644" || job_DID=="410645" || job_DID=="410658" || job_DID=="410659") ) correct_did = true;
      if (std::string(argv[1])=="ttV" && (job_DID=="410155" || job_DID=="410156" || job_DID=="410157" || job_DID=="410218" || job_DID=="410219" || job_DID=="410220" || job_DID=="410276" || job_DID=="410277" || job_DID=="410278") ) correct_did = true;
      if (std::string(argv[1])=="ttH" && (job_DID=="346345") ) correct_did = true;
      if (std::string(argv[1])=="diboson" && (job_DID=="364250" || job_DID=="364253" || job_DID=="364254" || (std::stoi(job_DID)>=364283 && std::stoi(job_DID)<=364290 && job_DID!="364286") || job_DID=="345705" || job_DID=="345706" || job_DID=="345723" || job_DID=="363356" || job_DID=="363358") ) correct_did = true;
      if (std::string(argv[1])=="z_jets" && (std::stoi(job_DID)>=364100 && std::stoi(job_DID)<=364141) ) correct_did = true;
      if (std::string(argv[1])=="other" && (job_DID=="410560" || job_DID=="410408" || job_DID=="346678" || job_DID=="346676" || job_DID=="412043") ) correct_did = true;
      
      if (correct_did==false) continue;
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
        TTree *tree_nominal = (TTree*)ntuple->Get("nominal");


	// Set all the needed branches
	#include "branches.h"
      


	// Loop over entries
	Int_t nEntries = tree_nominal->GetEntries();
        std::cout << "\tEntries = " << nEntries << std::endl;
	for (int entry=0; entry<nEntries; entry++)  {
	  
	  // Show events counter
	  if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
          tree_nominal->GetEntry(entry);


	  // Compute weights
	  double weight = 1;
	  #include "compute_weight.h"
	    
	  
	  // Declare cuts names
	  bool emu_cut = false;
          bool OS_cut = false;
          bool jets_n_cut = false;
          bool btags_n2_cut = false;
          bool btags_n3_cut = false;
          bool topHFFF_cut = false;
	  
	  
	  // Define cuts themselves
	  if ((*el_pt).size()==1 && (*mu_pt).size()==1) emu_cut = true;
          if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	  
          int jets_n = (*jet_pt).size();
          if (jets_n >=3) jets_n_cut = true;
	  
          int btags_n = 0;
          for (int i=0; i<(*jet_pt).size(); i++) { if ((*jet_DL1r_77)[i]==1) btags_n++; }
          if (btags_n >=2) btags_n2_cut = true;
          if (btags_n ==3) btags_n3_cut = true;
	  
          if ( only_410472==true || ( (topHFFF==1 && job_DID=="411076") || (topHFFF==2 && job_DID=="411077") || (topHFFF==3 && job_DID=="411078") || (topHFFF==0 && job_DID=="410472") ) ) topHFFF_cut = true;
	  if (std::string(argv[1])!="tt") topHFFF_cut = true;
	  
	  
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
	  // 2b (tag) incl, emu, OS
	  // ///
	  if (emu_cut*OS_cut*btags_n2_cut*topHFFF_cut*jets_n_cut == true) {
	   
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
	    

	      // ///
	      // jets parameters - sig/bkg
	      if (std::string(argv[1])=="tt") { 
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

		if (std::string(argv[1])=="tt") {
		  if ((*topHadronOriginFlag)[jet_i]==4) {
		    min_dR0_top = std::min(min_dR0_top, dR0);
		    min_dR1_top = std::min(min_dR1_top, dR1);}
		  else {
		    min_dR0_not_top = std::min(min_dR0_not_top, dR0);
		    min_dR1_not_top = std::min(min_dR1_not_top, dR0); }
		} // if "tt" 
              } // [if] DL1r tag
	    

	      
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

	      if (std::string(argv[1])=="tt") {
	      
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

	      if (std::string(argv[1])=="tt") {
		
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

	      if (std::string(argv[1])=="tt") {
	      
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
              if (std::string(argv[1])=="tt") { 
                #include "populate_mva_ntuple.h"
	      }
	      
	    } // [jet_i] - loop over jets (populating MVA input ntuple)
	    
	    
	    
	    h_min_dR_lep0_btags_from_top->Fill(min_dR0_top, weight);
	    h_min_dR_lep0_btags_not_from_top->Fill(min_dR0_not_top, weight);
	    h_min_dR_lep1_btags_from_top->Fill(min_dR1_top, weight);
	    h_min_dR_lep1_btags_not_from_top->Fill(min_dR1_not_top, weight);
	    
	    float m_three_tags = three_btags_lvecs.M();
	    float m_four_tags = four_btags_lvecs.M();
	    if (n_btags_tmp==3) h_m_three_tags->Fill(m_three_tags, weight);
	    if (n_btags_tmp>=4) h_m_four_tags->Fill(m_four_tags, weight);
	    
	  } // 2b (tags), emu, OS cuts
	  
	  
	} // [entry] - loop over entries
	
	ntuple->Close();
	
      } // [ntuple_number] - loop over ntuple pieces
      
    } // [job_number] - loop over given DIDs
    
  } // [dir_counter] - loop over mc16/data campaigns
	  

  
  // Save trees to the MVA ntuple
  if (std::string(argv[1])=="tt") {
    MVA_sig_tree->SetDirectory(MVA_tfile);
    MVA_bkg_tree->SetDirectory(MVA_tfile);
    MVA_tfile->cd();
    MVA_sig_tree->Write("Signal", TTree::kOverwrite);
    MVA_bkg_tree->Write("Background", TTree::kOverwrite);
    MVA_tfile->Close();
  }
  
  
  // Save hists
  TString savename = std::string("results/hists_") + std::string(argv[1]) + std::string("_test.root");
  TFile *hists_file = new TFile(savename, "RECREATE");
  hists_file->cd();
  #include "write_hists_data_mc.h"
  if (std::string(argv[1])=="tt") {
    #include "write_hists_sig_bkg.h"
  }
  hists_file->Close();
	  
  return 0;
} // END OF MAIN
