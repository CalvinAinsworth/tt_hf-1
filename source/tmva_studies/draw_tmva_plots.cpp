#include "../common_functions.h"
#include "../draw_functions.h"

// ################
// ##    MAIN    ##
// ################
int main(int argc, char *argv[])
{
  // Leptons pT cut choise
  TString lep_pt_cut_suffix = leptons_pt_cut_suffix();


  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");


  // Create folders for plots
  gSystem->Exec("mkdir Plots");

  
  // Open the TMVA output file
  TString in_fname = "results/TMVA_pl_nominal" + lep_pt_cut_suffix + ".root";
  TFile *tmva_file = new TFile(in_fname);

  
  // Open hists
  
  TString var_hists_address = "dataset/Method_" + tmva_config_info[0] + "/" + tmva_config_info[0] + "/";

  // Signal hists  
  TH1 *h_dR_bjet_lep0_S = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep0__Signal");
  TH1 *h_dR_bjet_lep1_S = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep1__Signal");
  TH1 *h_min_dR_bjet_lep_S = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_lep__Signal");
  TH1 *h_m_bjet_el_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_el__Signal");
  TH1 *h_m_bjet_mu_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_mu__Signal");
  TH2 *h_corr_mtrx_S = (TH2F*)tmva_file->Get("dataset/CorrelationMatrixS");
  
  // Background hists
  TH1 *h_dR_bjet_lep0_B = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep0__Background");
  TH1 *h_dR_bjet_lep1_B = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep1__Background");
  TH1 *h_min_dR_bjet_lep_B = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_lep__Background");
  TH1 *h_m_bjet_el_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_el__Background");
  TH1 *h_m_bjet_mu_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_mu__Background");
  TH2 *h_corr_mtrx_B = (TH2F*)tmva_file->Get("dataset/CorrelationMatrixB");
  
  // Other hists
  TH1 *h_rejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_rejBvsS");
  TH1 *h_effBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_effBvsS");
  TH1 *h_trainingEffBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_trainingEffBvsS");
  TH1 *h_trainingRejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_trainingRejBvsS");
  TH1 *h_classifier_output_S = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_S");
  TH1 *h_classifier_output_B = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_B");
  TH1 *h_trainRejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + tmva_config_info[0] + "_trainingRejBvsS");

  // Draw Sig_Bkgd pairs
  int dR_bjet_lep0_draw = draw_tmva_hists(h_dR_bjet_lep0_S, h_dR_bjet_lep0_B, "#bf{#DeltaR(jet - lep0)}", "dR_jet_lep0");
  int dR_bjet_lep1_draw = draw_tmva_hists(h_dR_bjet_lep1_S, h_dR_bjet_lep1_B, "#bf{#DeltaR(jet - lep1)}", "dR_jet_lep1");
  int min_dR_bjet_lep_draw = draw_tmva_hists(h_min_dR_bjet_lep_S, h_min_dR_bjet_lep_B, "#bf{#DeltaR_{min}(jet, lep)}", "min_dR_jet_lep");
  int m_bjet_el_draw = draw_tmva_hists(h_m_bjet_el_S, h_m_bjet_el_B, "#bf{m_{inv}(jet, el)}", "m_jet_el");
  int m_bjet_mu_draw = draw_tmva_hists(h_m_bjet_mu_S, h_m_bjet_mu_B, "#bf{m_{inv}(jet, mu)}", "m_jet_mu");
  

  // Draw correlation matrices
  std::vector<TString> axis_labels = {"#bf{#DeltaR(jet - lep0)}", "#bf{#DeltaR(jet - lep1)}", "#bf{#DeltaR_{min}(jet, lep)}", "#bf{m_{inv}(jet, el)}", "#bf{m_{inv}(jet, mu)}", "#bf{m_{inv}^{max}(jet, lep)}", "#bf{#DeltaR_{min}(jet, bjet)}"};
  int corr_mtrx_S_draw = draw_correlations(h_corr_mtrx_S, "Correlation Matrix: Signal", axis_labels, "corr_mtrx_S");
  int corr_mtrx_B_draw = draw_correlations(h_corr_mtrx_B, "Correlation Matrix: Background", axis_labels,"corr_mtrx_B");
  
  // Draw other hists
  int effBvsS_draw = draw_tmva_hists(h_rejBvsS, h_effBvsS, "#bf{Signal eff.}", "eff-rej_BvsS", {"Bkgd rej.", "Bkgd eff."}, false);
  int trainingEffBvsS = draw_tmva_hists(h_trainingRejBvsS, h_trainingEffBvsS, "#bf{Signal eff.}", "training_eff-rej_BvsS", {"Bkgd rej.", "Bkgd eff."}, false);
  int classifier_output_draw = draw_tmva_hists(h_classifier_output_S, h_classifier_output_B, "#bf{" + tmva_config_info[0] + "}", "classifier_output");
  int train_test_rejBvsS = draw_tmva_hists(h_trainRejBvsS, h_rejBvsS, "#bf{Signal eff.}", "train_test_RejBvsS", {"Training", "Testing"}, false);


  return 0;
}
