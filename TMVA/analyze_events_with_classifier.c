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
  TH1 h_jet_pt_topHOF = new TH1F("lead_jet_pT_topHOF", "lead_jet_pT_topHOF", 20, 0, 1000);
  TH1 h_jet_pt_classifier = new TH1F("lead_jet_pT_classifier", "lead_jet_pT_classifier", 20, 0, 1000);


  // Open the ttile - skimmed
  TFile *in_file = new TFile("skimmed_test_ntuple.root");
  TTree *in_tree = (TTree*)in_file->Get("test");

  
  // Set branches
  vector<int> *topHadronOriginFlag, *jet_isbtagged_DL1r_77, *jet_truthflav;
  vector<float> *jet_DL1r, *classifier, *jet_pt, *jet_eta, *jet_phi, *jet_e;
  topHadronOriginFlag = jet_isbtagged_DL1r_77 jet_truthflav;
  jet_DL1r = classifier = jet_pt = jet_pt = jet_eta = jet_phi = jet_e = 0;
  float tot_event_weight, met, el_pt, el_eta, el_phi, el_e, el_charge, mu_pt, mu_eta, mu_phi, mu_e, mu_charge;

  in_tree->SetBranchAddress("topHadronOriginFlag", &topHadronOriginFlag);
  in_tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77);
  in_tree->SetBranchAddress("jet_truthflav", &jet_truthflav);
  in_tree->SetBranchAddress("jet_DL1r", &jet_DL1r);
  in_tree->SetBranchAddress(method_name, &classifier);
  in_tree->SetBranchAddress("jet_pt", &jet_pt);
  in_tree->SetBranchAddress("jet_eta", &jet_eta);
  in_tree->SetBranchAddress("jet_phi", &jet_phi);
  in_tree->SetBranchAddress("jet_e", &jet_e);

  in_tree->SetBranchAddress("tot_event_weight", &tot_event_weight);
  in_tree->SetBranchAddress("met", &met);
  in_tree->SetBranchAddress("el_pt", &el_pt);
  in_tree->SetBranchAddress("el_eta", &el_eta);
  in_tree->SwtBranchAddress("el_phi", &el_phi);
  in_tree->SetBranchAddress("el_e", &el_e);
  in_tree->SetBranchAddress("el_charge", &el_charge);
  in_tree->SetBranchAddress("mu_pt", &mu_pt);
  in_tree->SetBranchAddress("mu_eta", &mu_eta);
  in_tree->SetBranchAddress("mu_phi", &mu_phi);
  in_tree->SetBranchAddress("mu_e", &mu_e);
  in_tree->SetBranchAddress("mu_charge", &mu_charge);


  // Get number of entries
  nEntries = in_tree->GetEntries();
  cout << "nEntreis = " << nEntries << endl;


  // Counter for events where two b-tags are from top
  float n_tl = 0; // total number of non-b-jets from top
  float n_nl = 0; // total number 
  float n_total = 0;

  // Loop over the events
  for (int entry=0; entry<nEntries; entry++) {
    
    in_tree->GetEntry(entry);

    int n_b_from_top = 0;
    
    // Loop over jets
    for (jet_t=0; jet_i<jet_pt.size(); jet_i++) {
     
    } // [jet_i] - loop over jets

    // Increment counters of events
    n_total += tot_event_weight;
    
  } // [entry] - loop over entries

}
