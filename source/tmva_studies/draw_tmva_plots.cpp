#include "../common_functions.h"
#include "../draw_functions.h"

// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");

  
  // Create directory for plots
  gSystem->Exec("mkdir results/plots");
  gSystem->Exec("mkdir results/plots/tmva_plots");


  // Open the TMVA output file
  TFile *tmva_file = new TFile("results/TMVA.root");

  
  // Open hists

  TString var_hists_address = "dataset/Method_" + tmva_config_info[0] + "/" + tmva_config_info[0] + "/";

  // Siganl hists
  TH1 *h_MVA_dR_bjet_lep0_S = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep0__Signal");
  TH1 *h_MVA_dR_bjet_lep1_S = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep1__Signal");
  TH1 *h_MVA_min_dR_bjet_lep_S = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_lep__Signal");
  TH1 *h_MVA_m_bjet_el_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_el__Signal");
  TH1 *h_MVA_m_bjet_mu_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_mu__Signal");
  TH1 *h_MVA_m_bjet_lep_max_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_lep_max__Signal");
  TH1 *h_MVA_min_dR_jet_bjet_S = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_bjet__Signal");
  TH1 *h_MVA_jet_pt_S = (TH1F*)tmva_file->Get(var_hists_address+"jet_pt__Signal");
  TH1 *h_MVA_jet_eta_S = (TH1F*)tmva_file->Get(var_hists_address+"jet_eta__Signal");
  TH2 *h_MVA_corr_mtrx_S = (TH2F*)tmva_file->Get("dataset/CorrelationMatrixS");

  // Background hists
  TH1 *h_MVA_dR_bjet_lep0_B = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep0__Background");
  TH1 *h_MVA_dR_bjet_lep1_B = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep1__Background");
  TH1 *h_MVA_min_dR_bjet_lep_B = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_lep__Background");
  TH1 *h_MVA_m_bjet_el_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_el__Background");
  TH1 *h_MVA_m_bjet_mu_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_mu__Background");
  TH1 *h_MVA_m_bjet_lep_max_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_lep_max__Background");
  TH1 *h_MVA_min_dR_jet_bjet_B = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_bjet__Background");
  TH1 *h_MVA_jet_pt_B = (TH1F*)tmva_file->Get(var_hists_address+"jet_pt__Background");
  TH1 *h_MVA_jet_eta_B =(TH1F*)tmva_file->Get(var_hists_address+"jet_eta__Background");
  TH2 *h_MVA_corr_mtrx_B = (TH2F*)tmva_file->Get("dataset/CorrelationMatrixB");

  // Other hists
  TH1 *h_MVA_rejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_rejBvsS");
  TH1 *h_MVA_effBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_effBvsS");
  TH1 *h_MVA_trainingEffBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_trainingEffBvsS");
  TH1 *h_MVA_trainingRejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_trainingRejBvsS");
  TH1 *h_MVA_classifier_output_S = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_S");
  TH1 *h_MVA_classifier_output_B = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_B");
  TH1 *h_MVA_trainRejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_trainingRejBvsS");


  // Draw Sig/Bkg pairs
  int dR_bjet_lep0_draw = draw_tmva_hists(h_MVA_dR_bjet_lep0_S, h_MVA_dR_bjet_lep0_B, "#bf{#DeltaR(jet - lep0)}", "dR_jet_lep0");
  int dR_bjet_lep1_draw = draw_tmva_hists(h_MVA_dR_bjet_lep1_S, h_MVA_dR_bjet_lep1_B, "#bf{#DeltaR(jet - lep1)}", "dR_jet_lep1");
  int min_dR_bjet_lep_draw = draw_tmva_hists(h_MVA_min_dR_bjet_lep_S, h_MVA_min_dR_bjet_lep_B, "#bf{#DeltaR_{min}(jet, lep)}", "min_dR_jet_lep");
  int m_bjet_el_draw = draw_tmva_hists(h_MVA_m_bjet_el_S, h_MVA_m_bjet_el_B, "#bf{m_{inv}(jet, el)}", "m_jet_el");
  int m_bjet_mu_draw = draw_tmva_hists(h_MVA_m_bjet_mu_S, h_MVA_m_bjet_mu_B, "#bf{m_{inv}(jet, mu)}", "m_jet_mu");
  int m_bjet_lep_max = draw_tmva_hists(h_MVA_m_bjet_lep_max_S, h_MVA_m_bjet_lep_max_B, "#bf{m_{inv}^{max}(jet, lep)}", "m_jet_lep_max");
  int min_dR_jet_bjet_draw = draw_tmva_hists(h_MVA_min_dR_jet_bjet_S, h_MVA_min_dR_jet_bjet_B, "#bf{#DeltaR_{min}(jet, bjet)}", "min_dR_jet_bjet");
  int jet_pt_draw = draw_tmva_hists(h_MVA_jet_pt_S, h_MVA_jet_pt_B, "#bf{p_{T}^{jet}}", "jet_pt");
  int jet_eta_draw = draw_tmva_hists(h_MVA_jet_eta_S, h_MVA_jet_eta_B, "#bf{#eta^{jet}}", "jet_eta");

  
  // Draw correlation matrices
  std::vector<TString> axis_labels = {"#bf{#DeltaR(jet - lep0)}", "#bf{#DeltaR(jet - lep1)}", "#bf{#DeltaR_{min}(jet, lep)}", "#bf{m_{inv}(jet, el)}", "#bf{m_{inv}(jet, mu)}", "#bf{m_{inv}^{max}(jet, lep)}", "#bf{#DeltaR_{min}(jet, bjet)}"};
  int MVA_corr_mtrx_S_draw = draw_correlations(h_MVA_corr_mtrx_S, "Correlation Matrix: Signal", axis_labels, "corr_mtrx_S");
  int MVA_corr_mtrx_B_draw = draw_correlations(h_MVA_corr_mtrx_B, "Correlation Matrix: Background", axis_labels,"corr_mtrx_B");


  // Draw other hists
  int MVA_effBvsS_draw = draw_tmva_hists(h_MVA_rejBvsS, h_MVA_effBvsS, "#bf{Signal eff.}", "eff-rej_BvsS", {"Bkgd rej.", "Bkgd eff."}, false);
  int MVA_trainingEffBvsS = draw_tmva_hists(h_MVA_trainingRejBvsS, h_MVA_trainingEffBvsS, "#bf{Signal eff.}", "training_eff-rej_BvsS", {"Bkgd rej.", "Bkgd eff."}, false);
  int MVA_classifier_output_draw = draw_tmva_hists(h_MVA_classifier_output_S, h_MVA_classifier_output_B, "#bf{" + tmva_config_info[0] + "}", "classifier_output");
  int MVA_train_test_rejBvsS = draw_tmva_hists(h_MVA_trainRejBvsS, h_MVA_rejBvsS, "#bf{Signal eff.}", "train_test_RejBvsS", {"Training", "Testing"}, false);
  

  return 0;
}
