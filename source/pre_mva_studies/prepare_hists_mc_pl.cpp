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


  // Set rand seed
  srand(time(0));

  
  // Create directory for results
  gSystem->Exec("mkdir results");
  
  
  // Create a list of directories with ntuples
  TString path_to_ntuples = "/eos/atlas/atlascerngroupdisk/phys-top/ttjets/v4/";
  std::vector<TString> dir_paths = get_list_of_files(path_to_ntuples);
  
  
  // Declare histograms
  #include "include/declare_hists_sig_bkg.h"
  
  
  // Declare TFile, TTree, TBranches and variables for MVA
  //TString tmva_file_name = std::string("results/") + std::string(argv[1]) + std::string("_MVA_input.root");
  //TFile *MVA_tfile =  new TFile(tmva_file_name, "RECREATE");
  TFile *MVA_tfile;
  if (std::string(argv[1]) == "tt") MVA_tfile = new TFile("results/tt_hf_MVA_input_pl_small.root", "RECREATE");
  TTree *MVA_sig_tree = new TTree("Signal", "inputS");
  TTree *MVA_bkg_tree = new TTree("Background", "inputB");
  #include "include/set_mva_ntuple_branches.h"


  // Declare events counters for each mc16 campaign
  TString job_dids_str[4] = {"410472", "411076", "411077", "411078"};
  TString mc16_str[3] = {"mc16a", "mc16d", "mc16e"};
  int n_entries[3][4] = {0};
  int limit[3][4] = {{105039, 6936, 6430, 7433}, {130823, 8768, 8026, 9253}, {183073, 11407, 10594, 12219}}; // target 250k
  int total[3][4] = {{8084630, 533836, 494911, 572080}, {10069100, 674811, 617779, 712204}, {14090700, 877935, 815403, 940430}};
  int processed[3][4] = {0};

  
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


    // Test option
    //if (is_mc16a!=true) continue;

    
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
        TTree *tree_pl = (TTree*)ntuple->Get("particleLevel");


	// Set all the needed branches
	#include "include/branches_pl.h"
      

	
	// Loop over entries
	Int_t nEntries = tree_pl->GetEntries();
        std::cout << "\tEntries = " << nEntries << std::endl;
	for (int entry=0; entry<nEntries; entry++)  {
	  
	  // Show events counter
	  if (entry%1000==0) { std::cout << "\t" << entry << "\r"; std::cout.flush(); }
          tree_pl->GetEntry(entry);


	  // Compute weights
	  double weight = 1;
	  #include "include/compute_weight.h"
	  weight = w_mc*weight_lumi;
	    
	  
	  // Declare cuts names
	  bool emu_cut = false;
          bool OS_cut = false;
          bool jets_n_cut = false;
          bool bjets_n2_cut = false;
          bool bjets_n3_cut = false;
          bool topHFFF_cut = false;
	  
	  
	  // Define cuts themselves
	  if ((*el_pt).size()==1 && (*mu_pt).size()==1) emu_cut = true;
          if ((*el_charge)[0]!=(*mu_charge)[0]) OS_cut = true;
	  
          int jets_n = (*jet_pt).size();
          if (jets_n >=3) jets_n_cut = true;
	  
          int bjets_n = 0;
          for (int i=0; i<(*jet_pt).size(); i++) { if ((*jet_nGhosts_bHadron)[i]>0) bjets_n++; }
          if (bjets_n >=2) bjets_n2_cut = true;
          if (bjets_n >=3) bjets_n3_cut = true;
	  
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
	  TLorentzVector four_bjets_lvecs; // excl
	  TLorentzVector three_bjets_lvecs; // incl
	  four_bjets_lvecs.SetPtEtaPhiE(0, 0, 0, 0);
	  three_bjets_lvecs.SetPtEtaPhiE(0, 0, 0, 0);
	  int n_bjets_tmp = 0;
	  
	  
	  
	  // ///
	  // 3b (jet) incl, emu, OS
	  // ///
	  if (emu_cut*OS_cut*bjets_n3_cut*topHFFF_cut*jets_n_cut == true) {	    

	    
	    // ///
	    // Variables for min dR (sub)leaning bjets and leptons, and dR between bjet and 1st/2nd leptons - sig/bkg and data/mc
	    double min_dR0_top = 999999.; // leading lepton
	    double min_dR1_top = 999999.; // subleading lepton
	    double min_dR0_not_top = 999999.; // leading lepton
	    double min_dR1_not_top = 999999.; // subleading lepton
 
	 
   
	    // Loop over jets
	    for (int jet_i=0; jet_i<(*jet_pt).size(); jet_i++) {


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
	      // min dR (sub)leaning bjets and leptons, and dR between bjet and 1st/2nd leptons - sig/bkg and data/mc
	      
              if ((*jet_nGhosts_bHadron)[jet_i]>0) {
		
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
              } // [if] bjet
	    

	      
	      // ///
	      // min dR between defferent objets for MVA - sig/bkg discrimination
              
	      double min_dR_bjet_from_top_to_bjet = 999999.;
              double min_dR_bjet_not_from_top_to_bjet = 999999.;
              double min_dR_not_bjet_to_bjet = 999999.;
              double min_dR_bjet_from_top_to_jet = 999999.;
              double min_dR_bjet_not_from_top_to_jet = 999999.;
              double min_dR_not_bjet_to_jet = 999999.;
              double min_dR_bjet_from_top_to_lep = 999999.;
              double min_dR_bjet_not_from_top_to_lep = 999999.;
              double min_dR_not_bjet_to_lep = 999999.;

	      if (std::string(argv[1])=="tt") {
	      
		for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
		  if (jet_i==jet_j) continue;
		  
		  // dR_min
		  if ((*jet_nGhosts_bHadron)[jet_i]>0 && (*topHadronOriginFlag)[jet_i]==4 && (*jet_nGhosts_bHadron)[jet_j]>0) {
		    double dR_bjet_from_top_to_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		    if (dR_bjet_from_top_to_bjet < min_dR_bjet_from_top_to_bjet) min_dR_bjet_from_top_to_bjet = dR_bjet_from_top_to_bjet; }
		  
		  if ((*jet_nGhosts_bHadron)[jet_i]>0 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_nGhosts_bHadron)[jet_j]>0) {
		    double dR_bjet_not_from_top_to_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		    if (dR_bjet_not_from_top_to_bjet < min_dR_bjet_not_from_top_to_bjet) min_dR_bjet_not_from_top_to_bjet = dR_bjet_not_from_top_to_bjet; }
		  
		  if ((*jet_nGhosts_bHadron)[jet_i]==0 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_nGhosts_bHadron)[jet_j]>0) {
		    double dR_not_bjet_to_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		    if (dR_not_bjet_to_bjet < min_dR_not_bjet_to_bjet) min_dR_not_bjet_to_bjet = dR_not_bjet_to_bjet; }
		  
		  if ((*jet_nGhosts_bHadron)[jet_i]>0 && (*topHadronOriginFlag)[jet_i]==4 && (*jet_nGhosts_bHadron)[jet_j]==0) {
		    double dR_bjet_from_top_to_jet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		    if (dR_bjet_from_top_to_jet < min_dR_bjet_from_top_to_jet) min_dR_bjet_from_top_to_jet = dR_bjet_from_top_to_jet; }
		  
		  if ((*jet_nGhosts_bHadron)[jet_i]>0 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_nGhosts_bHadron)[jet_j]==0) {
		    double dR_bjet_not_from_top_to_jet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		    if (dR_bjet_not_from_top_to_jet < min_dR_bjet_not_from_top_to_jet) min_dR_bjet_not_from_top_to_jet = dR_bjet_not_from_top_to_jet; }
		  
		  if ((*jet_nGhosts_bHadron)[jet_i]==0 && (*topHadronOriginFlag)[jet_i]!=4 && (*jet_nGhosts_bHadron)[jet_j]==0) {
		    double dR_not_bjet_to_jet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		    if (dR_not_bjet_to_jet < min_dR_not_bjet_to_jet) min_dR_not_bjet_to_jet = dR_not_bjet_to_jet; }
		} // loop over [jet_j]
		
		if ((*jet_nGhosts_bHadron)[jet_i]>0 && (*topHadronOriginFlag)[jet_i]==4) {
		  double dR_bjet_from_top_to_el = jets_lvec[jet_i].DeltaR(el_lvec);
		  double dR_bjet_from_top_to_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		  double dR_bjet_from_top_to_lep = std::min(dR_bjet_from_top_to_el, dR_bjet_from_top_to_mu);
		  if (dR_bjet_from_top_to_lep < min_dR_bjet_from_top_to_lep) min_dR_bjet_from_top_to_lep = dR_bjet_from_top_to_lep; }
		
		if ((*jet_nGhosts_bHadron)[jet_i]>0 && (*topHadronOriginFlag)[jet_i]!=4) {
		  double dR_bjet_not_from_top_to_el = jets_lvec[jet_i].DeltaR(el_lvec);
		  double dR_bjet_not_from_top_to_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		  double dR_bjet_not_from_top_to_lep = std::min(dR_bjet_not_from_top_to_el, dR_bjet_not_from_top_to_mu);
		  if (dR_bjet_not_from_top_to_lep < min_dR_bjet_not_from_top_to_lep) min_dR_bjet_not_from_top_to_lep = dR_bjet_not_from_top_to_lep; }
		
		if ((*jet_nGhosts_bHadron)[jet_i]==0 && (*topHadronOriginFlag)[jet_i]!=4) {
		  double dR_not_bjet_to_el = jets_lvec[jet_i].DeltaR(el_lvec);
		  double dR_not_bjet_to_mu = jets_lvec[jet_i].DeltaR(mu_lvec);;
		  double dR_not_bjet_to_lep = std::min(dR_not_bjet_to_el, dR_not_bjet_to_mu);
		  if (dR_not_bjet_to_lep < min_dR_not_bjet_to_lep) min_dR_not_bjet_to_lep = dR_not_bjet_to_lep; }

		
		if (min_dR_bjet_from_top_to_bjet!=999999) h_min_dR_btag_from_top_to_btag->Fill(min_dR_bjet_from_top_to_bjet, weight);
                if (min_dR_bjet_not_from_top_to_bjet!=999999) h_min_dR_btag_not_from_top_to_btag->Fill(min_dR_bjet_not_from_top_to_bjet, weight);
                if (min_dR_not_bjet_to_bjet!=999999) h_min_dR_not_btag_to_btag->Fill(min_dR_not_bjet_to_bjet, weight);
                if (min_dR_bjet_from_top_to_jet!=999999) h_min_dR_btag_from_top_to_jet->Fill(min_dR_bjet_from_top_to_jet, weight);
                if (min_dR_bjet_not_from_top_to_jet!=999999) h_min_dR_btag_not_from_top_to_jet->Fill(min_dR_bjet_not_from_top_to_jet, weight);
                if (min_dR_not_bjet_to_jet!=999999) h_min_dR_not_btag_to_jet->Fill(min_dR_not_bjet_to_jet, weight);
                if (min_dR_bjet_from_top_to_lep!=999999) h_min_dR_btag_from_top_to_lep->Fill(min_dR_bjet_from_top_to_lep, weight);
                if (min_dR_bjet_not_from_top_to_lep!=999999) h_min_dR_btag_not_from_top_to_lep->Fill(min_dR_bjet_not_from_top_to_lep, weight);
                if (min_dR_not_bjet_to_lep!=999999) h_min_dR_not_btag_to_lep->Fill(min_dR_not_bjet_to_lep, weight);
		
	      } // if "tt"
	      


	      // ///
	      // M {lep, bjet/jet} - sig/bkg discrimination

              double min_m_lep_bjet_from_top = 999999;
              double max_m_lep_bjet_from_top = 0;
              double min_m_lep_bjet_not_from_top = 999999;
              double max_m_lep_bjet_not_from_top = 0;
              double min_m_lep_other_jet = 999999;
              double max_m_lep_other_jet = 0;

	      if (std::string(argv[1])=="tt") {
		
		// bjet and the closest lepton
		if ((*jet_nGhosts_bHadron)[jet_i]>0) {
		  if ((*topHadronOriginFlag)[jet_i]==4) {
		    double dr_j_el = jets_lvec[jet_i].DeltaR(el_lvec);
		    double dr_j_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		    double m_j_lep = 0;
		    if (dr_j_el <= dr_j_mu) { m_j_lep = (jets_lvec[jet_i] + el_lvec).M(); }
		    else { m_j_lep = (jets_lvec[jet_i] + mu_lvec).M();
		    if (m_j_lep != 0) h_m_lep_btag_from_top_min_dR->Fill(m_j_lep, weight); }
		  }  
                else {
		    double dr_j_el = jets_lvec[jet_i].DeltaR(el_lvec);
		    double dr_j_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
		    double m_j_lep = 0;
		    if (dr_j_el <= dr_j_mu) { m_j_lep = (jets_lvec[jet_i] + el_lvec).M(); }
		    else { m_j_lep = (jets_lvec[jet_i] + mu_lvec).M(); }
		    if (m_j_lep != 0) h_m_lep_btag_not_from_top_min_dR->Fill(m_j_lep, weight); } } // [else], [if jet_i is b] closing
		
		// bjet and leptons - min and max M
		if ((*jet_nGhosts_bHadron)[jet_i]>0) {
		  if ((*topHadronOriginFlag)[jet_i]==4) {
		    double min_m_lep_bjet_from_top_tmp = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		    double max_m_lep_bjet_from_top_tmp = std::max( (jets_lvec[jet_i] + mu_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		    min_m_lep_bjet_from_top = std::min(min_m_lep_bjet_from_top_tmp, min_m_lep_bjet_from_top);
		    max_m_lep_bjet_from_top = std::max(max_m_lep_bjet_from_top_tmp, max_m_lep_bjet_from_top_tmp); }
		  else {
		    double min_m_lep_bjet_not_from_top_tmp = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		    double max_m_lep_bjet_not_from_top_tmp = std::max( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		    min_m_lep_bjet_not_from_top = std::min(min_m_lep_bjet_not_from_top_tmp, min_m_lep_bjet_not_from_top);
		    max_m_lep_bjet_not_from_top = std::max(max_m_lep_bjet_not_from_top_tmp, max_m_lep_bjet_not_from_top); } }
		
		// other than bjets and leptons - min and max M
		else {
		  double min_m_lep_other_jet_tmp = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  double max_m_lep_other_jet_tmp = std::max( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  min_m_lep_other_jet = std::min( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() );
		  max_m_lep_other_jet = std::max( (jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M() ); }
		
		// Fill the min/max M hists
		if (min_m_lep_bjet_from_top!=999999) h_min_m_lep_btag_from_top->Fill(min_m_lep_bjet_from_top, weight);
                if (max_m_lep_bjet_from_top!=0) h_max_m_lep_btag_from_top->Fill(max_m_lep_bjet_from_top, weight);
                if (min_m_lep_bjet_not_from_top!=999999) h_min_m_lep_btag_not_from_top->Fill(min_m_lep_bjet_not_from_top, weight);
                if (max_m_lep_bjet_not_from_top!=0) h_max_m_lep_btag_not_from_top->Fill(max_m_lep_bjet_not_from_top, weight);
                if (min_m_lep_other_jet!=999999) h_min_m_lep_other_jet->Fill(min_m_lep_other_jet, weight);
                if (max_m_lep_other_jet!=0) h_max_m_lep_other_jet->Fill(max_m_lep_other_jet, weight);
	      } // if "tt"
	      
	      
	      
	      // ///
	      // M of a jet
	      
              float m_jet = jets_lvec[jet_i].M();
	      
	      
	      // ///
	      // M of bjets and their combinations - sig/bkg
	      
	      float jet_m = jets_lvec[jet_i].M();
	      
	      float m_min_bjet_top_bjet_top = 999999.;
	      float m_max_bjet_top_bjet_top = 0.;
	      float m_min_bjet_top_bjet_other = 999999.;
	      float m_max_bjet_top_bjet_other = 0.;
	      float m_min_bjet_other_bjet_other = 999999.;
	      float m_max_bjet_other_bjet_other = 0.;
	      
              float m_min_jet_jet = 999999.;
              float m_max_jet_jet = 0.;
	      
	      if (std::string(argv[1])=="tt") {
		
		if ( (*jet_nGhosts_bHadron)[jet_i]>=1) {
		  n_bjets_tmp++ ;
		  if ( (*topHadronOriginFlag)[jet_i]==4 ) { h_m_btag_top->Fill(m_jet, weight); }
		  else { h_m_btag_other->Fill(m_jet, weight); } }
		
		for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
		  
		  if (jet_i==jet_j) continue;
		  
		  float m_jet_jet = (jets_lvec[jet_i] + jets_lvec[jet_j]).M();
		  m_min_jet_jet = std::min(m_min_jet_jet, m_jet_jet);
		  m_max_jet_jet = std::max(m_max_jet_jet, m_jet_jet);
		  
		  if ( (*jet_nGhosts_bHadron)[jet_j]==0 ) continue;
		  
		  if ( (*topHadronOriginFlag)[jet_i]==4 && (*topHadronOriginFlag)[jet_j]==4) {
		    h_m_btag_top_btag_top->Fill(m_jet_jet, weight);
		    m_min_bjet_top_bjet_top = std::min(m_jet_jet, m_min_bjet_top_bjet_top);
		    m_max_bjet_top_bjet_top = std::max(m_jet_jet, m_max_bjet_top_bjet_top); }
		  if ( (*topHadronOriginFlag)[jet_i]==4 ^  (*topHadronOriginFlag)[jet_j]==4) {
		    h_m_btag_top_btag_other->Fill(m_jet_jet, weight);
		    m_min_bjet_top_bjet_other = std::min(m_jet_jet, m_min_bjet_top_bjet_other);
		    m_max_bjet_top_bjet_other = std::max(m_jet_jet, m_max_bjet_top_bjet_other); }
		  if ( (*topHadronOriginFlag)[jet_i]!=4 && (*topHadronOriginFlag)[jet_j]!=4) {
		    h_m_btag_other_btag_other->Fill(m_jet_jet, weight);
		    m_min_bjet_other_bjet_other = std::min(m_jet_jet, m_min_bjet_other_bjet_other);
		    m_max_bjet_other_bjet_other = std::max(m_jet_jet, m_max_bjet_other_bjet_other); }
		  
		} // [jet_j] - loop over jets
		
		three_bjets_lvecs += jets_lvec[jet_i];
		if (n_bjets_tmp<=3) three_bjets_lvecs += jets_lvec[jet_i];
		
		if (m_min_bjet_top_bjet_top!=999999.) h_m_min_btag_top_btag_top->Fill(m_min_bjet_top_bjet_top, weight);
                if (m_max_bjet_top_bjet_top!=0)       h_m_max_btag_top_btag_top->Fill(m_max_bjet_top_bjet_top, weight);
                if (m_min_bjet_top_bjet_other!=999999.) h_m_min_btag_top_btag_other->Fill(m_min_bjet_top_bjet_other, weight);
                if (m_max_bjet_top_bjet_other!=0)       h_m_max_btag_top_btag_other->Fill(m_max_bjet_top_bjet_other, weight);
                if (m_min_bjet_other_bjet_other!=999999.) h_m_min_btag_other_btag_other->Fill(m_min_bjet_other_bjet_other, weight);
                if (m_max_bjet_other_bjet_other!=0)       h_m_max_btag_other_btag_other->Fill(m_max_bjet_other_bjet_other, weight);
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

	      
	      // ///
	      // M of bjet and the closest lepton
	      double m_jet_lep = 0;
              double dr_jet_el = jets_lvec[jet_i].DeltaR(el_lvec);
              double dr_jet_mu = jets_lvec[jet_i].DeltaR(mu_lvec);
              if (dr_jet_el <= dr_jet_mu) { m_jet_lep = (jets_lvec[jet_i] + el_lvec).M(); }
              else { m_jet_lep = (jets_lvec[jet_i] + mu_lvec).M(); }
	      
	      
	      // ///
	      // M min and max of bjet and lepton
	      double m_jet_lep_max = std::max((jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M());
              double m_jet_lep_min = std::min((jets_lvec[jet_i] + el_lvec).M(), (jets_lvec[jet_i] + mu_lvec).M());
	      
	      
	      // ///
	      // dR between the jet [jet_i] and the closes bjet
	      double min_dR_jet_bjet = 999999.;
              for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
                if (jet_i==jet_j) continue;
                if ( (*jet_nGhosts_bHadron)[jet_j]>0) {
                  double dR_jet_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
		  min_dR_jet_bjet = std::min(min_dR_jet_bjet, dR_jet_bjet);
                } // if - jet_j is b
	      } // [jet_j] - loop over jets
	      
	      
	      // ///
	      // Populate the MVA ntuple with values
              if (std::string(argv[1])=="tt" && (*jet_nGhosts_bHadron)[jet_i]>=1) {

		for (int i=0; i<3; i++) {
		  for (int j=0; j<4; j++){
		    if (mc16_str[i]==dir_path_components[last_element_index] && job_DID==job_dids_str[j]) {
		      
		      int randomval = rand()%6; // random in the range of  [0,5] (throw a dice! :) )
		      // Select events if: 
		      // 1. random==1 and limit hasn't been reached
		      // 2. or when there is almost no room left to reach the limit. 
		      // Give it factor of 4 headroom: we have 3 flavors of dilepton events (ee(25%), emu(50%), mumu(25%)) randomly saved
		      //if ( randomval==0 && n_entries[i][j]<5*limit[i][j]) {
			n_entries[i][j]++;      
			#include "include/populate_mva_ntuple.h"
			//} // if - an event was selected (randomly) and the limit is not exeeded
		      
		    } // if - figuring out the actual mc16+DID pair
		  } // [j] - loop over 4 DIDs
		} // [i] - loop over mc16X

	      } // if - tt samples
	      
	    } // [jet_i] - loop over jets (populating MVA input ntuple)

	    
	    // Increment the number of processed entries per mc16+did
	    for (int i=0; i<3; i++) {
	      for (int j=0; j<4; j++) {
		if (mc16_str[i]==dir_path_components[last_element_index] && job_DID==job_dids_str[j]) processed[i][j]++;
	      }
	    }

	  } // 3b, emu-OS

	  
	} // [entry] - loop over PL entries
	
	ntuple->Close();
	
      } // [ntuple_number] - loop over ntuple pieces
      
    } // [job_number] - loop over given DIDs
    
  } // [dir_counter] - loop over mc16/data campaigns



  // Show statistics for ftations
  for (int i=0; i<3; i++) {
    std::cout << "\nTotal " << mc16_str[i] << " = " << processed[i][0]+processed[i][1]+processed[i][2]+processed[i][3] << std::endl;
    for (int j=0; j<4; j++) {
      std::cout << mc16_str[i] << " , " << job_dids_str[j] << " = " << processed[i][j] << std::endl;
    }
  }


  
  // Write hists
  TString savename = std::string("results/hists_") + std::string(argv[1]) + std::string("_test_pl.root");
  TFile *hists_file = new TFile(savename, "RECREATE");
  hists_file->cd();
  #include "include/write_hists_sig_bkg.h"

  
  // Save trees to the MVA ntuple
  if (std::string(argv[1])=="tt") {
    MVA_sig_tree->SetDirectory(MVA_tfile);
    MVA_bkg_tree->SetDirectory(MVA_tfile);
    MVA_tfile->cd();
    MVA_sig_tree->Write("Signal", TTree::kOverwrite);
    MVA_bkg_tree->Write("Background", TTree::kOverwrite);
    MVA_tfile->Close();
  }
  
  for (int i=0; i<3; i++) {
    std::cout << mc16_str[i] << std::endl;
    for (int j=0; j<4; j++)
      {
	std::cout << job_dids_str[j] << ", n_saved: " << n_entries[i][j] << " ,\t limit: " << limit[i][j] << std::endl;
       }
  }
  

  return 0;
} // END OF MAIN
