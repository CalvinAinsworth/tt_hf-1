  // Signal tree variables
  Float_t dR_jet_lep0_MVA_sig;
  Float_t dR_jet_lep1_MVA_sig;
  Float_t min_dR_jet_lep_MVA_sig;
  Float_t m_jet_lep_min_dR_MVA_sig;
  Float_t m_jet_el_MVA_sig; 
  Float_t m_jet_mu_MVA_sig;
  Float_t m_jet_lep_max_MVA_sig;
  Float_t m_jet_lep_min_MVA_sig;
  Float_t min_dR_jet_bjet_MVA_sig;
  Float_t m_min_jet_jet_MVA_sig;
  Float_t m_max_jet_jet_MVA_sig;
  Float_t tot_event_weight_MVA_sig;
  Float_t jet_pt_MVA_sig; 
  Float_t jet_eta_MVA_sig; 
  Float_t jet_m_MVA_sig;
  Int_t topHOF_sig;

  // Bkg tree variables
  Float_t dR_jet_lep0_MVA_bkg;
  Float_t dR_jet_lep1_MVA_bkg;
  Float_t min_dR_jet_lep_MVA_bkg;
  Float_t m_jet_lep_min_dR_MVA_bkg;
  Float_t m_jet_el_MVA_bkg;
  Float_t m_jet_mu_MVA_bkg;
  Float_t m_jet_lep_max_MVA_bkg;
  Float_t m_jet_lep_min_MVA_bkg;
  Float_t min_dR_jet_bjet_MVA_bkg;
  Float_t m_min_jet_jet_MVA_bkg;
  Float_t m_max_jet_jet_MVA_bkg;
  Float_t tot_event_weight_MVA_bkg;
  Float_t jet_pt_MVA_bkg;
  Float_t jet_eta_MVA_bkg;
  Float_t jet_m_MVA_bkg;
  Int_t topHOF_bkg;

  // Sig branches
  MVA_sig_tree->Branch("dR_jet_lep0", &dR_jet_lep0_MVA_sig, "dR_jet_lep0_MVA_sig/F");
  MVA_sig_tree->Branch("dR_jet_lep1", &dR_jet_lep1_MVA_sig, "dR_jet_lep1_MVA_sig/F");
  MVA_sig_tree->Branch("min_dR_jet_lep", &min_dR_jet_lep_MVA_sig, "min_dR_jet_lep/F");
  MVA_sig_tree->Branch("m_jet_lep_min_dR", &m_jet_lep_min_dR_MVA_sig, "m_jet_lep_min_dR/F");
  MVA_sig_tree->Branch("m_jet_el", &m_jet_el_MVA_sig, "m_jet_el/F");
  MVA_sig_tree->Branch("m_jet_mu", &m_jet_mu_MVA_sig, "m_jet_mu/F");
  MVA_sig_tree->Branch("m_jet_lep_max", &m_jet_lep_max_MVA_sig, "m_jet_lep_max/F");
  MVA_sig_tree->Branch("m_jet_lep_min", &m_jet_lep_min_MVA_sig, "m_jet_lep_min/F");
  MVA_sig_tree->Branch("min_dR_jet_bjet", &min_dR_jet_bjet_MVA_sig, "min_dR_jet_bjet/F");
  MVA_sig_tree->Branch("m_min_jet_jet", &m_min_jet_jet_MVA_sig, "m_min_jet_jet/F");
  MVA_sig_tree->Branch("m_max_jet_jet", &m_max_jet_jet_MVA_sig, "m_max_jet_jet/F");
  MVA_sig_tree->Branch("tot_event_weight", &tot_event_weight_MVA_sig, "tot_event_weight/F");
  MVA_sig_tree->Branch("jet_pt", &jet_pt_MVA_sig, "jet_pt/F");
  MVA_sig_tree->Branch("jet_eta", &jet_eta_MVA_sig, "jet_eta/F");
  MVA_sig_tree->Branch("jet_m", &jet_m_MVA_sig, "jet_m_MVA_sig/F");
  MVA_sig_tree->Branch("jet_GBHInit_topHadronOriginFlag", &topHOF_sig, "jet_GBHInit_topHadronOriginFlag/I");

  
  // Bkg branches
  MVA_bkg_tree->Branch("dR_jet_lep0", &dR_jet_lep0_MVA_bkg, "dR_jet_lep0_MVA_bkg/F");
  MVA_bkg_tree->Branch("dR_jet_lep1", &dR_jet_lep1_MVA_bkg, "dR_jet_lep1_MVA_bkg/F");
  MVA_bkg_tree->Branch("min_dR_jet_lep", &min_dR_jet_lep_MVA_bkg, "min_dR_jet_lep/F");
  MVA_bkg_tree->Branch("m_jet_lep_min_dR", &m_jet_lep_min_dR_MVA_bkg, "m_jet_lep_min_dR/F");
  MVA_bkg_tree->Branch("m_jet_el", &m_jet_el_MVA_bkg, "m_jet_el/F");
  MVA_bkg_tree->Branch("m_jet_mu", &m_jet_mu_MVA_bkg, "m_jet_mu/F");
  MVA_bkg_tree->Branch("m_jet_lep_max", &m_jet_lep_max_MVA_bkg, "m_jet_lep_max/F");
  MVA_bkg_tree->Branch("m_jet_lep_min", &m_jet_lep_min_MVA_bkg, "m_jet_lep_min/F");
  MVA_bkg_tree->Branch("min_dR_jet_bjet", &min_dR_jet_bjet_MVA_bkg, "min_dR_jet_bjet/F");
  MVA_bkg_tree->Branch("m_min_jet_jet", &m_min_jet_jet_MVA_bkg, "m_min_jet_jet/F");
  MVA_bkg_tree->Branch("m_max_jet_jet", &m_max_jet_jet_MVA_bkg, "m_max_jet_jet/F");
  MVA_bkg_tree->Branch("tot_event_weight", &tot_event_weight_MVA_bkg, "tot_event_weight/F");
  MVA_bkg_tree->Branch("jet_pt", &jet_pt_MVA_bkg, "jet_pt/F");
  MVA_bkg_tree->Branch("jet_eta", &jet_eta_MVA_bkg, "jet_eta/F");
  MVA_bkg_tree->Branch("jet_m", &jet_m_MVA_bkg, "jet_m_MVA_bkg/F");
  MVA_bkg_tree->Branch("jet_GBHInit_topHadronOriginFlag", &topHOF_bkg, "jet_GBHInit_topHadronOriginFlag/I");
