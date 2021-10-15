
  // Signal - b-tags  not from top
  if ( (*topHadronOriginFlag)[jet_i]!=4 ) {
    dR_jet_lep0_MVA_sig = dR0;
    dR_jet_lep1_MVA_sig = dR1;
    min_dR_jet_lep_MVA_sig = std::min(dR0, dR1);
    m_jet_lep_min_dR_MVA_sig = m_jet_lep;
    m_jet_el_MVA_sig = (jets_lvec[jet_i] + el_lvec).M();
    m_jet_mu_MVA_sig = (jets_lvec[jet_i] + mu_lvec).M();
    m_jet_lep_max_MVA_sig = m_jet_lep_max;
    m_jet_lep_min_MVA_sig = m_jet_lep_min;
    min_dR_jet_bjet_MVA_sig = min_dR_jet_bjet;
    m_min_jet_jet_MVA_sig = m_min_jet_jet;
    m_max_jet_jet_MVA_sig = m_max_jet_jet;
    tot_event_weight_MVA_sig = weight;
    jet_pt_MVA_sig = (*jet_pt)[jet_i];
    jet_eta_MVA_sig = (*jet_eta)[jet_i];
    jet_m_MVA_sig = jet_m*1000; // GeV --> MeV
    topHOF_sig = (*topHadronOriginFlag)[jet_i];
    
    MVA_sig_tree->Fill();
  }
  
  // Bkg - b-tags not from top
  else {
    dR_jet_lep0_MVA_bkg = dR0;
    dR_jet_lep1_MVA_bkg = dR1;
    min_dR_jet_lep_MVA_bkg = std::min(dR0, dR1);
    m_jet_lep_min_dR_MVA_bkg = m_jet_lep;
    m_jet_el_MVA_bkg = (jets_lvec[jet_i] + el_lvec).M();
    m_jet_mu_MVA_bkg = (jets_lvec[jet_i] + mu_lvec).M();
    m_jet_lep_max_MVA_bkg = m_jet_lep_max;
    m_jet_lep_min_MVA_bkg = m_jet_lep_min;
    min_dR_jet_bjet_MVA_bkg = min_dR_jet_bjet;
    m_min_jet_jet_MVA_bkg = m_min_jet_jet;
    m_max_jet_jet_MVA_bkg = m_max_jet_jet;
    tot_event_weight_MVA_bkg = weight;
    jet_pt_MVA_bkg = (*jet_pt)[jet_i];
    jet_eta_MVA_bkg = (*jet_eta)[jet_i];
    jet_m_MVA_bkg = (*jet_eta)[jet_i];
    jet_m_MVA_bkg = jet_m*1000; // GeV --> MeV
    topHOF_bkg = (*topHadronOriginFlag)[jet_i];

    MVA_bkg_tree->Fill();
  }
