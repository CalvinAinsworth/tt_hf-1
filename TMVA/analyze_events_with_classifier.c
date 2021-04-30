#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>

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


  // Counter for events where two b-tags are from top
  float n_tl = 0; // total number of non-b-jets from top
  float n_nl = 0; // total number of non-b-jets not from top
  float n_tb = 0; // total number of b-jets from top
  float n_nb = 0; // total number of b-jets not from top
  float n_b = 0; // total number of truth b-jets
  float n_l = 0; // total number of truth non-b-jets
  float n_t = 0; // total number of jets from top
  float n_n = 0; // total number of jets not from top
  float n_b_tagged_and_truth = 0; // total number of truth b-jets tagged as b-jets
  float n_b_tagged_not_truth = 0; // total number of truth non-b-jets tagged as b-jets
  float n_classifier_and_truth = 0; // total number of jets from top on truth and classifier levels
  float n_classifier_not_truth = 0; // total number of jets from top on classifer level and not on truth level
  float n_total = 0; // total number of events

  // Loop over the events
  for (int entry=0; entry<nEntries; entry++) {
    
    // events counter
    if (entry%1000==0) { cout << "\t" << entry << '\r'; cout.flush(); }
    
    in_tree->GetEntry(entry);
    
    // Loop over jets
    for (int jet_i = 0; jet_i<(*jet_pt).size(); jet_i++) {
      
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
      if ( (*topHadronOriginFlag)[jet_i]==4 ) { n_t += weight; }
      else { n_n += weight; }

      // n_b_tagged_and_truth and n_b_tagged_not_truth
      if ( (*jet_isbtagged_DL1r_77)[jet_i]==1 ) {
	if ( (*jet_truthflav)[jet_i]==5 ) { n_b_tagged_and_truth += weight; }
	else { n_b_tagged_not_truth += weight; }
      } // if jet is b-tagged
      
      // n_classifier_and_truth and n_classifier_not_truth
      if ( (*classifier)[jet_i] >= 0.2 ) {
	if ( (*topHadronOriginFlag)[jet_i]==4 ) { n_classifier_and_truth += weight; }
	else { n_classifier_not_truth += weight; }
      } // classifier - from top
      
      
    } // [jet_i] - loop over jets

    // Increment counters of events
    n_total += weight;
    
  } // [entry] - loop over entries



  // Compute elements for matrices A_tag and A_classifier
  float eta_b = n_b_tagged_and_truth / n_b;
  float eta_l = n_b_tagged_not_truth / n_l;
  float eta_t = n_classifier_and_truth / n_t;
  float eta_n = n_classifier_not_truth / n_n;
  cout << "1 - eta_l = " << 1-eta_l << "\n1 - eta_b = " << 1-eta_b << "\neta_l = " << eta_l << "\neta_b = " << eta_b << "\n1 - eta_n = " << 1-eta_n << "\n1 - eta_t = " << 1-eta_t << "\neta_n = " << eta_n << "\neta_t = " << eta_t << endl << endl;

  // Create all the matrices
  TMatrixF A_tag(2, 2);
  TArrayF A_tag_arr(4);
  A_tag_arr[0] = 1 - eta_l;
  A_tag_arr[1] = 1 - eta_b;
  A_tag_arr[2] = eta_l;
  A_tag_arr[3] = eta_b;
  A_tag.SetMatrixArray(A_tag_arr.GetArray());
  cout << "#######   A_tag matrix   #######" << endl;
  A_tag.Print();
    
  TMatrixF A_classifier(2, 2);
  TArrayF A_classifier_arr(4);
  A_classifier_arr[0] = 1 - eta_n;
  A_classifier_arr[1] = 1 - eta_t;
  A_classifier_arr[2] = eta_n;
  A_classifier_arr[3] = eta_t;
  A_classifier.SetMatrixArray(A_classifier_arr.GetArray());
  cout << "#######   A_classifier matrix   #######" << endl;
  A_classifier.Print();

  TMatrixF A_classifier_T = A_classifier.T();
  cout << "#######   A_classifier^T matrix   #######" << endl; 
  A_classifier_T.Print();

  TMatrixF N(2, 2);
  TArrayF N_arr(4);
  N_arr[0] = n_tl;
  N_arr[1] = n_nl;
  N_arr[2] = n_tb;
  N_arr[3] = n_nb;
  N.SetMatrixArray(N_arr.GetArray());
  cout << "#######   N matrix   #######" << endl;
  N.Print();

  TMatrixF N_prime(2, 2);
  N_prime = A_tag * N * A_classifier_T;
  cout << "#######   N_prime matrix   #######" << endl;
  N_prime.Print();
    
}
