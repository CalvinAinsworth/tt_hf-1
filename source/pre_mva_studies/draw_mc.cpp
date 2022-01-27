#include "../common_functions.h"
#include "../draw_functions.h"


// ///
// MAIN
// ///
int main(int agrc, char *argv[])
{
  // Ask: Reco or PL plots to draw?
  // _pl.root or _reco.root
  TString reco_or_pl = "";
  std::cout << "Which hists do you want to draw?" << std::endl;
  std::cout << "-- Reco level: \"reco\"\n" << "-- Particle level: \"pl\"\nType in yout choice: ";
  std::cin >> reco_or_pl;
  std::cout << std::endl;
  while (reco_or_pl!="reco" && reco_or_pl!="pl" && reco_or_pl!="quit") {
    std::cout << "Invvalid choice. Select \"reco\" or \"pl\", or \"quit\" to quit";
    std::cin >> reco_or_pl;
    std::cout << std::endl;
  }
  if (reco_or_pl=="quit") return 0;
  TString savedir = "mc/" + reco_or_pl + "/";


  // Get MC config info
  std::map<TString, TString> mc_config_info = get_mc_config_info(std::string(argv[1]));
  if (mc_config_info.size()==0) return 0;
  std::map<TString, TString>::iterator it;
  for (it=mc_config_info.begin(); it!=mc_config_info.end(); it++) {
    std::cout << it->first << " :\t" << it->second << std::endl;
  }
  TString process = mc_config_info["process"];
  TString generator = mc_config_info["generator"];
  TString lep_pt_cut_suffix = mc_config_info["lep_pt_cut_suffix"];
  TString campaign = mc_config_info["campaign"];


  gSystem->Exec("mkdir results/plots");
  gSystem->Exec("mkdir results/plots/mc");
  
  
  // Open the hist file
  TString hists_file_name = "results/hists_tt_"+reco_or_pl+"_nominal"+lep_pt_cut_suffix+".root";
  TFile *hists_file_mc = TFile::Open(hists_file_name);


  // Get histograms
  
  // dR_min - sig/bkg discrimination
  TH1 *h_min_dR_lep0_btags_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_min_dR_lep0_btag_from_top");
  TH1 *h_min_dR_lep1_btags_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_min_dR_lep1_btag_from_top");
  TH1* h_min_dR_lep0_btags_not_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_min_dR_lep0_btag_not_from_top");
  TH1 *h_min_dR_lep1_btags_not_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_min_dR_lep1_btag_not_from_top");
  TH1 *h_min_dR_btag_from_top_to_btag = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_dR_btag_from_top_to_btag");
  TH1 *h_min_dR_btag_not_from_top_to_btag = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_dR_btag_not_from_top_to_btag");
  TH1 *h_min_dR_btag_from_top_to_jet = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_dR_btag_from_top_to_jet");
  TH1 *h_min_dR_btag_not_from_top_to_jet = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_dR_btag_not_from_top_to_jet");
  TH1 *h_min_dR_btag_from_top_to_lep = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_dR_btag_from_top_to_lep");
  TH1 *h_min_dR_btag_not_from_top_to_lep = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_dR_btag_not_from_top_to_lep");

  // jets parameters - sig/bkg discrimination
  TH1 *h_all_jets_not_from_top_pt = (TH1*)hists_file_mc->Get("3b_emu_OS_all_jets_not_from_top_pt");
  TH1 *h_all_jets_from_top_pt = (TH1*)hists_file_mc->Get("3b_emu_OS_all_jets_from_top_pt");
  TH1 *h_all_jets_not_from_top_eta = (TH1*)hists_file_mc->Get("3b_emu_OS_all_jets_not_from_top_eta");
  TH1 *h_all_jets_from_top_eta = (TH1*)hists_file_mc->Get("3b_emu_OS_all_jets_from_top_eta");
  TH1 *h_all_jets_not_from_top_phi = (TH1*)hists_file_mc->Get("3b_emu_OS_all_jets_not_from_top_phi");
  TH1 *h_all_jets_from_top_phi = (TH1*)hists_file_mc->Get("3b_emu_OS_all_jets_from_top_phi");

  // invariant mass - sig/bkg discrimination 
  TH1 *h_m_lep_btag_from_top_min_dR = (TH1*)hists_file_mc->Get("3b_emu_OS_h_m_lep_btag_from_top_min_dR");
  TH1 *h_m_lep_btag_not_from_top_min_dR = (TH1*)hists_file_mc->Get("3b_emu_OS_h_m_lep_btag_not_from_top_min_dR");
  TH1 *h_min_m_lep_btag_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_m_lep_btag_from_top");
  TH1 *h_max_m_lep_btag_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_max_m_lep_btag_from_top");
  TH1 *h_min_m_lep_btag_not_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_min_m_lep_btag_not_from_top");
  TH1 *h_max_m_lep_btag_not_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_max_m_lep_btag_not_from_top");

  // invariant mass of btags and their combinations
  TH1 *h_m_btag_top = (TH1*)hists_file_mc->Get("3b_emu_OS_m_btag_from_top");
  TH1 *h_m_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_btag_not_from_top");
  TH1 *h_m_btag_top_btag_top = (TH1*)hists_file_mc->Get("3b_emu_OS_m_btag_top_btag_top");
  TH1 *h_m_min_btag_top_btag_top = (TH1*)hists_file_mc->Get("3b_emu_OS_m_min_btag_top_btag_top");
  TH1 *h_m_max_btag_top_btag_top = (TH1*)hists_file_mc->Get("3b_emu_OS_m_max_btag_top_btag_top");
  TH1 *h_m_btag_top_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_btag_top_btag_other");
  TH1 *h_m_min_btag_top_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_min_btag_top_btag_other");
  TH1 *h_m_max_btag_top_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_max_btag_top_btag_other");
  TH1 *h_m_btag_other_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_btag_other_btag_other");
  TH1 *h_m_min_btag_other_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_min_btag_other_btag_other");
  TH1 *h_m_max_btag_other_btag_other = (TH1*)hists_file_mc->Get("3b_emu_OS_m_max_btag_other_btag_other");
  TH1 *h_m_three_tags = (TH1*)hists_file_mc->Get("3b_emu_OS_m_three_btags_excl");
  TH1 *h_m_four_tags = (TH1*)hists_file_mc->Get("4b_emu_OS_m_four_btags_incl");

  // invariant mass of btar and el/mu - sig/bkg discrimination
  TH1 *h_m_btag_el_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_m_btag_el_from_top");
  TH1 *h_m_btag_el_not_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_m_btag_el_not_from_top");
  TH1 *h_m_btag_mu_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_m_btag_mu_from_top");
  TH1 *h_m_btag_mu_not_from_top = (TH1*)hists_file_mc->Get("3b_emu_OS_h_m_btag_mu_not_from_top");



  // Draw plots
  // (1) prepare different combinations of hists (vectors) to draw them on one canvas
  // (2) call a function to draw the collection of hists on one canvas
  
  // Common legend entries for sig/bkg comparisons
  std::vector<TString> discr_legend_titles = {"from top", "not from top"};


  // dR_btag_lep0
  std::vector<TH1*> h_min_dR_lep0_btags = {h_min_dR_lep0_btags_from_top, h_min_dR_lep0_btags_not_from_top};
  int draw_min_dR_lep0_btags = draw_n_hists(h_min_dR_lep0_btags, discr_legend_titles, "#bf{#DeltaR(btag, leading lepton)}", savedir+"dR_btag_lep0", true, 2*pow(10,-3), 0.2);

  // dR_btag_lep1
  std::vector<TH1*> h_min_dR_lep1_btags = {h_min_dR_lep1_btags_from_top, h_min_dR_lep1_btags_not_from_top};
  int draw_min_dR_lep1_btags = draw_n_hists(h_min_dR_lep1_btags, discr_legend_titles, "#bf{#DeltaR(btag, sub-leading lepton)}", savedir+"dR_btag_lep1", true, 2*pow(10,-3), 0.2);

  // dR_min(btag, other btag)
  std::vector<TH1*> h_min_dR_btag_to_btag = {h_min_dR_btag_from_top_to_btag, h_min_dR_btag_not_from_top_to_btag};
  int draw_min_dR_btag_to_btag = draw_n_hists(h_min_dR_btag_to_btag, discr_legend_titles, "#bf{#DeltaR_{min}(btag, other btag)}", savedir+"min_dR_btag_to_btag", true, 3*pow(10,-3), 0.3);

  // dR_min(btag, not btag)
  std::vector<TH1*> h_min_dR_btag_to_jet = {h_min_dR_btag_from_top_to_jet, h_min_dR_btag_not_from_top_to_jet};
  int draw_min_dR_btag_to_jet = draw_n_hists(h_min_dR_btag_to_jet, discr_legend_titles, "#bf{#DeltaR_{min}(btag, not btag)}", savedir+"min_dR_btag_to_not_b", true, 3*pow(10,-3), 0.3);

  // dR_min(btag, lep)
  std::vector<TH1*> h_min_dR_btag_to_lep = {h_min_dR_btag_from_top_to_lep, h_min_dR_btag_not_from_top_to_lep};
  int draw_min_dR_btag_to_lep = draw_n_hists(h_min_dR_btag_to_lep, discr_legend_titles, "#bf{#DeltaR_{min}(btag, lep)}", savedir+"min_dR_btag_to_lep", true, 3*pow(10,-3), 0.3);

  // m(btag, closest lep)
  std::vector<TH1*> h_m_lep_btag = {h_m_lep_btag_from_top_min_dR, h_m_lep_btag_not_from_top_min_dR};
  int drw_m_lep_btag = draw_n_hists(h_m_lep_btag, discr_legend_titles, "#bf{m(btag, closest lep)}", savedir+"m_btag_closest_lep", true, pow(10,-3), 0.1);

  // m_min(btag, lep)
  std::vector<TH1*> h_min_m_lep_btag = {h_min_m_lep_btag_from_top, h_min_m_lep_btag_not_from_top};
  int draw_min_m_lep_btag = draw_n_hists(h_min_m_lep_btag, discr_legend_titles, "#bf{m_{min}(btag, lep)}", savedir+"m_min_btag_lep", true, pow(10,-3), 0.1);

  // m_max(btag, lep) 
  std::vector<TH1*> h_max_m_lep_btag = {h_max_m_lep_btag_from_top, h_max_m_lep_btag_not_from_top};
  int draw_max_m_lep_btag = draw_n_hists(h_max_m_lep_btag, discr_legend_titles, "#bf{m_{max}(btag, lep)}", savedir+"m_max_btag_lep", true, pow(10,-3), 0.1);


  // jet_pt
  std::vector<TH1*> h_all_jets_pt = {h_all_jets_from_top_pt, h_all_jets_not_from_top_pt};
  int draw_all_jets_pt = draw_n_hists(h_all_jets_pt, discr_legend_titles, "#bf{p_{T}^{jets}}", savedir+"all_jets_pt", true, 0.01, 1.0);

  // jet_eta
  std::vector<TH1*> h_all_jets_eta = {h_all_jets_from_top_eta, h_all_jets_not_from_top_eta};
  int draw_all_jets_eta = draw_n_hists(h_all_jets_eta, discr_legend_titles, "#bf{jet #eta}", savedir+"all_jets_eta", true, 0.01, 0.1);

  // jet_phi
  std::vector<TH1*> h_all_jets_phi = {h_all_jets_from_top_phi, h_all_jets_not_from_top_phi};
  int draw_all_jets_phi = draw_n_hists(h_all_jets_phi, discr_legend_titles, "#bf{jet #phi}", savedir+"all_jets_phi", true, 7*pow(10,-3), 7*pow(10,-2));

  // btag invariant mass
  std::vector<TH1*> h_m_btag = {h_m_btag_top, h_m_btag_other};
  int draw_m_btag = draw_n_hists(h_m_btag, discr_legend_titles, "#bf{m(btag)}", savedir+"m_btag", true, 2*pow(10,-3), 2*pow(10,-1));

  // three pairs of btags wrt origin: {two from top}, {one from top}, {none from top}
  std::vector<TString> m_btags_pairs_legend = {"top-top", "top-other", "other-other"};
  std::vector<TH1*> h_m_btags_pairs = {h_m_btag_top_btag_top, h_m_btag_top_btag_other, h_m_btag_other_btag_other};
  int draw_m_btags_pairs = draw_n_hists(h_m_btags_pairs, m_btags_pairs_legend, "#bf{m(btag, btag)}", savedir+"m_btag_btag", true, 5*pow(10,-4), 5*pow(10,-2));
  std::vector<TH1*> h_m_min_btags_pairs = {h_m_min_btag_top_btag_top, h_m_min_btag_top_btag_other, h_m_min_btag_other_btag_other};
  int draw_m_min_btags_pairs = draw_n_hists(h_m_min_btags_pairs, m_btags_pairs_legend, "#bf{m_{min}(btag, btag)}", savedir+"m_min_btag_btag", true, 5*pow(10,-4), 5*pow(10,-2));
  std::vector<TH1*> h_m_max_btags_pairs = {h_m_max_btag_top_btag_top, h_m_max_btag_top_btag_other, h_m_max_btag_other_btag_other};
  int draw_m_max_btags_pairs = draw_n_hists(h_m_max_btags_pairs, m_btags_pairs_legend, "#bf{m_{max}(btag, btag)}", savedir+"m_max_btag_btag", true, 5*pow(10,-4), 5*pow(10,-2));

  // m_btag_el - sig/bkg
  //std::vector<TH1*> h_m_btag_el = {h_m_btag_el_from_top, h_m_btag_el_not_from_top};
  //int draw_m_btag_el = draw_n_hists(h_m_btag_el, discr_legend_titles, "#bf{m(b, electron)}", savedir+"m_b_el", true, pow(10,-3), 1.0);

  // m_btag_mu - sig/bkg
  //std::vector<TH1*> h_m_btag_mu = {h_m_btag_mu_from_top, h_m_btag_mu_not_from_top};
  //int draw_m_btag_mu = draw_n_hists(h_m_btag_mu, discr_legend_titles, "#bf{m(b, muon)}", savedir+"m_b_mu", true, pow(10,-3), 1.0);

  // tree (excl) and four (incl) btags system masses
  //std::vector<TString> multiple_btags_legend = {"three btags (excl)", "four btags (incl)"};
  //std::vector<TH1*> h_multiple_btags = {h_m_three_tags, h_m_four_tags};
  //int draw_multiple_btags = draw_n_hists(h_multiple_btags, multiple_btags_legend, "#bf{m(N btags)}", "pl/m_n_btags", true, 0, 0.1);

  // Close the hists file
  hists_file_mc->Close();
  
  
  return 0;
} // END OF MAIN
