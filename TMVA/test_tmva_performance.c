#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>


///////////////////
//     MAIN      //
///////////////////
void test_tmva_performance()
{
  // Open the tfile - mtva ouput
  TFile *tmva_file = new TFile("TMVA.root");
  
  
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
  //map<int, map<int, vector<double>> > event_jetn_dl1r_kbdt; 
  // { {event_0: {jet_0 - {DL1r, KBDT}}, {jet_1 - {DL1r, KBDT}} ... },
  //   {event_1: {jet_0 - {DL1r, KBDT}}, {jet_1 - {DL1r, KBDT}} ... }, etc }
  
  
  // Get number of entries
  Int_t nEntries = test_tree->GetEntries();
  
  
  for (int entry=0; entry<nEntries; entry++) {
    
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    test_tree->GetEntry(entry);
    cout << entry << endl;
    
    // Black magic
    /*  map<int, vector<double> > jetn_dl1r_kbdt;
    vector<double> dl1r_kbdt = {jet_DL1r_weight, kbdt_val};
    
    map<int, map<int, vector<double>> >::iterator it = event_jetn_dl1r_kbdt.find(entry);
    if ( it != event_jetn_dl1r_kbdt.end() ) {
      jetn_dl1r_kbdt = it->second;
      int n_jets_already = jetn_dl1r_kbdt.size();
      jetn_dl1r_kbdt.insert(make_pair(n_jets_already, dl1r_kbdt));
      it->second = jetn_dl1r_kbdt; }
    else {
      jetn_dl1r_kbdt.insert(make_pair(0, dl1r_kbdt));
      event_jetn_dl1r_kbdt.insert(make_pair(0, jetn_dl1r_kbdt)); }
    */
    
  
    
  } // [entry] - loop over entries
  
  
  
}
