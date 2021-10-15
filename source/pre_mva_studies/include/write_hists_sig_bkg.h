
  // dR_min between btags and leptons - sig/bkg discrimination 
  h_min_dR_lep0_btags_from_top->Write("2b_emu_OS_min_dR_lep0_btag_from_top");
  h_min_dR_lep1_btags_from_top->Write("2b_emu_OS_min_dR_lep1_btag_from_top");
  h_min_dR_lep0_btags_not_from_top->Write("2b_emu_OS_min_dR_lep0_btag_not_from_top");
  h_min_dR_lep1_btags_not_from_top->Write("2b_emu_OS_min_dR_lep1_btag_not_from_top");

  // dR_min - sig/bkg discrimination
  h_min_dR_btag_from_top_to_btag->Write("2b_emu_OS_h_min_dR_btag_from_top_to_btag");
  h_min_dR_btag_not_from_top_to_btag->Write("2b_emu_OS_h_min_dR_btag_not_from_top_to_btag");
  h_min_dR_not_btag_to_btag->Write("2b_emu_OS_h_min_dR_not_btag_to_btag");
  h_min_dR_btag_from_top_to_jet->Write("2b_emu_OS_h_min_dR_btag_from_top_to_jet");
  h_min_dR_btag_not_from_top_to_jet->Write("2b_emu_OS_h_min_dR_btag_not_from_top_to_jet");
  h_min_dR_not_btag_to_jet->Write("2b_emu_OS_h_min_dR_not_btag_to_jet");
  h_min_dR_btag_from_top_to_lep->Write("2b_emu_OS_h_min_dR_btag_from_top_to_lep");
  h_min_dR_btag_not_from_top_to_lep->Write("2b_emu_OS_h_min_dR_btag_not_from_top_to_lep");
  h_min_dR_not_btag_to_lep->Write("2b_emu_OS_h_min_dR_not_btag_to_lep");

  // jets parameters - sig/bkg discrimination
  h_all_jets_not_from_top_pt->Write("2b_emu_OS_all_jets_not_from_top_pt");
  h_all_jets_from_top_pt->Write("2b_emu_OS_all_jets_from_top_pt");
  h_all_jets_not_from_top_eta->Write("2b_emu_OS_all_jets_not_from_top_eta");
  h_all_jets_from_top_eta->Write("2b_emu_OS_all_jets_from_top_eta");
  h_all_jets_not_from_top_phi->Write("2b_emu_OS_all_jets_not_from_top_phi");
  h_all_jets_from_top_phi->Write("2b_emu_OS_all_jets_from_top_phi");
  
  // Invariant mass - sig/bkg discrimination
  h_m_lep_btag_from_top_min_dR->Write("2b_emu_OS_h_m_lep_btag_from_top_min_dR");
  h_m_lep_btag_not_from_top_min_dR->Write("2b_emu_OS_h_m_lep_btag_not_from_top_min_dR");
  h_min_m_lep_btag_from_top->Write("2b_emu_OS_h_min_m_lep_btag_from_top");
  h_max_m_lep_btag_from_top->Write("2b_emu_OS_h_max_m_lep_btag_from_top");
  h_min_m_lep_btag_not_from_top->Write("2b_emu_OS_h_min_m_lep_btag_not_from_top");
  h_max_m_lep_btag_not_from_top->Write("2b_emu_OS_h_max_m_lep_btag_not_from_top");
  h_min_m_lep_other_jet->Write("2b_emu_OS_h_min_m_lep_other_jet");
  h_max_m_lep_other_jet->Write("2b_emu_OS_h_max_m_lep_other_jet");
  
  // Invariant mass of btags (and bjets) combinations - sig/bkg discrimination
  h_m_btag_top->Write("2b_emu_OS_m_btag_from_top");
  h_m_btag_other->Write("2b_emu_OS_m_btag_not_from_top");
  h_m_btag_top_btag_top->Write("2b_emu_OS_m_btag_top_btag_top");
  h_m_min_btag_top_btag_top->Write("2b_emu_OS_m_min_btag_top_btag_top");
  h_m_max_btag_top_btag_top->Write("2b_emu_OS_m_max_btag_top_btag_top");
  h_m_btag_top_btag_other->Write("2b_emu_OS_m_btag_top_btag_other");
  h_m_min_btag_top_btag_other->Write("2b_emu_OS_m_min_btag_top_btag_other");
  h_m_max_btag_top_btag_other->Write("2b_emu_OS_m_max_btag_top_btag_other");
  h_m_btag_other_btag_other->Write("2b_emu_OS_m_btag_other_btag_other");
  h_m_min_btag_other_btag_other->Write("2b_emu_OS_m_min_btag_other_btag_other");
  h_m_max_btag_other_btag_other->Write("2b_emu_OS_m_max_btag_other_btag_other");
  h_m_three_tags->Write("3b_emu_OS_m_three_btags_excl");
  h_m_four_tags->Write("4b_emu_OS_m_four_btags_incl");
