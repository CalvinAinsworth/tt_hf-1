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
  TH1 *h_min_dR_btag_from_top_to_btag = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_dR_btag_from_top_to_btag");
  TH1 *h_min_dR_btag_not_from_top_to_btag = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_dR_btag_not_from_top_to_btag");
  TH1 *h_min_dR_btag_from_top_to_jet = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_dR_btag_from_top_to_jet");
  TH1 *h_min_dR_btag_not_from_top_to_jet = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_dR_btag_not_from_top_to_jet");
  TH1 *h_min_dR_btag_from_top_to_lep = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_dR_btag_from_top_to_lep");
  TH1 *h_min_dR_btag_not_from_top_to_lep = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_dR_btag_not_from_top_to_lep");

  // jets parameters - sig/bkg discrimination
  TH1 *h_all_jets_not_from_top_pt = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_not_from_top_pt");
  TH1 *h_all_jets_from_top_pt = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_from_top_pt");
  TH1 *h_all_jets_not_from_top_eta = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_not_from_top_eta");
  TH1 *h_all_jets_from_top_eta = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_from_top_eta");
  TH1 *h_all_jets_not_from_top_phi = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_not_from_top_phi");
  TH1 *h_all_jets_from_top_phi = (TH1*)hists_file_mc->Get("2b_emu_OS_all_jets_from_top_phi");
  
  // invariant mass - sig/bkg discrimination
  TH1 *h_m_lep_btag_from_top_min_dR = (TH1*)hists_file_mc->Get("2b_emu_OS_h_m_lep_btag_from_top_min_dR");
  TH1 *h_m_lep_btag_not_from_top_min_dR = (TH1*)hists_file_mc->Get("2b_emu_OS_h_m_lep_btag_not_from_top_min_dR");
  TH1 *h_min_m_lep_btag_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_m_lep_btag_from_top");
  TH1 *h_max_m_lep_btag_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_max_m_lep_btag_from_top");
  TH1 *h_min_m_lep_btag_not_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_min_m_lep_btag_not_from_top");
  TH1 *h_max_m_lep_btag_not_from_top = (TH1*)hists_file_mc->Get("2b_emu_OS_h_max_m_lep_btag_not_from_top");

  // invariant mass of btags and their combinations
  TH1 *h_m_btag_top = (TH1*)hists_file_mc->Get("2b_emu_OS_m_btag_from_top");
  TH1 *h_m_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_btag_not_from_top");
  TH1 *h_m_btag_top_btag_top = (TH1*)hists_file_mc->Get("2b_emu_OS_m_btag_top_btag_top");
  TH1 *h_m_min_btag_top_btag_top = (TH1*)hists_file_mc->Get("2b_emu_OS_m_min_btag_top_btag_top");
  TH1 *h_m_max_btag_top_btag_top = (TH1*)hists_file_mc->Get("2b_emu_OS_m_max_btag_top_btag_top");
  TH1 *h_m_btag_top_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_btag_top_btag_other");
  TH1 *h_m_min_btag_top_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_min_btag_top_btag_other");
  TH1 *h_m_max_btag_top_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_max_btag_top_btag_other");
  TH1 *h_m_btag_other_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_btag_other_btag_other");
  TH1 *h_m_min_btag_other_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_min_btag_other_btag_other");
  TH1 *h_m_max_btag_other_btag_other = (TH1*)hists_file_mc->Get("2b_emu_OS_m_max_btag_other_btag_other");
  TH1 *h_m_three_tags = (TH1*)hists_file_mc->Get("3b_emu_OS_m_three_btags_excl");
  TH1 *h_m_four_tags = (TH1*)hists_file_mc->Get("4b_emu_OS_m_four_btags_incl");
  
  
  
  // Draw the plots in two steps:
  // (1) prepare different combinations of hists (vectors) to draw them on one canvas
  // (2) call a function to draw the collection of hists on one canvas
  
  // Common legend entries for sig/bkg comparisons
  vector<TString> discr_legend_titles = {"from top", "not from top"};
  
  // dR_min(btag, other btag)
  vector<TH1*> h_min_dR_btag_to_btag = {h_min_dR_btag_from_top_to_btag, h_min_dR_btag_not_from_top_to_btag};
  int draw_min_dR_btag_to_btag = draw_n_hists(h_min_dR_btag_to_btag, discr_legend_titles, "#bf{#DeltaR_{min}(btag, other btag)}", "min_dR_btag_to_btag", true, 0, 0.5);
  
  // dR_min(btag, not btag)
  vector<TH1*> h_min_dR_btag_to_jet = {h_min_dR_btag_from_top_to_jet, h_min_dR_btag_not_from_top_to_jet};
  int draw_min_dR_btag_to_jet = draw_n_hists(h_min_dR_btag_to_jet, discr_legend_titles, "#bf{#DeltaR_{min}(btag, not btag)}", "min_dR_btag_to_not_b", true, 0, 0.5);
  
  // dR_min(btag, lep)
  vector<TH1*> h_min_dR_btag_to_lep = {h_min_dR_btag_from_top_to_lep, h_min_dR_btag_not_from_top_to_lep};
  int draw_min_dR_btag_to_lep = draw_n_hists(h_min_dR_btag_to_lep, discr_legend_titles, "#bf{#DeltaR_{min}(btag, lep)}", "min_dR_btag_to_lep", true, 0, 0.5);
  
  // m(btag, closest lep) 
  vector<TH1*> h_m_lep_btag = {h_m_lep_btag_from_top_min_dR, h_m_lep_btag_not_from_top_min_dR};
  int drw_m_lep_btag = draw_n_hists(h_m_lep_btag, discr_legend_titles, "#bf{m(btag, closest lep)}", "m_btag_closest_lep", true, 0, 0.02);
  
  // m_min(btag, lep)
  vector<TH1*> h_min_m_lep_btag = {h_min_m_lep_btag_from_top, h_min_m_lep_btag_not_from_top};
  int draw_min_m_lep_btag = draw_n_hists(h_min_m_lep_btag, discr_legend_titles, "#bf{m_{min}(btag, lep)}", "m_min_btag_lep", true, 0, 0.02);
  
  // m_max(btag, lep)
  vector<TH1*> h_max_m_lep_btag = {h_max_m_lep_btag_from_top, h_max_m_lep_btag_not_from_top};
  int draw_max_m_lep_btag = draw_n_hists(h_max_m_lep_btag, discr_legend_titles, "#bf{m_{max}(btag, lep)}", "m_max_btag_lep", true, 0, 0.02);
  
  // jet_pt
  vector<TH1*> h_all_jets_pt = {h_all_jets_from_top_pt, h_all_jets_not_from_top_pt};
  int draw_all_jets_pt = draw_n_hists(h_all_jets_pt, discr_legend_titles, "#bf{p_{T}^{jets}}", "all_jets_pt", true, 0, 0.4);
  
  // jet_eta
  vector<TH1*> h_all_jets_eta = {h_all_jets_from_top_eta, h_all_jets_not_from_top_eta};
  int draw_all_jets_eta = draw_n_hists(h_all_jets_eta, discr_legend_titles, "#bf{jet #eta}", "all_jets_eta", true, 0, 0.1); 
  
  // jet_phi
  vector<TH1*> h_all_jets_phi = {h_all_jets_from_top_phi, h_all_jets_not_from_top_phi};
  int draw_all_jets_phi = draw_n_hists(h_all_jets_phi, discr_legend_titles, "#bf{jet #phi}", "all_jets_phi", true, 0, 0.1);
  
  // btag invariant mass
  vector<TH1*> h_m_btag = {h_m_btag_top, h_m_btag_other};
  int draw_m_btag = draw_n_hists(h_m_btag, discr_legend_titles, "#bf{m(btag)}", "m_btag", true, 0, 0.2);
  
  // three pairs of btags wrt origin: tt, tl, ll
  vector<TString> m_btags_pairs_legend = {"top-top", "top-other", "other-other"};
  vector<TH1*> h_m_btags_pairs = {h_m_btag_top_btag_top, h_m_btag_top_btag_other, h_m_btag_other_btag_other};
  int draw_m_btags_pairs = draw_n_hists(h_m_btags_pairs, m_btags_pairs_legend, "#bf{m(btag, btag)}", "m_btag_btag", true, 0, 0.1);
  vector<TH1*> h_m_min_btags_pairs = {h_m_min_btag_top_btag_top, h_m_min_btag_top_btag_other, h_m_min_btag_other_btag_other};
  int draw_m_min_btags_pairs = draw_n_hists(h_m_min_btags_pairs, m_btags_pairs_legend, "#bf{m_{min}(bteg, btag)}", "m_min_btag_btag", true, 0, 0.1);
  vector<TH1*> h_m_max_btags_pairs = {h_m_max_btag_top_btag_top, h_m_max_btag_top_btag_other, h_m_max_btag_other_btag_other};
  int draw_m_max_btags_pairs = draw_n_hists(h_m_max_btags_pairs, m_btags_pairs_legend, "#bf{m_{max}(btag, btag)}", "m_max_btag_btag", true, 0, 0.1);
  
  
  // tree (excl) and four (incl) btags system masses
  vector<TString> multiple_btags_legend = {"three btags (excl)", "four btags (incl)"};
  vector<TH1*> h_multiple_btags = {h_m_three_tags, h_m_four_tags};
  int draw_multiple_btags = draw_n_hists(h_multiple_btags, multiple_btags_legend, "#bf{m(N btags)}", "m_n_btags", true, 0, 0.1);


  // Close the hists file
  hists_file_mc->Close();
}


