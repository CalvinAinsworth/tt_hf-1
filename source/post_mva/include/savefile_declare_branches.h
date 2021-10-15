
  // //
  // Nominal tree
  
  // "Standard" jets variables
  std::vector<Float_t> *jet_pt_out;
  std::vector<Float_t> *jet_DL1r_out;
  std::vector<Float_t> *jet_eta_out;
  std::vector<Float_t> *jet_phi_out;
  std::vector<Float_t> *jet_e_out;
  std::vector<Float_t> *jet_jvt_out;
  std::vector<Float_t> *jet_tagWeightBin_DL1r_Continuous_out;
  std::vector<Int_t> *jet_truthflav_out;
  std::vector<Int_t> *jet_truthPartonLabel_out;
  std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag_out;
  std::vector<Int_t> *jet_nGhosts_bHadron_out;
  std::vector<Int_t> *jet_nGhosts_cHadron_out;
  std::vector<char> *jet_DL1r_77_out;
  std::vector<char> *jet_isTrueHS_out;
  std::vector<Float_t> *MVA_score;
  
  jet_pt_out = jet_DL1r_out = jet_eta_out = jet_phi_out = jet_e_out = jet_jvt_out = jet_tagWeightBin_DL1r_Continuous_out = MVA_score = 0;
  jet_truthflav_out = jet_truthPartonLabel_out = jet_GBHInit_topHadronOriginFlag_out = jet_nGhosts_bHadron_out = jet_nGhosts_cHadron_out = 0;
  jet_DL1r_77_out = jet_isTrueHS_out = 0;
  

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
  std::vector<Int_t> *el_true_type_out;
  std::vector<Int_t> *el_true_origin_out;
  std::vector<Int_t> *el_true_firstEgMotherTruthType_out;
  std::vector<Int_t> *el_true_firstEgMotherTruthOrigin_out;
  std::vector<Int_t> *el_true_firstEgMotherPdgId_out;
  std::vector<char> *el_true_isPrompt_out;
  std::vector<char> *el_true_isChargeFl_out;

  el_pt_out = el_eta_out = el_cl_eta_out = el_phi_out = el_charge_out = el_e_out = el_topoetcone20_out = el_ptvarcone20_out = el_CF_out = el_d0sig_out = el_delta_z0_sintheta_out = 0;
  el_true_type_out = el_true_origin_out = el_true_firstEgMotherTruthType_out = el_true_firstEgMotherTruthOrigin_out = el_true_firstEgMotherPdgId_out = 0;
  el_true_isPrompt_out = el_true_isChargeFl_out = 0;


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
  std::vector<Int_t> *mu_true_type_out;
  std::vector<Int_t> *mu_true_origin_out;
  std::vector<char> *mu_true_isPrompt_out;

  mu_pt_out = mu_eta_out = mu_phi_out = mu_charge_out = mu_e_out = mu_topoetcone20_out = mu_ptvarcone30_out = mu_d0sig_out = mu_delta_z0_sintheta_out = 0;
  mu_true_type_out = mu_true_origin_out = 0;
  mu_true_isPrompt_out = 0;
  

  // Other
  Float_t met_out;
  Float_t met_phi_out;
  UInt_t runNumber_out;
  ULong64_t eventNumber_out;
  Int_t topHeavyFlavorFilterFlag_out;
  
  
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


  // Weighs
  Float_t w_mc_out;
  Float_t w_pu_out;
  Float_t w_leptonSF_out;
  Float_t w_DL1r_77_out;
  Float_t w_jvt_out;


  // Vertices
  std::vector<Int_t> *vertex_type_out;
  std::vector<Double_t> *vertex_z_out;

  vertex_type_out = 0;
  vertex_z_out = 0;



  // ///
  // Particle level tree
  
  // Jet variables
  std::vector<Float_t> *jet_pt_pl_out;
  std::vector<Float_t> *jet_eta_pl_out;
  std::vector<Float_t> *jet_phi_pl_out;
  std::vector<Float_t> *jet_e_pl_out;
  std::vector<Int_t> *jet_nGhosts_bHadron_pl_out;
  std::vector<Int_t> *jet_nGhosts_cHadron_pl_out;
  std::vector<Int_t> *jet_truthPartonLabel_pl_out;
  std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag_pl_out;
  std::vector<Float_t> *MVA_score_pl;

  jet_pt_pl_out = jet_eta_pl_out = jet_phi_pl_out = jet_e_pl_out = MVA_score_pl = 0;
  jet_nGhosts_bHadron_pl_out = jet_nGhosts_cHadron_pl_out = jet_truthPartonLabel_pl_out = jet_GBHInit_topHadronOriginFlag_pl_out = 0;

  
  // Electrons
  std::vector<Float_t> *el_pt_pl_out;
  std::vector<Float_t> *el_eta_pl_out;
  std::vector<Float_t> *el_phi_pl_out;
  std::vector<Float_t> *el_e_pl_out;
  std::vector<Float_t> *el_charge_pl_out;
  std::vector<Float_t> *el_pt_bare_out;
  std::vector<Float_t> *el_eta_bare_out;
  std::vector<Float_t> *el_phi_bare_out;
  std::vector<Float_t> *el_e_bare_out;
  
  el_pt_pl_out = el_eta_pl_out = el_phi_pl_out = el_e_pl_out = el_charge_pl_out = el_pt_bare_out = el_eta_bare_out = el_phi_bare_out = el_e_bare_out = 0;
  

  // Muons
  std::vector<Float_t> *mu_pt_pl_out;
  std::vector<Float_t> *mu_eta_pl_out;
  std::vector<Float_t> *mu_phi_pl_out;
  std::vector<Float_t> *mu_e_pl_out;
  std::vector<Float_t> *mu_charge_pl_out;
  std::vector<Float_t> *mu_pt_bare_out;
  std::vector<Float_t> *mu_eta_bare_out;
  std::vector<Float_t> *mu_phi_bare_out;
  std::vector<Float_t> *mu_e_bare_out;

  mu_pt_pl_out = mu_eta_pl_out = mu_phi_pl_out = mu_e_pl_out = mu_charge_pl_out = mu_pt_bare_out = mu_eta_bare_out = mu_phi_bare_out = mu_e_bare_out = 0;

  
  // Other
  Float_t met_pl_out;
  Float_t met_phi_pl_out;
  UInt_t runNumber_pl_out;
  ULong64_t eventNumber_pl_out;
  Int_t topHeavyFlavorFilterFlag_pl_out;

  
  // Weights
  Float_t w_mc_pl_out;
  Float_t w_pu_pl_out;
  

  // Computable variables - MVA discriminating variables
  std::vector<Float_t> *dR_jet_lep0_pl_out;
  std::vector<Float_t> *dR_jet_lep1_pl_out;
  std::vector<Float_t> *min_dR_jet_lep_pl_out;
  std::vector<Float_t> *m_jet_el_pl_out;
  std::vector<Float_t> *m_jet_mu_pl_out;
  std::vector<Float_t> *m_jet_lep_max_pl_out;
  std::vector<Float_t> *jet_m_pl_out;
  std::vector<Float_t> *m_min_jet_jet_pl_out;
  std::vector<Float_t> *m_max_jet_jet_pl_out;
  std::vector<Float_t> *min_dR_jet_bjet_pl_out;

  dR_jet_lep0_pl_out = dR_jet_lep1_pl_out = min_dR_jet_lep_pl_out = m_jet_el_pl_out = m_jet_mu_pl_out = m_jet_lep_max_pl_out = jet_m_pl_out = m_min_jet_jet_pl_out = m_max_jet_jet_pl_out = min_dR_jet_bjet_pl_out = 0;



  // ///
  // Create the branches
  out_tree->Branch("jet_pt", &jet_pt_out);
  out_tree->Branch("jet_eta", &jet_eta_out);
  out_tree->Branch("jet_phi", &jet_phi_out);
  out_tree->Branch("jet_e", &jet_e_out);
  out_tree->Branch("jet_jvt", &jet_jvt_out);
  out_tree->Branch("jet_DL1r", &jet_DL1r_out);
  out_tree->Branch("jet_isbtagged_DL1r_77", &jet_DL1r_77_out);
  out_tree->Branch("jet_tagWeightBin_DL1r_Continuous", &jet_tagWeightBin_DL1r_Continuous_out);
  out_tree->Branch("jet_truthflav", &jet_truthflav_out);
  out_tree->Branch("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag_out);
  out_tree->Branch("jet_nGhosts_bHadron", &jet_nGhosts_bHadron_out);
  out_tree->Branch("jet_nGhosts_cHadron", &jet_nGhosts_cHadron_out);
  out_tree->Branch("jet_truthPartonLabel", &jet_truthPartonLabel_out);
  out_tree->Branch("jet_isTrueHS", &jet_isTrueHS_out);
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
  out_tree->Branch("el_true_type", &el_true_type_out);
  out_tree->Branch("el_true_origin", &el_true_origin_out);
  out_tree->Branch("el_true_firstEgMotherTruthType", &el_true_firstEgMotherTruthType_out);
  out_tree->Branch("el_true_firstEgMotherTruthOrigin", &el_true_firstEgMotherTruthOrigin_out);
  out_tree->Branch("el_true_firstEgMotherPdgId", &el_true_firstEgMotherPdgId_out);
  out_tree->Branch("el_true_isPrompt", &el_true_isPrompt_out);
  out_tree->Branch("el_true_isChargeFl", &el_true_isChargeFl_out);

  out_tree->Branch("mu_pt", &mu_pt_out);
  out_tree->Branch("mu_eta", &mu_eta_out);
  out_tree->Branch("mu_phi", &mu_phi_out);
  out_tree->Branch("mu_charge", &mu_charge_out);
  out_tree->Branch("mu_e", &mu_e_out);
  out_tree->Branch("mu_topoetcone20", &mu_topoetcone20_out);
  out_tree->Branch("mu_ptvarcone30", &mu_ptvarcone30_out);
  out_tree->Branch("mu_d0sig", &mu_d0sig_out);
  out_tree->Branch("mu_delta_z0_sintheta", &mu_delta_z0_sintheta_out);
  out_tree->Branch("mu_true_type", &mu_true_type_out);
  out_tree->Branch("mu_true_origin", &mu_true_origin_out);
  out_tree->Branch("mu_true_isPrompt", &mu_true_isPrompt_out);

  out_tree->Branch("met_met", &met_out, "met_met/F");
  out_tree->Branch("met_phi", &met_phi_out, "met_phi/F");
  out_tree->Branch("runNumber", &runNumber_out, "runNumber/I");
  out_tree->Branch("eventNumber", &eventNumber_out, "eventNumber/l");
  out_tree->Branch("topHeavyFlavorFilterFlag", &topHeavyFlavorFilterFlag_out, "topHeavyFlavorFilterFlag/I");
  
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

  out_tree->Branch("weight_mc", &w_mc_out, "weight_mc/F");
  out_tree->Branch("weight_pileup", &w_pu_out, "weight_pileup/F");
  out_tree->Branch("weight_leptonSF", &w_leptonSF_out, "weight_leptonSF/F");
  out_tree->Branch("weight_bTagSF_DL1r_77", &w_DL1r_77_out, "weight_bTagSF_DL1r_77/F");
  out_tree->Branch("weight_jvt", &w_jvt_out, "weight_jvt/F");

  out_tree->Branch("vertex_type", &vertex_type_out);
  out_tree->Branch("vertex_z", &vertex_z_out);


  out_tree_pl->Branch("jet_pt", &jet_pt_pl_out);
  out_tree_pl->Branch("jet_eta", &jet_eta_pl_out);
  out_tree_pl->Branch("jet_phi", &jet_phi_pl_out);
  out_tree_pl->Branch("jet_e", &jet_e_pl_out);
  out_tree_pl->Branch("jet_nGhosts_bHadron", &jet_nGhosts_bHadron_pl_out);
  out_tree_pl->Branch("jet_nGhosts_cHadron", &jet_nGhosts_cHadron_pl_out);
  out_tree_pl->Branch("jet_truthPartonLabel", &jet_truthPartonLabel_pl_out);
  out_tree_pl->Branch("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag_pl_out);
  out_tree_pl->Branch(tmva_config_info[0], &MVA_score_pl);

  out_tree_pl->Branch("el_pt", &el_pt_pl_out);
  out_tree_pl->Branch("el_eta", &el_eta_pl_out);
  out_tree_pl->Branch("el_phi", &el_phi_pl_out);
  out_tree_pl->Branch("el_e", &el_e_pl_out);
  out_tree_pl->Branch("el_charge", &el_charge_pl_out);
  out_tree_pl->Branch("el_pt_bare", &el_pt_bare_out);
  out_tree_pl->Branch("el_eta_bare", &el_eta_bare_out);
  out_tree_pl->Branch("el_phi_bare", &el_phi_bare_out);
  out_tree_pl->Branch("el_e_bare", &el_e_bare_out);

  out_tree_pl->Branch("mu_pt", &mu_pt_pl_out);
  out_tree_pl->Branch("mu_eta", &mu_eta_pl_out);
  out_tree_pl->Branch("mu_phi", &mu_phi_pl_out);
  out_tree_pl->Branch("mu_e", &mu_e_pl_out);
  out_tree_pl->Branch("mu_charge", &mu_charge_pl_out);
  out_tree_pl->Branch("mu_pt_bare", &mu_pt_bare_out);
  out_tree_pl->Branch("mu_eta_bare", &mu_eta_bare_out);
  out_tree_pl->Branch("mu_phi_bare", &mu_phi_bare_out);
  out_tree_pl->Branch("mu_e_bare", &mu_e_bare_out);

  out_tree_pl->Branch("met_met", &met_pl_out, "met_met/F");
  out_tree_pl->Branch("met_phi", &met_phi_pl_out, "met_phi/F");
  out_tree_pl->Branch("runNumber", &runNumber_pl_out, "runNumber/i");
  out_tree_pl->Branch("eventNumber", &eventNumber_pl_out, "eventNumber/l");
  out_tree_pl->Branch("topHeavyFlavorFilterFlag", &topHeavyFlavorFilterFlag_pl_out, "topHeavyFlavorFilterFlag/I");

  out_tree_pl->Branch("weight_mc", &w_mc_pl_out, "weight_mc/F");
  out_tree_pl->Branch("weight_pileup", &w_pu_pl_out, "weight_puliup/F");

  out_tree_pl->Branch("dR_jet_lep0", &dR_jet_lep0_pl_out);
  out_tree_pl->Branch("dR_jet_lep1", &dR_jet_lep1_pl_out);
  out_tree_pl->Branch("min_dR_jet_lep", &min_dR_jet_lep_pl_out);
  out_tree_pl->Branch("m_jet_el", &m_jet_el_pl_out);
  out_tree_pl->Branch("m_jet_mu", &m_jet_mu_pl_out);
  out_tree_pl->Branch("m_jet_lep_max", &m_jet_lep_max_pl_out);
  out_tree_pl->Branch("jet_m", &jet_m_pl_out);
  out_tree_pl->Branch("m_min_jet_jet", &m_min_jet_jet_pl_out);
  out_tree_pl->Branch("m_max_jet_jet", &m_max_jet_jet_pl_out);
  out_tree_pl->Branch("min_dR_jet_bjet", &min_dR_jet_bjet_pl_out);
