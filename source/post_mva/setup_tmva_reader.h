  // Create the reader object 		
  TMVA::Reader *reader = new TMVA::Reader("!Color=!Silent");

  // Create a set of variables and declare them to the reader
  float dR_jet_lep0_reader, dR_jet_lep1_reader, min_dR_jet_lep_reader, m_jet_lep_min_dR_reader, m_jet_el_reader, m_jet_mu_reader, m_jet_lep_max_reader, m_jet_lep_min_reader, min_dR_jet_bjet_reader, jet_pt_reader, jet_eta_reader, jet_m_reader, m_min_jet_jet_reader, m_max_jet_jet_reader;
  reader->AddVariable("dR_jet_lep0", &dR_jet_lep0_reader);
  reader->AddVariable("dR_jet_lep1", &dR_jet_lep1_reader);
  reader->AddVariable("min_dR_jet_lep", &min_dR_jet_lep_reader);
  reader->AddVariable("m_jet_el", &m_jet_el_reader);
  reader->AddVariable("m_jet_mu", &m_jet_mu_reader);
  reader->AddVariable("m_jet_lep_max", &m_jet_lep_max_reader);
  reader->AddVariable("min_dR_jet_bjet", &min_dR_jet_bjet_reader);
  reader->AddVariable("jet_pt", &jet_pt_reader);
  reader->AddVariable("jet_eta", &jet_eta_reader);
  reader->AddVariable("jet_m", &jet_m_reader);
  reader->AddVariable("m_min_jet_jet", &m_min_jet_jet_reader);
  reader->AddVariable("m_max_jet_jet", &m_max_jet_jet_reader);

  // Add spectators from the training
  /*int topHadronOriginFlag_reader, jet_isbtagged_DL1r_77_reader, jet_truthflav_reader;
  int eventNumber_reader;
  float jet_DL1r_reader, tot_event_weight_reader, met_reader, jet_phi_reader, jet_e_reader, mu_pt_reader, mu_eta_reader, mu_phi_reader, mu_e_reader, mu_charge_reader, el_pt_reader, el_eta_reader, el_phi_reader, el_e_reader, el_charge_reader;
  reader->AddSpectator("topHadronOriginFlag", &topHadronOriginFlag_reader);
  reader->AddSpectator("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77_reader);
  reader->AddSpectator("jet_truthflav", &jet_truthflav_reader);
  reader->AddSpectator("jet_DL1r", &jet_DL1r_reader);
  reader->AddSpectator("event_number", &eventNumber_reader);
  reader->AddSpectator("tot_event_weight", &tot_event_weight_reader);
  reader->AddSpectator("met", &met_reader);
  reader->AddSpectator("jet_phi", &jet_phi_reader);
  reader->AddSpectator("jet_e", &jet_e_reader);
  reader->AddSpectator("mu_pt", &mu_pt_reader);
  reader->AddSpectator("mu_eta", &mu_eta_reader);
  reader->AddSpectator("mu_phi", &mu_phi_reader);
  reader->AddSpectator("mu_e", &mu_e_reader);
  reader->AddSpectator("mu_charge", &mu_charge_reader);
  reader->AddSpectator("el_pt", &el_pt_reader);
  reader->AddSpectator("el_eta", &el_eta_reader);
  reader->AddSpectator("el_phi", &el_phi_reader);
  reader->AddSpectator("el_e", &el_e_reader);
  reader->AddSpectator("el_charge", &el_charge_reader);
  */
  // Book MVA method
  //TString dir = "../TMVA/dataset/weights/";
  TString dir = "dataset/weights/";
  reader->BookMVA(tmva_config_info[0], dir+"MVAnalysis_"+tmva_config_info[0]+".weights.xml");
  
