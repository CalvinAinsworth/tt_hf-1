#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>


///////////////////
//     MAIN      //
///////////////////
void create_skimmed_ntuple()
{
  // Get name of the TMVA method
  TString method_name = "";
  ifstream config_file("tmva_config.txt", ifstream::binary);
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



  // Set some variables for the analysis
  double classifier_cut = 0.1;
  int n_jets_from_top = 0;
  map<int, map<int, vector<double>> > event_jet_jetinfo;
  // { {event_0: {jet_0 - {DL1r, KBDT, etc}}, {jet_1 - {DL1r, KBDT, etc}} ... },
  //   {event_1: {jet_0 - {DL1r, KBDT, etc}}, {jet_1 - {DL1r, KBDT, etc}} ... }, etc }
  map<int, vector<double>> event_lepinfo; // + weight and met!
  // { {event_0: {weight, met, el_pt, el_eta, el_phi, el_e, el_charge, mu_pt, ..},
  //   {event_1: {weight, met, el_pt, el_eta, el_phi, el_e, el_charge, mu_pt, ..}, etc }


  // Open the tfile - mtva output
  TFile *tmva_file = new TFile("TMVA.root");
  
  
  // Open trees
  vector<TString> tree_names = {"TestTree", "TrainTree"};
  for (int tree_count=0; tree_count<tree_names.size(); tree_count++) {
    TTree *tmva_tree = (TTree*)tmva_file->Get("dataset/" + tree_names[tree_count]);
    cout << "working with " << tree_names[tree_count] << " tree!" << endl;
    
    
    // Set branches
    float topHOF, jet_isbtagged_DL1r_77, jet_DL1r, classifier, weight, jet_pt, jet_eta, jet_phi, jet_e;
    float mu_pt, mu_eta, mu_phi, mu_e, mu_charge;
    float el_pt, el_eta, el_phi, el_e, el_charge; 
    float event_number, met;
    topHOF = jet_isbtagged_DL1r_77 = jet_DL1r = weight = jet_pt = jet_eta = jet_phi, jet_e = 0;
    mu_pt = mu_eta = mu_phi = mu_e = mu_charge = 0;
    el_pt = el_eta = el_phi = el_e = el_charge = 0;
    event_number = met = 0;

    tmva_tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
    tmva_tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77);
    tmva_tree->SetBranchAddress("NN_jet_DL1r", &jet_DL1r);
    tmva_tree->SetBranchAddress(method_name, &classifier);
    tmva_tree->SetBranchAddress("NN_tot_event_weight", &weight);
    tmva_tree->SetBranchAddress("NN_jet_pt", &jet_pt);
    tmva_tree->SetBranchAddress("NN_jet_eta", &jet_eta);
    tmva_tree->SetBranchAddress("NN_jet_phi", &jet_phi);
    tmva_tree->SetBranchAddress("NN_jet_e", &jet_e);
    tmva_tree->SetBranchAddress("NN_mu_pt", &mu_pt);
    tmva_tree->SetBranchAddress("NN_mu_eta", &mu_eta);
    tmva_tree->SetBranchAddress("NN_mu_phi", &mu_phi);
    tmva_tree->SetBranchAddress("NN_mu_e", &mu_e);
    tmva_tree->SetBranchAddress("NN_mu_charge", &mu_charge);
    tmva_tree->SetBranchAddress("NN_el_pt", &el_pt);
    tmva_tree->SetBranchAddress("NN_el_eta", &el_eta);
    tmva_tree->SetBranchAddress("NN_el_phi", &el_phi);
    tmva_tree->SetBranchAddress("NN_el_e", &el_e);
    tmva_tree->SetBranchAddress("NN_el_charge", &el_charge);
    tmva_tree->SetBranchAddress("NN_event_number", &event_number);
    tmva_tree->SetBranchAddress("NN_met", &met);

    
    // Get number of entries
    Int_t nEntries = tmva_tree->GetEntries();
    cout << "nEntries: " << nEntries << endl << endl;
    
  
    for (int entry=0; entry<nEntries; entry++) {
      
      if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
      tmva_tree->GetEntry(entry);
   
    
      // Black magic
      // jets info
      map<int, vector<double> > jet_jetinfo;
      vector<double> jetinfo = {topHOF, jet_isbtagged_DL1r_77, jet_DL1r, classifier, jet_pt, jet_eta, jet_phi, jet_e};
      int jetn;
    
      map<int, map<int, vector<double>> >::iterator it1 = event_jet_jetinfo.find(event_number);
      if ( it1 != event_jet_jetinfo.end() ) {
	jet_jetinfo = it1->second;
	jetn = jet_jetinfo.size();
	jet_jetinfo.insert(make_pair(jetn, jetinfo));
	it1->second = jet_jetinfo; }
      else {
	jet_jetinfo.insert(make_pair(0, jetinfo));
	event_jet_jetinfo.insert(make_pair(event_number, jet_jetinfo)); }
      
      // leptons info + met + weight
      vector<double> lepinfo = {weight, met, el_pt, el_eta, el_phi, el_e, el_charge, mu_pt, mu_eta, mu_phi, mu_e, mu_charge};
      map<int, vector<double>>::iterator it2 = event_lepinfo.find(event_number);
      if ( event_lepinfo.count(event_number) == 0 ) event_lepinfo.insert(make_pair(event_number, lepinfo));
      
    } // [entry] - loop over entries
    
  } // [tree_count] - loop over trees
    
  tmva_file->Close();


  // ///
  // Work with the map of events
  // ///

  int n_jets_in_map_map = 0;
  
  // Create a new TFile and TTree
  TFile *ntuple = new TFile("skimmed_test_ntuple.root", "RECREATE");
  TTree *tree = new TTree("test", "Tree with entries from test tmva tree");

  // Declare variables for a tree
  int event_number_save;
  vector<float> topHOF_save, jet_isbtagged_DL1r_77_save, jet_DL1r_save, classifier_save, jet_pt_save, jet_eta_save, jet_phi_save, jet_e_save;
  float weight_save;
  float mu_pt_save, mu_eta_save, mu_phi_save, mu_e_save, mu_charge_save;
  float el_pt_save, el_eta_save, el_phi_save, el_e_save, el_charge_save;
  float met_save;
  event_number_save = 0;

  // Declare new branches
  tree->Branch("topHadronOriginFlag", &topHOF_save);
  tree->Branch("jet_isbtagged_DL1r_77", &jet_isbtagged_DL1r_77_save);
  tree->Branch("jet_DL1r", &jet_DL1r_save);
  tree->Branch(method_name, &classifier_save);
  tree->Branch("tot_event_weight", &weight_save, "tot_event_weight/F");
  tree->Branch("jet_pt", &jet_pt_save);
  tree->Branch("jet_eta", &jet_eta_save);
  tree->Branch("jet_phi", &jet_phi_save);
  tree->Branch("jet_e", &jet_e_save);
  tree->Branch("mu_pt", &mu_pt_save);
  tree->Branch("mu_eta", &mu_eta_save);
  tree->Branch("mu_phi", &mu_phi_save);
  tree->Branch("mu_e", &mu_e_save);
  tree->Branch("mu_charge", &mu_charge_save);
  tree->Branch("el_pt", &el_pt_save);
  tree->Branch("el_eta", &el_eta_save);
  tree->Branch("el_phi", &el_phi_save);
  tree->Branch("el_e", &el_e_save);
  tree->Branch("el_charge", &el_charge_save);
  tree->Branch("event_number", &event_number_save, "event_number/I");
  tree->Branch("met", &met_save, "met/F");
  

  // Run a loop over keys - event numbers
  map<int, map<int, vector<double>> >::iterator it1;
  for (it1 = event_jet_jetinfo.begin(); it1 != event_jet_jetinfo.end(); it1++) {

    // Work out jets info
    topHOF_save.clear();
    jet_isbtagged_DL1r_77_save.clear();
    jet_DL1r_save.clear();
    classifier_save.clear();
    jet_pt_save.clear();
    jet_eta_save.clear();
    jet_phi_save.clear();
    jet_e_save.clear();
    
    //cout << "\n" << it1->first << endl;

    // Set a map for jets=keys and properties=values for the event
    map<int, vector<double> > jet_jetinfo = it1->second;
    
    // Run a loop over keys of one event - jets (that are not in any reasonable order)
    map<int, vector<double> >::iterator it2;
    for (it2 = jet_jetinfo.begin(); it2 != jet_jetinfo.end(); it2++) {
      int jetn = it2->first;
      vector<double> jet_pars = it2->second;
      n_jets_in_map_map++;
      
      //cout << "\tjet_# = " << jetn << ";\t dl1r = " << jet_pars[0] << ";\t classifier = " << jet_pars[1] << ";\tweight = " << jet_pars[2] << ";\ttopHOF = " << jet_pars[3] << endl; 

      topHOF_save.push_back(jet_pars[0]);
      jet_isbtagged_DL1r_77_save.push_back(jet_pars[1]);
      jet_DL1r_save.push_back(jet_pars[2]);
      classifier_save.push_back(jet_pars[3]);
      jet_pt_save.push_back(jet_pars[4]);
      jet_eta_save.push_back(jet_pars[5]);
      jet_phi_save.push_back(jet_pars[6]);
      
    } // [it2] - loop over jets of an event
   

    // Work out leptons info, met and event number
    event_number_save = it1->first;
    weight_save = (event_lepinfo[event_number_save])[0];
    met_save = (event_lepinfo[event_number_save])[1];
    el_pt_save = (event_lepinfo[event_number_save])[2];
    el_eta_save = (event_lepinfo[event_number_save])[3];
    el_phi_save = (event_lepinfo[event_number_save])[4];
    el_e_save = (event_lepinfo[event_number_save])[5];
    el_charge_save = (event_lepinfo[event_number_save])[6];
    mu_pt_save = (event_lepinfo[event_number_save])[7];
    mu_eta_save = (event_lepinfo[event_number_save])[8];
    mu_phi_save = (event_lepinfo[event_number_save])[9];
    mu_e_save = (event_lepinfo[event_number_save])[10];
    mu_charge_save = (event_lepinfo[event_number_save])[11];

    
    tree->Fill();
    
    // Release some space
    event_jet_jetinfo.erase(it1);
    
  } // [it1] - loop over events
  

  tree->Write("test", TTree::kOverwrite);
 

}
