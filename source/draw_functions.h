int draw_data_mc_plot(TH1 *h_data, std::vector<TH1*> h_mc, TString title, TString savename, std::vector<TString> legend_entries, bool norm_to_1 = true);

int draw_n_hists(std::vector<TH1*> h_vec, std::vector<TString> h_title, TString x_axis_title, TString title, bool normalize=false, Double_t y_min=1, Double_t y_max=10000, bool setlogy=true);

int draw_tmva_hists(TH1 *h_s, TH1 *h_b, TString x_axis_title, TString savename, std::vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1 = true);

int draw_correlations(TH2 *h_corr, TString title, std::vector<TString> axis_labels, TString savename);

int draw_sig_bkgd_plot(TH1 *h_s, TH1 *h_b, TH1 *h_r, TString title, TString savename, std::vector<TString> legend_entries_titles= {"Sig", "Bkgd"}, bool norm_to_1=false);

int draw_graphs(std::vector<TGraph*> gr, TString x_axis_title, TString y_axis_title, std::vector<TString> legend_entries, TString savename);
