#include "common_headers.h"

// ###############################
// ## Draw Data/Bkgd Comparison ##
// ###############################
int draw_data_mc_plot(TH1 *h_data, std::vector<TH1*> h_mc, TString title, TString savename, std::vector<TString> legend_entries, bool norm_to_1){
  std::cout << "Drawing " << title << std::endl;
  std::vector<Int_t> colors = {4, 616-9, 416+1, 2, 9, 5, 18};


  if (h_mc.size()==0) {
    std::cout << "ERROR: Vector of mc hists is empty!" << std::endl;
    return 0; }
  if (h_mc.size() != legend_entries.size()) {
    std::cout << "WARNING: Number of MC hists != Number of legend entries!\n";
    int n_hists = h_mc.size();
    int n_titles = legend_entries.size();
    std::cout << n_hists << " vs " << n_titles << std::endl;
  }
    

  // Create a canvas
  TCanvas *c = new TCanvas("c", "c", 1600, 1200);


  // Remove stats
  gStyle->SetOptStat(0);

  
  // Define two TPads for distributions and ratio 
  TPad *tPad = new TPad("tPad", "tPad", 0, 0.31, 1, 1.0);
  TPad *bPad = new TPad("bPad", "bPad", 0, 0,    1, 0.3);
  tPad->Draw();
  bPad->Draw("same");

  
  // Get integrals and normalize if needed
  double data_int = h_data->Integral(0, h_data->GetNbinsX()+1);
  double mc_int = 0;
  for (int i=0; i<h_mc.size(); i++) { 
    double mc_int_tmp = h_mc[i]->Integral(0, h_mc[i]->GetNbinsX()+1);
    mc_int += mc_int_tmp;
    std::cout << legend_entries[i] << ": " << mc_int_tmp << std::endl; }
  if (norm_to_1==true) {
    h_data->Scale(1/data_int);
    for (int i=0; i<h_mc.size(); i++) {
      h_mc[i]->Scale(1/mc_int); } }


  // Create a stack of hists
  THStack *mc_stack = new THStack();
  for (int i=0; i<h_mc.size(); i++) { 
    h_mc[i]->SetFillColor(colors[i]);
    mc_stack->Add(h_mc[i]); }


  // Create one hist as a sum of mc (for data/mc ratio)
  TH1 *h_mc_combined = (TH1*)h_mc[0]->Clone();
  for (int i=1; i<h_mc.size(); i++) { h_mc_combined->Add(h_mc[i]); }

  
  // Top pad: hists
  tPad->cd();
  tPad->SetGrid();
  tPad->SetRightMargin(0.05);
  tPad->SetLeftMargin(0.07);
  tPad->SetBottomMargin(0.02);
  //tPad->SetTopMargin(0.03);

  mc_stack->Draw("hist");
  mc_stack->SetTitle("");
  mc_stack->GetXaxis()->SetLabelSize(0);
  mc_stack->GetYaxis()->SetLabelSize(0.04);
  mc_stack->GetYaxis()->SetTitleOffset(0.9);
  if (norm_to_1 == true) { mc_stack->GetYaxis()->SetTitle("norm. to 1");
  } else {
    gPad->SetLogy();
    mc_stack->GetYaxis()->SetTitle("#bf{N Events}"); 
    mc_stack->SetMinimum(1);
  }
  c->Update();

  TLatex *ltx = new TLatex();
  ltx->SetNDC();
  ltx->SetTextFont(42);
  ltx->SetTextSize(0.05);
  ltx->DrawLatex(0.07, 0.93, "#it{#bf{ATLAS}}");
  ltx->DrawLatex(0.25, 0.93, "Work in progress");
  ltx->DrawLatex(0.72, 0.93, "#sqrt{s}=13 TeV, 139.0 fb^{-1}");
  
  h_data->Draw("same E1P");
  h_data->SetMarkerStyle(20);
  h_data->SetMarkerColor(1);
  h_data->SetMarkerSize(1.5);
  h_data->SetLineWidth(1);

  double legend_y_min = 0.95 - (1 + h_mc.size())*0.04;
  TLegend *legend = new TLegend(0.75, legend_y_min, 0.95, 0.90);
  legend->SetNColumns(2);
  legend->SetFillStyle(0);
  legend->AddEntry(h_data, "Data");
  for (int i=0; i<h_mc.size(); i++) { legend->AddEntry(h_mc[i], legend_entries[i]); }
  legend->Draw("same");

  
  // Bottom pad: ratio
  bPad->cd();
  bPad->SetRightMargin(0.05);
  bPad->SetLeftMargin(0.07);
  bPad->SetTopMargin(0.02);
  bPad->SetBottomMargin(0.4);
  bPad->SetGrid();
  TH1 *h_ratio = (TH1*)h_data->Clone();

  h_ratio->Divide(h_mc_combined);
  h_ratio->SetTitle("");

  h_ratio->GetXaxis()->SetLabelSize(0.10);
  h_ratio->GetXaxis()->SetTitle(title);
  h_ratio->GetXaxis()->SetTitleOffset(1);
  h_ratio->GetXaxis()->SetTitleSize(0.12);

  h_ratio->GetYaxis()->SetLabelSize(0.07);
  h_ratio->GetYaxis()->SetTitle("#bf{Data/mc}");
  h_ratio->GetYaxis()->SetTitleOffset(0.4);
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
  c->Print("results/plots/data_mc/"+savename+".png");

  std::cout << "Data integral = " << data_int << "\nmc integral =   " << mc_int << "\n\n" << std::endl;
  

  return 0;
} // END of draw_data_mc





// ###########################
// ## Draw a few histograms ##
// ###########################
int draw_n_hists(std::vector<TH1*> h_vec, std::vector<TString> h_title, TString x_axis_title, TString title, bool normalize, Double_t y_min, Double_t y_max)
{
  // Draws N histograms in one canvas (not stacked)
  std::cout << "Start drawing " << title << " !" << std::endl;
  if (h_vec.size()==0) { std::cout << "h_vec is emmpty, aborting!!!" << std::endl; return 0; }

  std::vector<Int_t> colors = {632, 416+1, 600, 800-3, 432+2, 616+1, 400+1};

  gStyle->SetPaintTextFormat("0.3f");

  TCanvas *c = new TCanvas(h_title[0], h_title[0], 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();
  if (normalize==false) gPad->SetLogy();
  double legend_height = 0.09*h_vec.size();
  double legend_y1 = 0.90 - legend_height;
  TLegend *legend = new TLegend(0.70, legend_y1, 0.90, 0.90);

  for (int i=0; i<h_vec.size(); i++){

    if (!h_vec[i]) std::cout << "Requested object TH_[" << i << "] wasn't found!" << std::endl;

    double h_int = h_vec[i]->Integral(0, h_vec[i]->GetNbinsX()+1);
    double sf = 1/h_int;
    std::cout << "Hist: " << h_title[i] << " , Integral: " << h_int << std::endl;

    h_vec[i]->SetMarkerStyle(20);
    h_vec[i]->SetMarkerSize(2);
    h_vec[i]->SetMarkerColor(colors[i]);
    h_vec[i]->SetLineColor(colors[i]);
    h_vec[i]->SetLineWidth(4);
    if (normalize==true) h_vec[i]->Scale(sf);

    if (i==0) {
      h_vec[i]->Draw("hist text00");
      //h_vec[i]->Draw("hist"); 
      h_vec[i]->SetTitle(title);

      if (normalize==true) {
        h_vec[i]->GetYaxis()->SetRangeUser(y_min, y_max);
        h_vec[i]->GetYaxis()->SetTitle("#bf{Events, norm to 1}"); }
      else {
        if (y_min==0) y_min = 1;
        h_vec[i]->GetYaxis()->SetRangeUser(y_min, y_max);
        h_vec[i]->GetYaxis()->SetTitle("#bf{Events}"); }

      h_vec[i]->GetXaxis()->SetTitle(x_axis_title); }

    else { h_vec[i]->Draw("hist same text00"); }
    //else { h_vec[i]->Draw("hist same"); }
    legend->AddEntry(h_vec[i], h_title[i]); }
  legend->Draw("same");
  
  c->Print("results/plots/mc/" + title + ".png");
  std::cout << "Drawn " + title + " !\n\n" << std::endl;

  return 0;
} // END OF draw_n_hists





// ##########################
// ##   DRAW TMVA HISTS    ##
// ##########################
int draw_tmva_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, std::vector<TString> legend_entries_titles, bool norm_to_1)
{
  std::cout << "Drawing " << savename << std::endl;

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

  c->Print("results/plots/tmva_plots/"+savename+".png");
  std::cout << "Plotted " << savename << "\n\n" << std::endl;

  return 0;
} // END OF draw_tmva_hists





// ############################
// ##   Draw Correlations    ##
// ############################
int draw_correlations(TH2 *h_corr, TString title, std::vector<TString> axis_labels, TString savename)
{
  TCanvas *c = new TCanvas(savename, savename, 1600, 1200);
  TExec *ex = new TExec("exec", "gStyle->SetPaintTextFormat(\"0.2f\");");
  gStyle->SetOptStat(0);
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

  c->Print("results/tmva_plots/"+savename+".png");

  std::cout << "Plotted " << title << "\n\n" << std::endl;

  return 0;

} // END OF draw_correlations
