#include <TH2.h>
#include <TTree.h>
#include <TFile.h>
#include <TSystemFile.h>
#include <TSystemDirectory.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPad.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TAttLine.h>
#include <TLine.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>



// ###############################
// ## Draw Data/Bkgd Comparison ##
// ###############################
int draw_data_mc_plot(TH1 *h_data, TH1 *h_mc, TString title, TString savename, vector<TString> legend_entries = {"Data", "MC"}, bool norm_to_1 = false)



// ##################
// ##     MAIN     ##
// ##################
void draw_data_mc()
{
  // Get mc hists
  TFile *mc_hists_file = new TFile("hists_mc.root");
  TH1 *h_mc_jets_n = (TH1*)mc_hists_file->Get("2b_emu_OS_jets_n");
  TH1 *h_mc_btags_n = (TH1*)mc_hists_file->Get("2b_emu_OS_btags_n");
  TH1 *h_mc_NN__m_btag_lep_min_dR = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_m_btag_lep_min_dR");
  TH1 *h_mc_NN__m_btag_lep_min = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_m_btag_lep_min");
  TH1 *h_mc_NN__m_btag_lep_max = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_m_btag_lep_max");
  TH1 *h_mc_NN__m_bjet_el = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_m_bjet_el");
  TH1 *h_mc_NN__m_bjet_mu = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_m_bjet_mu");
  TH1 *h_mc_NN__dR_btag_lep0 = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_dR_btag_lep0");
  TH1 *h_mc_NN__dR_btag_lep1 = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_dR_btag_lep1");
  TH1 *h_mc_NN__min_dR_btag_lep = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_min_dR_btag_lep");
  TH1 *h_mc_NN__min_dR_jet_bjet = (TH1*)mc_hists_file->Get("NN__2b_emu_OS_min_dR_jet_bjet");
  TH1 *h_mc_NN__all_jets_pt = (TH1*)mc_hists_file->Get("2b_emu_OS_all_jets_pt");
  TH1 *h_mc_NN__all_jets_eta = (TH1*)mc_hists_file->Get("2b_emu_OS_all_jets_eta");
  TH1 *h_mc_all_jets_phi = (TH1*)mc_hists_file->Get("2b_emu_OS_all_jets_phi");
  TH1 *h_mc_m_btag = (TH1*)mc_hists_file->Get("2b_emu_OS_m_btag");
  TH1 *h_mc_m_notbtag = (TH1*)mc_hists_file->Get("2b_emu_OS_m_notbtag");
  TH1 *h_mc_m_btag_btag = (TH1*)mc_hists_file->Get("2b_emu_OS_m_btag_btag");
  TH1 *h_mc_m_btag_notbtag = (TH1*)mc_hists_file->Get("2b_emu_OS_m_btag_notbtag");
  TH1 *h_mc_m_notbtag_notbtag = (TH1*)mc_hists_file->Get("2b_emu_OS_m_notgtag_notbtag");

  // Get data hists
  TFile *data_hists_file = new TFile("hists_data.root");
  TH1 *h_data_jets_n = (TH1*)data_hists_file->Get("2b_emu_OS_jets_n");
  TH1 *h_data_btags_n = (TH1*)data_hists_file->Get("2b_emu_OS_btags_n");
  TH1 *h_data_NN__m_btag_lep_min_dR = (TH1*)data_hists_file->Get("NN__2b_emu_OS_m_btag_lep_min_dR");
  TH1 *h_data_NN__m_btag_lep_min = (TH1*)data_hists_file->Get("NN__2b_emu_OS_m_btag_lep_min");
  TH1 *h_data_NN__m_btag_lep_max = (TH1*)data_hists_file->Get("NN__2b_emu_OS_m_btag_lep_max");
  TH1 *h_data_NN__m_bjet_el = (TH1*)data_hists_file->Get("NN__2b_emu_OS_m_bjet_el");
  TH1 *h_data_NN__m_bjet_mu = (TH1*)data_hists_file->Get("NN__2b_emu_OS_m_bjet_mu");
  TH1 *h_data_NN__dR_btag_lep0 = (TH1*)data_hists_file->Get("NN__2b_emu_OS_dR_btag_lep0");
  TH1 *h_data_NN__dR_btag_lep1 = (TH1*)data_hists_file->Get("NN__2b_emu_OS_dR_btag_lep1");
  TH1 *h_data_NN__min_dR_btag_lep = (TH1*)data_hists_file->Get("NN__2b_emu_OS_min_dR_btag_lep");
  TH1 *h_data_NN__min_dR_jet_bjet = (TH1*)data_hists_file->Get("NN__2b_emu_OS_min_dR_jet_bjet");
  TH1 *h_data_NN__all_jets_pt = (TH1*)data_hists_file->Get("2b_emu_OS_all_jets_pt");
  TH1 *h_data_NN__all_jets_eta = (TH1*)data_hists_file->Get("2b_emu_OS_all_jets_eta");
  TH1 *h_data_all_jets_phi = (TH1*)data_hists_file->Get("2b_emu_OS_all_jets_phi");
  TH1 *h_data_m_btag = (TH1*)data_hists_file->Get("2b_emu_OS_m_btag");
  TH1 *h_data_m_notbtag = (TH1*)data_hists_file->Get("2b_emu_OS_m_notbtag");
  TH1 *h_data_m_btag_btag = (TH1*)data_hists_file->Get("2b_emu_OS_m_btag_btag");
  TH1 *h_data_m_btag_notbtag = (TH1*)data_hists_file->Get("2b_emu_OS_m_btag_notbtag");
  TH1 *h_data_m_notbtag_notbtag =(TH1*)data_hists_file->Get("2b_emu_OS_m_notgtag_notbtag");
  
  
  // Draw different hists
  
  int draw_NN__m_btag_lep_min_dR = draw_data_mc_plot(h_data_NN__m_btag_lep_min_dR, h_mc_NN__m_btag_lep_min_dR, "#bf{m_{inv}^{min dR}(bjet, lep)}", "2b_emu_OS_m_btag_lep_min_dR");
  int draw_NN__m_btag_lep_min = draw_data_mc_plot(h_data_NN__m_btag_lep_min, h_mc_NN__m_btag_lep_min, "#bf{m_{inv}^{min}(bjet, lep)}", "2b_emu_OS_m_btag_lep_min");
  int draw_NN__m_btag_lep_max = draw_data_mc_plot(h_data_NN__m_btag_lep_max, h_mc_NN__m_btag_lep_max, "#bf{m_{inv}^{max}(bjet, lep)}", "2b_emu_OS_m_btag_lep_max");
  int draw_NN__m_bjet_el = draw_data_mc_plot(h_data_NN__m_bjet_el, h_mc_NN__m_bjet_el, "#bf{m_{inv}(bjet, el)}", "2b_emu_OS_m_bjet_el");
  int draw_NN__m_bjet_mu = draw_data_mc_plot(h_data_NN__m_bjet_mu, h_mc_NN__m_bjet_mu, "#bf{m_{inv}(bjet, mu)}", "2b_emu_OS_m_bjet_mu");
  int draw_NN__dR_btag_lep0 = draw_data_mc_plot(h_data_NN__dR_btag_lep0, h_mc_NN__dR_btag_lep0, "#bf{dR(bjet, lep0)}", "2b_emu_OS_dR_btag_lep0");
  int draw_NN__dR_btag_lep1 = draw_data_mc_plot(h_data_NN__dR_btag_lep1, h_mc_NN__dR_btag_lep1, "#bf{dR(bjet, lep1)}", "2b_emu_OS_dR_btag_lep1");
  int draw_NN__min_dR_btag_lep = draw_data_mc_plot(h_data_NN__min_dR_btag_lep, h_mc_NN__min_dR_btag_lep, "#bf{dR_{min}(the bjet, lep)}", "2b_emu_OS_min_dR_btag_lep");
  int draw_NN__min_dR_jet_bjet = draw_data_mc_plot(h_data_NN__min_dR_jet_bjet, h_mc_NN__min_dR_jet_bjet, "#bf{dR_{min}(the jet, bjet)}", "2b_emu_OS_min_dR_jet_bjet");
  int draw_NN__all_jets_pt = draw_data_mc_plot(h_data_NN__all_jets_pt, h_mc_NN__all_jets_pt, "#bf{p_{T}^{jets}}", "2b_emu_OS_all_jets_pt");
  int draw_NN__all_jets_eta = draw_data_mc_plot(h_mc_NN__all_jets_eta, h_data_NN__all_jets_eta, "#bf{jets #eta}", "2b_emu_OS_all_jets_eta");
  //int draw_NN__all_jets_phi = draw_data_mc_plot(h_mc_NN__all_jets_phi, h_data_NN__all_jets_phi, "#bf{jets #phi}", "2b_emu_OS_all_jets_phi");
  int draw_NN__m_btag = draw_data_mc_plot(h_data_m_btag, h_mc_m_btag, "#bf{m(btag)}", "2b_emu_OS_m_btag");
  int draw_NN__m_notbtag = draw_data_mc_plot(h_data_m_notbtag, h_mc_m_notbtag, "#bf{m(notbtag)}", "2b_emu_OS_m_notbtag");
  int draw_NN__m_btag_btag = draw_data_mc_plot(h_data_m_btag_btag, h_mc_m_btag_btag, "#bf{m(btag, btag)}", "2b_emu_OS_m_btag_btag");
  int draw_NN__m_btag_notbtag = draw_data_mc_plot(h_data_m_btag_notbtag, h_mc_m_btag_notbtag, "#bf{m(btag, not-btag)}", "2b_emu_OS_m_btag_notbtag");
  int draw_NN_m_notbtag_notbtag = draw_data_mc_plot(h_data_m_notbtag_notbtag, h_mc_m_notbtag_notbtag, "#bf{m(not-btag, not-btag)}", "2b_emu_OS_m_notbtag_notbtag");

} // END OF MAIN




// ###############################
// ## Draw Data/Bkgd Comparison ##
// ###############################
int draw_data_mc_plot(TH1 *h_data, TH1 *h_mc, TString title, TString savename, vector<TString> legend_entries = {"Data", "MC"}, bool norm_to_1 = false)
{
  cout << "Drawing " << title << endl;

  // Create a canvas
  TCanvas *c = new TCanvas("c", "c", 1600, 1200);
  gStyle->SetOptStat(0);


  // Define two TPads for distributions and ratio
  TPad *tPad = new TPad("tPad", "tPad", 0, 0.3, 1, 1);
  TPad *bPad = new TPad("bPad", "bPad", 0, 0,   1, 0.3);
  tPad->Draw();
  bPad->Draw("same");


  // Get integrals and normalize if needed
  double data_int = h_data->Integral(0, h_data->GetNbinsX()+1);
  double mc_int = h_mc->Integral(0, h_mc->GetNbinsX()+1);
  if (norm_to_1==true) {
    h_data->Scale(1/data_int);
    h_mc->Scale(1/mc_int); }


  // Top pad: hists
  tPad->cd();
  tPad->SetGrid();
  tPad->SetRightMargin(0.05);
  tPad->SetLeftMargin(0.07);
  tPad->SetBottomMargin(0.02);
  tPad->SetTopMargin(0.03);

  h_data->Draw("E1P");
  h_data->SetMarkerStyle(20);
  h_data->SetMarkerColor(1);
  h_data->SetMarkerSize(1.5);
  h_data->SetLineWidth(1);
  h_data->SetTitle("");
  h_data->GetXaxis()->SetLabelSize(0);
  if (norm_to_1 == true) {
    h_data->GetYaxis()->SetTitle("norm. to 1"); }
  else {
    gPad->SetLogy();
    h_data->GetYaxis()->SetTitle("#bf{N Events}"); }
  h_mc->Draw("e2 hist same");
  h_mc->SetLineColor(4);
  h_mc->SetLineWidth(4);
  h_mc->SetLineColor(2);

  TLegend *legend = new TLegend(0.7, 0.7, 0.95, 0.95);
  legend->AddEntry(h_data, legend_entries[0]);
  legend->AddEntry(h_mc, legend_entries[1]);
  legend->Draw("same");


  // Bottom pad: ratio
  bPad->cd();
  bPad->SetRightMargin(0.05);
  bPad->SetLeftMargin(0.07);
  bPad->SetTopMargin(0.02);
  bPad->SetBottomMargin(0.4);
  bPad->SetGrid();
  TH1 *h_ratio = (TH1*)h_data->Clone();

  h_ratio->Divide(h_mc);
  h_ratio->SetTitle("");

  h_ratio->GetXaxis()->SetLabelSize(0.10);
  h_ratio->GetXaxis()->SetTitle(title);
  h_ratio->GetXaxis()->SetTitleOffset(1.3);
  h_ratio->GetXaxis()->SetTitleSize(0.12);

  h_ratio->GetYaxis()->SetLabelSize(0.05);
  h_ratio->GetYaxis()->SetTitle("#bf{Data/mc}");
  h_ratio->GetYaxis()->SetTitleOffset(0.5);
  h_ratio->GetYaxis()->SetTitleSize(0.07);
  h_ratio->GetYaxis()->CenterTitle();
  h_ratio->GetYaxis()->SetRangeUser(0,2);
  h_ratio->GetYaxis()->SetNdivisions(8);
  h_ratio->Draw("E1");

  // Draw a line at R=1
  TLine *line = new TLine(h_ratio->GetXaxis()->GetXmin(), 1, h_ratio->GetXaxis()->GetXmax(), 1);
  line->SetLineColor(9);
  line->SetLineWidth(3);
  line->SetLineStyle(7);
  line->Draw("same");

  // Save the plot and print yileds
  c->Print("Plots/data_mc_comparison/"+savename+".png");

  cout << "Data integral = " << data_int << "\nmc integral =   " << mc_int << "\n\n" << endl;

  return 0;
}
