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
#include <TColor.h>

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

//Colors: red , green, blue, orange, cyan, magenta, yellow
vector<Int_t> colors = {632, 416+1, 600, 800-3, 432+2, 616+1, 400+1};



// ###########################
// ## Draw a few histograms ## 
// ###########################
int draw_n_hists(vector<TH1*> h_vec, vector<TString> h_title, TString x_axis_title, TString title, bool normalize=false, Double_t y_min=0, Double_t y_max=10000)
{
  // Draws N histogram on one canvas (not stacked)
  cout << "Start drawing " << title << " !" << endl;
  if (h_vec.size()==0) { cout << "h_vec is emmpty, aborting!!!" << endl; return 0; }
  
  
  TCanvas *c = new TCanvas(h_title[0], h_title[0], 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();
  if (normalize==false) gPad->SetLogy();
  double legend_height = 0.09*h_vec.size();
  double legend_y1 = 0.90 - legend_height;
  TLegend *legend = new TLegend(0.70, legend_y1, 0.90, 0.90);

  for (int i=0; i<h_vec.size(); i++){
     
    if (!h_vec[i]) cout << "Requested object TH_[" << i << "] wasn't found!" << endl;
    
    double h_int = h_vec[i]->Integral(0, h_vec[i]->GetNbinsX()+1);
    double sf = 1/h_int;
    cout << "Hist: " << h_title[i] << " , Integral: " << h_int << endl;
    
    h_vec[i]->SetMarkerStyle(20);
    h_vec[i]->SetMarkerSize(2);
    h_vec[i]->SetMarkerColor(colors[i]);
    h_vec[i]->SetLineColor(colors[i]);
    h_vec[i]->SetLineWidth(4);
    if (normalize==true) h_vec[i]->Scale(sf);
    
    if (i==0) {
      h_vec[i]->Draw("hist");
      h_vec[i]->SetTitle(title);
      
      if (normalize==true) {
	h_vec[i]->GetYaxis()->SetRangeUser(y_min, y_max);
	h_vec[i]->GetYaxis()->SetTitle("#bf{Events, norm to 1}"); }
      else {
	if (y_min==0) y_min = 1;
	h_vec[i]->GetYaxis()->SetRangeUser(y_min, y_max);
	h_vec[i]->GetYaxis()->SetTitle("#bf{Events}"); 
	gPad->SetLogy(); }
      
      h_vec[i]->GetXaxis()->SetTitle(x_axis_title); }
    
    else { h_vec[i]->Draw("hist same"); }
    legend->AddEntry(h_vec[i], h_title[i]); }
  legend->Draw("same");
  
  c->Print("Plots/mc_studies/" + title + ".png");
  cout << "Drawn " + title + " !\n\n" << endl;
  
  return 0;
}



// ################
// ##    MAIN    ##
// ################
void draw_hists()
{
  // Open the file with histograms
  TFile *hists_file_mc = TFile::Open("hists_mc.root");


  // Get histograms
  
  // dR_min - sig/bkg discrimination
  TH1 *h_minDeltaR_b_from_top_to_b = (TH1*)hists_file_mc->Get("2b_emu_OS_h_minDeltaR_b_from_top_to_b");
  TH1 *h_minDeltaR_b_not_from_top_to_b = (TH1*)hists_file_mc->Get("2b_emu_OS_h_minDeltaR_b_not_from_top_to_b");
  TH1 *h_minDeltaR_b_from_top_to_jet = (TH1*)hists_file_mc->Get("2b_emu_OS_h_minDeltaR_b_from_top_to_jet");
  TH1 *h_minDeltaR_b_not_from_top_to_jet = (TH1*)hists_file_mc->Get("2b_emu_OS_h_minDeltaR_b_not_from_top_to_jet");
  TH1 *h_minDeltaR_b_from_top_to_lep = (TH1*)hists_file_mc->Get("2b_emu_OS_h_minDeltaR_b_from_top_to_lep");
  TH1 *h_minDeltaR_b_not_from_top_to_lep = (TH1*)hists_file_mc->Get("2b_emu_OS_h_minDeltaR_b_not_from_top_to_lep");

  // jets parameters - sig/bkg discrimination
  TH1 *h_all_jets_not_from_top_pt = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_not_from_top_pt");
  TH1 *h_all_jets_from_top_pt = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_from_top_pt");
  TH1 *h_all_jets_not_from_top_eta = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_not_from_top_eta");
  TH1 *h_all_jets_from_top_eta = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_from_top_eta");

  // invariant mass - sig/bkg discrimination
  TH1 *h_inv_mass_lep_bjet_from_top_min_dR = (TH1*)hists_file_mc->Get("2b_emu_OS_h_inv_mass_lep_bjet_from_top_min_dR");
  TH1 *h_inv_mass_lep_bjet_not_from_top_min_dR = (TH1*)hists_file_mc->Get("2b_emu_OS_h_inv_mass_lep_bjet_not_from_top_min_dR");
  TH1 *h_min_inv_mass_lep_bjet_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_inv_mass_lep_bjet_from_top");
  TH1 *h_max_inv_mass_lep_bjet_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_max_inv_mass_lep_bjet_from_top");
  TH1 *h_min_inv_mass_lep_bjet_not_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_inv_mass_lep_bjet_not_from_top");
  TH1 *h_max_inv_mass_lep_bjet_not_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_max_inv_mass_lep_bjet_not_from_top");


  // Draw the plots in two steps:
  // (1) prepare different combinations of hists (vectors) to draw them on one canvas
  // (2) call a function to draw the collection of hists on one canvas

  // Common legend entries for sig/bkg comparisons
  vector<TString> discr_legend_titles = {"from top", "not from top"};

  // dR_min(b, other b)
  vector<TH1*> h_minDeltaR_b_to_b = {h_minDeltaR_b_from_top_to_b, h_minDeltaR_b_not_from_top_to_b};
  int draw_minDeltaR_b_to_b = draw_n_hists(h_minDeltaR_b_to_b, discr_legend_titles, "#bf{#DeltaR_{min}(b, other b)}", "min_dR_b_to_b", true, 0, 0.5);
  
  // dR_min(b, not b)
  vector<TH1*> h_minDeltaR_b_to_jet = {h_minDeltaR_b_from_top_to_jet, h_minDeltaR_b_not_from_top_to_jet};
  int draw_minDeltaR_b_to_jet = draw_n_hists(h_minDeltaR_b_to_jet, discr_legend_titles, "#bf{#DeltaR_{min}(b, not b)}", "min_dR_b_to_not_b", true, 0, 0.5);

  // dR_min(b, lep)
  vector<TH1*> h_minDeltaR_b_to_lep = {h_minDeltaR_b_from_top_to_lep, h_minDeltaR_b_not_from_top_to_lep};
  int draw_minDeltaR_b_to_lep = draw_n_hists(h_minDeltaR_b_to_lep, discr_legend_titles, "#bf{#DeltaR_{min}(b, lep)}", "min_dR_b_to_lep", true, 0, 0.5);

  // m(b, closest lep) 
  vector<TH1*> h_inv_mass_lep_bjet = {h_inv_mass_lep_bjet_from_top_min_dR, h_inv_mass_lep_bjet_not_from_top_min_dR};
  int drw_inv_mass_lep_bjet = draw_n_hists(h_inv_mass_lep_bjet, discr_legend_titles, "#bf{m(b, closest lep)}", "m_b_closest_lep", true, 0, 0.02);

  // m_min(b, lep)
  vector<TH1*> h_min_inv_mass_lep_bjet = {h_min_inv_mass_lep_bjet_from_top, h_min_inv_mass_lep_bjet_not_from_top};
  int draw_min_inv_mass_lep_bjet = draw_n_hists(h_min_inv_mass_lep_bjet, discr_legend_titles, "#bf{m_{min}(b, lep)}", "m_min_b_lep", true, 0, 0.02);

  // m_max(b, lep)
  vector<TH1*> h_max_inv_mass_lep_bjet = {h_max_inv_mass_lep_bjet_from_top, h_max_inv_mass_lep_bjet_not_from_top};
  int draw_max_inv_mass_lep_bjet = draw_n_hists(h_max_inv_mass_lep_bjet, discr_legend_titles, "#bf{m_{max}(b, lep)}", "m_max_b_lep", true, 0, 0.02);

  // jet_pt
  vector<TH1*> h_all_jets_pt = {h_all_jets_from_top_pt, h_all_jets_not_from_top_pt};
  int draw_all_jets_pt = draw_n_hists(h_all_jets_pt, discr_legend_titles, "#bf{p_{T}^{jets}}", "all_jets_pt", true, 0, 0.4);

  // jet_eta
  vector<TH1*> h_all_jets_eta = {h_all_jets_from_top_eta, h_all_jets_not_from_top_eta};
  int draw_all_jets_eta = draw_n_hists(h_all_jets_eta, discr_legend_titles, "#bf{jet #eta}", "all_jets_eta", true, 0, 0.1); 


  // Close the hists file
  hists_file_mc->Close();
}


