// ################################
// ##   Draw Sig/Bkgd Comparison ##
// ################################
int draw_sig_bkgd_plot(TH1 *h_s, TH1 *h_b, TH1 *h_r, TString title, TString savename, vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1=false)
{
  cout << "Drawing " << title << endl;

  // Create a canvas
  TCanvas *c = new TCanvas("c", "c", 1600, 1200);
  gStyle->SetOptStat(0);

  
  // Normalize if needed
  double h_s_int = h_s->Integral(0, h_s->GetNbinsX()+1);
  double h_b_int = h_b->Integral(0, h_b->GetNbinsX()+1);
  if (norm_to_1 == true) {
    h_s->Scale(1/h_s_int);
    h_b->Scale(1/h_b_int); }


  // Define two TPads for distributions and ratio
  TPad *tPad = new TPad("tPad", "tPad", 0, 0.3, 1, 1);
  TPad *bPad = new TPad("bPad", "bPad", 0, 0,   1, 0.3);
  tPad->Draw();
  bPad->Draw("same");


  // Top pad: hists
  tPad->cd();
  tPad->SetGrid();
  tPad->SetRightMargin(0.05);
  tPad->SetLeftMargin(0.07);
  tPad->SetBottomMargin(0.02);
  tPad->SetTopMargin(0.03);

  h_s->SetLineColor(2);
  h_s->SetLineWidth(4);
  h_s->Draw("hist");
  h_s->SetTitle("");
  h_s->GetXaxis()->SetTitle("");
  h_s->GetXaxis()->SetLabelSize(0);
  if (norm_to_1 == true) {
    h_s->GetYaxis()->SetTitle("#bf{norm.}");
    h_s->GetYaxis()->SetRangeUser(0, 0.2); }
  else {
    //gPad->SetLogY();
    h_s->GetYaxis()->SetTitle("#bf{Jets}"); }
  h_s->GetYaxis()->SetTitleOffset(0.9);
  
  h_b->SetLineColor(4);
  h_b->SetLineWidth(4);
  h_b->Draw("same hist");

  TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(h_s, legend_entries_titles[0]);
  legend->AddEntry(h_b, legend_entries_titles[1]);
  legend->SetTextSize(0.04);
  legend->Draw("same");
  

  // Bottom pad: ratio 
  bPad->cd();
  bPad->SetGrid();
  bPad->SetRightMargin(0.05);
  bPad->SetLeftMargin(0.07);
  bPad->SetTopMargin(0.02);
  bPad->SetBottomMargin(0.3);
  
  h_r->SetTitle("");
  h_r->SetLineColor(1);
  h_r->SetLineWidth(4);

  h_r->GetXaxis()->SetLabelSize(0.10);
  h_r->GetXaxis()->SetTitle(title);
  h_r->GetXaxis()->SetTitleOffset(1.2);
  h_r->GetXaxis()->SetTitleSize(0.1);

  h_r->GetYaxis()->SetLabelSize(0.05);
  h_r->GetYaxis()->SetTitle("#int^{max}_{bin}Sig  #bf{/} #sqrt{#int^{max}_{bin}Bkg}");
  h_r->GetYaxis()->SetTitleOffset(0.4);
  h_r->GetYaxis()->SetTitleSize(0.07);
  h_r->GetYaxis()->CenterTitle();
  h_r->GetYaxis()->SetRangeUser(0,30);
  h_r->GetYaxis()->SetNdivisions(8);
  h_r->Draw("hist");


  // Save the plot
  c->Print("Plots/"+savename+".png");
  

  return 0;
}




// #################
// ##    MAIN     ##
// #################
void optimize_classifier_cut()
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


  // Open the TMVA output file
  TFile *tmva_file = new TFile("TMVA.root");


  // Open classifier hists
  TString var_hists_address = "dataset/Method_" + method_name + "/" + method_name + "/";
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_S");
  TH1 *h_NN_classifier_output_B = (TH1F*)tmva_file->Get(var_hists_address+"MVA_" + method_name + "_B");
 
  
  // Prepare hists
  int n_bins =  h_NN_classifier_output_S->GetNbinsX();
  double sig_total = h_NN_classifier_output_S->Integral(0, h_NN_classifier_output_S->GetNbinsX()+1);
  double bkg_total = h_NN_classifier_output_B->Integral(0, h_NN_classifier_output_B->GetNbinsX()+1);
 

  // Create a hist for ratio
  double x_min = h_NN_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_NN_classifier_output_S->GetXaxis()->GetBinUpEdge(h_NN_classifier_output_S->GetNbinsX());
  cout << "x_min = " << x_min << ";\tx_max = " << x_max << endl;
  TH1 *h_classifiers_ratio = new TH1F("ratio", "ratio", n_bins, x_min, x_max);


  // Open the test tree
  TTree *test_tree = (TTree*)tmva_file->Get("TestTree");
  float *weight, *KBDT_val;
  int *topHOF, *event_number;
  weight = KBDT_val = 0;
  topHOF = event_number = 0;
  

  // Find the most optimal KBDT value
  double classifiers_ratio_best = -99.; 
  double optimal_KBDT_cut = -99.;  
  
  for (int bin=0; bin<n_bins; bin++) {
    double sig_with_cut = h_NN_classifier_output_S->Integral(bin, n_bins);
    double bkg_with_cut = h_NN_classifier_output_B->Integral(bin, n_bins);
    if (bkg_with_cut==0) continue;
    
    double classifiers_ratio_tmp = sig_with_cut/sqrt(bkg_with_cut);
    h_classifiers_ratio->SetBinContent(bin, classifiers_ratio_tmp);
    cout << "Bin: " << bin << ";    Ratio: " << classifiers_ratio_tmp << ";   classifier val: " << h_NN_classifier_output_S->GetBinCenter(bin) << endl;
    if (classifiers_ratio_tmp > classifiers_ratio_best) {
      classifiers_ratio_best = classifiers_ratio_tmp;
      optimal_KBDT_cut = h_NN_classifier_output_S->GetBinCenter(bin); }
    
  } // [bin] loop over bins 

  cout << "\n\nThe optimal classifier value is " << optimal_KBDT_cut << endl;

  
  int classifier_draw = draw_sig_bkgd_plot(h_NN_classifier_output_S, h_NN_classifier_output_B, h_classifiers_ratio, method_name, "optimize_" + method_name + "_cut", {"Sig", "Bkgd"}, true);
}
