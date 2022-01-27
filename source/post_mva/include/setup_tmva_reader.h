  // Create the reader object 		
  TMVA::Reader *reader = new TMVA::Reader("!Color=!Silent");

  // Create a set of variables and declare them to the reader
  float dR_jet_lep0_reader, dR_jet_lep1_reader, min_dR_jet_lep_reader, m_jet_lep_min_dR_reader, m_jet_el_reader, m_jet_mu_reader, m_jet_lep_max_reader, m_jet_lep_min_reader, min_dR_jet_bjet_reader, jet_pt_reader, jet_eta_reader, jet_m_reader, m_min_jet_jet_reader, m_max_jet_jet_reader;
  int topHOF_reader;
  reader->AddVariable("dR_jet_lep0", &dR_jet_lep0_reader);
  reader->AddVariable("dR_jet_lep1", &dR_jet_lep1_reader);
  reader->AddVariable("min_dR_jet_lep", &min_dR_jet_lep_reader);
  reader->AddVariable("m_jet_el", &m_jet_el_reader);
  reader->AddVariable("m_jet_mu", &m_jet_mu_reader);
  
  reader->AddSpectator("jet_GBHInit_topHadronOriginFlag", &topHOF_reader);

  TString dir = "results/dataset_nominal" + lep_pt_cut_suffix + "/weights/";
  reader->BookMVA(tmva_config_info[0], dir+"MVAnalysis_"+tmva_config_info[0]+".weights.xml");
