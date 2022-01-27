
  // jets_n - data/mc
  h_jets_n->Write("3b_emu_OS_jets_n.N_{jets}.nan");
  h_btags_n->Write("3b_emu_OS_btags_n.N_{btags}.nan");

  // MVA variables hists - data/mc
  h_m_btag_lep_min_dR->Write("MVA_3b_emu_OS_m_btag_lep_min_dR.m(btag, closest lep).GeV");
  h_m_btag_lep_min->Write("MVA_3b_emu_OS_m_btag_lep_min.m_{min}(btag, lep).GeV");
  h_m_btag_lep_max->Write("MVA_3b_emu_OS_m_btag_lep_max.m_{max}(btag, lep).GeV");
  h_m_btag_el->Write("MVA_3b_emu_OS_m_btag_el.m(btag, el).GeV");
  h_m_btag_mu->Write("MVA_3b_emu_OS_m_btag_mu.m(btag, mu).GeV");
  h_dR_btag_lep0->Write("MVA_3b_emu_OS_dR_btag_lep0.dR(btag, leading lep).nan");
  h_dR_btag_lep1->Write("MVA_3b_emu_OS_dR_btag_lep1.dR(btag, subleading lep).nan");
  h_min_dR_btag_lep->Write("MVA_3b_emu_OS_min_dR_btag_lep.dR(btag, closest lep).nan");
  h_min_dR_jet_btag->Write("MVA_3b_emu_OS_min_dR_jet_btag.dR(jet, closest btag).nan");

  // jets variables - data/mc
  h_all_jets_pt->Write("3b_emu_OS_all_jets_pt.p_{T}^{all jets}.GeV");
  h_all_jets_eta->Write("3b_emu_OS_all_jets_eta.eta(all jets).nan");
  h_all_jets_phi->Write("3b_emu_OS_all_jets_phi.phi(all jets).nan");

  // Invariant masses of btags and other and their combinations - data/mc
  h_m_btag->Write("3b_emu_OS_m_btag.m(btag)).GeV");
  h_m_notbtag->Write("3b_emu_OS_m_notbtag.m(not btag).GeV");
  h_m_btag_btag->Write("3b_emu_OS_m_btag_btag.m(btag, btag).GeV");
  h_m_btag_notbtag->Write("3b_emu_OS_m_btag_notbtag.m(btag, not btag).GeV");
  h_m_notbtag_notbtag->Write("3b_emu_OS_m_notgtag_notbtag.m(not btag, not btag).GeV");
