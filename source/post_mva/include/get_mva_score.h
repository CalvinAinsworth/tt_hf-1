  // Compute variables used in the MVA
  float dR_jet_lep0, dR_jet_lep1;
  if ( (*el_pt)[0] >= (*mu_pt)[0] ) {
    dR_jet_lep0 = jets_lvec[jet_i].DeltaR(el_lvec);
    dR_jet_lep1 = jets_lvec[jet_i].DeltaR(mu_lvec); }
  else {
    dR_jet_lep0 = jets_lvec[jet_i].DeltaR(mu_lvec);
    dR_jet_lep1 = jets_lvec[jet_i].DeltaR(el_lvec); }

  float min_dR_jet_lep = std::min(dR_jet_lep0, dR_jet_lep1);

  float m_jet_el, m_jet_mu, m_jet_lep_max;
  m_jet_el = (jets_lvec[jet_i] + el_lvec).M();
  m_jet_mu = (jets_lvec[jet_i] + mu_lvec).M();
  m_jet_lep_max = std::max(m_jet_el, m_jet_mu);

  float jet_m = (jets_lvec[jet_i]).M();

  float m_min_jet_jet = 999999.0;
  float m_max_jet_jet = 0;
  float min_dR_jet_bjet = 999999.0;
  for (int jet_j=0; jet_j<(*jet_pt).size(); jet_j++) {
    if (jet_i==jet_j) continue;

    float m_jet_jet = (jets_lvec[jet_i] + jets_lvec[jet_j]).M();
    if (m_jet_jet <= m_min_jet_jet) m_min_jet_jet = m_jet_jet;
    if (m_jet_jet >= m_max_jet_jet) m_max_jet_jet = m_jet_jet;

    bool jet_is_b = false;
    if (obj_name!="particleLevel") {
      if ((*jet_DL1r_77)[jet_j]==1) jet_is_b = true;
    } else {
      if ( (*jet_nGhosts_bHadron)[jet_j]>=1) jet_is_b = true;
    }
    if (jet_is_b==true) {
      float dR_jet_bjet = jets_lvec[jet_i].DeltaR(jets_lvec[jet_j]);
      if (dR_jet_bjet <= min_dR_jet_bjet) min_dR_jet_bjet = dR_jet_bjet; 
    }
  
  } // [jet_j] - nested loop over jets
  
  
  // Set localVar = treeVar
  dR_jet_lep0_reader = dR_jet_lep0;
  dR_jet_lep1_reader = dR_jet_lep1;
  min_dR_jet_lep_reader = min_dR_jet_lep;
  m_jet_el_reader = m_jet_el;
  m_jet_mu_reader = m_jet_mu;

  float mvaValue = reader->EvaluateMVA(tmva_config_info[0]); 
