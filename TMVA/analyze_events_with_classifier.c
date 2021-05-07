#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>

int const n_steps = 20; // number of steps in classifier cut on its whole range
int const n_channels = 3; // 2b3j excl, 3b3j excl, 4b4j incl - for the event level efficiency study
vector<TString> channel_names = {"2b3j excl.", "3b3j excl.", "4b4j incl."};
vector<Int_t> colors = {4, 417, 617, 433, 2};



//////////////////////////
//     Draw TGraphs     //
/////////////////////////
int draw_graphs(vector<TGraph*> gr, TString x_axis_title, TString y_axis_title, vector<TString> legend_entries, TString savename)
{
  // Create a canvas
  TCanvas *c = new TCanvas("c", "c", 1600, 1200);
  gStyle->SetOptStat(0);
  gPad->SetGrid();

  // Create a legend
  TLegend *legend = new TLegend(0.7, 0.6, 0.9, 0.8);

  // Draw everything nicely
  for (int i=0; i<gr.size(); i++) {
    gr[i]->SetLineColor(colors[i]);
    gr[i]->SetLineWidth(4);

    if (i==0) {
      gr[i]->Draw("AC");
      gr[i]->SetTitle("");
      gr[i]->GetXaxis()->SetTitle(x_axis_title);
      gr[i]->GetYaxis()->SetTitle(y_axis_title);
      gr[i]->GetYaxis()->SetRangeUser(0, 1.0); }
    else {
      gr[i]->Draw("same"); }

    legend->AddEntry(gr[i], legend_entries[i]);

    // Reference values from slide 11: 
    // https://indico.cern.ch/event/888748/contributions/3747758/attachments/1986520/3310303/AssigningbJetsToTops.pdf
    TLine *line_3j3b = new TLine(gr[0]->GetXaxis()->GetXmin(), 0.53, gr[0]->GetXaxis()->GetXmax(), 0.53);
    line_3j3b->SetLineColor(colors[1]);
    line_3j3b->SetLineWidth(3);
    line_3j3b->SetLineStyle(7);
    line_3j3b->Draw("same");

    TLine *line_4j4jb = new TLine(gr[0]->GetXaxis()->GetXmin(), 0.56, gr[0]->GetXaxis()->GetXmax(), 0.56);
    line_4j4jb->SetLineColor(colors[2]);
    line_4j4jb->SetLineWidth(3);
    line_4j4jb->SetLineStyle(7);
    line_4j4jb->Draw("same");
    
  } // [i] - loop over graphs

  legend->Draw("save");

  // Save the plot
  c->Print("Plots/" + savename + ".png");
  
  return 0;
}



//////////////////
//     MAIN     //
/////////////////
void analyze_events_with_classifier()
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
  

  // Set Classifier cuts in the whole range of the classifier
  TFile *tmva_file = new TFile("TMVA.root");
  TH1 *h_NN_classifier_output_S = (TH1F*)tmva_file->Get("dataset/Method_" + method_name + "/" + method_name + "/MVA_" + method_name + "_S");
  double x_min = h_NN_classifier_output_S->GetXaxis()->GetBinLowEdge(1);
  double x_max = h_NN_classifier_output_S->GetXaxis()->GetBinUpEdge(h_NN_classifier_output_S->GetNbinsX());
  double step = (x_max - x_min)/n_steps;
  vector<float> classifier_cuts = {};
  for (double i=x_min; i<=x_max; i += step) classifier_cuts.push_back(i);
  tmva_file->Close();
  

  // Declare hists
  TH1F *h_jet_pt_topHOF = new TH1F("lead_jet_pT_topHOF", "lead_jet_pT_topHOF", 20, 0, 1000);
  TH1F *h_jet_pt_classifier = new TH1F("lead_jet_pT_classifier", "lead_jet_pT_classifier", 20, 0, 1000);


  // Open the ttile - skimmed
  TFile *in_file = new TFile("skimmed_test_ntuple.root");
  TTree *in_tree = (TTree*)in_file->Get("test");

  
  // Set branches
  vector<int> *topHadronOriginFlag, *jet_isbtagged_DL1r_77, *jet_truthflav;
  vector<float> *jet_DL1r, *classifier, *jet_pt, *jet_eta, *jet_phi, *jet_e;
  topHadronOriginFlag = jet_isbtagged_DL1r_77 = jet_truthflav = 0;
  jet_DL1r = classifier = jet_pt = jet_pt = jet_eta = jet_phi = jet_e = 0;
  float weight, met, el_pt, el_eta, el_phi, el_e, el_charge, mu_pt, mu_eta, mu_phi, mu_e, mu_charge;

  in_tree->SetBranchAddress("topHadronOriginFlag", &topHadronOriginFlag);
  in_tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77);
  in_tree->SetBranchAddress("jet_truthflav", &jet_truthflav);
  in_tree->SetBranchAddress("jet_DL1r", &jet_DL1r);
  in_tree->SetBranchAddress(method_name, &classifier);
  in_tree->SetBranchAddress("jet_pt", &jet_pt);
  in_tree->SetBranchAddress("jet_eta", &jet_eta);
  in_tree->SetBranchAddress("jet_phi", &jet_phi);
  in_tree->SetBranchAddress("jet_e", &jet_e);

  in_tree->SetBranchAddress("tot_event_weight", &weight);
  in_tree->SetBranchAddress("met", &met);
  in_tree->SetBranchAddress("el_pt", &el_pt);
  in_tree->SetBranchAddress("el_eta", &el_eta);
  in_tree->SetBranchAddress("el_phi", &el_phi);
  in_tree->SetBranchAddress("el_e", &el_e);
  in_tree->SetBranchAddress("el_charge", &el_charge);
  in_tree->SetBranchAddress("mu_pt", &mu_pt);
  in_tree->SetBranchAddress("mu_eta", &mu_eta);
  in_tree->SetBranchAddress("mu_phi", &mu_phi);
  in_tree->SetBranchAddress("mu_e", &mu_e);
  in_tree->SetBranchAddress("mu_charge", &mu_charge);


  // Get number of entries
  int nEntries = in_tree->GetEntries();
  cout << "nEntreis = " << nEntries << endl;


  // Counters for events where two b-tags are from top
  float n_tl = 0; // total number of non-b-jets from top
  float n_nl = 0; // total number of non-b-jets not from top
  float n_tb = 0; // total number of b-jets from top
  float n_nb = 0; // total number of b-jets not from top
  float n_b = 0; // total number of truth b-jets
  float n_l = 0; // total number of truth non-b-jets
  float n_t[n_steps]; // total number of jets from top
  float n_n[n_steps]; // total number of jets not from top
  float n_b_tagged_and_truth = 0; // total number of truth b-jets tagged as b-jets
  float n_b_tagged_not_truth = 0; // total number of truth non-b-jets tagged as b-jets
  float n_classifier_and_truth[n_steps]; // total number of jets from top on truth and classifier levels
  float n_classifier_not_truth[n_steps]; // total number of jets from top on classifer level and not on truth level
  float n_total = 0; // total number of events


  // Set counters for event level efficiency calculations
  float n_good_events[n_channels][n_steps] = {0}; // number of evetns with correctly identified jets from top
  float n_all_events[n_channels][n_steps] = {0}; // number of all events
  float event_level_efficiency[n_channels][n_steps] = {0}; // [4] = 2b3j excl, 3b3j excl, 4b4j incl



  // Loop over the events
  for (int entry=0; entry<nEntries; entry++) {
    
    // events counter
    if (entry%1000==0) { cout << "\t" << entry << '\r'; cout.flush(); }
    
    in_tree->GetEntry(entry);

    
    // Selections
    bool b_tags_selection[n_channels] = {false};
    int n_tagged_jets = 0;
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      if ( (*jet_isbtagged_DL1r_77)[jet_i]==1 ) n_tagged_jets++ ;
    } // [jet_i] - loop over jets
    if (n_tagged_jets==2) b_tags_selection[0] = true;
    if (n_tagged_jets==3) b_tags_selection[1] = true;
    if (n_tagged_jets>=4) b_tags_selection[2] = true;
    
    

    // A trick to "arrange" jets wrt other than pT variable
    
    // create a vector of pairs that will consist of {classifier_val, jet_#} pairs
    vector<pair<float, int>> jet_classifier_jetnumber = {};
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      jet_classifier_jetnumber.push_back( make_pair( (*classifier)[jet_i], jet_i) );
    } // [jet_i] - loop over jets
    
    // Sort the vector of pair wrt the classifier value from the lowest to the highst
    sort(jet_classifier_jetnumber.begin(), jet_classifier_jetnumber.end());
    
    // Declare a vector of indeces from the original order for the jets "ordered" wrt the classifier value
    vector<int> JN;
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      JN.push_back(jet_classifier_jetnumber[jet_i].second);
    } // [jet_i] - loop over jets
    


    // Event level efficiency study
    int n_missidentified[n_channels][n_steps] = {0}; // number of b-tags missidentified by BDT as jets **from top**
    
    for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
      //cout << "\n\nBDT cut: " << classifier_cuts[cut_iter] << endl;
      for (int channel=0; channel<n_channels; channel++) {
	
	//cout << "Channel: " << channel_names[channel] << endl;
	if (b_tags_selection[channel]==false) continue;

	int n_bftc = 0; // number of B-tagged && From Top by Classifier (BFTC) jets 
	
	for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
	
	  //cout << "\tJet[" << jet_i << "]: BDT = " << (*classifier)[JN[jet_i]] << ", tag = " << (*jet_isbtagged_DL1r_77)[JN[jet_i]] << ", topHOF = " << (*topHadronOriginFlag)[JN[jet_i]] << endl; 
	  
	  if ( (*classifier)[JN[jet_i]] < classifier_cuts[cut_iter] && (*jet_isbtagged_DL1r_77)[JN[jet_i]]==1 ) {
	    n_bftc++ ;
	    if (n_bftc > 2) continue;
	    if ( (*topHadronOriginFlag)[JN[jet_i]]!=4 )  n_missidentified[channel][cut_iter]++ ;
	  } // [if] - classifier<cut && b-tagged
	  
	} // [jet_i] - loop over jets 

	//cout << "\tN missidentified jets: " << n_missidentified[channel][cut_iter] << endl;
	//cout << "\tN b-tagged jets from top: " << n_bftc << endl;
	if (n_bftc>=2 && n_missidentified[channel][cut_iter]==0) n_good_events[channel][cut_iter] += weight ;
        n_all_events[channel][cut_iter] += weight;

      } // [channel] - loop over 2b3j, 3b3j, 4b4j channels
    } // [cut_iter] - loop over classifier cuts



    // Predicted number of events study varuables
    // Loop over jets
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      
      // Jets number predictions
      // n_tb, n_tl, n_nb, n_nl
      if ( (*topHadronOriginFlag)[jet_i]==4 ) {
	if ( (*jet_truthflav)[jet_i]==5 ) { n_tb += weight; }
	else { n_tl += weight; }
      } // from top
      else {
	if ( (*jet_truthflav)[jet_i]==5 ) { n_nb += weight; }
	else { n_nl += weight; }
      } // not from top
      
      // n_b and n_l
      if ( (*jet_truthflav)[jet_i]==5 ) { n_b += weight; }
      else { n_l += weight; }
      
      // n_t and n_n
      for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
	if ( (*topHadronOriginFlag)[jet_i]==4 ) { n_t[cut_iter] += weight; }
	else { n_n[cut_iter] += weight; }
      } // [cut_iter] - loop over classifier cuts

      // n_b_tagged_and_truth and n_b_tagged_not_truth
      if ( (*jet_isbtagged_DL1r_77)[jet_i]==1 ) {
	if ( (*jet_truthflav)[jet_i]==5 ) { n_b_tagged_and_truth += weight; }
	else { n_b_tagged_not_truth += weight; }
      } // if jet is b-tagged
      
      // n_classifier_and_truth and n_classifier_not_truth
      for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
	if ( (*classifier)[jet_i] >= classifier_cuts[cut_iter] ) {
	  if ( (*topHadronOriginFlag)[jet_i]==4 ) { n_classifier_and_truth[cut_iter] += weight; }
	  else { n_classifier_not_truth[cut_iter] += weight; }
	} // classifier - from top
      } // [cut_iter] - loop over classifier cuts
      	
      
    } // [jet_i] - loop over jets
    

    // Increment counters of events
    n_total += weight;
    
  } // [entry] - loop over entries



  // Event level efficiency
  for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
    cout << "\n\n" << method_name << " cut: " << classifier_cuts[cut_iter] << endl;
    for (int channel=0; channel<n_channels; channel++) {
      
      event_level_efficiency[channel][cut_iter] = n_good_events[channel][cut_iter] / n_all_events[channel][cut_iter];
      cout << "Channel: " << channel_names[channel] << ",\teff. = " << event_level_efficiency[channel][cut_iter] << ",\tN good events = " << n_good_events[channel][cut_iter] << ",\tN all events = " << n_all_events[channel][cut_iter] << endl;
      
    } // [channel] - loop over 2b3j, 3b3j, 4b4j channels
  } // [cut_iter] - loop over classifier cuts

  vector<TGraph*> gr_efficiency;
  for (int channel=0; channel<n_channels; channel++) {
    TGraph *gr_tmp = new TGraph(n_steps, &classifier_cuts[0], event_level_efficiency[channel]);
    gr_efficiency.push_back(gr_tmp);
  } // [channel]
  int draw_gr_efficiency = draw_graphs(gr_efficiency, method_name, "Efficiency", channel_names, "event_level_efficiencies");
  


  // Compute elements for matrices A_tag and A_classifier
  float eta_b = n_b_tagged_and_truth / n_b;
  float eta_l = n_b_tagged_not_truth / n_l;
  float eta_t[n_steps];
  float eta_n[n_steps];
  for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
    eta_t[cut_iter] = n_classifier_and_truth[cut_iter] / n_t[cut_iter];
    eta_n[cut_iter] = n_classifier_not_truth[cut_iter] / n_n[cut_iter];
  } // [cut_iter] - loop over classifier cuts
  //cout << "1 - eta_l = " << 1-eta_l << "\n1 - eta_b = " << 1-eta_b << "\neta_l = " << eta_l << "\neta_b = " << eta_b << "\n1 - eta_n = " << 1-eta_n << "\n1 - eta_t = " << 1-eta_t << "\neta_n = " << eta_n << "\neta_t = " << eta_t << endl << endl;

  // Create all the matrices
  TMatrixF A_tag(2, 2);
  TArrayF A_tag_arr(4);
  A_tag_arr[0] = 1 - eta_l;
  A_tag_arr[1] = 1 - eta_b;
  A_tag_arr[2] = eta_l;
  A_tag_arr[3] = eta_b;
  A_tag.SetMatrixArray(A_tag_arr.GetArray());
  //cout << "#######   A_tag matrix   #######" << endl;
  //A_tag.Print();
    
  TMatrixF N(2, 2);
  TArrayF N_arr(4);
  N_arr[0] = n_tl;
  N_arr[1] = n_nl;
  N_arr[2] = n_tb;
  N_arr[3] = n_nb;
  N.SetMatrixArray(N_arr.GetArray());
  //cout << "#######   N matrix   #######" << endl;
  //N.Print();
  
  // Create a txt file with results
  ofstream results_file("n_events_predictions.txt", ios::binary);
  
  for (int cut_iter=0; cut_iter<n_steps; cut_iter++) {
    //cout << "\n\nCLassifier cut: " << classifier_cuts[cut_iter] << endl;

    TMatrixF A_classifier(2, 2);
    TArrayF A_classifier_arr(4);
    A_classifier_arr[0] = 1 - eta_n[cut_iter];
    A_classifier_arr[1] = 1 - eta_t[cut_iter];
    A_classifier_arr[2] = eta_n[cut_iter];
    A_classifier_arr[3] = eta_t[cut_iter];
    A_classifier.SetMatrixArray(A_classifier_arr.GetArray());
    //cout << "#######   A_classifier matrix   #######" << endl;
    //A_classifier.Print();
    
    TMatrixF A_classifier_T = A_classifier.T();
    //cout << "#######   A_classifier^T matrix   #######" << endl; 
    //A_classifier_T.Print();
    
    TMatrixF N_prime(2, 2);
    N_prime = A_tag * N * A_classifier_T;
    //cout << "#######   N_prime matrix   #######" << endl;
    //N_prime.Print();

    //cout << "N[rown = 0][coln = 0] = " << N_prime(0, 0) << endl;
    //cout << "N[rown = 0][coln = 1] = " << N_prime(0, 1) << endl;
    //cout << "N[rown = 1][coln = 0] = " << N_prime(1, 0) << endl;
    //cout << "N[rown = 1][coln = 1] = " << N_prime(1, 1) << endl;

    results_file << "Predictions in Data for " + method_name + " cut >= " <<  classifier_cuts[cut_iter] << ":" << endl;
    results_file << "N(" + method_name + " from top, untagged) = " << N_prime(0, 0) << endl;
    results_file << "N(" + method_name + " not from top, untagged) = " <<  N_prime(0, 1) << endl;
    results_file << "N(" + method_name + " from top, tagged) = " << N_prime(1, 0) << endl;
    results_file << "N(" + method_name + " not from top, tagged = " <<N_prime(1, 1) << endl << endl; 
    
  } // [cut_iter] - loop over classifier cuts

  results_file.close();

    
}
