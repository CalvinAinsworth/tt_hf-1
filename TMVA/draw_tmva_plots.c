#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH2.h>
#include <fstream>

// #####################
// ##   DRAW HISTS    ##
// #####################
int draw_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1 = true)
{
  cout << "Drawing " << savename << endl;

  TCanvas *c = new TCanvas(savename, savename, 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();
  
  double h_s_int = h_s->Integral(0, h_s->GetNbinsX()+1);
  double h_b_int = h_b->Integral(0, h_b->GetNbinsX()+1);
  if (norm_to_1 == true) {
    h_s->Scale(1/h_s_int);
    h_b->Scale(1/h_b_int); }

  h_s->SetLineColor(2);
  h_s->SetLineWidth(4);
  h_s->Draw("hist");
  h_s->SetTitle("");
  if (norm_to_1 == true) { h_s->GetYaxis()->SetRangeUser(0, 0.3); }
  else { h_s->GetYaxis()->SetRangeUser(0, 1.2); }
  if (norm_to_1 == true) h_s->GetYaxis()->SetTitle("#bf{norm to 1.}");
  h_s->GetXaxis()->SetTitle(x_axis_title);

  h_b->SetLineColor(4);
  h_b->SetLineWidth(4);
  h_b->Draw("hist same");

  TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(h_s, legend_entries_titles[0]);
  legend->AddEntry(h_b, legend_entries_titles[1]);
  legend->SetTextSize(0.04);
  legend->Draw("same");

  c->Print("Plots/"+savename+".png");
  cout << "Plotted " << savename << endl << endl << endl;

  return 0;
}


// ############################
// ##   Draw Correlations    ##
// ############################
int draw_correlations(TH2 *h_corr, TString title, vector<TString> axis_labels, TString savename)
{
  TCanvas *c = new TCanvas(savename, savename, 1600, 1200);
  TExec *ex = new TExec("exec", "gStyle->SetPaintTextFormat(\"0.2f\");");
  gStyle->SetPalette(kDeepSea);
  gPad->SetLeftMargin(0.2);
  gPad->SetBottomMargin(0.2);
  ex->Draw();
  h_corr->Draw("COLZ TEXT");
  h_corr->SetTitle(title);

  if (axis_labels.size() == h_corr->GetNbinsX()) {
    for (int i=0; i<h_corr->GetNbinsX(); i++) {
      h_corr->GetXaxis()->SetBinLabel(i+1, axis_labels[i]);
      h_corr->GetYaxis()->SetBinLabel(i+1, axis_labels[i]); }
  } // [if] - axis labels
  h_corr->GetXaxis()->SetLabelSize(0.04);
  h_corr->GetYaxis()->SetLabelSize(0.04);
  
  c->Print("Plots/"+savename+".png");

  cout << "Plotted " << title << endl << endl << endl;

  return 0;
}




// ################
// ##    MAIN    ##
// ################
void draw_tmva_plots()
{
  // Get name of the TMVA method
  TString method_name = "";
  ifstream config_file("tmva_config.txt", ios::binary);
  if (config_file.is_open()) {
    string str1;
    string delim = ": ";
    while(getline(config_file, str1)) {
      string par_name = str1.substr(0, str1.find(delim));
      string par_val  = str1.substr(str1.find(delim)+2, str1.size()); // +2 due to delim length 
      if (par_name == "TMVA Method")  method_name += par_val;
    }
  }
  config_file.close();


  // Create folders for plots
  gSystem->Exec("mkdir Plots");

  
  // Open the TMVA output file
  TFile *tmva_file = new TFile("TMVA.root");

  
  // Open hists
  
  TString var_hists_address = "dataset/Method_" + method_name + "/" + method_name + "/";

  // Signal hists  
  TH1 *h_NN_dR_bjet_lep0_S = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep0__Signal");
  TH1 *h_NN_dR_bjet_lep1_S = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep1__Signal");
  TH1 *h_NN_min_dR_bjet_lep_S = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_lep__Signal");
  TH1 *h_NN_m_bjet_el_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_el__Signal");
  TH1 *h_NN_m_bjet_mu_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_mu__Signal");
  TH1 *h_NN_m_bjet_lep_max_S = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_lep_max__Signal");
  TH1 *h_NN_min_dR_jet_bjet_S = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_bjet__Signal");
  TH1 *h_NN_jet_pt_S = (TH1F*)tmva_file->Get(var_hists_address+"jet_pt__Signal");
  TH1 *h_NN_jet_eta_S = (TH1F*)tmva_file->Get(var_hists_address+"jet_eta__Signal");
  TH2 *h_NN_corr_mtrx_S = (TH2F*)tmva_file->Get("dataset/CorrelationMatrixS");
  
  // Background hists
  TH1 *h_NN_dR_bjet_lep0_B = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep0__Background");
  TH1 *h_NN_dR_bjet_lep1_B = (TH1F*)tmva_file->Get(var_hists_address+"dR_jet_lep1__Background");
  TH1 *h_NN_min_dR_bjet_lep_B = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_lep__Background");
  TH1 *h_NN_m_bjet_el_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_el__Background");
  TH1 *h_NN_m_bjet_mu_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_mu__Background");
  TH1 *h_NN_m_bjet_lep_max_B = (TH1F*)tmva_file->Get(var_hists_address+"m_jet_lep_max__Background");
  TH1 *h_NN_min_dR_jet_bjet_B = (TH1F*)tmva_file->Get(var_hists_address+"min_dR_jet_bjet__Background");
  TH1 *h_NN_jet_pt_B = (TH1F*)tmva_file->Get(var_hists_address+"jet_pt__Background");
  TH1 *h_NN_jet_eta_B =(TH1F*)tmva_file->Get(var_hists_address+"jet_eta__Background");
  TH2 *h_NN_corr_mtrx_B = (TH2F*)tmva_file->Get("dataset/CorrelationMatrixB");
  
  // Other hists
  TH1 *h_NN_rejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_rejBvsS");
  TH1 *h_NN_effBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_effBvsS");
  TH1 *h_NN_trainingEffBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_trainingEffBvsS");
  TH1 *h_NN_trainingRejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_trainingRejBvsS");
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_S");
  TH1 *h_NN_classifier_output_B = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_B");
  TH1 *h_NN_trainRejBvsS = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_trainingRejBvsS");

  // Draw Sig_Bkgd pairs
  int dR_bjet_lep0_draw = draw_hists(h_NN_dR_bjet_lep0_S, h_NN_dR_bjet_lep0_B, "#bf{#DeltaR(jet - lep0)}", "dR_jet_lep0");
  int dR_bjet_lep1_draw = draw_hists(h_NN_dR_bjet_lep1_S, h_NN_dR_bjet_lep1_B, "#bf{#DeltaR(jet - lep1)}", "dR_jet_lep1");
  int min_dR_bjet_lep_draw = draw_hists(h_NN_min_dR_bjet_lep_S, h_NN_min_dR_bjet_lep_B, "#bf{#DeltaR_{min}(jet, lep)}", "min_dR_jet_lep");
  int m_bjet_el_draw = draw_hists(h_NN_m_bjet_el_S, h_NN_m_bjet_el_B, "#bf{m_{inv}(jet, el)}", "m_jet_el");
  int m_bjet_mu_draw = draw_hists(h_NN_m_bjet_mu_S, h_NN_m_bjet_mu_B, "#bf{m_{inv}(jet, mu)}", "m_jet_mu");
  int m_bjet_lep_max = draw_hists(h_NN_m_bjet_lep_max_S, h_NN_m_bjet_lep_max_B, "#bf{m_{inv}^{max}(jet, lep)}", "m_jet_lep_max");
  int min_dR_jet_bjet_draw = draw_hists(h_NN_min_dR_jet_bjet_S, h_NN_min_dR_jet_bjet_B, "#bf{#DeltaR_{min}(jet, bjet)}", "min_dR_jet_bjet");
  int jet_pt_draw = draw_hists(h_NN_jet_pt_S, h_NN_jet_pt_B, "#bf{p_{T}^{jet}}", "jet_pt");
  int jet_eta_draw = draw_hists(h_NN_jet_eta_S, h_NN_jet_eta_B, "#bf{#eta^{jet}}", "jet_eta");

  // Draw correlation matrices
  vector<TString> axis_labels = {"#bf{#DeltaR(jet - lep0)}", "#bf{#DeltaR(jet - lep1)}", "#bf{#DeltaR_{min}(jet, lep)}", "#bf{m_{inv}(jet, el)}", "#bf{m_{inv}(jet, mu)}", "#bf{m_{inv}^{max}(jet, lep)}", "#bf{#DeltaR_{min}(jet, bjet)}"};
  int NN_corr_mtrx_S_draw = draw_correlations(h_NN_corr_mtrx_S, "Correlation Matrix: Signal", axis_labels, "corr_mtrx_S");
  int NN_corr_mtrx_B_draw = draw_correlations(h_NN_corr_mtrx_B, "Correlation Matrix: Background", axis_labels,"corr_mtrx_B");
  
  // Draw other hists
  int NN_effBvsS_draw = draw_hists(h_NN_rejBvsS, h_NN_effBvsS, "#bf{Signal eff.}", "eff-rej_BvsS", {"Bkgd rej.", "Bkgd eff."}, false);
  int NN_trainingEffBvsS = draw_hists(h_NN_trainingRejBvsS, h_NN_trainingEffBvsS, "#bf{Signal eff.}", "training_eff-rej_BvsS", {"Bkgd rej.", "Bkgd eff."}, false);
  int NN_classifier_output_draw = draw_hists(h_NN_classifier_output_S, h_NN_classifier_output_B, "#bf{" + method_name + "}", "classifier_output");
  int NN_train_test_rejBvsS = draw_hists(h_NN_trainRejBvsS, h_NN_rejBvsS, "#bf{Signal eff.}", "train_test_RejBvsS", {"Training", "Testing"}, false);

}
