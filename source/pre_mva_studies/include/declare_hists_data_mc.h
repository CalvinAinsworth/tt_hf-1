  // jets_n - data/mc
  TH1 *h_jets_n = new TH1F("h_jets_n", "h_jets_n", 8, 2, 10);
  TH1 *h_btags_n = new TH1F("h_btags_n", "h_btags_n", 6, 2, 8);

  //Disciminative Variables 
  TH1 *h_dR_jel = new TH1F("h_dR_jel", "h_dR_jel", 40, 0, 5);
  TH1 *h_dR_jmu = new TH1F("h_dR_jmu", "h_dR_jmu", 40, 0, 5);
  TH1 *h_dR_jl = new TH1F("h_dR_jl", "h_dR_jl", 40, 0, 5);
  TH1 *h_dR_jj = new TH1F("h_dR_jj", "h_dR_jj", 40, 0, 5);
  TH1 *h_m_jel = new TH1F("h_m_jel", "h_m_jel", 40, 0, 800);
  TH1 *h_m_jmu = new TH1F("h_m_jmu", "h_m_jmu", 40, 0, 800);
  TH1 *h_m_jl_closest = new TH1F("h_m_jl_closest", "h_m_jl_closest", 40, 0, 800);// invariant mass of the jet + closest lepton
  TH1 *h_m_jl_min = new TH1F("h_m_jl_min", "h_m_jl_min", 40, 0, 800);// minimal invariant mass of jet-lepton pair

  // MVA variables hists - data/mc 
  TH1 *h_m_btag_lep_min_dR = new TH1F("h_m_btag_lep_min_dR", "h_m_btag_lep_min_dR", 50, 0, 1000);
  TH1 *h_m_btag_lep_min = new TH1F("h_m_btag_lep_min", "h_m_btag_lep_min", 50, 0, 1000);
  TH1 *h_m_btag_lep_max = new TH1F("h_m_btag_lep_max", "h_m_btag_lep_max", 50, 0, 1000);
  TH1 *h_m_btag_el = new TH1F("h_m_bjet_el", "h_m_bjet_el", 50, 0, 1000);
  TH1 *h_m_btag_mu = new TH1F("h_m_bjet_mu", "h_m_bjet_mu", 50, 0, 1000);
  TH1 *h_dR_btag_lep0 = new TH1F("h_dR_btag_lep0", "h_dR_btag_lep0", 28, 0, 7);
  TH1 *h_dR_btag_lep1 = new TH1F("h_dR_btag_lep1", "h_dR_btag_lep1", 28, 0, 7);
  TH1 *h_min_dR_btag_lep = new TH1F("h_min_dR_btag_lep", "h_min_dR_btag_lep", 28, 0, 7);
  TH1 *h_min_dR_jet_btag = new TH1F("h_min_dR_jet_bjet", "h_min_dR_jet_bjet", 28, 0, 7);


  // jets parameters - data/mc
  TH1 *h_all_jets_pt = new TH1F("all_jets_pt", "all_jets_pt", 20, 0, 1000);
  TH1 *h_all_jets_eta = new TH1F("all_jets_eta", "all_jets_eta", 40, -4, 4);
  TH1 *h_all_jets_phi = new TH1F("all_jets_phi", "all_jets_phi", 40, -4, 4);


  // Invariant mass of btags combinations - data/mc
  TH1 *h_m_btag = new TH1F("h_m_btag", "h_m_btag", 25, 0, 100);
  TH1 *h_m_notbtag = new TH1F("h_m_other", "h_m_other", 25, 0, 100);
  TH1 *h_m_btag_btag = new TH1F("h_m_btag_btag", "h_m_btag_btag", 100, 0, 1000);
  TH1 *h_m_btag_notbtag = new TH1F("h_m_btag_other", "h_m_btag_other", 100, 0, 1000);
  TH1 *h_m_notbtag_notbtag = new TH1F("h_m_other_other", "h_m_other_other", 100, 0, 1000);
