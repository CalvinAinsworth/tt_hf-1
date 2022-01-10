
  // dR_min between btags and leptons - sig/bkg discrimination
  TH1 *h_min_dR_lep0_btags_from_top = new TH1F("h_min_dR_lep0_btags_from_top", "h_min_dR_lep0_btags_from_top", 28, 0, 7);
  TH1 *h_min_dR_lep0_btags_not_from_top = new TH1F("h_min_dR_lep0_btags_not_from_top", "h_min_dR_lep0_btags_not_from_top", 28, 0, 7);
  TH1 *h_min_dR_lep1_btags_from_top = new TH1F("h_min_dR_lep1_btags_from_top", "h_min_dR_lep1_btags_from_top", 28, 0, 7);
  TH1 *h_min_dR_lep1_btags_not_from_top = new TH1F("h_min_dR_lep1_btags_not_from_top", "h_min_dR_lep1_btags_not_from_top", 28, 0, 7);


  // dR_min - sig/bkg discrimination
  TH1 *h_min_dR_btag_from_top_to_btag = new TH1F("h_min_dR_btag_from_top_to_btag", "h_min_dR_btag_from_top_to_btag", 28, 0, 7);
  TH1 *h_min_dR_btag_not_from_top_to_btag = new TH1F("h_min_dR_btag_not_from_top_to_btag", "h_min_dR_btag_not_from_top_to_btag", 28, 0, 7);
  TH1 *h_min_dR_not_btag_to_btag = new TH1F("h_min_dR_not_btag_to_btag", "h_min_dR_not_btag_to_btag", 28, 0, 7);
  TH1 *h_min_dR_btag_from_top_to_jet = new TH1F("h_min_dR_btag_from_top_to_jet", "h_min_dR_btag_from_top_to_jet", 20, 0, 7);
  TH1 *h_min_dR_btag_not_from_top_to_jet = new TH1F("h_min_dR_btag_not_from_top_to_jet", "h_min_dR_btag_not_from_top_to_jet", 28, 0, 7);
  TH1 *h_min_dR_not_btag_to_jet = new TH1F("h_min_dR_not_btag_to_jet", "h_min_dR_not_btag_to_jet", 28, 0, 7);
  TH1 *h_min_dR_btag_from_top_to_lep = new TH1F("h_min_dR_btag_from_top_to_lep", "h_min_dR_btag_from_top_to_lep", 20, 0, 7);
  TH1 *h_min_dR_btag_not_from_top_to_lep = new TH1F("h_min_dR_btag_not_from_top_to_lep", "h_min_dR_btag_not_from_top_to_lep", 28, 0, 7);
  TH1 *h_min_dR_not_btag_to_lep = new TH1F("h_min_dR_not_btag_to_lep", "h_min_dR_not_btag_to_lep", 28, 0, 7);


  // jets parameters - sig/bkg discrimination
  TH1 *h_all_jets_not_from_top_pt = new TH1F("all_jets_not_from_top_pt", "all_jets_not_from_top_pt", 50, 0, 1000);
  TH1 *h_all_jets_from_top_pt = new TH1F("all_jets_from_top_pt", "all_jets_from_top_pt", 50, 0, 1000);
  TH1 *h_all_jets_not_from_top_eta = new TH1F("all_jets_not_from_top_eta", "all_jets_not_from_top_eta", 40, -4, 4);
  TH1 *h_all_jets_from_top_eta = new TH1F("all_jets_from_top_eta", "all_jets_from_top_eta", 40, -4, 4);
  TH1 *h_all_jets_not_from_top_phi = new TH1F("all_jets_not_from_top_phi", "all_jets_not_from_top_phi", 40, -4, 4);
  TH1 *h_all_jets_from_top_phi = new TH1F("all_jets_from_top_phi", "all_jets_from_top_phi", 40, -4, 4);

  // Invariant mass - sig/bkg discrimination
  TH1 *h_m_lep_btag_from_top_min_dR = new TH1F("h_m_lep_btag_from_top_min_dR", "h_m_lep_btag_from_top_min_dR", 200, 0, 1000);
  TH1 *h_m_lep_btag_not_from_top_min_dR = new TH1F("h_m_lep_btag_not_from_top_min_dR", "h_m_lep_btag_not_from_top_min_dR", 200, 0, 1000);
  TH1 *h_min_m_lep_btag_from_top = new TH1F("h_min_m_lep_btag_from_top", "h_min_m_lep_btag_from_top", 200, 0, 1000);
  TH1 *h_max_m_lep_btag_from_top = new TH1F("h_max_m_lep_btag_from_top", "h_max_m_lep_btag_from_top", 200, 0, 1000);
  TH1 *h_min_m_lep_btag_not_from_top = new TH1F("h_min_m_lep_btag_not_from_top", "h_min_m_lep_btag_not_from_top", 200, 0, 1000);
  TH1 *h_max_m_lep_btag_not_from_top = new TH1F("h_max_m_lep_btag_not_from_top", "h_max_m_lep_btag_not_from_top", 200, 0, 1000);
  TH1 *h_min_m_lep_other_jet = new TH1F("h_min_m_lep_other_jet", "h_min_m_lep_other_jet", 200, 0, 1000);
  TH1 *h_max_m_lep_other_jet = new TH1F("h_max_m_lep_other_jet", "h_max_m_lep_other_jet", 200, 0, 1000);
  TH1 *h_m_btag_el_from_top = new TH1F("m_btag_el_from_top", "m_btag_el_from_top", 25, 0, 500);
  TH1 *h_m_btag_el_not_from_top = new TH1F("m_btag_el_not_from_top", "m_btag_el_not_from_top", 25, 0, 500);
  TH1 *h_m_btag_mu_from_top = new TH1F("m_btag_mu_from_top", "m_btag_mu_from_top", 25, 0, 500);
  TH1 *h_m_btag_mu_not_from_top = new TH1F("m_btag_mu_not_from_top", "m_btag_mu_not_from_top", 25, 0, 500);



  // Invariant mass of btags (and bjets) combinations - sig/bkg discrimination
  TH1 *h_m_btag_top = new TH1F("h_m_btag_top", "h_m_btag_top", 100, 0, 100);
  TH1 *h_m_btag_other = new TH1F("h_m_btag_not_from_top", "h_m_btag_not_from_top", 100, 0, 100);
  TH1 *h_m_btag_top_btag_top = new TH1F("h_m_btag_top_btag_top", "h_m_btag_top_btag_top", 100, 0, 500);
  TH1 *h_m_min_btag_top_btag_top = new TH1F("h_m_min_btag_top_btag_top", "h_m_min_btag_top_btag_top", 100, 0, 500);
  TH1 *h_m_max_btag_top_btag_top = new TH1F("h_m_max_btag_top_btag_top", "h_m_max_btag_top_btag_top", 100, 0, 500);
  TH1 *h_m_btag_top_btag_other = new TH1F("h_m_btag_top_btag_other", "h_m_btag_top_btag_other", 100, 0, 500);
  TH1 *h_m_min_btag_top_btag_other = new TH1F("h_m_min_btag_top_btag_other", "h_m_min_btag_top_btag_other", 100, 0, 500);
  TH1 *h_m_max_btag_top_btag_other = new TH1F("h_m_max_btag_top_btag_other", "h_m_max_btag_top_btag_other", 100, 0, 500);
  TH1 *h_m_btag_other_btag_other = new TH1F("h_m_btag_other_btag_other", "h_m_btag_other_btag_other", 100, 0, 500);
  TH1 *h_m_min_btag_other_btag_other = new TH1F("h_m_min_btag_other_btag_other", "h_m_min_btag_other_btag_other", 100, 0, 500);
  TH1 *h_m_max_btag_other_btag_other = new TH1F("h_m_max_btag_other_btag_other", "h_m_max_btag_other_btag_other", 100, 0, 500);
  TH1 *h_m_three_tags = new TH1F("h_m_three_tags", "h_m_three_tags", 100, 0, 500);
  TH1 *h_m_four_tags = new TH1F("h_m_four_tags", "h_m_four_tags", 100, 0, 500);
