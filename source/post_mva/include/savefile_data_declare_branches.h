
  // ///
  // Declare variables
  
  // "Standard" jet variables
  std::vector<Float_t> *jet_pt_out;
  std::vector<Float_t> *jet_DL1r_out;
  std::vector<Float_t> *jet_eta_out;
  std::vector<Float_t> *jet_phi_out;
  std::vector<Float_t> *jet_e_out;
  std::vector<Float_t> *jet_jvt_out;
  std::vector<Float_t> *jet_tagWeightBin_DL1r_Continuous_out;
  std::vector<char> *jet_DL1r_77_out;
  std::vector<Float_t> *MVA_score;
  
  jet_pt_out = jet_DL1r_out = jet_eta_out = jet_phi_out = jet_e_out = jet_jvt_out = jet_tagWeightBin_DL1r_Continuous_out = MVA_score = 0;
  jet_DL1r_77_out = 0;


  // Electrons
  std::vector<Float_t> *el_pt_out;
  std::vector<Float_t> *el_eta_out;
  std::vector<Float_t> *el_cl_eta_out;
  std::vector<Float_t> *el_phi_out;
  std::vector<Float_t> *el_charge_out;
  std::vector<Float_t> *el_e_out;
  std::vector<Float_t> *el_topoetcone20_out;
  std::vector<Float_t> *el_ptvarcone20_out;
  std::vector<Float_t> *el_CF_out;
  std::vector<Float_t> *el_d0sig_out;
  std::vector<Float_t> *el_delta_z0_sintheta_out;

  el_pt_out = el_eta_out = el_cl_eta_out = el_phi_out = el_charge_out = el_e_out = el_topoetcone20_out = el_ptvarcone20_out = el_CF_out = el_d0sig_out = el_delta_z0_sintheta_out = 0;

  
  // Muons
  std::vector<Float_t> *mu_pt_out;
  std::vector<Float_t> *mu_eta_out;
  std::vector<Float_t> *mu_phi_out;
  std::vector<Float_t> *mu_charge_out;
  std::vector<Float_t> *mu_e_out;
  std::vector<Float_t> *mu_topoetcone20_out;
  std::vector<Float_t> *mu_ptvarcone30_out;
  std::vector<Float_t> *mu_d0sig_out;
  std::vector<Float_t> *mu_delta_z0_sintheta_out;

  mu_pt_out = mu_eta_out = mu_phi_out = mu_charge_out = mu_e_out = mu_topoetcone20_out = mu_ptvarcone30_out = mu_d0sig_out = mu_delta_z0_sintheta_out = 0;


  // Other
  Float_t met_out;
  Float_t met_phi_out;
  UInt_t runNumber_out;
  ULong64_t eventNumber_out;


  // Computable variables - MVA discriminating variables
  std::vector<Float_t> *dR_jet_lep0_out;
  std::vector<Float_t> *dR_jet_lep1_out;
  std::vector<Float_t> *min_dR_jet_lep_out;
  std::vector<Float_t> *m_jet_el_out;
  std::vector<Float_t> *m_jet_mu_out;
  std::vector<Float_t> *m_jet_lep_max_out;
  std::vector<Float_t> *jet_m_out;
  std::vector<Float_t> *m_min_jet_jet_out;
  std::vector<Float_t> *m_max_jet_jet_out;
  std::vector<Float_t> *min_dR_jet_bjet_out;
  
  dR_jet_lep0_out = dR_jet_lep1_out = min_dR_jet_lep_out = m_jet_el_out = m_jet_mu_out = m_jet_lep_max_out = jet_m_out = m_min_jet_jet_out = m_max_jet_jet_out = min_dR_jet_bjet_out = 0;

  
  // Vertices
  std::vector<Int_t> *vertex_type_out;
  std::vector<Double_t> *vertex_z_out;

  vertex_type_out = 0;
  vertex_z_out = 0;



  // ///
  // Declare branches

  out_tree->Branch("jet_pt", &jet_pt_out);
  out_tree->Branch("jet_eta", &jet_eta_out);
  out_tree->Branch("jet_phi", &jet_phi_out);
  out_tree->Branch("jet_e", &jet_e_out);
  out_tree->Branch("jet_jvt", &jet_jvt_out);
  out_tree->Branch("jet_DL1r", &jet_DL1r_out);
  out_tree->Branch("jet_isbtagged_DL1r_77", &jet_DL1r_77_out);
  out_tree->Branch("jet_tagWeightBin_DL1r_Continuous", &jet_tagWeightBin_DL1r_Continuous_out);
  out_tree->Branch(tmva_config_info[0], &MVA_score);

  out_tree->Branch("el_pt", &el_pt_out);
  out_tree->Branch("el_eta", &el_eta_out);
  out_tree->Branch("el_cl_eta", &el_cl_eta_out);
  out_tree->Branch("el_phi", &el_phi_out);
  out_tree->Branch("el_charge", &el_charge_out);
  out_tree->Branch("el_e", &el_e_out);
  out_tree->Branch("el_topoetcone20", &el_topoetcone20_out);
  out_tree->Branch("el_ptvarcone20", &el_ptvarcone20_out);
  out_tree->Branch("el_CF", &el_CF_out);
  out_tree->Branch("el_d0sig", &el_d0sig_out);
  out_tree->Branch("el_delta_z0_sintheta", &el_delta_z0_sintheta_out);

  out_tree->Branch("mu_pt", &mu_pt_out);
  out_tree->Branch("mu_eta", &mu_eta_out);
  out_tree->Branch("mu_phi", &mu_phi_out);
  out_tree->Branch("mu_charge", &mu_charge_out);
  out_tree->Branch("mu_e", &mu_e_out);
  out_tree->Branch("mu_topoetcone20", &mu_topoetcone20_out);
  out_tree->Branch("mu_ptvarcone30", &mu_ptvarcone30_out);
  out_tree->Branch("mu_d0sig", &mu_d0sig_out);
  out_tree->Branch("mu_delta_z0_sintheta", &mu_delta_z0_sintheta_out);

  out_tree->Branch("met_met", &met_out, "met_met/F");
  out_tree->Branch("met_phi", &met_phi_out, "met_phi/F");
  out_tree->Branch("runNumber", &runNumber_out, "runNumber/I");
  out_tree->Branch("eventNumber", &eventNumber_out, "eventNumber/l");

  out_tree->Branch("dR_jet_lep0", &dR_jet_lep0_out);
  out_tree->Branch("dR_jet_lep1", &dR_jet_lep1_out);
  out_tree->Branch("min_dR_jet_lep", &min_dR_jet_lep_out);
  out_tree->Branch("m_jet_el", &m_jet_el_out);
  out_tree->Branch("m_jet_mu", &m_jet_mu_out);
  out_tree->Branch("m_jet_lep_max", &m_jet_lep_max_out);
  out_tree->Branch("min_dR_jet_bjet", &min_dR_jet_bjet_out);
  out_tree->Branch("jet_m", &jet_m_out);
  out_tree->Branch("m_min_jet_jet", &m_min_jet_jet_out);
  out_tree->Branch("m_max_jet_jet", &m_max_jet_jet_out);

  out_tree->Branch("vertex_type", &vertex_type_out);
  out_tree->Branch("vertex_z", &vertex_z_out);
