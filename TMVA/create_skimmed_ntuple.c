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
  // Open the tfile - mtva ouput
  TFile *tmva_file = new TFile("TMVA400.root");
  
  
  // Open the test tree
  TTree *test_tree = (TTree*)tmva_file->Get("dataset/TestTree");
  
  
  // Set branches
  float kbdt_val, weight, topHOF, jet_DL1r_weight;
  float event_number;
  kbdt_val = weight = topHOF = jet_DL1r_weight = 0;
  event_number = 0;
  
  test_tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
  test_tree->SetBranchAddress("weight", &weight);
  test_tree->SetBranchAddress("KBDT", &kbdt_val);
  test_tree->SetBranchAddress("NN_jet_DL1r", &jet_DL1r_weight);
  test_tree->SetBranchAddress("NN_event_number", &event_number);
  
  
  // Set some variables for the analysis
  double kbdt_cut = 0.1;
  int n_jets_from_top = 0;
  map<int, map<int, vector<double>> > event_jetn_dl1r_kbdt;
  // { {event_0: {jet_0 - {DL1r, KBDT}}, {jet_1 - {DL1r, KBDT}} ... },
  //   {event_1: {jet_0 - {DL1r, KBDT}}, {jet_1 - {DL1r, KBDT}} ... }, etc }
  
  
  // Get number of entries
  Int_t nEntries = test_tree->GetEntries();
  
  
  for (int entry=0; entry<nEntries; entry++) {
    
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    test_tree->GetEntry(entry);
   
    
    // Black magic
    map<int, vector<double> > jetn_dl1r_kbdt;
    vector<double> dl1r_kbdt = {jet_DL1r_weight, kbdt_val, weight, topHOF};
    int jetn;
    
    map<int, map<int, vector<double>> >::iterator it2 = event_jetn_dl1r_kbdt.find(event_number);
    if ( it2 != event_jetn_dl1r_kbdt.end() ) {
      jetn_dl1r_kbdt = it2->second;
      jetn = jetn_dl1r_kbdt.size();
      jetn_dl1r_kbdt.insert(make_pair(jetn, dl1r_kbdt));
      it2->second = jetn_dl1r_kbdt; }
    else {
      jetn_dl1r_kbdt.insert(make_pair(0, dl1r_kbdt));
      event_jetn_dl1r_kbdt.insert(make_pair(event_number, jetn_dl1r_kbdt)); }
      
      
  } // [entry] - loop over entries
  
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
  vector<float> DL1r_tag_weight_save;
  vector<float> kbdt_save;
  vector<float> weight_save;
  vector<float> topHOF_save;
  event_number_save = 0;

  // Declare new branches
  tree->Branch("event_number", &event_number_save, "event_number/I");
  tree->Branch("jet_DL1r", &DL1r_tag_weight_save, "jet_DL1r/F");
  tree->Branch("KBDT", &kbdt_save, "KBDT/F");
  tree->Branch("weight", &weight_save, "weight/F");
  tree->Branch("topHadronOriginFlag", &topHOF_save, "topHadronOriginFlag/F");
  

  // Run a loop over keys - event numbers
  map<int, map<int, vector<double>> >::iterator it1;
  for (it1 = event_jetn_dl1r_kbdt.begin(); it1 != event_jetn_dl1r_kbdt.end(); it1++) {
    
    DL1r_tag_weight_save.clear();
    kbdt_save.clear();
    weight_save.clear();
    topHOF_save.clear();
    
    //cout << "\n" << it1->first << endl;

    // Set a map for jets=keys and properties=values for the event
    map<int, vector<double> > jetn_dl1r_kbdt = it1->second;
    
    // Run a loop over keys of one event - jets (that are not in any reasonable order)
    map<int, vector<double> >::iterator it2;
    for (it2 = jetn_dl1r_kbdt.begin(); it2 != jetn_dl1r_kbdt.end(); it2++) {
      int jetn = it2->first;
      vector<double> jet_pars = it2->second;
      n_jets_in_map_map++;
      
      //cout << "\tjet_# = " << jetn << ";\t dl1r = " << jet_pars[0] << ";\t kbdt = " << jet_pars[1] << ";\tweight = " << jet_pars[2] << ";\ttopHOF = " << jet_pars[3] << endl; 

      DL1r_tag_weight_save.push_back(jet_pars[0]);
      kbdt_save.push_back(jet_pars[1]);
      weight_save.push_back(jet_pars[2]);
      topHOF_save.push_back(jet_pars[3]);

      
    } // [it2] - loop over jets of an event
   
    event_number_save = it1->first;
    
    tree->Fill();
    
    // Release some space
    event_jetn_dl1r_kbdt.erase(it1);
    
  } // [it1] - loop over events
  

  tree->Write("test", TTree::kOverwrite);
 

}
