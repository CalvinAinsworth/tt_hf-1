#include "../common_functions.h"
#include "../draw_functions.h"

// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");
  
  
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

  
  // Open the hists files
  TString hists_file_name = std::string("results/hists_bdt_eff_study_" + generator + lep_pt_cut_suffix + ".root");
  TFile *hists_file = TFile::Open(hists_file_name);


  // ///
  // Get histograms
  
  // MC hists
  std::vector<TString> discr_vars_str_mc = {"_reco_all", "_reco_bdt_ft", "_reco_bdt_nft", "_reco_truth_ft", "_reco_truth_nft", "_pl_all", "_pl_bdt_ft", "_pl_bdt_nft", "_pl_truth_ft", "_pl_truth_nft"};
  TH1 *h_dR_jet_lep0_mc[10];
  TH1 *h_dR_jet_lep1_mc[10];
  TH1 *h_min_dR_jet_lep_mc[10];
  TH1 *h_m_jet_el_mc[10];
  TH1 *h_m_jet_mu_mc[10];
  TH1 *h_jet_eta_mc[10];
  TH1 *h_mva_score_mc[10];
  for (int i=0; i<10; i++) {
    h_dR_jet_lep0_mc[i] = (TH1*)hists_file->Get("3b_incl_dR_jet_lep0" + discr_vars_str_mc[i]);
    h_dR_jet_lep1_mc[i] = (TH1*)hists_file->Get("3b_incl_dR_jet_lep1" + discr_vars_str_mc[i]);
    h_min_dR_jet_lep_mc[i] = (TH1*)hists_file->Get("3b_incl_min_dR_jet_lep" + discr_vars_str_mc[i]);
    h_m_jet_el_mc[i] = (TH1*)hists_file->Get("3b_incl_m_jet_el" + discr_vars_str_mc[i]);
    h_m_jet_mu_mc[i] = (TH1*)hists_file->Get("3b_incl_m_jet_mu" + discr_vars_str_mc[i]);
    h_jet_eta_mc[i] = (TH1*)hists_file->Get("3b_incl_jet_eta" + discr_vars_str_mc[i]);
    h_mva_score_mc[i] = (TH1*)hists_file->Get("3b_incl_mva_score" + discr_vars_str_mc[i]);
  }
  TH1* h_bjets_n_pl = (TH1*)hists_file->Get("bjets_n_pl");
  TH1* h_btags_n_reco = (TH1*)hists_file->Get("btags_n_reco");
  TH1* h_jets_n_ft_pl = (TH1*)hists_file->Get("jets_n_ft_pl");
  TH1* h_jets_n_ft_reco = (TH1*)hists_file->Get("jets_n_ft_reco");
  TH1* h_bjets_n_ft_pl = (TH1*)hists_file->Get("bjets_n_ft_pl");
  TH1* h_btags_n_ft_reco = (TH1*)hists_file->Get("btags_n_ft_reco");
  TH1* h_jets_origin_stat_pl_3b = (TH1*)hists_file->Get("jets_origin_stat_pl_3b");
  TH1* h_jets_origin_stat_reco_3b = (TH1*)hists_file->Get("jets_origin_stat_reco_3b");
  TH1* h_jets_origin_stat_pl_4b = (TH1*)hists_file->Get("jets_origin_stat_pl_4b");
  TH1* h_jets_origin_stat_reco_4b = (TH1*)hists_file->Get("jets_origin_stat_reco_4b");
  TH1* h_origin_tag_eff_pl_3b = (TH1*)hists_file->Get("origin_tag_eff_pl_3b");
  TH1* h_origin_tag_eff_reco_3b = (TH1*)hists_file->Get("origin_tag_eff_reco_3b");
  TH1* h_origin_tag_eff_pl_4b = (TH1*)hists_file->Get("origin_tag_eff_pl_4b");
  TH1* h_origin_tag_eff_reco_4b = (TH1*)hists_file->Get("origin_tag_eff_reco_4b");


  // Data hists
  std::vector<TString> discr_vars_str_data = {"_data_all", "_data_ft", "_data_nft"};
  TH1 *h_dR_jet_lep0_data[3];
  TH1 *h_dR_jet_lep1_data[3];
  TH1 *h_min_dR_jet_lep_data[3];
  TH1 *h_m_jet_el_data[3];
  TH1 *h_m_jet_mu_data[3];
  TH1 *h_jet_eta_data[3];
  TH1 *h_mva_score_data[3];
  for (int i=0; i<3; i++) {
    h_dR_jet_lep0_data[i] = (TH1*)hists_file->Get("3b_incl_dR_jet_lep0" + discr_vars_str_data[i]);
    h_dR_jet_lep1_data[i] = (TH1*)hists_file->Get("3b_incl_dR_jet_lep1" + discr_vars_str_data[i]);
    h_min_dR_jet_lep_data[i] = (TH1*)hists_file->Get("3b_incl_min_dR_jet_lep" + discr_vars_str_data[i]);
    h_m_jet_el_data[i] = (TH1*)hists_file->Get("3b_incl_m_jet_el" + discr_vars_str_data[i]);
    h_m_jet_mu_data[i] = (TH1*)hists_file->Get("3b_incl_m_jet_mu" + discr_vars_str_data[i]);
    h_jet_eta_data[i] = (TH1*)hists_file->Get("3b_incl_jet_eta" + discr_vars_str_data[i]);
    h_mva_score_data[i] = (TH1*)hists_file->Get("3b_incl_mva_score" + discr_vars_str_data[i]);
  }
  

  
  // ///
  // Draw hists

  // Discriminative variable for reco (not) from tops
  std::vector<TString> mc_origin_legend = {tmva_config_info[0]+" score from top", tmva_config_info[0]+" score not from top", "topHOF from top", "topHOF not from top"};
  
  std::vector<TH1*> h_dR_jet_lep0_reco_vec = {h_dR_jet_lep0_mc[1], h_dR_jet_lep0_mc[2], h_dR_jet_lep0_mc[3], h_dR_jet_lep0_mc[4]};
  int draw_dR_jet_lep0_reco = draw_n_hists(h_dR_jet_lep0_reco_vec, mc_origin_legend, "#bf{#DeltaR(btag, lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep0_reco", true, 0, 0.25);
  
  std::vector<TH1*> h_dR_jet_lep1_reco_vec = {h_dR_jet_lep1_mc[1], h_dR_jet_lep1_mc[2], h_dR_jet_lep1_mc[3], h_dR_jet_lep1_mc[4]};
  int draw_dR_jet_lep1_reco = draw_n_hists(h_dR_jet_lep1_reco_vec, mc_origin_legend, "#bf{#DeltaR(btag, sub-lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep1_reco", true, 0, 0.25);

  std::vector<TH1*> h_min_dR_jet_lep_reco_vec = {h_min_dR_jet_lep_mc[1], h_min_dR_jet_lep_mc[2], h_min_dR_jet_lep_mc[3], h_min_dR_jet_lep_mc[4]};
  int draw_min_dR_jet_lep_reco = draw_n_hists(h_min_dR_jet_lep_reco_vec, mc_origin_legend, "#bf{#DeltaR(btag, closest lepton}", "tmva_plots/discr_vars/min_dR_jet_lep_reco", true, 0, 0.35);
  
  std::vector<TH1*> h_m_jet_el_reco_vec = {h_m_jet_el_mc[1], h_m_jet_el_mc[2], h_m_jet_el_mc[3], h_m_jet_el_mc[4]};
  int draw_m_jet_el_reco = draw_n_hists(h_m_jet_el_reco_vec, mc_origin_legend, "#bf{m(btag, e)}", "tmva_plots/discr_vars/m_jet_el_reco_vec", true, 0, 1);
  
  std::vector<TH1*> h_m_jet_mu_reco_vec = {h_m_jet_mu_mc[1], h_m_jet_mu_mc[2], h_m_jet_mu_mc[3], h_m_jet_mu_mc[4]};
  int draw_m_jet_mu_reco = draw_n_hists(h_m_jet_mu_reco_vec, mc_origin_legend, "#bf{m(btag, mu)}", "tmva_plots/discr_vars/m_jet_mu_reco_vec", true, 0, 1);
  
  std::vector<TH1*> h_jet_eta_reco_vec = {h_jet_eta_mc[1], h_jet_eta_mc[2], h_jet_eta_mc[3], h_jet_eta_mc[4]};
  int draw_jet_eta_reco = draw_n_hists(h_jet_eta_reco_vec, mc_origin_legend, "#bf{btag eta}", "tmva_plots/discr_vars/jet_eta_reco", true, 0, 0.07);

  std::vector<TH1*> h_mva_score_reco_vec = {h_mva_score_mc[1], h_mva_score_mc[2], h_mva_score_mc[3], h_mva_score_mc[4]};
  int draw_mva_score_reco = draw_n_hists(h_mva_score_reco_vec, mc_origin_legend, "#bf{"+tmva_config_info[0]+" score}", "tmva_plots/discr_vars/mva_score_reco", true, 0, 0.05);
  

  // Discriminative variables for PL (not) from tops
  
  std::vector<TH1*> h_dR_jet_lep0_pl_vec = {h_dR_jet_lep0_mc[6], h_dR_jet_lep0_mc[7], h_dR_jet_lep0_mc[8], h_dR_jet_lep0_mc[9]};
  int draw_dR_jet_lep0_pl = draw_n_hists(h_dR_jet_lep0_pl_vec, mc_origin_legend, "#bf{#DeltaR(bjet, lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep0_pl", true, 0, 0.25);

  std::vector<TH1*> h_dR_jet_lep1_pl_vec = {h_dR_jet_lep1_mc[6], h_dR_jet_lep1_mc[7], h_dR_jet_lep1_mc[8], h_dR_jet_lep1_mc[9]};
  int draw_dR_jet_lep1_pl = draw_n_hists(h_dR_jet_lep1_pl_vec, mc_origin_legend, "#bf{#DeltaR(bjet, sub-lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep1_pl", true, 0, 0.25);

  std::vector<TH1*> h_min_dR_jet_lep_pl_vec = {h_min_dR_jet_lep_mc[6], h_min_dR_jet_lep_mc[7], h_min_dR_jet_lep_mc[8], h_min_dR_jet_lep_mc[9]};
  int draw_min_dR_jet_lep_pl = draw_n_hists(h_min_dR_jet_lep_pl_vec, mc_origin_legend, "#bf{#DeltaR(bjet, closest lepton}", "tmva_plots/discr_vars/min_dR_jet_lep_pl", true, 0, 0.3);

  std::vector<TH1*> h_m_jet_el_pl_vec = {h_m_jet_el_mc[6], h_m_jet_el_mc[7], h_m_jet_el_mc[8], h_m_jet_el_mc[9]};
  int draw_m_jet_el_pl = draw_n_hists(h_m_jet_el_pl_vec, mc_origin_legend, "#bf{m(bjet, e)}", "tmva_plots/discr_vars/m_jet_el_pl", true, 0, 1);

  std::vector<TH1*> h_m_jet_mu_pl_vec = {h_m_jet_mu_mc[6], h_m_jet_mu_mc[7], h_m_jet_mu_mc[8], h_m_jet_mu_mc[9]};
  int draw_m_jet_mu_pl = draw_n_hists(h_m_jet_mu_pl_vec, mc_origin_legend, "#bf{m(bjet, mu)}", "tmva_plots/discr_vars/m_jet_mu_pl", true, 0, 1);

  std::vector<TH1*> h_jet_eta_pl_vec = {h_jet_eta_mc[6], h_jet_eta_mc[7], h_jet_eta_mc[8], h_jet_eta_mc[9]};
  int draw_jet_eta_pl = draw_n_hists(h_jet_eta_pl_vec, mc_origin_legend, "#bf{bjet eta}", "tmva_plots/discr_vars/jet_eta_pl", true, 0, 0.07);

  std::vector<TH1*> h_mva_score_pl_vec = {h_mva_score_mc[6], h_mva_score_mc[7], h_mva_score_mc[8], h_mva_score_mc[9]};
  int draw_mva_score_pl = draw_n_hists(h_mva_score_pl_vec, mc_origin_legend, "#bf{"+tmva_config_info[0]+" score}", "tmva_plots/discr_vars/mva_score_pl", true, 0, 0.05);


  // Discriminative variables for Data (not) from tops
  std::vector<TString> data_origin_legend = {tmva_config_info[0]+" score from top", tmva_config_info[0]+" score not from top"};

  std::vector<TH1*> h_dR_jet_lep0_data_vec = {h_dR_jet_lep0_data[1], h_dR_jet_lep0_mc[2]};
  int draw_h_dR_jet_lep0_data = draw_n_hists(h_dR_jet_lep0_data_vec, data_origin_legend, "#bf{#DeltaR(btag, lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep0_data", true, 0, 0.25);

  std::vector<TH1*> h_dR_jet_lep1_data_vec = {h_dR_jet_lep1_data[1], h_dR_jet_lep1_data[2]};
  int draw_dR_jet_lep1_data = draw_n_hists(h_dR_jet_lep1_data_vec, data_origin_legend, "#bf{#DeltaR(btag, sub-lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep1_data", true, 0, 0.25);

  std::vector<TH1*> h_min_dR_jet_lep_data_vec = {h_min_dR_jet_lep_data[1], h_min_dR_jet_lep_data[2]};
  int draw_min_dR_jet_lep_data = draw_n_hists(h_min_dR_jet_lep_data_vec, data_origin_legend, "#bf{#DeltaR(btag, closest lepton}", "tmva_plots/discr_vars/min_dR_jet_lep_data", true, 0, 0.32);
  
  std::vector<TH1*> h_m_jet_el_data_vec = {h_m_jet_el_data[1], h_m_jet_el_data[2]};
  int draw_m_jet_el_data = draw_n_hists(h_m_jet_el_data_vec, data_origin_legend, "#bf{m(btag, e)}", "tmva_plots/discr_vars/m_jet_el_data", true, 0, 1);
  
  std::vector<TH1*> h_m_jet_mu_data_vec = {h_m_jet_mu_data[1], h_m_jet_mu_data[2]};
  int draw_m_jet_mu_data = draw_n_hists(h_m_jet_mu_data_vec, data_origin_legend, "#bf{m(btag, mu)}", "tmva_plots/discr_vars/m_jet_mu_data", true, 0, 1);

  std::vector<TH1*> h_jet_eta_data_vec = {h_jet_eta_data[1], h_jet_eta_data[2]};
  int draw_jet_eta_data = draw_n_hists(h_jet_eta_data_vec, data_origin_legend, "#bf{btag eta}", "tmva_plots/discr_vars/jet_eta_data", true, 0, 0.08);

  std::vector<TH1*> h_mva_score_data_vec = {h_mva_score_data[1], h_mva_score_data[2]};
  int draw_mva_score_data = draw_n_hists(h_mva_score_data_vec, data_origin_legend, "#bf{"+tmva_config_info[0]+" score}", "tmva_plots/discr_vars/mva_score_data", true, 0, 0.05);
  

  // Data / mc 
  std::vector<TString> data_mc_legend = {"Data", "Reco", "PL"};

  std::vector<TH1*> h_dR_jet_lep0_datamc_vec = {h_dR_jet_lep0_data[0], h_dR_jet_lep0_mc[0], h_dR_jet_lep0_mc[5]};
  int draw_dR_jet_lep0_datamc = draw_n_hists(h_dR_jet_lep0_datamc_vec, data_mc_legend, "#bf{#DeltaR(btag, lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep0_datamc", true, 0.001, 1.0);

  std::vector<TH1*> h_dR_jet_lep1_datamc_vec = {h_dR_jet_lep1_data[0], h_dR_jet_lep1_mc[0], h_dR_jet_lep1_mc[5]};
  int draw_dR_jet_lep1_datamc = draw_n_hists(h_dR_jet_lep1_datamc_vec, data_mc_legend, "#bf{#DeltaR(btag, sub-lead. p_{T} lepton)}", "tmva_plots/discr_vars/dR_jet_lep1_datamc", true, 0.001, 1.0);

  std::vector<TH1*> h_min_dR_jet_lep_datamc_vec = {h_min_dR_jet_lep_data[0], h_min_dR_jet_lep_mc[0], h_min_dR_jet_lep_mc[5]};
  int draw_min_dR_jet_lep_datamc = draw_n_hists(h_min_dR_jet_lep_datamc_vec, data_mc_legend, "#bf{#DeltaR(btag, closest lepton)}", "tmva_plots/discr_vars/min_dR_jet_lep_datamc", true, 0.001, 1.0);

  std::vector<TH1*> h_m_jet_el_datamc_vec = {h_m_jet_el_data[0], h_m_jet_el_mc[1], h_m_jet_el_mc[5]};
  int draw_m_jet_el_datamc = draw_n_hists(h_m_jet_el_datamc_vec, data_mc_legend, "#bf{m(btag, e)}", "tmva_plots/discr_vars/m_jet_el_datamc", true, 0.001, 1.0);
  
  std::vector<TH1*> h_m_jet_mu_datamc_vec = {h_m_jet_mu_data[0], h_m_jet_mu_mc[1], h_m_jet_mu_mc[5]};
  int draw_m_jet_mu_datamc = draw_n_hists(h_m_jet_mu_datamc_vec, data_mc_legend, "#bf{m(btag, mu)}", "tmva_plots/discr_vars/m_jet_mu_datamc", true, 0.001, 1.0);

  std::vector<TH1*> h_jet_eta_datamc_vec = {h_jet_eta_data[0], h_jet_eta_mc[0], h_jet_eta_mc[5]};
  int draw_jet_eta_datamc = draw_n_hists(h_jet_eta_datamc_vec, data_mc_legend, "#bf{btag eta}", "tmva_plots/discr_vars/jet_eta_datamc", true, 0, 0.08);

  std::vector<TH1*> h_mva_score_datamc_vec = {h_mva_score_data[0], h_mva_score_mc[0], h_mva_score_mc[5]};
  int draw_mva_score_datamc = draw_n_hists(h_mva_score_datamc_vec, data_mc_legend, "#bf{"+tmva_config_info[0]+" score}", "tmva_plots/discr_vars/mva_score_datamc", true, 0, 0.04);
  

  // Draw number of b
  std::vector<TH1*> h_b_n_vec = {h_bjets_n_pl, h_btags_n_reco};
  std::vector<TString> h_b_n_title = {"bjets, PL", "btags DL1r 77, Reco"};
  int draw_h_b_n = draw_n_hists(h_b_n_vec, h_b_n_title, "#bf{N_{bjets or btags}}", "tmva_plots/bjets_btags_n", false, 1, 100000);
  
  // Draw number of jets from top
  std::vector<TH1*> h_jets_n_ft_vec = {h_jets_n_ft_pl, h_jets_n_ft_reco};
  std::vector<TString> h_jets_n_ft_title = {"particle level", "reco level"};
  int draw_jets_n_ft = draw_n_hists(h_jets_n_ft_vec, h_jets_n_ft_title, "number fo jets from top", "tmva_plots/jets_n_from_top", false, 1, 100000);

  // Draw number of b from top
  std::vector<TH1*> h_b_n_ft_vec = {h_bjets_n_ft_pl, h_btags_n_ft_reco};
  std::vector<TString> h_b_n_ft_title = {"bjets, PL", "btags DL1r 77, Reco"};
  int draw_b_n_ft = draw_n_hists(h_b_n_ft_vec, h_b_n_ft_title, "number of b from top", "tmva_plots/bjets_btags_n_from_top", false, 1, 100000);

  // Draw stats hist
  std::vector<TH1*> h_stats_3b_vec = {h_jets_origin_stat_pl_3b, h_jets_origin_stat_reco_3b};
  std::vector<TString> h_stats_title = {"particle level", "reco level"};
  int draw_stats_3b = draw_n_hists(h_stats_3b_vec, h_stats_title, "bjets/btags pairs from top (truth info) in "+tmva_config_info[0]+" order", "tmva_plots/bjets_pairs_from_tops_study_"+tmva_config_info[0]+"_3b", true, 0, 1, false);
  
  std::vector<TH1*> h_stats_4b_vec = {h_jets_origin_stat_pl_4b, h_jets_origin_stat_reco_4b};
  int draw_stats_4b = draw_n_hists(h_stats_4b_vec, h_stats_title, "bjets/btags pairs from top (truth info) in "+tmva_config_info[0]+" order", "tmva_plots/bjets_pairs_from_tops_study_"+tmva_config_info[0]+"_4b", true, 0, 1, false);

  // Draw jets origin identification efficiency with pT taken into account
  std::vector<TH1*> h_origin_tag_eff_3b = {h_origin_tag_eff_pl_3b, h_origin_tag_eff_reco_3b};
  int draw_origin_tag_eff_3b = draw_n_hists(h_origin_tag_eff_3b, h_stats_title, "bjets/btags origin identification eff. in pT order", "tmva_plots/bjets_from_tops_study_"+tmva_config_info[0]+"_3b", false, 0, 1, false);

  std::vector<TH1*> h_origin_tag_eff_4b= {h_origin_tag_eff_pl_4b, h_origin_tag_eff_reco_4b};
  int draw_origin_tag_eff_4b = draw_n_hists(h_origin_tag_eff_4b, h_stats_title, "bjets/btags origin identification eff. in pT order", "tmva_plots/bjets_from_tops_study_"+tmva_config_info[0]+"_4b", false, 0, 1, false);

  

  // Close hists files
  hists_file->Close();

  return 0;

} // END of MAIN
